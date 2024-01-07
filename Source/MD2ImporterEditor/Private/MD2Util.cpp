// Copyright 2024 Jered McFerron
#include "MD2Util.h"

// UE Includes
#include "ObjectTools.h"
#include "HAL/PlatformApplicationMisc.h"

// Project Includes

void UMD2Util::GetCenterPosForWindow( FVector2D& OutWindowPos, float InWindowWidth, float InWindowHeight )
{
	// Compute centered window position based on max window size, which include when all categories are expanded
	FVector2D WindowSize = FVector2D( InWindowWidth, InWindowHeight ); // Max window size it can get based on current slate

	FSlateRect WorkAreaRect = FSlateApplicationBase::Get( ).GetPreferredWorkArea( );
	FVector2D DisplayTopLeft( WorkAreaRect.Left, WorkAreaRect.Top );
	FVector2D DisplaySize( WorkAreaRect.Right - WorkAreaRect.Left, WorkAreaRect.Bottom - WorkAreaRect.Top );

	float ScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint( DisplayTopLeft.X, DisplayTopLeft.Y );
	WindowSize *= ScaleFactor;

	OutWindowPos = (DisplayTopLeft + (DisplaySize - WindowSize) / 2.0f) / ScaleFactor;
}

float UMD2Util::ScaleForDPI( float Value )
{
	FSlateRect WorkAreaRect = FSlateApplicationBase::Get( ).GetPreferredWorkArea( );
	FVector2D DisplayTopLeft( WorkAreaRect.Left, WorkAreaRect.Top );
	FVector2D DisplaySize( WorkAreaRect.Right - WorkAreaRect.Left, WorkAreaRect.Bottom - WorkAreaRect.Top );

	float ScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint( DisplayTopLeft.X, DisplayTopLeft.Y );

	return ScaleFactor * Value;
}

void UMD2Util::CreateDefaultMaterialAssetName( FString& OutAssetName, const FString& ParentAssetName, const FString& AssetName )
{
	OutAssetName = ObjectTools::SanitizeObjectName( TEXT( "M_" ) + ParentAssetName + TEXT( "_" ) + AssetName );
}

void UMD2Util::CreateDefaultTextureAssetName( FString& OutAssetName, const FString& ParentAssetName, const FString& AssetName )
{
	OutAssetName = ObjectTools::SanitizeObjectName( TEXT( "T_" ) + ParentAssetName + TEXT( "_" ) + AssetName + TEXT( "_D" ) );
}
