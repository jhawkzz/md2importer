
#include "MD2Tests.h"

#ifdef MD2_ENABLE_TESTS

// UE Includes
#include "Engine/StaticMeshActor.h"

// Project Includes
#include "SMD2MessageBoxWidget.h"

void UMD2Tests::TestMessageBox( )
{
	SMD2MessageBoxWidget::ShowMessageBox( TEXT( "MD2 Import Error" ), TEXT( "All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. All work and no play makes Jered pretty tired. Check the Output Log for more detail. " ), TEXT( "Ok" ) );
}

void UMD2Tests::TestCreateRawMesh( FRawMesh& OutRawMesh )
{
	FVector3f EmptyVector = FVector3f( 0, 0, 0 );
	FColor WhiteVertex = FColor( 255, 255, 255, 255 );

	OutRawMesh.VertexPositions.Emplace( FVector3f( 0.f, 0.f, 0.f ) );
	OutRawMesh.VertexPositions.Emplace( FVector3f( 100.f, 0.f, 0.f ) );
	OutRawMesh.VertexPositions.Emplace( FVector3f( 100.f, 100.f, 0.f ) );
	OutRawMesh.VertexPositions.Emplace( FVector3f( 0.f, 100.f, 0.f ) );

	OutRawMesh.WedgeIndices.Emplace( 0 );
	OutRawMesh.WedgeIndices.Emplace( 1 );
	OutRawMesh.WedgeIndices.Emplace( 2 );

	OutRawMesh.WedgeIndices.Emplace( 0 );
	OutRawMesh.WedgeIndices.Emplace( 2 );
	OutRawMesh.WedgeIndices.Emplace( 3 );

	OutRawMesh.WedgeIndices.Emplace( 1 );
	OutRawMesh.WedgeIndices.Emplace( 0 );
	OutRawMesh.WedgeIndices.Emplace( 3 );

	OutRawMesh.WedgeIndices.Emplace( 1 );
	OutRawMesh.WedgeIndices.Emplace( 3 );
	OutRawMesh.WedgeIndices.Emplace( 2 );

	for ( int32 i = 0; i < 4; ++i )
	{
		OutRawMesh.WedgeColors.Add( WhiteVertex );
		OutRawMesh.WedgeColors.Add( WhiteVertex );
		OutRawMesh.WedgeColors.Add( WhiteVertex );

		OutRawMesh.WedgeTangentX.Add( EmptyVector );
		OutRawMesh.WedgeTangentX.Add( EmptyVector );
		OutRawMesh.WedgeTangentX.Add( EmptyVector );

		OutRawMesh.WedgeTangentY.Add( EmptyVector );
		OutRawMesh.WedgeTangentY.Add( EmptyVector );
		OutRawMesh.WedgeTangentY.Add( EmptyVector );

		OutRawMesh.WedgeTangentZ.Add( EmptyVector );
		OutRawMesh.WedgeTangentZ.Add( EmptyVector );
		OutRawMesh.WedgeTangentZ.Add( EmptyVector );

		for ( int32 UVIndex = 0; UVIndex < MAX_MESH_TEXTURE_COORDS; UVIndex++ )
		{
			OutRawMesh.WedgeTexCoords[ UVIndex ].Add( FVector2f( 0.0f, 0.0f ) );
			OutRawMesh.WedgeTexCoords[ UVIndex ].Add( FVector2f( 0.0f, 0.0f ) );
			OutRawMesh.WedgeTexCoords[ UVIndex ].Add( FVector2f( 0.0f, 0.0f ) );
		}

		OutRawMesh.FaceMaterialIndices.Add( 0 );
		OutRawMesh.FaceSmoothingMasks.Add( 0xFFFFFFFF ); // Phong ?
	}
}

void UMD2Tests::TestAttachToNewActor( const UStaticMesh& StaticMesh )
{
	// JHM Note - uncomment this to create an actor and put the static mesh on it.
	// Might want this for an eventual preview window?
	// Creating the Actor and Positioning it in the World based in the Static Mesh
	UWorld* currentWorld = GEditor->GetEditorWorldContext( ).World( );
	ULevel* currentLevel = currentWorld->GetCurrentLevel( );

	UClass* staticMeshActorClass = AStaticMeshActor::StaticClass( );

	// Name & Transform
	FVector objectPosition( 0, 0, 0 );
	FRotator objectRotation( 0, 0, 0 ); //in degrees
	FVector objectScale( 1, 1, 1 );
	FTransform objectTransform( objectRotation, objectPosition, objectScale );
	AActor* newActorCreated = GEditor->AddActor( currentLevel, staticMeshActorClass, objectTransform, true, RF_Public | RF_Standalone | RF_Transactional );

	AStaticMeshActor* smActor = Cast<AStaticMeshActor>( newActorCreated );

	smActor->GetStaticMeshComponent( )->SetStaticMesh( StaticMesh );
	smActor->SetActorScale3D( objectScale );

	// ID Name & Visible Name
	smActor->Rename( TEXT( "MyStaticMeshInTheWorld" ) );
	smActor->SetActorLabel( "MyStaticMeshInTheWorld" );

	GEditor->EditorUpdateComponents( );
	smActor->GetStaticMeshComponent( )->RegisterComponentWithWorld( currentWorld );
	currentWorld->UpdateWorldComponents( true, false );
	smActor->RerunConstructionScripts( );
	//GLevelEditorModeTools().MapChangeNotify();
	//

	return nullptr;
}
#endif