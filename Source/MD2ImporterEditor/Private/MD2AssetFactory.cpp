// Fill out your copyright notice in the Description page of Project Settings.

#include "MD2AssetFactory.h"
#include "MD2Asset.h"
#include "RawMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine.h"
#include "Factories/TextureFactory.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "PackageTools.h"
#include "AssetToolsModule.h"
#include "ObjectTools.h"
#include "EditorFramework/AssetImportData.h"
#include "Materials/Material.h"
#include "Factories/MaterialFactoryNew.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "HAL/FileManagerGeneric.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Interfaces/IMainFrameModule.h"
#include "MD2OptionsWindow.h"

// note - so much help can be found in FbxMaterialImport.cpp (texture/material creation) and FbxMainImport.cpp (UI)

// Helper struct to organize the texture / material / filenames
struct FMD2SkinImportData
{
	FString TextureFilename;
	FString TextureExtension;

	FString TextureAssetName;
	FString MaterialAssetName;
};

UMD2AssetFactory::UMD2AssetFactory( )
{
	Formats.Add( FString( TEXT( "md2;" ) ) + NSLOCTEXT( "UMD2AssetFactory", "FormatMD2", "Quake II Model File" ).ToString( ) );
	SupportedClass = UMD2Asset::StaticClass( );
	bCreateNew = false;
	bEditorImport = true;
}

UObject* UMD2AssetFactory::FactoryCreateFile( UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	const FString& Filename,
	const TCHAR* Parms,
	FFeedbackContext* Warn,
	bool& bOutOperationCanceled )
{
	// try to get the full path, but if the inparent is somehow null, at least show the 
	// asset name.
	if ( InParent == nullptr )
	{
		UE_LOG( LogTemp, Warning, TEXT( "InParent is null. Can't import %s" ), *Filename );

		bOutOperationCanceled = true;
		return nullptr;
	}
	
	//Importing is done and working, including re-import!
	//TODO: There are two minor bugs on static mesh: 
	// 1. I can't seem to rename the created asset. If it doesn't match the filename, Content Drawer won't show it.
	// 2. When re-importing, it doesn't find an Existing Object, but it does overwrite the asset correctly. Weird.
			// Only difference I can see from Textures/Materials is that I'm NOT using a factory to create the mesh.

	// TODO: UI! It should populate texture slots based on whats found below, and suggest names
	// based on what is being created below too.

	// todo: offer the option to use a single material with a lerp for textures

	//TODO: Figure out what to do with the second+ materials

	// start by breaking apart the filename so we can use it to find / name assets
	FString MeshFileBasePath, MeshFileName, MeshExtension;
	FPaths::Split( Filename, MeshFileBasePath, MeshFileName, MeshExtension );
	MeshFileName = ObjectTools::SanitizeObjectName( MeshFileName );

	// build the final mesh asset name
	// TODO: UE 5.3 seems to require imports to maintain the filename of their source file
	FString MeshAssetName = /*TEXT("SM_") + */MeshFileName;

	UMD2Asset* MD2Asset = NewObject<UMD2Asset>( );
	bool bResult = MD2Asset->Load( Filename );
	if ( bResult == false )
	{
		//todo: Show a message box?
		UE_LOG( LogTemp, Warning, TEXT( "Failed to load MD2 Asset %s" ), *Filename );
		return nullptr;
	}

	TArray<FString> PCXTextureNames;
	MD2Asset->GetPCXTextureList( PCXTextureNames );

	//todo: We now have what we need to show the import dialog!
	// we have a valid md2 model, and we have the texture list!!
	if ( GetImportOptions( InParent->GetPathName( ) ) == false )
	{
		bOutOperationCanceled = true;
		return nullptr;
	}

	TArray<TWeakObjectPtr<UObject>> CreatedObjects;

	UStaticMesh* StaticMesh = ImportMD2Asset( InParent, MD2Asset, Filename, MeshAssetName, CreatedObjects );

	// The MD2 spec provides the relative skin path in the model. Since it's unlikely that path still exists,
	// assume those files will be in a folder or subfolder of the MD2.

	// todo: This will instead iterate over whatever was provided from md2options
	TArray<FString> PCXFiles;
	FindPCXFiles( MeshFileBasePath, PCXTextureNames, PCXFiles );

	// build a list of filename / texture asset name pairings
	TArray<FMD2SkinImportData> SkinImports;
	BuildSkinAssetNames( PCXFiles, MeshFileName, SkinImports );


	TArray<TWeakObjectPtr<UMaterial>> Materials;
	for ( int i = 0; i < SkinImports.Num( ); i++ )
	{
		FString TextureFullFilename = MeshFileBasePath / SkinImports[ i ].TextureFilename + TEXT( "." ) + SkinImports[ i ].TextureExtension;
		UTexture* Texture = ImportTexture( InParent, TextureFullFilename, SkinImports[ i ].TextureAssetName, SkinImports[ i ].TextureExtension, CreatedObjects );

		UMaterial* Material = CreateMaterial( InParent, Texture, SkinImports[ i ].MaterialAssetName, CreatedObjects );
		Materials.Add( Material );
	}

	// assign materials to the static mesh
	for ( int i = 0; i < Materials.Num( ); i++ )
	{
		StaticMesh->GetStaticMaterials( ).Add( Materials[ i ].Get( ) );
		StaticMesh->GetSectionInfoMap( ).Set( 0, 0, FMeshSectionInfo( i ) );
	}
	StaticMesh->MarkPackageDirty( );

	return StaticMesh;
}

