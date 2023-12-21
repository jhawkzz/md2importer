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

UMD2AssetFactory::UMD2AssetFactory( )
{
	Formats.Add( FString( TEXT( "md2;" ) ) + NSLOCTEXT( "UMD2AssetFactory", "FormatMD2", "Quake II Model File" ).ToString( ) );
	SupportedClass = UMD2Asset::StaticClass( );
	bCreateNew = false;
	bEditorImport = true;
}

UTexture* UMD2AssetFactory::ImportPCXTexture( UObject* InParent, const FString& Filename, TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects )
{
	// note - a lot of this was taken from FbxMaterialImport.cpp - UTexture* UnFbx::FFbxImporter::ImportTexture(FbxFileTexture* FbxTexture, bool bSetupAsNormalMap)

	// Textures - for now, look for PCX files in the same folder - later we'll add a widget panel to browse for them.
	FString InNameFileBasePath, InNamePartFileName, InNamePartExtension;
	FPaths::Split( Filename, InNameFileBasePath, InNamePartFileName, InNamePartExtension );

	// get the actual filename for the texture to import, and build the UTexture asset name
	FString AbsoluteFilename = InNameFileBasePath + UTF8_TO_TCHAR( "/skin.pcx" );
	FString Extension = FPaths::GetExtension( AbsoluteFilename ).ToLower( );
	FString TextureName = FPaths::GetBaseFilename( AbsoluteFilename );
	TextureName = ObjectTools::SanitizeObjectName( TextureName );

	// set where to place the textures
	FString BasePackageName = FPackageName::GetLongPackagePath( InParent->GetOutermost( )->GetName( ) ) / TextureName;
	BasePackageName = UPackageTools::SanitizePackageName( BasePackageName );

	UTexture* ExistingTexture = NULL;
	UPackage* TexturePackage = NULL;
	// First check if the asset already exists.
	{
		FString ObjectPath = BasePackageName + TEXT( "." ) + TextureName;
		ExistingTexture = LoadObject<UTexture>( NULL, *ObjectPath, nullptr, LOAD_Quiet | LOAD_NoWarn );
	}

	if ( !ExistingTexture )
	{
		const FString Suffix( TEXT( "" ) );

		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>( "AssetTools" );
		FString FinalPackageName;
		AssetToolsModule.Get( ).CreateUniqueAssetName( BasePackageName, Suffix, FinalPackageName, TextureName );

		TexturePackage = CreatePackage( *FinalPackageName );
	}
	else
	{
		TexturePackage = ExistingTexture->GetOutermost( );
	}

	FString FinalFilePath;
	if ( IFileManager::Get( ).FileExists( *AbsoluteFilename ) )
	{
		// try opening from absolute path
		FinalFilePath = AbsoluteFilename;
	}
	else if ( IFileManager::Get( ).FileExists( *(InNameFileBasePath / TextureName) ) )
	{
		// try fbx file base path + relative path
		FinalFilePath = InNameFileBasePath / TextureName;
	}
	else if ( IFileManager::Get( ).FileExists( *(InNameFileBasePath / AbsoluteFilename) ) )
	{
		// Some fbx files dont store the actual absolute filename as absolute and it is actually relative.  Try to get it relative to the FBX file we are importing
		FinalFilePath = InNameFileBasePath / AbsoluteFilename;
	}
	else
	{
		UE_LOG( LogTemp, Display, TEXT( "Unable to find Texture file %s" ), *AbsoluteFilename );
	}

	TArray<uint8> TextureData;
	if ( !FinalFilePath.IsEmpty( ) )
	{
		FFileHelper::LoadFileToArray( TextureData, *FinalFilePath );
	}

	UTexture* UnrealTexture = nullptr;
	if ( TextureData.Num( ) > 0 )
	{
		const uint8* DataPtr = TextureData.GetData( );

		UTextureFactory* TextureFactory = NewObject<UTextureFactory>( );
		TextureFactory->AddToRoot( );

		// save texture settings if texture exist
		TextureFactory->SuppressImportOverwriteDialog( );
		const bool bTextureAssetAlreadyExists = FindObject<UTexture>( TexturePackage, *TextureName ) != nullptr;

		UnrealTexture = (UTexture*)TextureFactory->FactoryCreateBinary(
			UTexture2D::StaticClass( ), TexturePackage, *TextureName,
			RF_Standalone | RF_Public, NULL, TEXT( "pcx" ),
			DataPtr, DataPtr + TextureData.Num( ), GWarn );

		if ( UnrealTexture != NULL )
		{
			// Make sure the AssetImportData points to the texture file
			UnrealTexture->AssetImportData->Update( IFileManager::Get( ).ConvertToAbsolutePathForExternalAppForRead( *FinalFilePath ) );

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

UStaticMesh* UMD2AssetFactory::ImportMD2Asset( UObject* InParent, const FString& Filename, TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects )
{
	FString InNameFileBasePath, InNamePartFileName, InNamePartExtension;
	FPaths::Split( Filename, InNameFileBasePath, InNamePartFileName, InNamePartExtension );

	// break out the parts of the file path
	FString AbsoluteFilename = Filename;
	FString Extension = FPaths::GetExtension( AbsoluteFilename ).ToLower( );
	FString MD2MeshName = FPaths::GetBaseFilename( AbsoluteFilename );
	MD2MeshName = ObjectTools::SanitizeObjectName( MD2MeshName );

	// set where to place the mesh
	FString BasePackageName = FPackageName::GetLongPackagePath( InParent->GetOutermost( )->GetName( ) ) / MD2MeshName;
	BasePackageName = UPackageTools::SanitizePackageName( BasePackageName );

	UStaticMesh* ExistingMesh = NULL;
	UPackage* MeshPackage = NULL;
	// First check if the asset already exists.
	{
		FString ObjectPath = BasePackageName + TEXT( "." ) + MD2MeshName;
		ExistingMesh = LoadObject<UStaticMesh>( NULL, *ObjectPath, nullptr, LOAD_Quiet | LOAD_NoWarn );
	}

	if ( !ExistingMesh )
	{
		const FString Suffix( TEXT( "" ) );

		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>( "AssetTools" );
		FString FinalPackageName;
		AssetToolsModule.Get( ).CreateUniqueAssetName( BasePackageName, Suffix, FinalPackageName, MD2MeshName );

		MeshPackage = CreatePackage( *FinalPackageName );
	}
	else
	{
		MeshPackage = ExistingMesh->GetOutermost( );
	}

	FString FinalFilePath;
	if ( IFileManager::Get( ).FileExists( *AbsoluteFilename ) )
	{
		// try opening from absolute path
		FinalFilePath = AbsoluteFilename;
	}
	else if ( IFileManager::Get( ).FileExists( *(InNameFileBasePath / MD2MeshName) ) )
	{
		// try fbx file base path + relative path
		FinalFilePath = InNameFileBasePath / MD2MeshName;
	}
	else if ( IFileManager::Get( ).FileExists( *(InNameFileBasePath / AbsoluteFilename) ) )
	{
		// Some fbx files dont store the actual absolute filename as absolute and it is actually relative.  Try to get it relative to the FBX file we are importing
		FinalFilePath = InNameFileBasePath / AbsoluteFilename;
	}
	else
	{
		UE_LOG( LogTemp, Display, TEXT( "Unable to find MD2 file %s" ), *AbsoluteFilename );
	}

	TArray<uint8> BinaryData;
	FFileHelper::LoadFileToArray( BinaryData, *Filename, 0 );

	UStaticMesh* StaticMesh = nullptr;
	if ( BinaryData.Num( ) > 0 )
	{
		UMD2Asset* MD2Asset = NewObject<UMD2Asset>( );
		MD2Asset->Load( &BinaryData );

		const bool bAssetAlreadyExists = FindObject<UStaticMesh>( MeshPackage, *MD2MeshName ) != nullptr;

		StaticMesh = NewObject<UStaticMesh>( InParent, UStaticMesh::StaticClass( ), *MD2MeshName, RF_Public | RF_Standalone );

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
			StaticMesh->AssetImportData->Update( IFileManager::Get( ).ConvertToAbsolutePathForExternalAppForRead( *FinalFilePath ) );

			if ( bAssetAlreadyExists == false )
			{
				OutCreatedObjects.Add( StaticMesh );
			}

			// Notify the asset registry
			FAssetRegistryModule::AssetCreated( StaticMesh );
		}
	}

	return StaticMesh;
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

	//TODO: Error handling
	//TODO: Texture selection & materials

	TArray<TWeakObjectPtr<UObject>> CreatedObjects;

	ImportPCXTexture( InParent, Filename, CreatedObjects );

	UStaticMesh* StaticMesh = ImportMD2Asset( InParent, Filename, CreatedObjects );

	return StaticMesh;
}

void UMD2AssetFactory::CreateRawMesh( FRawMesh& OutRawMesh )
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

void UMD2AssetFactory::AttachToNewActor( const UStaticMesh& StaticMesh )
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
