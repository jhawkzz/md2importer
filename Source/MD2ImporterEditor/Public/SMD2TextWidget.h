// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformCrt.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"

class MD2IMPORTEREDITOR_API SMD2TextWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2TextWidget )
		: _Text()
		{}

		SLATE_ARGUMENT( FText, Text )
	SLATE_END_ARGS( )

public:
	void Construct( const FArguments& InArgs );

	SMD2TextWidget( )
	{}

	inline TSharedPtr<STextBlock> GetTextBlock( )
	{
		return TextControl;
	}

private:
	TSharedPtr<STextBlock> TextControl;
	FText DefaultText;
};
