// Copyright 2024 Jered McFerron

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
DECLARE_DELEGATE_TwoParams( FOnErrorStateChanged, FSMD2TextureImportWidgetID, bool /*ErrorState*/ )

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
		SLATE_EVENT( FOnErrorStateChanged, OnErrorStateChanged )
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

	bool InErrorState( )
	{
		return bErrorState;
	}

private:
	void SetTextureImportAndDisplayAssetFilenames( const FString& InAssetFullFilename );

	FReply OnBrowse( );
	FReply OnRemove( );
	void OnAssetNameTextChanged( const FText& Text );
	void SetErrorState( bool bInErrorState );

private:
	TSharedPtr<SMD2TextBlockWidget> TextureAssetFilenameTB;
	TSharedPtr<SMD2EditableTextBoxWidget> TextureAssetNameETB;
	TSharedPtr<SMD2EditableTextBoxWidget> MaterialAssetNameETB;
	TSharedPtr<SBorder> HeaderBorder;
	FString DefaultBrowseFilepath;
	FString ParentMeshName;
	int32 ID{ -1 };
	FOnTextureWidgetRemoved OnTextureWidgetRemoved;
	FOnErrorStateChanged OnErrorStateChanged;
	bool bErrorState{ false };
	FSlateColor HeaderBorderDefaultBackgroundColor;

	//we differentiate display and import so the user doesn't see a textfield full of "../../../Users/" and nothing useful
	FString TextureFilenameForDisplay;
	FString TextureFilenameForImport;
};
