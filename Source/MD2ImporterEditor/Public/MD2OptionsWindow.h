// Copyright 2024 Jered McFerron

#pragma once

// UE Includes
#include "CoreMinimal.h"
#include "InputCoreTypes.h"

// Project Includes
#include "SMD2TextureImportWidget.h"

class SButton;
struct FGeometry;

struct FMD2ImportOptions
{
	FMD2ImportOptions( )
		: bShouldImport( false )
		, bSingleMaterialWithBlend( false )
	{}

	TArray<TPair<FString, FString>> TextureImportList;
	TArray<FString> MaterialNameList;

	bool bShouldImport;
	bool bSingleMaterialWithBlend;
};

class MD2IMPORTEREDITOR_API SMD2OptionsWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2OptionsWindow )
		: _WidgetWindow( )
		, _MD2AssetPath( )
		, _MD2FullFilepath( )
		, _MaxWindowHeight( 0.0f )
		, _MaxWindowWidth( 0.0f )
		, _TextureList( )
		, _ImportOptions( )
		{}

		SLATE_ARGUMENT( TSharedPtr<SWindow>, WidgetWindow )
		SLATE_ARGUMENT( FString, MD2AssetPath )
		SLATE_ARGUMENT( FString, MD2FullFilepath )
		SLATE_ARGUMENT( float, MaxWindowHeight )
		SLATE_ARGUMENT( float, MaxWindowWidth )
		SLATE_ARGUMENT( TArray<FString>, TextureList )
		SLATE_ARGUMENT( TSharedPtr<FMD2ImportOptions>, ImportOptions )
	SLATE_END_ARGS( )

public:
	static void ShowImportOptionsWindow( const FString& MD2AssetPath, const FString& MD2FullFilepath, TArray<FString>& TextureNames, TSharedPtr<FMD2ImportOptions> OutImportOptions );

	void Construct( const FArguments& InArgs );
	virtual bool SupportsKeyboardFocus( ) const override { return true; }

	FReply OnImport( );

	FReply OnCancel( )
	{
		ImportOptions->bShouldImport = false;
		if ( WidgetWindow.IsValid( ) )
		{
			WidgetWindow.Pin( )->RequestDestroyWindow( );
		}
		return FReply::Handled( );
	}

	virtual FReply OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent ) override
	{
		if ( InKeyEvent.GetKey( ) == EKeys::Escape )
		{
			return OnCancel( );
		}

		return FReply::Unhandled( );
	}

	SMD2OptionsWindow( )
	{}

private:
	EActiveTimerReturnType SetFocusPostConstruct( double InCurrentTime, float InDeltaTime );
	bool CanImport( ) const;
	FReply OnResetToDefaultClick( );
	void BuildTextureListFromData( TArray<FString>& TextureList );
	TSharedPtr<SMD2TextureImportWidget> GetTextureWidgetFromID( int32 ID );
	FReply OnAddTextureWidget( );
	void ToggleImportEnabled( bool bEnabled );
	void OnTextureWidgetErrorStateChanged( FSMD2TextureImportWidgetID WidgetID, bool bErrorState );
	void OnRemoveTextureWidget( FSMD2TextureImportWidgetID WidgetID );
	void TryEnableImport( );
	void AddTextureSlot( int32 ID, const FString& InTextureName, const FString& InDefaultTextureAssetName, const FString& InDefaultMaterialAssetName, const FString& InStartingMD2FullFilepath );

private:
	TWeakPtr<SWindow> WidgetWindow;
	TSharedPtr<FMD2ImportOptions> ImportOptions;
	TSharedPtr<SButton> ImportButton;
	TSharedPtr<SVerticalBox> TextureListBox;

	// used for initial setup and restoring defaults
	TArray<FString>		StartingTextureList;
	FString				StartingMD2AssetPath;
	FString				StartingMD2FullFilepath;
};
