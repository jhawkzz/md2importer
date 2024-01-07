// Copyright 2024 Jered McFerron

#pragma once

// UE Includes
#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MD2AssetFactory.generated.h"

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
	UStaticMesh* ImportMD2Asset( UObject* InParent, UMD2Asset* MD2Asset, const FString& MD2FullFilename, FString& InOutStaticMeshAssetName, FFeedbackContext* Warn, int32& NumWarnings );
	UTexture* ImportTexture( UObject* InParent, const FString& TextureFullFilename, const FString& InRequestedTextureAssetName, const FString& TextureExtension, FFeedbackContext* Warn, int32& NumWarnings );
	UMaterial* CreateMaterial( UObject* InParent, UTexture* InSourceTexture, FString& InOutMaterialAssetName, FFeedbackContext* Warn, int32& NumWarnings );
	void OnMBClosedResult( uint32 Result );
};
