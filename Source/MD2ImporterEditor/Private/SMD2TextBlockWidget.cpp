// Fill out your copyright notice in the Description page of Project Settings.
#include "SMD2TextBlockWidget.h"

#define LOCTEXT_NAMESPACE "SMD2TextBlockWidget"

void SMD2TextBlockWidget::Construct( const FArguments& InArgs )
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
				SNew( SBorder )
				.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
				.Padding( 8, 3, 8, 4 ) //stole from the default style for editable text. Looks good, but hardcoding in case UE changes
				[
					SAssignNew( TextBlock, STextBlock )
					.Text( InArgs._Text )
				]
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE