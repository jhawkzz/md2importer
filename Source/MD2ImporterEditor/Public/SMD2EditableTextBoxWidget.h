// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Simple wrapper for an editable text box that allows us to style for the look & feel of the plugin.
class MD2IMPORTEREDITOR_API SMD2EditableTextBoxWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2EditableTextBoxWidget )
		: _Text()
		{}
		SLATE_ARGUMENT( FText, Text )
	SLATE_END_ARGS( )

public:
	void Construct( const FArguments& InArgs );

	SMD2EditableTextBoxWidget( )
	{}

	inline TSharedPtr<SEditableTextBox> GetEditableTextBox( )
	{
		return EditableTextBox;
	}

private:

private:
	TSharedPtr<SEditableTextBox> EditableTextBox;
};
