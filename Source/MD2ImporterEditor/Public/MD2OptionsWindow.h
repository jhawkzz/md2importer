// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformCrt.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"
#include "SMD2TextureImportWidget.h"

class SButton;
struct FGeometry;

//todo: Put this in its own file
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

	void OnRemoveTextureSlot( FSMD2TextureImportWidgetSlot WidgetSlot );

	SMD2OptionsWindow( )
	{}

private:
	EActiveTimerReturnType SetFocusPostConstruct( double InCurrentTime, float InDeltaTime );
	bool CanImport( ) const;
	FReply OnResetToDefaultClick( ) const;
	FText GetImportTypeDisplayText( ) const;
	void RebuildTextureListFromData( TArray<FString>& TextureList );

private:
	TSharedPtr<class IDetailsView> DetailsView;
	TWeakPtr<SWindow> WidgetWindow;
	TSharedPtr<FMD2ImportOptions> ImportOptions;
	TSharedPtr<SButton> ImportAllButton;
	TSharedPtr<SVerticalBox> TextureListBox;
	TArray<FString>		TextureList;
	FString				MD2AssetPath;
	FString				MD2FullFilepath;
};
