// Copyright 2024 Jered McFerron
#include "MD2Asset.h"

// UE Includes
#include "RawMesh.h"

// Project Includes

const float UMD2Asset::NormalLookup[ PRECAL_NORM ][ 3 ]
{
	{ -0.525731f, 0.000000f, 0.850651f },
	{ -0.442863f, 0.238856f, 0.864188f },
	{ -0.295242f, 0.000000f, 0.955423f },
	{ -0.309017f, 0.500000f, 0.809017f },
	{ -0.162460f, 0.262866f, 0.951056f },
	{ 0.000000f, 0.000000f, 1.000000f },
	{ 0.000000f, 0.850651f, 0.525731f },
	{ -0.147621f, 0.716567f, 0.681718f },
	{ 0.147621f, 0.716567f, 0.681718f },
	{ 0.000000f, 0.525731f, 0.850651f },
	{ 0.309017f, 0.500000f, 0.809017f },
	{ 0.525731f, 0.000000f, 0.850651f },
	{ 0.295242f, 0.000000f, 0.955423f },
	{ 0.442863f, 0.238856f, 0.864188f },
	{ 0.162460f, 0.262866f, 0.951056f },
	{ -0.681718f, 0.147621f, 0.716567f },
	{ -0.809017f, 0.309017f, 0.500000f },
	{ -0.587785f, 0.425325f, 0.688191f },
	{ -0.850651f, 0.525731f, 0.000000f },
	{ -0.864188f, 0.442863f, 0.238856f },
	{ -0.716567f, 0.681718f, 0.147621f },
	{ -0.688191f, 0.587785f, 0.425325f },
	{ -0.500000f, 0.809017f, 0.309017f },
	{ -0.238856f, 0.864188f, 0.442863f },
	{ -0.425325f, 0.688191f, 0.587785f },
	{ -0.716567f, 0.681718f, -0.147621f },
	{ -0.500000f, 0.809017f, -0.309017f },
	{ -0.525731f, 0.850651f, 0.000000f },
	{ 0.000000f, 0.850651f, -0.525731f },
	{ -0.238856f, 0.864188f, -0.442863f },
	{ 0.000000f, 0.955423f, -0.295242f },
	{ -0.262866f, 0.951056f, -0.162460f },
	{ 0.000000f, 1.000000f, 0.000000f },
	{ 0.000000f, 0.955423f, 0.295242f },
	{ -0.262866f, 0.951056f, 0.162460f },
	{ 0.238856f, 0.864188f, 0.442863f },
	{ 0.262866f, 0.951056f, 0.162460f },
	{ 0.500000f, 0.809017f, 0.309017f },
	{ 0.238856f, 0.864188f, -0.442863f },
	{ 0.262866f, 0.951056f, -0.162460f },
	{ 0.500000f, 0.809017f, -0.309017f },
	{ 0.850651f, 0.525731f, 0.000000f },
	{ 0.716567f, 0.681718f, 0.147621f },
	{ 0.716567f, 0.681718f, -0.147621f },
	{ 0.525731f, 0.850651f, 0.000000f },
	{ 0.425325f, 0.688191f, 0.587785f },
	{ 0.864188f, 0.442863f, 0.238856f },
	{ 0.688191f, 0.587785f, 0.425325f },
	{ 0.809017f, 0.309017f, 0.500000f },
	{ 0.681718f, 0.147621f, 0.716567f },
	{ 0.587785f, 0.425325f, 0.688191f },
	{ 0.955423f, 0.295242f, 0.000000f },
	{ 1.000000f, 0.000000f, 0.000000f },
	{ 0.951056f, 0.162460f, 0.262866f },
	{ 0.850651f, -0.525731f, 0.000000f },
	{ 0.955423f, -0.295242f, 0.000000f },
	{ 0.864188f, -0.442863f, 0.238856f },
	{ 0.951056f, -0.162460f, 0.262866f },
	{ 0.809017f, -0.309017f, 0.500000f },
	{ 0.681718f, -0.147621f, 0.716567f },
	{ 0.850651f, 0.000000f, 0.525731f },
	{ 0.864188f, 0.442863f, -0.238856f },
	{ 0.809017f, 0.309017f, -0.500000f },
	{ 0.951056f, 0.162460f, -0.262866f },
	{ 0.525731f, 0.000000f, -0.850651f },
	{ 0.681718f, 0.147621f, -0.716567f },
	{ 0.681718f, -0.147621f, -0.716567f },
	{ 0.850651f, 0.000000f, -0.525731f },
	{ 0.809017f, -0.309017f, -0.500000f },
	{ 0.864188f, -0.442863f, -0.238856f },
	{ 0.951056f, -0.162460f, -0.262866f },
	{ 0.147621f, 0.716567f, -0.681718f },
	{ 0.309017f, 0.500000f, -0.809017f },
	{ 0.425325f, 0.688191f, -0.587785f },
	{ 0.442863f, 0.238856f, -0.864188f },
	{ 0.587785f, 0.425325f, -0.688191f },
	{ 0.688191f, 0.587785f, -0.425325f },
	{ -0.147621f, 0.716567f, -0.681718f },
	{ -0.309017f, 0.500000f, -0.809017f },
	{ 0.000000f, 0.525731f, -0.850651f },
	{ -0.525731f, 0.000000f, -0.850651f },
	{ -0.442863f, 0.238856f, -0.864188f },
	{ -0.295242f, 0.000000f, -0.955423f },
	{ -0.162460f, 0.262866f, -0.951056f },
	{ 0.000000f, 0.000000f, -1.000000f },
	{ 0.295242f, 0.000000f, -0.955423f },
	{ 0.162460f, 0.262866f, -0.951056f },
	{ -0.442863f, -0.238856f, -0.864188f },
	{ -0.309017f, -0.500000f, -0.809017f },
	{ -0.162460f, -0.262866f, -0.951056f },
	{ 0.000000f, -0.850651f, -0.525731f },
	{ -0.147621f, -0.716567f, -0.681718f },
	{ 0.147621f, -0.716567f, -0.681718f },
	{ 0.000000f, -0.525731f, -0.850651f },
	{ 0.309017f, -0.500000f, -0.809017f },
	{ 0.442863f, -0.238856f, -0.864188f },
	{ 0.162460f, -0.262866f, -0.951056f },
	{ 0.238856f, -0.864188f, -0.442863f },
	{ 0.500000f, -0.809017f, -0.309017f },
	{ 0.425325f, -0.688191f, -0.587785f },
	{ 0.716567f, -0.681718f, -0.147621f },
	{ 0.688191f, -0.587785f, -0.425325f },
	{ 0.587785f, -0.425325f, -0.688191f },
	{ 0.000000f, -0.955423f, -0.295242f },
	{ 0.000000f, -1.000000f, 0.000000f },
	{ 0.262866f, -0.951056f, -0.162460f },
	{ 0.000000f, -0.850651f, 0.525731f },
	{ 0.000000f, -0.955423f, 0.295242f },
	{ 0.238856f, -0.864188f, 0.442863f },
	{ 0.262866f, -0.951056f, 0.162460f },
	{ 0.500000f, -0.809017f, 0.309017f },
	{ 0.716567f, -0.681718f, 0.147621f },
	{ 0.525731f, -0.850651f, 0.000000f },
	{ -0.238856f, -0.864188f, -0.442863f },
	{ -0.500000f, -0.809017f, -0.309017f },
	{ -0.262866f, -0.951056f, -0.162460f },
	{ -0.850651f, -0.525731f, 0.000000f },
	{ -0.716567f, -0.681718f, -0.147621f },
	{ -0.716567f, -0.681718f, 0.147621f },
	{ -0.525731f, -0.850651f, 0.000000f },
	{ -0.500000f, -0.809017f, 0.309017f },
	{ -0.238856f, -0.864188f, 0.442863f },
	{ -0.262866f, -0.951056f, 0.162460f },
	{ -0.864188f, -0.442863f, 0.238856f },
	{ -0.809017f, -0.309017f, 0.500000f },
	{ -0.688191f, -0.587785f, 0.425325f },
	{ -0.681718f, -0.147621f, 0.716567f },
	{ -0.442863f, -0.238856f, 0.864188f },
	{ -0.587785f, -0.425325f, 0.688191f },
	{ -0.309017f, -0.500000f, 0.809017f },
	{ -0.147621f, -0.716567f, 0.681718f },
	{ -0.425325f, -0.688191f, 0.587785f },
	{ -0.162460f, -0.262866f, 0.951056f },
	{ 0.442863f, -0.238856f, 0.864188f },
	{ 0.162460f, -0.262866f, 0.951056f },
	{ 0.309017f, -0.500000f, 0.809017f },
	{ 0.147621f, -0.716567f, 0.681718f },
	{ 0.000000f, -0.525731f, 0.850651f },
	{ 0.425325f, -0.688191f, 0.587785f },
	{ 0.587785f, -0.425325f, 0.688191f },
	{ 0.688191f, -0.587785f, 0.425325f },
	{ -0.955423f, 0.295242f, 0.000000f },
	{ -0.951056f, 0.162460f, 0.262866f },
	{ -1.000000f, 0.000000f, 0.000000f },
	{ -0.850651f, 0.000000f, 0.525731f },
	{ -0.955423f, -0.295242f, 0.000000f },
	{ -0.951056f, -0.162460f, 0.262866f },
	{ -0.864188f, 0.442863f, -0.238856f },
	{ -0.951056f, 0.162460f, -0.262866f },
	{ -0.809017f, 0.309017f, -0.500000f },
	{ -0.864188f, -0.442863f, -0.238856f },
	{ -0.951056f, -0.162460f, -0.262866f },
	{ -0.809017f, -0.309017f, -0.500000f },
	{ -0.681718f, 0.147621f, -0.716567f },
	{ -0.681718f, -0.147621f, -0.716567f },
	{ -0.850651f, 0.000000f, -0.525731f },
	{ -0.688191f, 0.587785f, -0.425325f },
	{ -0.587785f, 0.425325f, -0.688191f },
	{ -0.425325f, 0.688191f, -0.587785f },
	{ -0.425325f, -0.688191f, -0.587785f },
	{ -0.587785f, -0.425325f, -0.688191f },
	{ -0.688191f, -0.587785f, -0.425325f }
};

