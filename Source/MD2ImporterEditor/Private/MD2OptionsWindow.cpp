// Fill out your copyright notice in the Description page of Project Settings.


#include "MD2OptionsWindow.h"
#include "MD2Asset.h"
#include "IDocumentation.h"
#include "Internationalization/Internationalization.h"
#include "SPrimaryButton.h"
#include "ObjectTools.h"
#include "Templates/SharedPointer.h"

#define LOCTEXT_NAMESPACE "MD2Option"

void SMD2OptionsWindow::Construct( const FArguments& InArgs )
{
	WidgetWindow = InArgs._WidgetWindow;
	StartingTextureList = InArgs._TextureList;
	StartingMD2AssetPath = InArgs._MD2AssetPath;
	StartingMD2FullFilepath = InArgs._MD2FullFilepath;
	ImportOptions = InArgs._ImportOptions;

	this->ChildSlot
		[
			SNew( SBox )
			.MaxDesiredHeight( InArgs._MaxWindowHeight )
			.MaxDesiredWidth( InArgs._MaxWindowWidth )
			[
				SNew( SVerticalBox )
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				.Padding( 2.0f )
				[
					SNew( SBorder )
					.Padding( FMargin( 3 ) )
					.BorderImage( FAppStyle::GetBrush( "ToolPanel.GroupBorder" ) )
					[
						SNew( SHorizontalBox )
							+ SHorizontalBox::Slot( )
							.VAlign( VAlign_Center )
							.AutoWidth( )
							[
								SNew( STextBlock )
									.Text( FText::FromString( TEXT( "Import Static Mesh" ) ) )
							]
							+ SHorizontalBox::Slot( )
							.VAlign( VAlign_Center )
							.AutoWidth( )
							[
								IDocumentation::Get( )->CreateAnchor( UMD2Asset::REFERENCE_URL )
							]
							+ SHorizontalBox::Slot( )
							.HAlign( HAlign_Right )
							[
								SNew( SHorizontalBox )
									+ SHorizontalBox::Slot( )
									.AutoWidth( )
									.Padding( FMargin( 2.0f, 0.0f ) )
									[
										SNew( SButton )
											.Text( LOCTEXT( "SMD2OptionsWindow_ResetOptions", "Reset to Default" ) )
											.OnClicked( this, &SMD2OptionsWindow::OnResetToDefaultClick )
									]
							]
					]
				]
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				.Padding( 2.0f )
				[
					SNew( SBorder )
					.Padding( FMargin( 3 ) )
					.BorderImage( FAppStyle::GetBrush( "ToolPanel.GroupBorder" ) )
					[
						SNew( SHorizontalBox )
						+ SHorizontalBox::Slot( )
						.AutoWidth( )
						[
							SNew( STextBlock )
								.Text( LOCTEXT( "SMD2OptionsWindow_CurrentAsset", "Current Asset: " ) )
						]
						+ SHorizontalBox::Slot( )
						.Padding( 5, 0, 0, 0 )
						.AutoWidth( )
						.VAlign( VAlign_Center )
						[
							SNew( STextBlock )
								.Text( FText::FromString( StartingMD2AssetPath ) )
								.ToolTipText( FText::FromString( StartingMD2AssetPath ) )
						]
					]
				]
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				.Padding( 2.0f, 0.0f, 2.0f, 0.0f )
				[
					SNew( SHorizontalBox )
					+ SHorizontalBox::Slot( )
					.Padding( 6.0f, 3.0f )
					.FillWidth( 1.0f )
					.VAlign( VAlign_Center )
					[
						SNew( SBorder )
						.Padding( 4.0f, 10.0f )
						.BorderImage( FAppStyle::GetBrush( "Menu.Background" ) )
						.ForegroundColor( FCoreStyle::Get( ).GetSlateColor( "DefaultForeground" ) )
						[
							SNew( STextBlock )

							.Text( FText::FromString( FString("Textures to Import") ) )
							.Font( FAppStyle::Get( ).GetFontStyle( "BoldFont" ) )
						]
					]
				]
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				.Padding( 2.0f, 0.0f, 2.0f, 0.0f )
				[
					SNew( SBox )
					.HAlign( HAlign_Left )
					.VAlign( VAlign_Top )
					.WidthOverride( InArgs._MaxWindowWidth )
					.HeightOverride( 500 )
					[
						SNew( SScrollBox )
						+ SScrollBox::Slot( )
						[
							SAssignNew( TextureListBox, SVerticalBox )
						]
					]
				]
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				.Padding( 2.0f, 10.0f, 2.0f, 2.0f )
				[
					SNew( SButton )
					.HAlign( HAlign_Right )
					.Text( LOCTEXT( "SMD2OptionsWindow_AddTexture", "Add Texture" ) )
					.OnClicked( this, &SMD2OptionsWindow::OnAddTextureWidget )
				]
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				.Padding( 2.0f )
				[
					SNew( SUniformGridPanel )
					.SlotPadding( 2.0f )
					+ SUniformGridPanel::Slot( 1, 0 )
					[
						SAssignNew( ImportButton, SPrimaryButton )
							.Text( LOCTEXT( "SMD2OptionsWindow_Import", "Import" ) )
							.ToolTipText( LOCTEXT( "SMD2OptionsWindow_Import_ToolTip", "Import MD2 file and selected textures" ) )
							.IsEnabled( this, &SMD2OptionsWindow::CanImport )
							.OnClicked( this, &SMD2OptionsWindow::OnImport )
					]
					+ SUniformGridPanel::Slot( 2, 0 )
					[
						SNew( SButton )
							.HAlign( HAlign_Center )
							.Text( LOCTEXT( "MD2OptionWindow_Cancel", "Cancel" ) )
							.ToolTipText( LOCTEXT( "MD2OptionWindow_Cancel_ToolTip", "Cancels importing this MD2 file" ) )
							.OnClicked( this, &SMD2OptionsWindow::OnCancel )
					]
				]
			]
		];

	BuildTextureListFromData( StartingTextureList );

	RegisterActiveTimer( 0.f, FWidgetActiveTimerDelegate::CreateSP( this, &SMD2OptionsWindow::SetFocusPostConstruct ) );
}

