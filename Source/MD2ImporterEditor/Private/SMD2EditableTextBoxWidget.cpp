// Copyright 2024 Jered McFerron
#include "SMD2EditableTextBoxWidget.h"

#define LOCTEXT_NAMESPACE "SMD2EditableTextBoxWidget"

void SMD2EditableTextBoxWidget::Construct( const FArguments& InArgs )
{
	TextLimit = InArgs._TextLimit;
	OnTextChanged = InArgs._OnTextChanged;

	this->ChildSlot
		[
			SNew( SBorder )
				.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
				[
					SNew( SHorizontalBox )
						+ SHorizontalBox::Slot( )
						.Padding( FMargin( 0, 1, 0, 1 ) )
						.VAlign( VAlign_Center )
						.FillWidth( 1 )
						[
							SAssignNew( EditableTextBox, SEditableTextBox )
								.OnTextChanged( this, &SMD2EditableTextBoxWidget::InternalOnTextChanged )
								.Padding( FMargin( 8, 3, 8, 4 ) ) //stole from the default style for editable text. Looks good, but hardcoding in case UE changes
								.Text( InArgs._Text )
						]
				]
		];

	EditableTextBox->SetBorderBackgroundColor( FLinearColor( FColor::Black ) );
}

void SMD2EditableTextBoxWidget::InternalOnTextChanged( const FText& Text )
{
	const FString& CurrText = Text.ToString( );
	if ( CurrText.Len( ) > TextLimit )
	{
		EditableTextBox->SetText( FText::FromString( CurrText.LeftChop( 1 ) ) );
	}

	OnTextChanged.ExecuteIfBound( Text );
}

#undef LOCTEXT_NAMESPACE