// Fill out your copyright notice in the Description page of Project Settings.


#include "MD2OptionsWindow.h"
#include "MD2Asset.h"
#include "IDocumentation.h"
#include "Internationalization/Internationalization.h"
#include "SPrimaryButton.h"

#define LOCTEXT_NAMESPACE "MD2Option"

void SMD2OptionsWindow::Construct( const FArguments& InArgs )
{
	WidgetWindow = InArgs._WidgetWindow;

	TSharedPtr<SBox> ImportTypeDisplay;
	TSharedPtr<SHorizontalBox> MD2HeaderButtons;
	TSharedPtr<SBox> InspectorBox;
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
							SAssignNew( ImportTypeDisplay, SBox )
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
												.Text( LOCTEXT( "Import_CurrentFileTitle", "Current Asset: " ) )
										]
										+ SHorizontalBox::Slot( )
										.Padding( 5, 0, 0, 0 )
										.AutoWidth( )
										.VAlign( VAlign_Center )
										[
											SNew( STextBlock )
												.Text( InArgs._FullPath )
												.ToolTipText( InArgs._FullPath )
										]
								]
						]
						+ SVerticalBox::Slot( )
						.AutoHeight( )
						.Padding( 2.0f )
						[
							SAssignNew( InspectorBox, SBox )
								.MaxDesiredHeight( 650.0f )
								.WidthOverride( 400.0f )
						]
						+ SVerticalBox::Slot( )
						.AutoHeight( )
						.Padding( 2.0f )
						[
							SNew( SUniformGridPanel )
								.SlotPadding( 2.0f )
								+ SUniformGridPanel::Slot( 1, 0 )
								[
									SAssignNew( ImportAllButton, SPrimaryButton )
										.Text( LOCTEXT( "MD2OptionWindow_Import", "Import" ) )
										.ToolTipText( LOCTEXT( "MD2OptionWindow_Import_ToolTip", "Import MD2 file and selected textures" ) )
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

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>( "PropertyEditor" );
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsView = PropertyEditorModule.CreateDetailView( DetailsViewArgs );

	InspectorBox->SetContent( DetailsView->AsShared( ) );

	ImportTypeDisplay->SetContent(
		SNew( SBorder )
		.Padding( FMargin( 3 ) )
		.BorderImage( FAppStyle::GetBrush( "ToolPanel.GroupBorder" ) )
		[
			SNew( SHorizontalBox )
				+ SHorizontalBox::Slot( )
				.AutoWidth( )
				.VAlign( VAlign_Center )
				[
					SNew( STextBlock )
						.Text( this, &SMD2OptionsWindow::GetImportTypeDisplayText )
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
					SAssignNew( MD2HeaderButtons, SHorizontalBox )
						+ SHorizontalBox::Slot( )
						.AutoWidth( )
						.Padding( FMargin( 2.0f, 0.0f ) )
						[
							SNew( SButton )
								.Text( LOCTEXT( "MD2OptionWindow_ResetOptions", "Reset to Default" ) )
								.OnClicked( this, &SMD2OptionsWindow::OnResetToDefaultClick )
						]
				]
		]
	);

	//DetailsView->SetObject( ImportUI );

	RegisterActiveTimer( 0.f, FWidgetActiveTimerDelegate::CreateSP( this, &SMD2OptionsWindow::SetFocusPostConstruct ) );

}
EActiveTimerReturnType SMD2OptionsWindow::SetFocusPostConstruct( double InCurrentTime, float InDeltaTime )
{
	if ( ImportAllButton.IsValid( ) )
	{
		FSlateApplication::Get( ).SetKeyboardFocus( ImportAllButton, EFocusCause::SetDirectly );
	}

	return EActiveTimerReturnType::Stop;

}
bool SMD2OptionsWindow::CanImport( )  const
{/*
	// do test to see if we are ready to import
	if ( ImportUI->MeshTypeToImport == FBXIT_Animation )
	{
		if ( ImportUI->Skeleton == NULL || !ImportUI->bImportAnimations )
		{
			return false;
		}
	}

	if ( ImportUI->AnimSequenceImportData->AnimationLength == FBXALIT_SetRange )
	{
		if ( ImportUI->AnimSequenceImportData->FrameImportRange.Min > ImportUI->AnimSequenceImportData->FrameImportRange.Max )
		{
			return false;
		}
	}
	*/
	return true;
}
FReply SMD2OptionsWindow::OnResetToDefaultClick( ) const
{
	/*ImportUI->ResetToDefault( );
	//Refresh the view to make sure the custom UI are updating correctly
	DetailsView->SetObject( ImportUI, true );*/
	return FReply::Handled( );
}
FText SMD2OptionsWindow::GetImportTypeDisplayText( ) const
{
	/*switch ( ImportUI->MeshTypeToImport )
	{
	case EFBXImportType::FBXIT_Animation:
		return ImportUI->bIsReimport ? LOCTEXT( "FbxOptionWindow_ReImportTypeAnim", "Reimport Animation" ) : LOCTEXT( "FbxOptionWindow_ImportTypeAnim", "Import Animation" );
	case EFBXImportType::FBXIT_SkeletalMesh:
		return ImportUI->bIsReimport ? LOCTEXT( "FbxOptionWindow_ReImportTypeSK", "Reimport Skeletal Mesh" ) : LOCTEXT( "FbxOptionWindow_ImportTypeSK", "Import Skeletal Mesh" );
	case EFBXImportType::FBXIT_StaticMesh:
		return ImportUI->bIsReimport ? LOCTEXT( "FbxOptionWindow_ReImportTypeSM", "Reimport Static Mesh" ) : LOCTEXT( "FbxOptionWindow_ImportTypeSM", "Import Static Mesh" );
	}*/
	return FText::GetEmpty( );
}
#undef LOCTEXT_NAMESPACE