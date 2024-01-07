// Copyright 2024 Jered McFerron

#pragma once

// UE Includes
#include "CoreMinimal.h"

// Simple wrapper for an editable text box that allows us to style for the look & feel of the plugin.
class MD2IMPORTEREDITOR_API SMD2EditableTextBoxWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2EditableTextBoxWidget )
		: _Text( )
		,_TextLimit(32)
		{}
		SLATE_ARGUMENT( FText, Text )
		SLATE_ARGUMENT( int32, TextLimit )
		SLATE_EVENT( FOnTextChanged, OnTextChanged )
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
	// giving a unique name to differentiate from the forwarding delegate
	void InternalOnTextChanged( const FText& SearchText );

private:
	TSharedPtr<SEditableTextBox> EditableTextBox;
	FOnTextChanged OnTextChanged;
	int32 TextLimit;
};