bool UMD2AssetFactory::GetImportOptions( const FString& FullPath )
{
	TSharedPtr<SWindow> ParentWindow;

	if ( FModuleManager::Get( ).IsModuleLoaded( "MainFrame" ) )
	{
		IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>( "MainFrame" );
		ParentWindow = MainFrame.GetParentWindow( );
	}

	// Compute centered window position based on max window size, which include when all categories are expanded
	const float MD2ImportWindowWidth = 450.0f;
	const float MD2ImportWindowHeight = 750.0f;
	FVector2D MD2ImportWindowSize = FVector2D( MD2ImportWindowWidth, MD2ImportWindowHeight ); // Max window size it can get based on current slate


	FSlateRect WorkAreaRect = FSlateApplicationBase::Get( ).GetPreferredWorkArea( );
	FVector2D DisplayTopLeft( WorkAreaRect.Left, WorkAreaRect.Top );
	FVector2D DisplaySize( WorkAreaRect.Right - WorkAreaRect.Left, WorkAreaRect.Bottom - WorkAreaRect.Top );

	float ScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint( DisplayTopLeft.X, DisplayTopLeft.Y );
	MD2ImportWindowSize *= ScaleFactor;

	FVector2D WindowPosition = (DisplayTopLeft + (DisplaySize - MD2ImportWindowSize) / 2.0f) / ScaleFactor;


	TSharedRef<SWindow> Window = SNew( SWindow )
		.Title( NSLOCTEXT( "UnrealEd", "MD2ImportOpionsTitle", "MD2 Import Options" ) )
		.SizingRule( ESizingRule::Autosized )
		.AutoCenter( EAutoCenter::None )
		.ClientSize( MD2ImportWindowSize )
		.ScreenPosition( WindowPosition );

	TSharedPtr<SMD2OptionsWindow> MD2OptionsWindow;
	Window->SetContent
	(
		SAssignNew( MD2OptionsWindow, SMD2OptionsWindow )
		.WidgetWindow( Window )
		.FullPath( FText::FromString( FullPath ) )
		.MaxWindowHeight( MD2ImportWindowHeight )
		.MaxWindowWidth( MD2ImportWindowWidth )
	);

	// @todo: we can make this slow as showing progress bar later
	FSlateApplication::Get( ).AddModalWindow( Window, ParentWindow, false );

	return MD2OptionsWindow->ShouldImport( );
}

