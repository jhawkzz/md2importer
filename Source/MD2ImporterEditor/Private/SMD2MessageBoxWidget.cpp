// Fill out your copyright notice in the Description page of Project Settings.
#include "SMD2MessageBoxWidget.h"

#include "Interfaces/IMainFrameModule.h"

#include "MD2Util.h"

#define LOCTEXT_NAMESPACE "SMD2MessageBoxWidget"

void SMD2MessageBoxWidget::ShowMessageBox( const FString& Title, const FString& Message )
{
	TSharedPtr<SWindow> ParentWindow;

	if ( FModuleManager::Get( ).IsModuleLoaded( "MainFrame" ) )
	{
		IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>( "MainFrame" );
		ParentWindow = MainFrame.GetParentWindow( );
	}

	// Compute centered window position based on max window size, which include when all categories are expanded
	const float MBWindowWidth = 450.0f;
	const float MBWindowHeight = 750.0f;
	FVector2D MBWindowSize = FVector2D( MBWindowWidth, MBWindowHeight ); // Max window size it can get based on current slate

	FVector2D MBWindowPosition;
	UMD2Util::GetCenterPosForWindow( MBWindowPosition, MBWindowWidth, MBWindowHeight );

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
	);

	FSlateApplication::Get( ).AddModalWindow( Window, ParentWindow, false );
}

void SMD2MessageBoxWidget::Construct( const FArguments& InArgs )
{
	WidgetWindow = InArgs._WidgetWindow;

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
							.Text( FText::FromString( InArgs._Message ) )
					]
				]
			]
		]
	];
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