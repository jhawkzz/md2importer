// Copyright 2024 Jered McFerron

#pragma once

// UE Includes
#include "CoreMinimal.h"

using FSMD2MessageBoxWidgetCloseResult = uint32;
DECLARE_DELEGATE_OneParam( FOnMessageBoxClosed, FSMD2MessageBoxWidgetCloseResult )

class MD2IMPORTEREDITOR_API SMD2MessageBoxWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMD2MessageBoxWidget )
		: _WidgetWindow( )
		, _Title( )
		, _Message( )
		, _AcceptButtonLabel( )
		, _CancelButtonLabel( )
		{}

		SLATE_ARGUMENT( TSharedPtr<SWindow>, WidgetWindow )
		SLATE_ARGUMENT( FString, Title )
		SLATE_ARGUMENT( FString, Message )
		SLATE_ARGUMENT( FString, AcceptButtonLabel )
		SLATE_ARGUMENT( FString, CancelButtonLabel )
		SLATE_EVENT( FOnMessageBoxClosed, OnMessageBoxClosed )
	SLATE_END_ARGS( )

	enum FMessageBoxResult
	{
		MB_Accept = 0,
		MB_Cancel = 1
	};

public:
	static void ShowMessageBox( const FString& Title, const FString& Message, const FString& Accept, const FString& Cancel = FString( ), FOnMessageBoxClosed OnClosed = FOnMessageBoxClosed( ) );

	void Construct( const FArguments& InArgs );

	FReply OnAccept( );
	FReply OnCancel( );
	virtual FReply OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent ) override;

	SMD2MessageBoxWidget( )
	{}

private:
	static const float MBWindowWidth;
	static const float MBMaxWindowHeight;

	TWeakPtr<SWindow> WidgetWindow;
	FOnMessageBoxClosed OnMessageBoxClosed;
};
