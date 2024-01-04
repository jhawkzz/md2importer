// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformCrt.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"

class SButton;
struct FGeometry;

using FSMD2TextureImportWidgetID = int32;

DECLARE_DELEGATE_OneParam( FOnTextureWidgetRemoved, FSMD2TextureImportWidgetID )

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
	SLATE_END_ARGS( )

public:
	void Construct( const FArguments& InArgs );
	virtual bool SupportsKeyboardFocus( ) const override { return true; }

	SMD2TextureImportWidget( )
	{}

	const FString& GetAssetFilename( )
	{
		return AssetFilenameTB->GetText( ).ToString( );
	}

	const FString& GetAssetName( )
	{
		return AssetNameTB->GetText( ).ToString( );
	}

	int32 GetID( )
	{
		return ID;
	}

private:
	void SetAssetFilename( const FString& InAssetFilename )
	{
		AssetFilenameTB->SetText( FText::FromString( InAssetFilename ) );
		AssetFilenameTB->SetToolTipText( FText::FromString( InAssetFilename ) );
	}

	FReply OnBrowse( );
	FReply OnRemove( );

private:
	TSharedPtr<STextBlock> AssetFilenameTB;
	TSharedPtr<SEditableText> AssetNameTB;;
	FString TextureFilename;
	FString TextureAssetName;
	FString DefaultBrowseFilepath;
	int32 ID;
	FOnTextureWidgetRemoved OnTextureWidgetRemoved;
};
