// Fill out your copyright notice in the Description page of Project Settings.


#include "SMD2TextureImportWidget.h"
#include "MD2Asset.h"
#include "IDocumentation.h"
#include "Internationalization/Internationalization.h"
#include "SPrimaryButton.h"

#define LOCTEXT_NAMESPACE "MD2TextureImportWidget"

void SMD2TextureImportWidget::Construct( const FArguments& InArgs )
{
	WidgetWindow = InArgs._WidgetWindow;
	TextureFilename = InArgs._TextureName;
	TextureAssetName = InArgs._TextureName;
	//todo: validate its existence, and setup state based on that

	this->ChildSlot
		[
			SNew( SBorder )
				.Padding( FMargin( 3 ) )
				.BorderImage( FAppStyle::GetBrush( "ToolPanel.GroupBorder" ) )
				[
					SNew( SVerticalBox )
						+ SVerticalBox::Slot( )
						.AutoHeight( )
						.Padding( 2.0f )
						[
							SNew( SHorizontalBox )
								+ SHorizontalBox::Slot( )
								.AutoWidth( )
								[
									SNew( STextBlock )
										.Text( FText::FromString( TextureAssetName ) )
								]
								+ SHorizontalBox::Slot( )
								.Padding( 5, 0, 0, 0 )
								.AutoWidth( )
								.VAlign( VAlign_Center )
								[
									SNew( STextBlock )
										.Text( FText::FromString( TextureAssetName ) )
								]
						]
						+ SVerticalBox::Slot( )
						.AutoHeight( )
						.Padding( 2.0f )
						[
							SNew( SUniformGridPanel )
								.SlotPadding( 2.0f )
								+ SUniformGridPanel::Slot( 1, 0 )
								[
									SNew( SButton )
										.Text( LOCTEXT( "SMD2TextureImportWidget_Browse", "Browse..." ) )
										.ToolTipText( LOCTEXT( "SMD2TextureImportWidget_Browse_TT", "Browse for texture" ) )
										.OnClicked( this, &SMD2TextureImportWidget::OnBrowse )
								]
								+ SUniformGridPanel::Slot( 2, 0 )
								[
									SNew( SButton )
										.HAlign( HAlign_Center )
										.Text( LOCTEXT( "SMD2TextureImportWidget_Remove", "Remove" ) )
										.ToolTipText( LOCTEXT( "SMD2TextureImportWidget_Remove+TT", "Remove this texture from import" ) )
										.OnClicked( this, &SMD2TextureImportWidget::OnRemove )
								]
						]
				]
		];

}

FReply SMD2TextureImportWidget::OnBrowse( )
{
	return FReply::Handled( );
}

FReply SMD2TextureImportWidget::OnRemove( )
{
	return FReply::Handled( );
}

#undef LOCTEXT_NAMESPACE