// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Simple wrapper for a text block that allows us to style for the look & feel of the plugin.

class MD2IMPORTEREDITOR_API SMD2TextBlockWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2TextBlockWidget )
		: _Text()
		{}

		SLATE_ARGUMENT( FText, Text )
	SLATE_END_ARGS( )

public:
	void Construct( const FArguments& InArgs );

	SMD2TextBlockWidget( )
	{}

	inline TSharedPtr<STextBlock> GetTextBlock( )
	{
		return TextBlock;
	}

private:
	TSharedPtr<STextBlock> TextBlock;
};