UTexture* UMD2AssetFactory::ImportTexture( UObject* InParent,
	const FString& TextureFullFilename,
	FString& InOutTextureAssetName,
	const FString& TextureExtension,
	TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects )
{
	// Put the texture in the base of the package they selected for import (So like, /Game/)
	FString BasePackageName = FPackageName::GetLongPackagePath( InParent->GetOutermost( )->GetName( ) );
	FString TexturePackageName = BasePackageName / InOutTextureAssetName;
	TexturePackageName = UPackageTools::SanitizePackageName( TexturePackageName );

	// First check if the asset already exists.
	UTexture* ExistingTexture = LoadObject<UTexture>( NULL, *TexturePackageName, nullptr, LOAD_Quiet | LOAD_NoWarn );

	UPackage* TexturePackage = NULL;
	if ( !ExistingTexture )
	{
		// it doesn't exist, so create the package with as close a name as possible (or the same if there's no name collision)
		const FString Suffix( TEXT( "" ) );

		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>( "AssetTools" );
		FString FinalPackageName;

		FString RequestedTextureAssetName = InOutTextureAssetName; //backup what they WANTED, so we can warn if it changed.
		AssetToolsModule.Get( ).CreateUniqueAssetName( TexturePackageName, Suffix, FinalPackageName, InOutTextureAssetName );

		if ( RequestedTextureAssetName != InOutTextureAssetName )
		{
			UE_LOG( LogTemp, Warning, TEXT( "Texture Asset Name Changed on import. Requested: %s Actual: %s" ), *RequestedTextureAssetName, *InOutTextureAssetName );
		}

		TexturePackage = CreatePackage( *FinalPackageName );
	}
	else
	{
		TexturePackage = ExistingTexture->GetOutermost( );
	}

	TArray<uint8> TextureData;
	FFileHelper::LoadFileToArray( TextureData, *TextureFullFilename );

	UTexture* UnrealTexture = nullptr;
	if ( TextureData.Num( ) > 0 )
	{
		const uint8* DataPtr = TextureData.GetData( );

		UTextureFactory* TextureFactory = NewObject<UTextureFactory>( );
		TextureFactory->AddToRoot( );

		// save texture settings if texture exist
		TextureFactory->SuppressImportOverwriteDialog( );
		const bool bTextureAssetAlreadyExists = FindObject<UTexture>( TexturePackage, *InOutTextureAssetName ) != nullptr;

		UnrealTexture = (UTexture*)TextureFactory->FactoryCreateBinary(
			UTexture2D::StaticClass( ), TexturePackage, *InOutTextureAssetName,
			RF_Standalone | RF_Public, NULL, *TextureExtension,
			DataPtr, DataPtr + TextureData.Num( ), GWarn );

		if ( UnrealTexture != NULL )
		{
			// Make sure the AssetImportData points to the texture file
			UnrealTexture->AssetImportData->Update( IFileManager::Get( ).ConvertToAbsolutePathForExternalAppForRead( *TextureFullFilename ) );

			if ( bTextureAssetAlreadyExists == false )
			{
				OutCreatedObjects.Add( UnrealTexture );
			}

			// Notify the asset registry
			FAssetRegistryModule::AssetCreated( UnrealTexture );

			// Set the dirty flag so this package will get saved later
			TexturePackage->SetDirtyFlag( true );
		}

		TextureFactory->RemoveFromRoot( );
	}

	return UnrealTexture;
}