UMD2Asset::UMD2Asset( )
{
}

UMD2Asset::~UMD2Asset( )
{
	UnLoad( );
}

bool UMD2Asset::Load( const FString& MD2Filename )
{
	if ( Filename.Len( ) > 0 )
	{
		UE_LOG( LogTemp, Warning, TEXT( "UMD2Asset already loaded." ) );
		return false;
	}

	Filename = MD2Filename;

	FFileHelper::LoadFileToArray( BinaryData, *Filename, 0 );
	uint8* CurrReadPos = BinaryData.GetData( );

	// read the header
	if ( BinaryData.Num( ) < sizeof( Model.Header ) )
	{
		return false;
	}

	memcpy( &Model.Header, CurrReadPos, sizeof( Model.Header ) );

	// verify valid MD2, values are within range, and that the file buffer is within range
	bool bIsValid = Model.Header.Ident == MAGIC_NUMBER
		&& Model.Header.Version == MODEL_VERSION
		&& Model.Header.NumTris <= MAX_TRIANGLES
		&& Model.Header.NumVertices <= MAX_VERTICES
		&& Model.Header.NumSt <= MAX_TEXTURE_COORDS
		&& Model.Header.NumFrames <= MAX_FRAMES
		&& Model.Header.NumSkins <= MAX_SKINS
		&& BinaryData.Num( ) >= Model.Header.OffsetEnd;

	if ( bIsValid == false )
	{
		return false;
	}

	// allocate memory
	Model.Skins = new FMD2Skin[ Model.Header.NumSkins ]; //skins
	Model.Texcoords = new FMD2TexCoord[ Model.Header.NumSt ]; //texture coords
	Model.Triangles = new FMD2Triangle[ Model.Header.NumTris ]; //triangles
	Model.Frames = new FMD2Frame[ Model.Header.NumFrames ]; //frames
	Model.Glcmds = new int[ Model.Header.NumGlcmds ]; //opengl commands

	// read model data
	CurrReadPos = BinaryData.GetData( ) + Model.Header.OffsetSkins;
	memcpy( Model.Skins, CurrReadPos, Model.Header.NumSkins * sizeof( FMD2Skin ) );

	CurrReadPos = BinaryData.GetData( ) + Model.Header.OffsetSt;
	memcpy( Model.Texcoords, CurrReadPos, Model.Header.NumSt * sizeof( FMD2TexCoord ) );

	CurrReadPos = BinaryData.GetData( ) + Model.Header.OffsetTris;
	memcpy( Model.Triangles, CurrReadPos, Model.Header.NumTris * sizeof( FMD2Triangle ) );

	CurrReadPos = BinaryData.GetData( ) + Model.Header.OffsetGlcmds;
	memcpy( Model.Glcmds, CurrReadPos, Model.Header.NumGlcmds * sizeof( int32 ) );

	// read frames
	CurrReadPos = BinaryData.GetData( ) + Model.Header.OffsetFrames;

	uint32 i;
	for ( i = 0; i < (uint32)Model.Header.NumFrames; i++ )
	{
		// allocate verts for this frame
		Model.Frames[ i ].Verts = new FMD2Vertex[ Model.Header.NumVertices ];

		// read frame data
		memcpy( &Model.Frames[ i ].Scale, CurrReadPos, sizeof( FVector3f ) );
		CurrReadPos += sizeof( FVector3f );

		memcpy( &Model.Frames[ i ].Translate, CurrReadPos, sizeof( FVector3f ) );
		CurrReadPos += sizeof( FVector3f );

		memcpy( Model.Frames[ i ].Name, CurrReadPos, sizeof( char ) * 16 );
		CurrReadPos += sizeof( int8 ) * 16;

		memcpy( Model.Frames[ i ].Verts, CurrReadPos, sizeof( FMD2Vertex ) * Model.Header.NumVertices );
		CurrReadPos += sizeof( FMD2Vertex ) * Model.Header.NumVertices;
	}

	return true;
}

