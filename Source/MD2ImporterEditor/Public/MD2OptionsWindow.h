// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformCrt.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"

class SButton;
struct FGeometry;

//todo: Put this in its own file
struct MD2ImportOptions
{
	TArray<TPair<FString,FString>> TextureImportList;
	TArray<FString> MaterialNameList;
	
	bool bSingleMaterialWithBlend;
};

class MD2IMPORTEREDITOR_API SMD2OptionsWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2OptionsWindow )
		: _WidgetWindow( )
		, _FullPath( )
		, _MaxWindowHeight( 0.0f )
		, _MaxWindowWidth( 0.0f )
		, _TextureList( )
		{}

		SLATE_ARGUMENT( TSharedPtr<SWindow>, WidgetWindow )
		SLATE_ARGUMENT( FString, FullPath )
		SLATE_ARGUMENT( float, MaxWindowHeight )
		SLATE_ARGUMENT( float, MaxWindowWidth )
		SLATE_ARGUMENT( TArray<FString>, TextureList )
	SLATE_END_ARGS( )

public:
	void Construct( const FArguments& InArgs );
	virtual bool SupportsKeyboardFocus( ) const override { return true; }

	FReply OnImport( );

	FReply OnCancel( )
	{
		bShouldImport = false;
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

	bool ShouldImport( ) const
	{
		return bShouldImport;
	}

	const MD2ImportOptions* GetImportOptions( ) const
	{
		return &ImportOptions;
	}

	SMD2OptionsWindow( )
		: bShouldImport( false )
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
	TSharedPtr<SButton> ImportAllButton;
	TSharedPtr<SVerticalBox> TextureListBox;
	TArray<FString>		TextureList;
	FString				MD2Fullpath;
	bool				bShouldImport;
	MD2ImportOptions    ImportOptions;
};
