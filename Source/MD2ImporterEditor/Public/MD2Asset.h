// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MD2Asset.generated.h"

// This loads the Quake II model format.
// This information taken from the glorious site: http://tfc.duke.free.fr/coding/md2-specs-en.html
struct FMD2Header
{
	int32 Ident;          /* magic number: "IDP2" */
	int32 Version;        /* version: must be 8 */

	int32 SkinWidth;      /* texture width */
	int32 SkinHeight;     /* texture height */

	int32 FrameSize;      /* size in bytes of a frame */

	int32 NumSkins;      /* number of skins */
	int32 NumVertices;   /* number of vertices per frame */
	int32 NumSt;         /* number of texture coordinates */
	int32 NumTris;       /* number of triangles */
	int32 NumGlcmds;     /* number of opengl commands */
	int32 NumFrames;     /* number of frames */

	int32 OffsetSkins;   /* offset skin data */
	int32 OffsetSt;      /* offset texture coordinate data */
	int32 OffsetTris;    /* offset triangle data */
	int32 OffsetFrames;  /* offset frame data */
	int32 OffsetGlcmds;  /* offset OpenGL command data */
	int32 OffsetEnd;     /* offset end of file */

};

/* texture name */
struct FMD2Skin
{
	char Name[ 64 ];   /* texture file name */
};

/* texture coords */
struct FMD2TexCoord
{
	int16 S;
	int16 T;

};

/* triangle data */
struct FMD2Triangle
{
	uint16 Vertex[ 3 ];   /* vertex indices of the triangle */
	uint16 St[ 3 ];       /* tex. coord. indices */

};

/* vertex data */
struct FMD2Vertex
{
	uint8 V[ 3 ];         /* position */
	uint8 NormalIndex;  /* normal vector index */

};

/* frame data */
struct FMD2Frame
{
	FVector3f   Scale;      /* scale factor */
	FVector3f   Translate;  /* translation vector */
	int8        Name[ 16 ];   /* frame name */
	FMD2Vertex* Verts;     /* list of frame's vertices */

};

/* md2 model structure */
struct  FMD2Model
{
	FMD2Header Header;

	FMD2Skin* Skins{ nullptr };
	FMD2TexCoord* Texcoords{ nullptr };
	FMD2Triangle* Triangles{ nullptr };
	FMD2Frame* Frames{ nullptr };
	int32* Glcmds{ nullptr };
};

/**
 *
 */
UCLASS( )
class MD2IMPORTEREDITOR_API UMD2Asset : public UObject
{
	GENERATED_BODY( )

public:
	UMD2Asset( );
	~UMD2Asset( );

	bool Load( const FString& MD2Filename );
	void GetPCXTextureList( TArray<FString>& OutPCXTextureNames );
	void Convert( struct FRawMesh& OutRawMesh );

private:
	void UnLoad( void );

public:
	static inline const FString REFERENCE_URL { "http://tfc.duke.free.fr/coding/md2-specs-en.html" };
	static inline const FString TEXTURE_FORMAT { "pcx" };

	static const uint32 MAGIC_NUMBER = 844121161;
	static const uint32 MODEL_VERSION = 8;

	static const uint32 MAX_TRIANGLES = 4096;
	static const uint32 MAX_VERTICES = 2048;
	static const uint32 MAX_TEXTURE_COORDS = 2048;
	static const uint32 MAX_FRAMES = 512;
	static const uint32 MAX_SKINS = 32;

private:
	static const uint32 PRECAL_NORM = 162;
	static const float NormalLookup[ PRECAL_NORM ][ 3 ];

	FMD2Model Model;
	FString Filename;
	TArray<uint8> BinaryData;
};
