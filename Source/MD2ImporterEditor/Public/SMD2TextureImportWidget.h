// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformCrt.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"

class SButton;
struct FGeometry;
class SMD2TextWidget;
class SMD2EditableTextWidget;

using FSMD2TextureImportWidgetID = int32;

DECLARE_DELEGATE_OneParam( FOnTextureWidgetRemoved, FSMD2TextureImportWidgetID )
DECLARE_DELEGATE_OneParam( FOnTextureNotFound, FSMD2TextureImportWidgetID )
DECLARE_DELEGATE_OneParam( FOnTextureSet, FSMD2TextureImportWidgetID )

class MD2IMPORTEREDITOR_API SMD2TextureImportWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2TextureImportWidget )
		: _TextureName( )
		, _DefaultAssetName( )
		, _DefaultBrowseFilepath( )
		, _ID( -1 )
		{}

		SLATE_ARGUMENT( FString, TextureName )
		SLATE_ARGUMENT( FString, DefaultAssetName )
		SLATE_ARGUMENT( FString, DefaultBrowseFilepath )
		SLATE_ARGUMENT( int32, ID )
		SLATE_EVENT( FOnTextureWidgetRemoved, OnTextureWidgetRemoved )
		SLATE_EVENT( FOnTextureNotFound, OnTextureNotFound )
		SLATE_EVENT( FOnTextureSet, OnTextureSet )
	SLATE_END_ARGS( )

public:
	void Construct( const FArguments& InArgs );
	virtual bool SupportsKeyboardFocus( ) const override { return true; }

	SMD2TextureImportWidget( )
	{}

	const FString& GetAssetFilename( );
	const FString& GetAssetName( );

	int32 GetID( )
	{
		return ID;
	}

	bool TextureFileExists( )
	{
		return bTextureFileExists;
	}

private:
	void SetAssetFilename( const FString& InAssetFilename );

	FReply OnBrowse( );
	FReply OnRemove( );

private:
	TSharedPtr<SMD2TextWidget> AssetFilenameTB;
	TSharedPtr<SMD2EditableTextWidget> AssetNameTB;
	TSharedPtr<SBorder> HeaderBorder;
	FString TextureFilename;
	FString TextureAssetName;
	FString DefaultBrowseFilepath;
	int32 ID{ -1 };
	FOnTextureWidgetRemoved OnTextureWidgetRemoved;
	FOnTextureNotFound OnTextureNotFound;
	FOnTextureSet OnTextureSet;
	bool bTextureFileExists{ false };
	FSlateColor HeaderBorderDefaultBackgroundColor;
};
