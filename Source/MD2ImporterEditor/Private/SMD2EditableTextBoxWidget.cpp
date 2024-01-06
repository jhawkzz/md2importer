// Fill out your copyright notice in the Description page of Project Settings.
#include "SMD2EditableTextBoxWidget.h"

#define LOCTEXT_NAMESPACE "SMD2EditableTextBoxWidget"

void SMD2EditableTextBoxWidget::Construct( const FArguments& InArgs )
{
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
				.Padding( FMargin(8, 3, 8, 4) ) //stole from the default style for editable text. Looks good, but hardcoding in case UE changes
				.Text( InArgs._Text )
			]
		]
	];

	EditableTextBox->SetBorderBackgroundColor( FLinearColor( FColor::Black ) );
}

#undef LOCTEXT_NAMESPACE