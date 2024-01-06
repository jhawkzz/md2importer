// Fill out your copyright notice in the Description page of Project Settings.


#include "SMD2EditableTextWidget.h"

#define LOCTEXT_NAMESPACE "SMD2EditableTextWidget"

void SMD2EditableTextWidget::Construct( const FArguments& InArgs )
{
	TSharedPtr<SBorder> InnerBorder;

	this->ChildSlot
	[
		SNew( SBorder )
		.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
		[
			SNew( SHorizontalBox )
				+ SHorizontalBox::Slot( )
				.Padding( FMargin( 0, 1, 0, 1 ) )
				.FillWidth( 1 )
				[
					SAssignNew( InnerBorder, SBorder )
						.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
						.Padding( FMargin( 0.0f, 2.0f ) )
						.VAlign( VAlign_Center )
				]
		]
	];

	InnerBorder->SetContent( SAssignNew( EditableTextControl, SEditableText )
		.Text( InArgs._Text )
		//.Font( FAppStyle::Get( ).GetFontStyle( "BoldFont" ) )
	);
}



#undef LOCTEXT_NAMESPACE