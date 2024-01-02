// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MD2AssetFactory.generated.h"

struct FMD2ImportOptions;
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
	UTexture* ImportTexture( UObject* InParent, const FString& TextureFullFilename, const FString& InRequestedTextureAssetName, const FString& TextureExtension, TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects );
	UMaterial* CreateMaterial( UObject* InParent, UTexture* InSourceTexture, FString& InOutMaterialAssetName, TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects );
	UStaticMesh* ImportMD2Asset( UObject* InParent, UMD2Asset* MD2Asset, const FString& MD2FullFilename, FString& InOutStaticMeshAssetName, TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects );

	void GetImportOptions( const FString& MD2AssetPath, const FString& MD2FullFilepath, TArray<FString>& TextureNames, TSharedPtr<FMD2ImportOptions> OutImportOptions );
	float ScaleForDPI( float Value );

	void TestCreateRawMesh( struct FRawMesh& OutRawMesh );
	void TestAttachToNewActor( const class UStaticMesh& StaticMesh );
};