UMaterial* UMD2AssetFactory::CreateMaterial( UObject* InParent,
	UTexture* InSourceTexture,
	FString& InOutMaterialAssetName,
	TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects )
{
	// this material isn't based on an actual source file, so we can just create its asset name
	FString BasePackageName = FPackageName::GetLongPackagePath( InParent->GetOutermost( )->GetName( ) );
	BasePackageName = UPackageTools::SanitizePackageName( BasePackageName );

	FString MaterialPackageName = BasePackageName / ObjectTools::SanitizeObjectName( InOutMaterialAssetName );

	UPackage* MaterialPackage = nullptr;
	UMaterial* ExistingMaterial = LoadObject<UMaterial>( NULL, *MaterialPackageName, nullptr, LOAD_Quiet | LOAD_NoWarn );
	if ( !ExistingMaterial )
	{
		// it doesn't exist, so create the package with as close a name as possible (or the same if there's no name collision)
		const FString Suffix( TEXT( "" ) );

		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>( "AssetTools" );
		FString FinalPackageName;

		FString RequstedMaterialAssetName = InOutMaterialAssetName;
		AssetToolsModule.Get( ).CreateUniqueAssetName( MaterialPackageName, Suffix, FinalPackageName, InOutMaterialAssetName );

		if ( RequstedMaterialAssetName != InOutMaterialAssetName )
		{
			UE_LOG( LogTemp, Warning, TEXT( "Material Asset Name Changed on import. Requested: %s Actual: %s" ), *RequstedMaterialAssetName, *InOutMaterialAssetName );
		}

		MaterialPackage = CreatePackage( *FinalPackageName );
	}
	else
	{
		MaterialPackage = ExistingMaterial->GetOutermost( );
	}

	// create an unreal material asset
	auto MaterialFactory = NewObject<UMaterialFactoryNew>( );
	MaterialFactory->AddToRoot( );

	const bool bAssetAlreadyExists = FindObject<UStaticMesh>( MaterialPackage, *InOutMaterialAssetName ) != nullptr;

	UMaterial* UnrealMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(
		UMaterial::StaticClass( ), MaterialPackage, *InOutMaterialAssetName, RF_Standalone | RF_Public, NULL, GWarn );

	if ( UnrealMaterial != nullptr )
	{
		if ( bAssetAlreadyExists == false )
		{
			OutCreatedObjects.Add( UnrealMaterial );
		}

		// Notify the asset registry
		FAssetRegistryModule::AssetCreated( UnrealMaterial );

		// Set the dirty flag so this package will get saved later
		MaterialPackage->SetDirtyFlag( true );

		if ( InSourceTexture != nullptr )
		{
			UMaterialExpressionTextureSample* TextureExpression = NewObject<UMaterialExpressionTextureSample>( UnrealMaterial );
			TextureExpression->Texture = InSourceTexture;
			TextureExpression->SamplerType = SAMPLERTYPE_Color;

			// -250 seems to be what most importers use (looking at code examples), and looks nice.
			// todo: if we need multiple textures, this will need to be updated
			TextureExpression->MaterialExpressionEditorX = -250;

			UnrealMaterial->GetExpressionCollection( ).AddExpression( TextureExpression );
			UnrealMaterial->GetEditorOnlyData( )->BaseColor.Expression = TextureExpression;
		}

		// let the material update itself if necessary
		UnrealMaterial->PreEditChange( NULL );
		UnrealMaterial->PostEditChange( );
	}

	MaterialFactory->RemoveFromRoot( );

	return UnrealMaterial;
}

UStaticMesh* UMD2AssetFactory::ImportMD2Asset( UObject* InParent,
	UMD2Asset* MD2Asset,
	const FString& MD2FullFilename,
	FString& InOutStaticMeshAssetName,
	TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects )
{
	// put the mesh in the base package they selected (again, like '/Game')
	FString BasePackageName = FPackageName::GetLongPackagePath( InParent->GetOutermost( )->GetName( ) );
	BasePackageName = UPackageTools::SanitizePackageName( BasePackageName );

	FString StaticMeshPackageName = BasePackageName / ObjectTools::SanitizeObjectName( InOutStaticMeshAssetName );

	UPackage* MeshPackage = nullptr;
	UStaticMesh* ExistingMesh = LoadObject<UStaticMesh>( NULL, *StaticMeshPackageName, nullptr, LOAD_Quiet | LOAD_NoWarn );
	if ( !ExistingMesh )
	{
		// it doesn't exist, so create the package with as close a name as possible (or the same if there's no name collision)
		const FString Suffix( TEXT( "" ) );

		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>( "AssetTools" );
		FString FinalPackageName;

		FString RequestedStaticMeshAssetName = InOutStaticMeshAssetName;
		AssetToolsModule.Get( ).CreateUniqueAssetName( StaticMeshPackageName, Suffix, FinalPackageName, InOutStaticMeshAssetName );

		if ( RequestedStaticMeshAssetName != InOutStaticMeshAssetName )
		{
			UE_LOG( LogTemp, Warning, TEXT( "Static Mesh Asset Name Changed on import. Requested: %s Actual: %s" ), *RequestedStaticMeshAssetName, *InOutStaticMeshAssetName );
		}

		MeshPackage = CreatePackage( *FinalPackageName );
	}
	else
	{
		MeshPackage = ExistingMesh->GetOutermost( );
	}

	const bool bAssetAlreadyExists = FindObject<UStaticMesh>( MeshPackage, *InOutStaticMeshAssetName ) != nullptr;
	UStaticMesh* StaticMesh = NewObject<UStaticMesh>( InParent, UStaticMesh::StaticClass( ), *InOutStaticMeshAssetName, RF_Public | RF_Standalone );
	if ( StaticMesh != nullptr )
	{
		StaticMesh->AddSourceModel( );
		FStaticMeshSourceModel& SourceModel = StaticMesh->GetSourceModel( 0 );
		FRawMesh RawMesh = FRawMesh( );

		MD2Asset->Convert( RawMesh );

		SourceModel.RawMeshBulkData->SaveRawMesh( RawMesh );

		// Model Configuration - todo: expose these to a UI
		SourceModel.BuildSettings.bRecomputeNormals = true;
		SourceModel.BuildSettings.bRecomputeTangents = true;
		SourceModel.BuildSettings.bUseMikkTSpace = false;
		SourceModel.BuildSettings.bGenerateLightmapUVs = true;
		SourceModel.BuildSettings.bRemoveDegenerates = true;
		SourceModel.BuildSettings.bBuildReversedIndexBuffer = false;
		SourceModel.BuildSettings.bUseFullPrecisionUVs = false;
		SourceModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
		StaticMesh->Build( );

		// Make sure the AssetImportData points to the mesh file
		StaticMesh->AssetImportData->Update( IFileManager::Get( ).ConvertToAbsolutePathForExternalAppForRead( *MD2FullFilename ) );

		if ( bAssetAlreadyExists == false )
		{
			OutCreatedObjects.Add( StaticMesh );
		}

		// Notify the asset registry
		FAssetRegistryModule::AssetCreated( StaticMesh );

		// flag dirty so it saves later
		MeshPackage->SetDirtyFlag( true );
	}

	return StaticMesh;
}

