// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformCrt.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"

class SButton;
struct FGeometry;

class MD2IMPORTEREDITOR_API SMD2OptionsWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2OptionsWindow )
		: _WidgetWindow( )
		, _FullPath( )
		, _MaxWindowHeight( 0.0f )
		, _MaxWindowWidth( 0.0f )
		{}

		SLATE_ARGUMENT( TSharedPtr<SWindow>, WidgetWindow )
		SLATE_ARGUMENT( FText, FullPath )
		SLATE_ARGUMENT( float, MaxWindowHeight )
		SLATE_ARGUMENT( float, MaxWindowWidth )
	SLATE_END_ARGS( )

public:
	void Construct( const FArguments& InArgs );
	virtual bool SupportsKeyboardFocus( ) const override { return true; }

	FReply OnImport( )
	{
		bShouldImport = true;
		if ( WidgetWindow.IsValid( ) )
		{
			WidgetWindow.Pin( )->RequestDestroyWindow( );
		}
		return FReply::Handled( );
	}

	FReply OnImportAll( )
	{
		bShouldImportAll = true;
		return OnImport( );
	}

	FReply OnCancel( )
	{
		bShouldImport = false;
		bShouldImportAll = false;
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

	bool ShouldImportAll( ) const
	{
		return bShouldImportAll;
	}

	SMD2OptionsWindow( )
		: bShouldImport( false )
		, bShouldImportAll( false )
	{}

private:
	EActiveTimerReturnType SetFocusPostConstruct( double InCurrentTime, float InDeltaTime );
	bool CanImport( ) const;
	FReply OnResetToDefaultClick( ) const;
	FText GetImportTypeDisplayText( ) const;

private:
	TSharedPtr<class IDetailsView> DetailsView;
	TWeakPtr< SWindow > WidgetWindow;
	TSharedPtr<SButton> ImportAllButton;
	bool			bShouldImport;
	bool			bShouldImportAll;
};
