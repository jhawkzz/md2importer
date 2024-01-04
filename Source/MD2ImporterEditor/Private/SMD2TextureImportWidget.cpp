// Fill out your copyright notice in the Description page of Project Settings.


#include "SMD2TextureImportWidget.h"
#include "MD2Asset.h"
#include "IDocumentation.h"
#include "Internationalization/Internationalization.h"
#include "SPrimaryButton.h"
#include "DesktopPlatformModule.h"
#include "HAL/FileManagerGeneric.h"

#define LOCTEXT_NAMESPACE "MD2TextureImportWidget"

void SMD2TextureImportWidget::Construct( const FArguments& InArgs )
{
	TextureFilename = InArgs._TextureName;
	DefaultBrowseFilepath = InArgs._DefaultBrowseFilepath;
	OnTextureWidgetRemoved = InArgs._OnTextureWidgetRemoved;
	ID = InArgs._ID;

	this->ChildSlot
		[
			SNew( SBorder )
				.Padding( FMargin( 3 ) )
				[
					SNew( SVerticalBox )
						+ SVerticalBox::Slot( )
						.AutoHeight( )
						.Padding( 2.0f )
						[
							SNew( SVerticalBox )
								+ SVerticalBox::Slot( )
								.AutoHeight( )
								[
									SNew( SHorizontalBox )
										+ SHorizontalBox::Slot( )
										.AutoWidth( )
										[
											SNew( STextBlock )
												.Text( LOCTEXT( "SMD2TextureImportWidget_SourceTexture_TT", "Source: " ) )
										]
										+ SHorizontalBox::Slot( )
										.Padding( 5, 0, 0, 0 )
										.AutoWidth( )
										.VAlign( VAlign_Center )
										[
											SAssignNew( AssetFilenameTB, STextBlock )
										]
								]
								+ SVerticalBox::Slot( )
								.Padding( 5, 0, 0, 0 )
								.AutoHeight( )
								.VAlign( VAlign_Center )
								[
									SNew( SHorizontalBox )
										+ SHorizontalBox::Slot( )
										.AutoWidth( )
										[
											SNew( STextBlock )
												.Text( LOCTEXT( "SMD2TextureImportWidget_AssetName_TT", "Asset Name: " ) )
										]
										+ SHorizontalBox::Slot( )
										.Padding( 5, 0, 0, 0 )
										.AutoWidth( )
										.VAlign( VAlign_Center )
										[
											SAssignNew( AssetNameTB, SEditableText )
												.Text( FText::FromString( InArgs._DefaultAssetName ) )
										]
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

	// if the file exists, put it in the asset filename tb. Otherwise,
	// leave it blank and alert that they need to resolve a missing asset.
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
		// todo: flag error, and dont allow the parent window to import
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

#undef LOCTEXT_NAMESPACE