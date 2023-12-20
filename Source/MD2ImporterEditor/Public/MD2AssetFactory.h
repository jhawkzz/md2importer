// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MD2AssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class MD2IMPORTEREDITOR_API UMD2AssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UMD2AssetFactory();

	virtual UObject* FactoryCreateFile(UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		const FString& Filename,
		const TCHAR* Parms,
		FFeedbackContext* Warn,
		bool& bOutOperationCanceled) override;

private:
	void CreateRawMesh(struct FRawMesh& OutRawMesh);
	UTexture* ImportPCXTexture(UObject* InParent, const FString& Filename, TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects );
	UStaticMesh* ImportMD2Asset(UObject* InParent, const FString& Filename, TArray<TWeakObjectPtr<UObject>>& OutCreatedObjects );
	void AttachToNewActor(const class UStaticMesh& StaticMesh);
};
