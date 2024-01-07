// Fill out your copyright notice in the Description page of Project Settings.
#include "MD2AssetFactory.h"

// UE Includes
#include "Engine.h"
#include "RawMesh.h"
#include "ObjectTools.h"
#include "PackageTools.h"
#include "AssetToolsModule.h"
#include "Materials/Material.h"
#include "HAL/FileManagerGeneric.h"
#include "Factories/TextureFactory.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Interfaces/IMainFrameModule.h"
#include "Factories/MaterialFactoryNew.h"
#include "EditorFramework/AssetImportData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Materials/MaterialExpressionTextureSample.h"

// Project Includes
#include "MD2Util.h"
#include "MD2Asset.h"
#include "MD2OptionsWindow.h"
#include "SMD2MessageBoxWidget.h"

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


	//
	// TODOs:
	// Expose mesh import options to the UI
	//

	// try to get the full path, but if the inparent is somehow null, at least show the 
	// asset name.
	if ( InParent == nullptr )
	{
		Warn->Log( ELogVerbosity::Error, TEXT( "InParent is null. Can't import '" ) + Filename + "'" );

		bOutOperationCanceled = true;
		return nullptr;
	}

	// NOTE: There are a few minor bugs on static mesh: 
	// 1. I can't seem to rename the created asset. If it doesn't match the filename, Content Drawer won't show it.
	// 2. When re-importing, it doesn't find an Existing Object, but it does overwrite the asset correctly. Weird. (This works fine with Textures / Materials)
	// 3. If you say "Yes" to overwriting an asset, and then there's an error or the user cancels, the mesh is deleted because we return nullptr.

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
		FOnMessageBoxClosed OnClosedResult;

		SMD2MessageBoxWidget::ShowMessageBox(
			TEXT( "MD2 Import Error" ),
			TEXT( "Failed to open MD2 Asset '" ) + Filename + TEXT( "'. Check the Output Log for more detail." ),
			TEXT( "Ok" ),
			FString( ),
			OnClosedResult.CreateLambda( [ this ]( uint32 Result ) { UMD2AssetFactory::OnMBClosedResult( Result ); } ) );

		Warn->Log( ELogVerbosity::Error, TEXT( "MD2 Asset '" ) + Filename + TEXT( "' failed to load. This is likely due to a corrupt or invalid file." ) );

		bOutOperationCanceled = true; //set to true since we're showing our own error.
		return nullptr;
	}

	TArray<FString> PCXTextureNames;
	MD2Asset->GetPCXTextureList( PCXTextureNames );

	// Note JHM - Note 100% sure this is the right way to create, but given that it needs to be passed thru slate,
	// it's safe and i know we wont leak / go out of scope.
	TSharedPtr<FMD2ImportOptions> ImportOptions = MakeShareable<FMD2ImportOptions>( new FMD2ImportOptions( ) );

	SMD2OptionsWindow::ShowImportOptionsWindow( InParent->GetPathName( ), Filename, PCXTextureNames, ImportOptions );
	if ( ImportOptions->bShouldImport == false )
	{
		bOutOperationCanceled = true;
		return nullptr;
	}

	// mark the number of warnings. If this increases at the end, we'll notify the user there were warnings to review.
	int32 NumWarnings = 0;

	UStaticMesh* StaticMesh = ImportMD2Asset( InParent, MD2Asset, Filename, MeshAssetName, Warn, NumWarnings );
	if ( StaticMesh == nullptr )
	{
		SMD2MessageBoxWidget::ShowMessageBox(
			TEXT( "MD2 Import Error" ),
			TEXT( "Failed to create Static Mesh from the MD2 Asset. Check the Output Log for more detail." ),
			TEXT( "Ok" ) );

		Warn->Log( ELogVerbosity::Error, TEXT( "MD2 Asset '" ) + Filename + TEXT( "'loaded, but failed during conversion to a Static Mesh. This could mean the file is corrupt or invalid." ) );

		bOutOperationCanceled = true;
		return nullptr;
	}

	// The MD2 spec provides the relative skin path in the model. Since it's unlikely that path still exists,
	// assume those files will be in a folder or subfolder of the MD2.

	TArray<TWeakObjectPtr<UMaterial>> Materials;
	for ( int32 i = 0; i < ImportOptions->TextureImportList.Num( ); i++ )
	{
		UTexture* Texture = ImportTexture( InParent, ImportOptions->TextureImportList[ i ].Key, ImportOptions->TextureImportList[ i ].Value, UMD2Asset::TEXTURE_FORMAT, Warn, NumWarnings );

		if ( Texture != nullptr )
		{
			UMaterial* Material = CreateMaterial( InParent, Texture, ImportOptions->MaterialNameList[ i ], Warn, NumWarnings );
			if ( Material != nullptr )
			{
				Materials.Add( Material );
			}
		}
	}

	// assign materials to the static mesh
	for ( int32 i = 0; i < Materials.Num( ); i++ )
	{
		StaticMesh->GetStaticMaterials( ).Add( Materials[ i ].Get( ) );
		StaticMesh->GetSectionInfoMap( ).Set( 0, 0, FMeshSectionInfo( i ) );
	}
	StaticMesh->MarkPackageDirty( );

	if ( NumWarnings == 0 )
	{
		SMD2MessageBoxWidget::ShowMessageBox(
			TEXT( "MD2 Import Success" ),
			TEXT( "MD2 Asset imported successfully." ),
			TEXT( "Ok" ) );
	}
	else
	{
		SMD2MessageBoxWidget::ShowMessageBox(
			TEXT( "MD2 Import Success With Warnings" ),
			TEXT( "MD2 Asset imported successfully with warnings. Please check the Output Log for more detail." ),
			TEXT( "Ok" ) );
	}

	return StaticMesh;
}

