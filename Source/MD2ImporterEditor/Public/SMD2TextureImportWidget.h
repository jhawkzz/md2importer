// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformCrt.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"

class SButton;
struct FGeometry;

class MD2IMPORTEREDITOR_API SMD2TextureImportWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2TextureImportWidget )
		: _WidgetWindow( )
		, _TextureName( )
		{}

		SLATE_ARGUMENT( TSharedPtr<SWindow>, WidgetWindow )
		SLATE_ARGUMENT( FString, TextureName )
	SLATE_END_ARGS( )

public:
	void Construct( const FArguments& InArgs );
	virtual bool SupportsKeyboardFocus( ) const override { return true; }

	SMD2TextureImportWidget( )
	{}

private:
	FReply OnBrowse( );
	FReply OnRemove( );

private:
	TWeakPtr<SWindow> WidgetWindow;
	FString TextureFilename;
	FString TextureAssetName;
};
