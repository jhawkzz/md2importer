// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MD2AssetFactory.generated.h"

/**
 *
 */
UCLASS( )
class MD2IMPORTEREDITOR_API UMD2AssetFactory : public UFactory
{
	GENERATED_BODY( )

public:
	UMD2AssetFactory( );

	virtual UObject* FactoryCreateFile( UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		const FString& Filename,
		const TCHAR* Parms,
		FFeedbackContext* Warn,
		bool& bOutOperationCanceled ) override;

private:
	UTexture* ImportTexture( UObject* InParent, const FString& TextureFullFilename, FString& InOutTextureAssetName, const FString& TextureExtension, TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects );
	UMaterial* CreateMaterial( UObject* InParent, UTexture* InSourceTexture, FString& InOutMaterialAssetName, TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects );
	UStaticMesh* ImportMD2Asset( UObject* InParent, UMD2Asset* MD2Asset, const FString& MD2FullFilename, FString& InOutStaticMeshAssetName, TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects );

	bool GetImportOptions( const FString& FullPath );
	void FindPCXFiles( const FString& SearchFileBasePath, TArray<FString>& InTextureNames, TArray<FString>& OutPCXFiles );
	void BuildSkinAssetNames( const TArray<FString>& PCXFiles, const FString& ParentMeshName, TArray<struct FMD2SkinImportData>& OutSkinImports );
	float ScaleForDPI(float Value);

	void TestCreateRawMesh( struct FRawMesh& OutRawMesh );
	void TestAttachToNewActor( const class UStaticMesh& StaticMesh );
};