void UMD2Asset::GetPCXTextureList( TArray<FString>& OutPCXTextureNames )
{
	// copy over the textures this md2 model wants to use
	for ( int32 i = 0; i < Model.Header.NumSkins; i++ )
	{
		FString SkinName( Model.Skins[ i ].Name );

		// Our UE importer doesn't care about the legacy paths, so take only the
		// filename.
		int32 FileNameIndex = 0;
		bool bResult = SkinName.FindLastChar( '/', FileNameIndex );

		if ( bResult == true )
		{
			SkinName = SkinName.RightChop( FileNameIndex + 1 );
		}

		OutPCXTextureNames.Add( SkinName );
	}
}

void UMD2Asset::Convert( FRawMesh& OutRawMesh )
{
	FVector3f EmptyVector = FVector3f( 0, 0, 0 );
	FColor WhiteVertex = FColor( 255, 255, 255, 255 );

	OutRawMesh.VertexPositions.Reserve( Model.Header.NumVertices );
	OutRawMesh.WedgeIndices.Reserve( Model.Header.NumTris * 3 );
	OutRawMesh.WedgeColors.Reserve( Model.Header.NumTris * 3 );
	OutRawMesh.WedgeTangentX.Reserve( Model.Header.NumTris * 3 );
	OutRawMesh.WedgeTangentY.Reserve( Model.Header.NumTris * 3 );
	OutRawMesh.WedgeTangentZ.Reserve( Model.Header.NumTris * 3 );
	OutRawMesh.FaceMaterialIndices.Reserve( Model.Header.NumTris );
	OutRawMesh.FaceSmoothingMasks.Reserve( Model.Header.NumTris );
	OutRawMesh.WedgeTexCoords[ 0 ].Reserve( Model.Header.NumTris * 3 ); //just one set of UVs

	// for now just use the first frame of animation

	// the actual vert data is stored in the frame
	for ( int32 i = 0; i < Model.Header.NumVertices; i++ )
	{
		FMD2Vertex& Vert = Model.Frames[ 0 ].Verts[ i ];

		// Ironically, for most engines Y and Z need to be flipped. But for UE with Z being up, they dont!
		FVector3f VertPos(
			(Vert.V[ 0 ] * Model.Frames[ 0 ].Scale[ 0 ]) + Model.Frames[ 0 ].Translate[ 0 ],
			(Vert.V[ 1 ] * Model.Frames[ 0 ].Scale[ 1 ]) + Model.Frames[ 0 ].Translate[ 1 ],
			(Vert.V[ 2 ] * Model.Frames[ 0 ].Scale[ 2 ]) + Model.Frames[ 0 ].Translate[ 2 ]
		);

		OutRawMesh.VertexPositions.Add( VertPos );
	}

	for ( int32 i = 0; i < Model.Header.NumTris; i++ )
	{
		FMD2Triangle& CurrTri = Model.Triangles[ i ];

		// triangle vert lookup
		OutRawMesh.WedgeIndices.Add( CurrTri.Vertex[ 0 ] );
		OutRawMesh.WedgeIndices.Add( CurrTri.Vertex[ 1 ] );
		OutRawMesh.WedgeIndices.Add( CurrTri.Vertex[ 2 ] );

		// vert coloring
		OutRawMesh.WedgeColors.Add( WhiteVertex );
		OutRawMesh.WedgeColors.Add( WhiteVertex );
		OutRawMesh.WedgeColors.Add( WhiteVertex );

		// normals
		FMD2Vertex& Vert0 = Model.Frames[ 0 ].Verts[ CurrTri.Vertex[ 0 ] ];
		FMD2Vertex& Vert1 = Model.Frames[ 0 ].Verts[ CurrTri.Vertex[ 1 ] ];
		FMD2Vertex& Vert2 = Model.Frames[ 0 ].Verts[ CurrTri.Vertex[ 2 ] ];

		FVector3f Normal0( -NormalLookup[ Vert0.NormalIndex ][ 0 ], -NormalLookup[ Vert0.NormalIndex ][ 1 ], -NormalLookup[ Vert0.NormalIndex ][ 2 ] );
		FVector3f Normal1( -NormalLookup[ Vert1.NormalIndex ][ 0 ], -NormalLookup[ Vert1.NormalIndex ][ 1 ], -NormalLookup[ Vert1.NormalIndex ][ 2 ] );
		FVector3f Normal2( -NormalLookup[ Vert2.NormalIndex ][ 0 ], -NormalLookup[ Vert2.NormalIndex ][ 1 ], -NormalLookup[ Vert2.NormalIndex ][ 2 ] );

		OutRawMesh.WedgeTangentZ.Add( Normal0 );
		OutRawMesh.WedgeTangentZ.Add( Normal1 );
		OutRawMesh.WedgeTangentZ.Add( Normal2 );

		// Tangents
		OutRawMesh.WedgeTangentX.Add( EmptyVector );
		OutRawMesh.WedgeTangentX.Add( EmptyVector );
		OutRawMesh.WedgeTangentX.Add( EmptyVector );

		OutRawMesh.WedgeTangentY.Add( EmptyVector );
		OutRawMesh.WedgeTangentY.Add( EmptyVector );
		OutRawMesh.WedgeTangentY.Add( EmptyVector );

		// Material / Textures
		OutRawMesh.FaceMaterialIndices.Add( 0 );
		OutRawMesh.FaceSmoothingMasks.Add( 0xFFFFFFFF );

		// UVs
		FVector2f Vert0UV(
			(float)Model.Texcoords[ CurrTri.St[ 0 ] ].S / Model.Header.SkinWidth,
			(float)Model.Texcoords[ CurrTri.St[ 0 ] ].T / Model.Header.SkinHeight
		);

		FVector2f Vert1UV(
			(float)Model.Texcoords[ CurrTri.St[ 1 ] ].S / Model.Header.SkinWidth,
			(float)Model.Texcoords[ CurrTri.St[ 1 ] ].T / Model.Header.SkinHeight
		);

		FVector2f Vert2UV(
			(float)Model.Texcoords[ CurrTri.St[ 2 ] ].S / Model.Header.SkinWidth,
			(float)Model.Texcoords[ CurrTri.St[ 2 ] ].T / Model.Header.SkinHeight
		);

		OutRawMesh.WedgeTexCoords[ 0 ].Add( Vert0UV );
		OutRawMesh.WedgeTexCoords[ 0 ].Add( Vert1UV );
		OutRawMesh.WedgeTexCoords[ 0 ].Add( Vert2UV );
	}
}

void UMD2Asset::UnLoad( void )
{
	uint32 i;

	//remove verts 
	for ( i = 0; i < (uint32)Model.Header.NumFrames; i++ )
	{
		delete[ ] Model.Frames[ i ].Verts;
	}

	// cleanup the rest
	if ( Model.Glcmds != nullptr )
	{
		delete[ ] Model.Glcmds;
	}

	if ( Model.Frames != nullptr )
	{
		delete[ ] Model.Frames;
	}

	if ( Model.Triangles != nullptr )
	{
		delete[ ] Model.Triangles;
	}

	if ( Model.Texcoords != nullptr )
	{
		delete[ ] Model.Texcoords;
	}

	if ( Model.Skins != nullptr )
	{
		delete[ ] Model.Skins;
	}

	memset( &Model, 0, sizeof( Model ) );
}
