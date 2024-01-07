// Fill out your copyright notice in the Description page of Project Settings.
#include "SMD2TextureImportWidget.h"

// UE Includes
#include "SPrimaryButton.h"
#include "IDocumentation.h"
#include "DesktopPlatformModule.h"
#include "HAL/FileManagerGeneric.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Internationalization/Internationalization.h"

// Project Includes
#include "SMD2EditableTextBoxWidget.h"
#include "SMD2TextBlockWidget.h"
#include "MD2Util.h"
#include "MD2Asset.h"

#define LOCTEXT_NAMESPACE "SMD2TextureImportWidget"

void SMD2TextureImportWidget::Construct( const FArguments& InArgs )
{
	TextureFilename = InArgs._TextureName;
	DefaultBrowseFilepath = InArgs._DefaultBrowseFilepath;
	ParentMeshName = InArgs._ParentMeshName;
	OnTextureWidgetRemoved = InArgs._OnTextureWidgetRemoved;
	OnTextureNotFound = InArgs._OnTextureNotFound;
	OnTextureSet = InArgs._OnTextureSet;
	ID = InArgs._ID;

	this->ChildSlot
		[
			// Outerbox
			SNew( SVerticalBox )
				// Header
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				.Padding( 2.0f, 0.0f )
				[
					SNew( SHorizontalBox )
						+ SHorizontalBox::Slot( )
						.Padding( 4.0f, 0.0f )
						.FillWidth( 1.0f )
						.VAlign( VAlign_Center )
						[
							SAssignNew( HeaderBorder, SBorder )
								.Padding( 4.0f, 2.0f )
								.BorderImage( FAppStyle::GetBrush( "Menu.Background" ) )
								.ForegroundColor( FCoreStyle::Get( ).GetSlateColor( "DefaultForeground" ) )
								[
									SNew( STextBlock )
										.Text( FText::FromString( FString( "Texture" ) ) )
										.Font( FAppStyle::Get( ).GetFontStyle( "BoldFont" ) )
								]
						]
				]
				// Source: Filename
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				.Padding( 2.0f, 0.0f, 2.0f, 0.0f )
				[
					SNew( SHorizontalBox )
						+ SHorizontalBox::Slot( )
						.Padding( 4.0f, 0.0f, 1.0f, 0.0f )
						.FillWidth( 1.0f )
						.VAlign( VAlign_Center )
						[
							SNew( SBorder )
								.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
								[
									SNew( SMD2TextBlockWidget )
										.Text( FText::FromString( FString( TEXT( "Source" ) ) ) )
								]
						]
						+ SHorizontalBox::Slot( )
						.Padding( 1.0f, 0.0f, 4.0f, 0.0f )
						.FillWidth( 1.0f )
						.VAlign( VAlign_Center )
						[
							SNew( SBorder )
								.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
								[
									SAssignNew( TextureAssetFilenameTB, SMD2TextBlockWidget )
								]
						]
				]
				// Texture Asset: Assetname
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				.Padding( 2.0f, 2.0f, 2.0f, 0.0f )
				[
					SNew( SHorizontalBox )
						+ SHorizontalBox::Slot( )
						.Padding( 4.0f, 0.0f, 1.0f, 0.0f )
						.FillWidth( 1.0f )
						.VAlign( VAlign_Center )
						[
							SNew( SBorder )
								.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
								[
									SNew( SMD2TextBlockWidget )
										.Text( FText::FromString( FString( TEXT( "Texture Asset Name" ) ) ) )
								]
						]
						+ SHorizontalBox::Slot( )
						.Padding( 1.0f, 0.0f, 4.0f, 0.0f )
						.FillWidth( 1.0f )
						.VAlign( VAlign_Center )
						[
							SNew( SBorder )
								.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
								[
									SAssignNew( TextureAssetNameETB, SMD2EditableTextBoxWidget )
										.Text( FText::FromString( InArgs._DefaultTextureAssetName ) )
								]
						]
				]
				// Material Asset: Assetname
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				.Padding( 2.0f )
				[
					SNew( SHorizontalBox )
						+ SHorizontalBox::Slot( )
						.Padding( 4.0f, 0.0f, 1.0f, 0.0f )
						.FillWidth( 1.0f )
						.VAlign( VAlign_Center )
						[
							SNew( SBorder )
								.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
								[
									SNew( SMD2TextBlockWidget )
										.Text( FText::FromString( FString( TEXT( "Material Asset Name" ) ) ) )
								]
						]
						+ SHorizontalBox::Slot( )
						.Padding( 1.0f, 0.0f, 4.0f, 0.0f )
						.FillWidth( 1.0f )
						.VAlign( VAlign_Center )
						[
							SNew( SBorder )
								.BorderImage( FAppStyle::Get( ).GetBrush( "Brushes.Panel" ) )
								[
									SAssignNew( MaterialAssetNameETB, SMD2EditableTextBoxWidget )
										.Text( FText::FromString( InArgs._DefaultMaterialAssetName ) )
								]
						]
				]
				// Buttons
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				.Padding( 1.0f, 0.0f, 1.0f, 10.0f )
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

	// store this in the event we change it to show an error and need to later restore it
	HeaderBorderDefaultBackgroundColor = HeaderBorder->GetBorderBackgroundColor( );

	// if the creator provided a texture filename, validate it exists
	// and populate our UI
	if ( TextureFilename.Len( ) > 0 )
	{
		TArray<FString> FoundFiles;
		FFileManagerGeneric FileManager;
		FileManager.FindFilesRecursive( FoundFiles, *DefaultBrowseFilepath, *TextureFilename, true, false, false );

		if ( FoundFiles.Num( ) > 0 )
		{
			// technically there could be multiple files found due to the recursive search,
			// so just use the first one.
			SetTextureAssetFilename( FoundFiles[ 0 ] );
			bTextureFileExists = true;
		}
		else
		{
			// since a default texture was provided, notify our parent that we couldn't find it
			OnTextureNotFound.ExecuteIfBound( ID );

			HeaderBorder->SetBorderBackgroundColor( FLinearColor( FColor::Red ) );
			bTextureFileExists = false;
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
			this->SetTextureAssetFilename( OpenFilenames[ 0 ] );

			FString Filename = FPaths::GetBaseFilename( OpenFilenames[ 0 ] );

			// if there's no asset name set, set a suggested one
			if ( TextureAssetNameETB->GetEditableTextBox( )->GetText( ).ToString( ).Len( ) == 0 )
			{
				FString TextureAssetName;
				UMD2Util::CreateDefaultTextureAssetName( TextureAssetName, ParentMeshName, Filename );
				TextureAssetNameETB->GetEditableTextBox( )->SetText( FText::FromString( TextureAssetName ) );
			}

			if ( MaterialAssetNameETB->GetEditableTextBox( )->GetText( ).ToString( ).Len( ) == 0 )
			{
				FString MaterialAssetName;
				UMD2Util::CreateDefaultMaterialAssetName( MaterialAssetName, ParentMeshName, Filename );
				MaterialAssetNameETB->GetEditableTextBox( )->SetText( FText::FromString( MaterialAssetName ) );
			}

			// restore the border background to a non error state and notify anyone that cares
			bTextureFileExists = true;
			HeaderBorder->SetBorderBackgroundColor( HeaderBorderDefaultBackgroundColor );
			OnTextureSet.ExecuteIfBound( ID );
		}
	}

	return FReply::Handled( );
}

FReply SMD2TextureImportWidget::OnRemove( )
{
	OnTextureWidgetRemoved.ExecuteIfBound( ID );

	return FReply::Handled( );
}

const FString& SMD2TextureImportWidget::GetTextureAssetFilename( )
{
	return TextureAssetFilenameTB->GetTextBlock( )->GetText( ).ToString( );
}

const FString& SMD2TextureImportWidget::GetTextureAssetName( )
{
	return TextureAssetNameETB->GetEditableTextBox( )->GetText( ).ToString( );
}

const FString& SMD2TextureImportWidget::GetMaterialAssetName( )
{
	return MaterialAssetNameETB->GetEditableTextBox( )->GetText( ).ToString( );
}

void SMD2TextureImportWidget::SetTextureAssetFilename( const FString& InAssetFilename )
{
	TextureAssetFilenameTB->GetTextBlock( )->SetText( FText::FromString( InAssetFilename ) );
	TextureAssetFilenameTB->GetTextBlock( )->SetToolTipText( FText::FromString( InAssetFilename ) );
}
#undef LOCTEXT_NAMESPACE