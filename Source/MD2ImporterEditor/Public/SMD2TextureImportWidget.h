// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// UE Includes
#include "CoreMinimal.h"
#include "HAL/PlatformCrt.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"

class SButton;
struct FGeometry;
class SMD2TextBlockWidget;
class SMD2EditableTextBoxWidget;

using FSMD2TextureImportWidgetID = int32;

DECLARE_DELEGATE_OneParam( FOnTextureWidgetRemoved, FSMD2TextureImportWidgetID )
DECLARE_DELEGATE_OneParam( FOnTextureNotFound, FSMD2TextureImportWidgetID )
DECLARE_DELEGATE_OneParam( FOnTextureSet, FSMD2TextureImportWidgetID )

class MD2IMPORTEREDITOR_API SMD2TextureImportWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2TextureImportWidget )
		: _TextureName( )
		, _DefaultTextureAssetName( )
		, _DefaultMaterialAssetName( )
		, _DefaultBrowseFilepath( )
		, _ParentMeshName( )
		, _ID( -1 )
		{}

		SLATE_ARGUMENT( FString, TextureName )
		SLATE_ARGUMENT( FString, DefaultTextureAssetName )
		SLATE_ARGUMENT( FString, DefaultMaterialAssetName )
		SLATE_ARGUMENT( FString, DefaultBrowseFilepath )
		SLATE_ARGUMENT( FString, ParentMeshName )
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

	const FString& GetTextureAssetFilename( );
	const FString& GetTextureAssetName( );
	const FString& GetMaterialAssetName( );

	int32 GetID( )
	{
		return ID;
	}

	bool TextureFileExists( )
	{
		return bTextureFileExists;
	}

private:
	void SetTextureAssetFilename( const FString& InAssetFilename );

	FReply OnBrowse( );
	FReply OnRemove( );

private:
	TSharedPtr<SMD2TextBlockWidget> TextureAssetFilenameTB;
	TSharedPtr<SMD2EditableTextBoxWidget> TextureAssetNameETB;
	TSharedPtr<SMD2EditableTextBoxWidget> MaterialAssetNameETB;
	TSharedPtr<SBorder> HeaderBorder;
	FString TextureFilename;
	FString DefaultBrowseFilepath;
	FString ParentMeshName;
	int32 ID{ -1 };
	FOnTextureWidgetRemoved OnTextureWidgetRemoved;
	FOnTextureNotFound OnTextureNotFound;
	FOnTextureSet OnTextureSet;
	bool bTextureFileExists{ false };
	FSlateColor HeaderBorderDefaultBackgroundColor;
};