void SMD2OptionsWindow::BuildTextureListFromData( TArray<FString>& InTextureList )
{
	TextureListBox->ClearChildren( );

	FString MD2Filename = FPaths::GetBaseFilename( StartingMD2FullFilepath, true );

	for ( int32 i = 0; i < InTextureList.Num( ); i++ )
	{
		// setup a default asset name to help the artist
		FString DefaultAssetName = ObjectTools::SanitizeObjectName( TEXT( "T_" ) + MD2Filename + TEXT( "_" ) + InTextureList[ i ] + TEXT( "_D" ) );
		AddTextureSlot( i, InTextureList[ i ], DefaultAssetName, StartingMD2FullFilepath );
	}
}

void SMD2OptionsWindow::AddTextureSlot( int32 ID, const FString& InTextureName, const FString& InDefaultAssetName, const FString& InStartingMD2FullFilepath )
{
	TextureListBox->AddSlot( )
		[
			SNew( SMD2TextureImportWidget )
				.TextureName( InTextureName )
				.DefaultBrowseFilepath( FPaths::GetPath( InStartingMD2FullFilepath ) )
				.DefaultAssetName( InDefaultAssetName )
				.OnTextureWidgetRemoved( this, &SMD2OptionsWindow::OnRemoveTextureWidget )
				.OnTextureNotFound( this, &SMD2OptionsWindow::OnTextureNotFound )
				.OnTextureSet( this, &SMD2OptionsWindow::OnTextureSet )
				.ID( ID )
		];
}

EActiveTimerReturnType SMD2OptionsWindow::SetFocusPostConstruct( double InCurrentTime, float InDeltaTime )
{
	if ( ImportButton.IsValid( ) )
	{
		FSlateApplication::Get( ).SetKeyboardFocus( ImportButton, EFocusCause::SetDirectly );
	}

	return EActiveTimerReturnType::Stop;
}

bool SMD2OptionsWindow::CanImport( )  const
{
	return true;
}