UStaticMesh* UMD2AssetFactory::ImportMD2Asset(
	UObject* InParent,
	UMD2Asset* MD2Asset,
	const FString& MD2FullFilename,
	FString& InOutStaticMeshAssetName,
	FFeedbackContext* Warn,
	int32& NumWarnings )
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
			NumWarnings++;
			Warn->Log( ELogVerbosity::Warning, TEXT( "Static Mesh Asset Name Changed on import. Requested: '" ) + RequestedStaticMeshAssetName + TEXT( "' Actual: '" ) + InOutStaticMeshAssetName + "'" );
		}

		MeshPackage = CreatePackage( *FinalPackageName );
	}
	else
	{
		MeshPackage = ExistingMesh->GetOutermost( );
	}

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

		// Notify the asset registry
		FAssetRegistryModule::AssetCreated( StaticMesh );

		// flag dirty so it saves later
		MeshPackage->SetDirtyFlag( true );
	}

	return StaticMesh;
}

UTexture* UMD2AssetFactory::ImportTexture( UObject* InParent,
	const FString& TextureFullFilename,
	const FString& InRequestedTextureAssetName,
	const FString& TextureExtension,
	FFeedbackContext* Warn,
	int32& NumWarnings )
{
	// Put the texture in the base of the package they selected for import (So like, /Game/)
	FString BasePackageName = FPackageName::GetLongPackagePath( InParent->GetOutermost( )->GetName( ) );
	FString TexturePackageName = BasePackageName / InRequestedTextureAssetName;
	TexturePackageName = UPackageTools::SanitizePackageName( TexturePackageName );

	// First check if the asset already exists.
	UTexture* ExistingTexture = LoadObject<UTexture>( NULL, *TexturePackageName, nullptr, LOAD_Quiet | LOAD_NoWarn );

	UPackage* TexturePackage = NULL;
	FString TextureAssetName = InRequestedTextureAssetName;
	if ( !ExistingTexture )
	{
		// it doesn't exist, so create the package with as close a name as possible (or the same if there's no name collision)
		const FString Suffix( TEXT( "" ) );

		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>( "AssetTools" );
		FString FinalPackageName;

		AssetToolsModule.Get( ).CreateUniqueAssetName( TexturePackageName, Suffix, FinalPackageName, TextureAssetName );

		if ( InRequestedTextureAssetName != TextureAssetName )
		{
			NumWarnings++;
			Warn->Log( ELogVerbosity::Warning, TEXT( "Texture Asset Name Changed on import. Requested: '" ) + InRequestedTextureAssetName + TEXT( "' Actual: '" ) + TextureAssetName + "'" );
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

		UnrealTexture = (UTexture*)TextureFactory->FactoryCreateBinary(
			UTexture2D::StaticClass( ), TexturePackage, *TextureAssetName,
			RF_Standalone | RF_Public, NULL, *TextureExtension,
			DataPtr, DataPtr + TextureData.Num( ), Warn );

		if ( UnrealTexture != NULL )
		{
			// Make sure the AssetImportData points to the texture file
			UnrealTexture->AssetImportData->Update( IFileManager::Get( ).ConvertToAbsolutePathForExternalAppForRead( *TextureFullFilename ) );

			// Notify the asset registry
			FAssetRegistryModule::AssetCreated( UnrealTexture );

			// Set the dirty flag so this package will get saved later
			TexturePackage->SetDirtyFlag( true );
		}

		TextureFactory->RemoveFromRoot( );
	}

	if ( UnrealTexture == nullptr )
	{
		NumWarnings++;
		Warn->Log( ELogVerbosity::Warning, TEXT( "Texture Asset '" ) + InRequestedTextureAssetName + TEXT( "' could not be imported." ) );
	}

	return UnrealTexture;
}

UMaterial* UMD2AssetFactory::CreateMaterial( UObject* InParent,
	UTexture* InSourceTexture,
	FString& InOutMaterialAssetName,
	FFeedbackContext* Warn,
	int32& NumWarnings )
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
			NumWarnings++;
			Warn->Log( ELogVerbosity::Warning, TEXT( "Material Asset Name Changed on import. Requested: '" ) + RequstedMaterialAssetName + TEXT( "' Actual: '" ) + InOutMaterialAssetName + "'" );
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

	UMaterial* UnrealMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(
		UMaterial::StaticClass( ), MaterialPackage, *InOutMaterialAssetName, RF_Standalone | RF_Public, NULL, Warn );

	if ( UnrealMaterial != nullptr )
	{
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
			TextureExpression->MaterialExpressionEditorX = -250;

			UnrealMaterial->GetExpressionCollection( ).AddExpression( TextureExpression );
			UnrealMaterial->GetEditorOnlyData( )->BaseColor.Expression = TextureExpression;
		}

		// let the material update itself if necessary
		UnrealMaterial->PreEditChange( NULL );
		UnrealMaterial->PostEditChange( );
	}

	MaterialFactory->RemoveFromRoot( );

	if ( UnrealMaterial == nullptr )
	{
		NumWarnings++;
		Warn->Log( ELogVerbosity::Warning, TEXT( "Material Asset '" ) + InOutMaterialAssetName + TEXT( "' Could not be created." ) );
	}

	return UnrealMaterial;
}

void UMD2AssetFactory::OnMBClosedResult( uint32 Result )
{
	UE_LOG( LogTemp, Warning, TEXT( "MB Closed. Result: %d" ), Result );
}
