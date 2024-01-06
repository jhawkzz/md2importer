// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UMD2Util
{
public:
	static void GetCenterPosForWindow( FVector2D& OutWindowPos, float InWindowWidth, float InWindowHeight );
	static float ScaleForDPI( float Value );
	static void CreateDefaultMaterialAssetName( FString& OutAssetName, const FString& ParentAssetName, const FString& AssetName );
	static void CreateDefaultTextureAssetName( FString& OutAssetName, const FString& ParentAssetName, const FString& AssetName );
};
