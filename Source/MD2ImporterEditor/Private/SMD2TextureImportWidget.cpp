// Fill out your copyright notice in the Description page of Project Settings.


#include "SMD2TextureImportWidget.h"
#include "MD2Asset.h"
#include "IDocumentation.h"
#include "Internationalization/Internationalization.h"
#include "SPrimaryButton.h"
#include "DesktopPlatformModule.h"
#include "HAL/FileManagerGeneric.h"
#include "SMD2TextWidget.h"
#include "SMD2EditableTextWidget.h"

#define LOCTEXT_NAMESPACE "SMD2TextureImportWidget"

void SMD2TextureImportWidget::Construct( const FArguments& InArgs )
{
	TextureFilename = InArgs._TextureName;
	DefaultBrowseFilepath = InArgs._DefaultBrowseFilepath;
	OnTextureWidgetRemoved = InArgs._OnTextureWidgetRemoved;
	OnTextureNotFound = InArgs._OnTextureNotFound;
	ID = InArgs._ID;

	this->ChildSlot
		[
			// Outerbox
			SNew( SVerticalBox )
			// Source: Filename
			+ SVerticalBox::Slot( )
			.AutoHeight( )
			.Padding( 2.0f )
			[
				SNew( SHorizontalBox )
				+ SHorizontalBox::Slot( )
				.Padding( 4.0f, 0.0f )
				.FillWidth( 1.0f )
				.VAlign( VAlign_Center )
				[
					SNew( SBorder )
					.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
					[
						SNew( SMD2TextWidget )
						.Text( FText::FromString( FString( TEXT( "Source" ) ) ) )
					]
				]
				+ SHorizontalBox::Slot( )
				.Padding( 4.0f, 0.0f )
				.FillWidth( 1.0f )
				.VAlign( VAlign_Center )
				[
					SNew( SBorder )
					.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
					[
						SAssignNew( AssetFilenameTB, SMD2TextWidget )
					]
				]
			]
			// Asset: Assetname
			+ SVerticalBox::Slot( )
			.AutoHeight( )
			.Padding( 2.0f )
			[
				SNew( SHorizontalBox )
				+ SHorizontalBox::Slot( )
				.Padding( 4.0f, 0.0f )
				.FillWidth( 1.0f )
				.VAlign( VAlign_Center )
				[
					SNew( SBorder )
					.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
					[
						SNew( SMD2TextWidget )
						.Text( FText::FromString( FString( TEXT( "Asset Name" ) ) ) )
					]
				]
				+ SHorizontalBox::Slot( )
				.Padding( 4.0f, 0.0f )
				.FillWidth( 1.0f )
				.VAlign( VAlign_Center )
				[
					SNew( SBorder )
					.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
					[
						SAssignNew( AssetNameTB, SMD2EditableTextWidget )
						.Text( FText::FromString( InArgs._DefaultAssetName ) )
					]
				]
			]
			// Buttons
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
		];

	// if the creator provided a texture filename, validate it exists
	// and populate our UI
	if ( TextureFilename.Len() > 0 )
	{
		TArray<FString> FoundFiles;
		FFileManagerGeneric FileManager;
		FileManager.FindFilesRecursive( FoundFiles, *DefaultBrowseFilepath, *TextureFilename, true, false, false );

		if ( FoundFiles.Num( ) > 0 )
		{
			// technically there could be multiple files found due to the recursive search,
			// so just use the first one.
			SetAssetFilename( FoundFiles[ 0 ] );
		}
		else
		{
			// since a default texture was provided, notify our parent that we couldn't find it
			OnTextureNotFound.ExecuteIfBound( ID );

			//WidgetBorder->SetBorderBackgroundColor( FLinearColor( FColor::Red ) );
		}
	}
}

FReply SMD2TextureImportWidget::OnBrowse( )
{
	TArray<FString> OpenFilenames;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get( );
	bool bOpened = false;

	const FString FileTypes = TEXT( "MD2 Skin Texture (*.pcx)|*.pcx" );
	if ( DesktopPlatform )
	{
		bOpened = DesktopPlatform->OpenFileDialog(
			FSlateApplication::Get( ).FindBestParentWindowHandleForDialogs( nullptr ),
			LOCTEXT( "SMD2TextureImportWidget_PCXTexture", "Load PCX Texture" ).ToString( ),
			DefaultBrowseFilepath,
			TEXT( "" ),
			FileTypes,
			EFileDialogFlags::None,
			OpenFilenames
		);

		// update the filename based on what was selected
		if ( bOpened && OpenFilenames.Num( ) > 0 )
		{
			this->SetAssetFilename( OpenFilenames[ 0 ] );
		}
	}

	return FReply::Handled( );
}

FReply SMD2TextureImportWidget::OnRemove( )
{
	OnTextureWidgetRemoved.ExecuteIfBound( ID );

	return FReply::Handled( );
}

const FString& SMD2TextureImportWidget::GetAssetFilename( )
{
	return AssetFilenameTB->GetTextBlock( )->GetText( ).ToString( );
}

const FString& SMD2TextureImportWidget::GetAssetName( )
{
	return AssetNameTB->GetTextBlock( )->GetText( ).ToString( );
}

void SMD2TextureImportWidget::SetAssetFilename( const FString& InAssetFilename )
{
	AssetFilenameTB->GetTextBlock( )->SetText( FText::FromString( InAssetFilename ) );
	AssetFilenameTB->GetTextBlock( )->SetToolTipText( FText::FromString( InAssetFilename ) );
}
#undef LOCTEXT_NAMESPACE