FReply SMD2OptionsWindow::OnResetToDefaultClick( )
{
	BuildTextureListFromData( StartingTextureList );

	//todo: restore checkboxes for default mesh options (once they're added)

	return FReply::Handled( );
}

FReply SMD2OptionsWindow::OnImport( )
{
	ImportOptions->bShouldImport = true;
	if ( WidgetWindow.IsValid( ) )
	{
		WidgetWindow.Pin( )->RequestDestroyWindow( );
	}

	ImportOptions->TextureImportList.Empty( );

	// iterate over the child widgets to get the textures for import
	FChildren* Children = TextureListBox->GetChildren( );
	for ( int32 i = 0; i < Children->Num( ); i++ )
	{
		TSharedRef<SMD2TextureImportWidget> TextWidget = StaticCastSharedRef<SMD2TextureImportWidget>( Children->GetChildAt( i ) );

		TPair<FString, FString> Pair( TextWidget->GetAssetFilename( ), TextWidget->GetAssetName( ) );
		ImportOptions->TextureImportList.Add( Pair );
	}

	return FReply::Handled( );
}

void SMD2OptionsWindow::OnRemoveTextureWidget( FSMD2TextureImportWidgetID WidgetID )
{
	TSharedPtr<SMD2TextureImportWidget> WidgetPtr = GetTextureWidgetFromID( WidgetID );

	if ( WidgetPtr.IsValid( ) )
	{
		TSharedRef<SMD2TextureImportWidget> WidgetRef = WidgetPtr.ToSharedRef( );
		TextureListBox->RemoveSlot( WidgetRef );

		// see if removing this texture slot fixed any possible import errors
		if ( ImportButton->IsEnabled( ) == false )
		{
			TryEnableImport( );
		}
	}
}

void SMD2OptionsWindow::OnTextureNotFound( FSMD2TextureImportWidgetID WidgetID )
{
	// if a texture couldn't be found, disable importing until they resolve it
	ToggleImportEnabled( false );
}

void SMD2OptionsWindow::OnTextureSet( FSMD2TextureImportWidgetID WidgetID )
{
	// we can't just enable the import button, because there could be other textures or systems with issues
	TryEnableImport( );
}

void SMD2OptionsWindow::TryEnableImport( )
{
	// assume we can, and we'll flip this to false if anything wrong is detected
	bool bEnableImport = true;

	// check to make sure all texture slots are happy
	FChildren* Children = TextureListBox->GetChildren( );
	for ( int32 i = 0; i < Children->Num( ); i++ )
	{
		TSharedPtr<SMD2TextureImportWidget> TextWidget = StaticCastSharedRef<SMD2TextureImportWidget>( Children->GetChildAt( i ) );

		if ( TextWidget->TextureFileExists( ) == false )
		{
			bEnableImport = false;
			break; //no point in continuing, we know it failed.
		}
	}

	if ( bEnableImport )
	{
		ToggleImportEnabled( true );
	}
}

void SMD2OptionsWindow::ToggleImportEnabled( bool bEnabled )
{
	if ( bEnabled )
	{
		ImportButton->SetEnabled( true );
	}
	else
	{
		ImportButton->SetEnabled( false );
	}
}

FReply SMD2OptionsWindow::OnAddTextureWidget( )
{
	AddTextureSlot( TextureListBox->GetChildren( )->Num( ) + 1, FString( ), FString( ), StartingMD2FullFilepath );

	// disable import, because we know for sure we have a missing texture
	ToggleImportEnabled( false );

	return FReply::Handled( );
}

TSharedPtr<SMD2TextureImportWidget> SMD2OptionsWindow::GetTextureWidgetFromID( int ID )
{
	FChildren* Children = TextureListBox->GetChildren( );
	for ( int32 i = 0; i < Children->Num( ); i++ )
	{
		TSharedPtr<SMD2TextureImportWidget> TextWidget = StaticCastSharedRef<SMD2TextureImportWidget>( Children->GetChildAt( i ) );

		if ( TextWidget->GetID( ) == ID )
		{
			return TextWidget;
		}
	}

	return TSharedPtr<SMD2TextureImportWidget>( );
}
#undef LOCTEXT_NAMESPACE