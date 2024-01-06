// Fill out your copyright notice in the Description page of Project Settings.


#include "SMD2TextWidget.h"
#include "MD2Asset.h"
#include "IDocumentation.h"
#include "Internationalization/Internationalization.h"
#include "SPrimaryButton.h"
#include "DesktopPlatformModule.h"
#include "HAL/FileManagerGeneric.h"

#define LOCTEXT_NAMESPACE "SMD2TextWidget"

void SMD2TextWidget::Construct( const FArguments& InArgs )
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
						.Padding( FMargin( 4.0f, 2.0f ) )
						.VAlign( VAlign_Center )
				]
		]
	];

	InnerBorder->SetContent( SAssignNew( TextControl, STextBlock )
		.Text( InArgs._Text )
		//.Font( FAppStyle::Get( ).GetFontStyle( "BoldFont" ) )
	);
}



#undef LOCTEXT_NAMESPACE