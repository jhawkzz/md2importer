// Fill out your copyright notice in the Description page of Project Settings.
#include "SMD2MessageBoxWidget.h"

// UE Includes
#include "Interfaces/IMainFrameModule.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Text/SMultiLineEditableText.h"

// Project Includes
#include "MD2Util.h"

#define LOCTEXT_NAMESPACE "SMD2MessageBoxWidget"

const float SMD2MessageBoxWidget::MBWindowWidth = 450.0f;
const float SMD2MessageBoxWidget::MBMaxWindowHeight = 750.0f;

void SMD2MessageBoxWidget::ShowMessageBox( const FString& Title, const FString& Message, const FString& Accept, const FString& Cancel /*= FString( )*/, FOnMessageBoxClosed OnClosed /*= FOnMessageBoxClosed( )*/ )
{
	TSharedPtr<SWindow> ParentWindow;

	if ( FModuleManager::Get( ).IsModuleLoaded( "MainFrame" ) )
	{
		IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>( "MainFrame" );
		ParentWindow = MainFrame.GetParentWindow( );
	}

	// Compute centered window position based on max window size, which include when all categories are expanded
	FVector2D MBWindowSize = FVector2D( MBWindowWidth, MBMaxWindowHeight ); // Max window size it can get based on current slate

	FVector2D MBWindowPosition;
	UMD2Util::GetCenterPosForWindow( MBWindowPosition, MBWindowWidth, MBMaxWindowHeight );

	TSharedRef<SWindow> Window = SNew( SWindow )
		.Title( FText::FromString( Title ) )
		.SizingRule( ESizingRule::Autosized )
		.AutoCenter( EAutoCenter::None )
		.ClientSize( MBWindowSize )
		.ScreenPosition( MBWindowPosition );

	TSharedPtr<SMD2MessageBoxWidget> MD2MessageBoxWidget;
	Window->SetContent
	(
		SAssignNew( MD2MessageBoxWidget, SMD2MessageBoxWidget )
		.WidgetWindow( Window )
		.Message( Message )
		.OnMessageBoxClosed(OnClosed)
		.AcceptButtonLabel( Accept.Len( ) > 0 ? Accept : TEXT( "Ok" ) )
		.CancelButtonLabel( Cancel )
	);

	FSlateApplication::Get( ).AddModalWindow( Window, ParentWindow, false );
}

void SMD2MessageBoxWidget::Construct( const FArguments& InArgs )
{
	WidgetWindow = InArgs._WidgetWindow;
	OnMessageBoxClosed = InArgs._OnMessageBoxClosed;
	TSharedPtr<SUniformGridPanel> ButtonGrid;

	this->ChildSlot
		[
			SNew( SBox )
				.MaxDesiredHeight( MBMaxWindowHeight )
				.MaxDesiredWidth( MBWindowWidth )
				[
					SNew( SVerticalBox )
						+ SVerticalBox::Slot( )
						.AutoHeight( )
						.VAlign( VAlign_Center )
						.Padding( 2.0f, 10, 2.0f, 2.0f )
						.MaxHeight( MBMaxWindowHeight - 250 ) // Leave space for the button controls
						[
							SNew( SMultiLineEditableText )
								.Text( FText::FromString( InArgs._Message ) )
								.IsReadOnly( true )
								.AutoWrapText( true )
						]
						// Buttons
						+ SVerticalBox::Slot( )
						.AutoHeight( )
						.Padding( 1.0f, 0.0f, 1.0f, 10.0f )
						[
							SAssignNew( ButtonGrid, SUniformGridPanel )
								.SlotPadding( 2.0f )
								+ SUniformGridPanel::Slot( 1, 0 )
								[
									SNew( SButton )
										.HAlign( HAlign_Center )
										.Text( FText::FromString( InArgs._AcceptButtonLabel ) )
										.OnClicked( this, &SMD2MessageBoxWidget::OnAccept )
								]
						]
				]
		];

	if ( InArgs._CancelButtonLabel.Len( ) > 0 )
	{
		ButtonGrid->AddSlot( 2, 0 )
			[
				SNew( SButton )
					.HAlign( HAlign_Center )
					.Text( FText::FromString( InArgs._CancelButtonLabel ) )
					.OnClicked( this, &SMD2MessageBoxWidget::OnCancel )
			];
	}
}

FReply SMD2MessageBoxWidget::OnAccept( )
{
	OnMessageBoxClosed.ExecuteIfBound( FMessageBoxResult::MB_Accept );

	if ( WidgetWindow.IsValid( ) )
	{
		WidgetWindow.Pin( )->RequestDestroyWindow( );
	}

	return FReply::Handled( );
}

FReply SMD2MessageBoxWidget::OnCancel( )
{
	OnMessageBoxClosed.ExecuteIfBound( FMessageBoxResult::MB_Cancel );

	if ( WidgetWindow.IsValid( ) )
	{
		WidgetWindow.Pin( )->RequestDestroyWindow( );
	}
	return FReply::Handled( );
}

FReply SMD2MessageBoxWidget::OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent )
{
	if ( InKeyEvent.GetKey( ) == EKeys::Escape )
	{
		return OnCancel( );
	}

	return FReply::Unhandled( );
}

#undef LOCTEXT_NAMESPACE