void UMD2AssetFactory::BuildSkinAssetNames( const TArray<FString>& PCXFiles, const FString& ParentMeshName, TArray<struct FMD2SkinImportData>& OutSkinImports )
{
	for ( auto PCXFile : PCXFiles )
	{
		FMD2SkinImportData SkinImportData;
		SkinImportData.TextureFilename = FPaths::GetBaseFilename( PCXFile, true );
		SkinImportData.TextureExtension = UMD2Asset::TEXTURE_FORMAT;
		SkinImportData.TextureAssetName = ObjectTools::SanitizeObjectName( TEXT( "T_" ) + ParentMeshName + TEXT( "_" ) + SkinImportData.TextureFilename + TEXT( "_D" ) );
		SkinImportData.MaterialAssetName = ObjectTools::SanitizeObjectName( TEXT( "M_" ) + ParentMeshName + TEXT( "_" ) + SkinImportData.TextureFilename + TEXT( "_D" ) );

		OutSkinImports.Add( SkinImportData );
	}
}

void UMD2AssetFactory::FindPCXFiles( const FString& SearchFileBasePath, TArray<FString>& InTextureNames, TArray<FString>& OutPCXFiles )
{
	TArray<FString> FoundFiles;

	// search for all files in the format supported by MD2
	FFileManagerGeneric FileManager;
	for ( int i = 0; i < InTextureNames.Num(); i++ )
	{
		int32 CurrNum = OutPCXFiles.Num( );
		FileManager.FindFilesRecursive( OutPCXFiles, *SearchFileBasePath, *InTextureNames[i], true, false, false );

		// if Num() didn't increase, the file wasn't there
		if ( CurrNum == OutPCXFiles.Num( ) )
		{
			UE_LOG( LogTemp, Warning, TEXT( "MD2 Import: Could not find texture: %s" ), *InTextureNames[ i ] );
		}
	}
}

float UMD2AssetFactory::ScaleForDPI( float Value )
{
	FSlateRect WorkAreaRect = FSlateApplicationBase::Get( ).GetPreferredWorkArea( );
	FVector2D DisplayTopLeft( WorkAreaRect.Left, WorkAreaRect.Top );
	FVector2D DisplaySize( WorkAreaRect.Right - WorkAreaRect.Left, WorkAreaRect.Bottom - WorkAreaRect.Top );

	float ScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint( DisplayTopLeft.X, DisplayTopLeft.Y );

	return ScaleFactor * Value;
}

void UMD2AssetFactory::TestCreateRawMesh( FRawMesh& OutRawMesh )
{
	FVector3f EmptyVector = FVector3f( 0, 0, 0 );
	FColor WhiteVertex = FColor( 255, 255, 255, 255 );

	OutRawMesh.VertexPositions.Emplace( FVector3f( 0.f, 0.f, 0.f ) );
	OutRawMesh.VertexPositions.Emplace( FVector3f( 100.f, 0.f, 0.f ) );
	OutRawMesh.VertexPositions.Emplace( FVector3f( 100.f, 100.f, 0.f ) );
	OutRawMesh.VertexPositions.Emplace( FVector3f( 0.f, 100.f, 0.f ) );

	OutRawMesh.WedgeIndices.Emplace( 0 );
	OutRawMesh.WedgeIndices.Emplace( 1 );
	OutRawMesh.WedgeIndices.Emplace( 2 );

	OutRawMesh.WedgeIndices.Emplace( 0 );
	OutRawMesh.WedgeIndices.Emplace( 2 );
	OutRawMesh.WedgeIndices.Emplace( 3 );

	OutRawMesh.WedgeIndices.Emplace( 1 );
	OutRawMesh.WedgeIndices.Emplace( 0 );
	OutRawMesh.WedgeIndices.Emplace( 3 );

	OutRawMesh.WedgeIndices.Emplace( 1 );
	OutRawMesh.WedgeIndices.Emplace( 3 );
	OutRawMesh.WedgeIndices.Emplace( 2 );

	for ( int i = 0; i < 4; ++i )
	{
		OutRawMesh.WedgeColors.Add( WhiteVertex );
		OutRawMesh.WedgeColors.Add( WhiteVertex );
		OutRawMesh.WedgeColors.Add( WhiteVertex );

		OutRawMesh.WedgeTangentX.Add( EmptyVector );
		OutRawMesh.WedgeTangentX.Add( EmptyVector );
		OutRawMesh.WedgeTangentX.Add( EmptyVector );

		OutRawMesh.WedgeTangentY.Add( EmptyVector );
		OutRawMesh.WedgeTangentY.Add( EmptyVector );
		OutRawMesh.WedgeTangentY.Add( EmptyVector );

		OutRawMesh.WedgeTangentZ.Add( EmptyVector );
		OutRawMesh.WedgeTangentZ.Add( EmptyVector );
		OutRawMesh.WedgeTangentZ.Add( EmptyVector );

		for ( int UVIndex = 0; UVIndex < MAX_MESH_TEXTURE_COORDS; UVIndex++ )
		{
			OutRawMesh.WedgeTexCoords[ UVIndex ].Add( FVector2f( 0.0f, 0.0f ) );
			OutRawMesh.WedgeTexCoords[ UVIndex ].Add( FVector2f( 0.0f, 0.0f ) );
			OutRawMesh.WedgeTexCoords[ UVIndex ].Add( FVector2f( 0.0f, 0.0f ) );
		}

		OutRawMesh.FaceMaterialIndices.Add( 0 );
		OutRawMesh.FaceSmoothingMasks.Add( 0xFFFFFFFF ); // Phong ?
	}
}

void UMD2AssetFactory::TestAttachToNewActor( const UStaticMesh& StaticMesh )
{
	// JHM Note - uncomment this to create an actor and put the static mesh on it.
	// Might want this for an eventual preview window?
	// Creating the Actor and Positioning it in the World based in the Static Mesh
	/*UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel * currentLevel = currentWorld->GetCurrentLevel();

	UClass* staticMeshActorClass = AStaticMeshActor::StaticClass();

	// Name & Transform
	FVector objectPosition(0, 0, 0);
	FRotator objectRotation(0, 0, 0); //in degrees
	FVector objectScale(1, 1, 1);
	FTransform objectTransform(objectRotation, objectPosition, objectScale);
	AActor* newActorCreated = GEditor->AddActor(currentLevel, staticMeshActorClass, objectTransform, true, RF_Public | RF_Standalone | RF_Transactional);

	AStaticMeshActor* smActor = Cast<AStaticMeshActor>(newActorCreated);

	smActor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
	smActor->SetActorScale3D(objectScale);

	// ID Name & Visible Name
	smActor->Rename(TEXT("MyStaticMeshInTheWorld"));
	smActor->SetActorLabel("MyStaticMeshInTheWorld");

	GEditor->EditorUpdateComponents();
	smActor->GetStaticMeshComponent()->RegisterComponentWithWorld(currentWorld);
	currentWorld->UpdateWorldComponents(true, false);
	smActor->RerunConstructionScripts();
	//GLevelEditorModeTools().MapChangeNotify();
	//

	return nullptr;*/
}
