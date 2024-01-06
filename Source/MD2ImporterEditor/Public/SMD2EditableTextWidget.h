// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class MD2IMPORTEREDITOR_API SMD2EditableTextWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2EditableTextWidget )
		: _Text()
		{}
		SLATE_ARGUMENT( FText, Text )
	SLATE_END_ARGS( )

public:
	void Construct( const FArguments& InArgs );

	SMD2EditableTextWidget( )
	{}

	inline TSharedPtr<SEditableText> GetTextBlock( )
	{
		return EditableTextControl;
	}

private:

private:
	TSharedPtr<SEditableText> EditableTextControl;
};
