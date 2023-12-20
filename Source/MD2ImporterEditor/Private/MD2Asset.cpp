// Fill out your copyright notice in the Description page of Project Settings.


#include "MD2Asset.h"
#include "RawMesh.h"

UMD2Asset::UMD2Asset()
{
}

UMD2Asset::~UMD2Asset()
{
    UnLoad();
}

bool UMD2Asset::Load(TArray<uint8>* BinaryData)
{
    uint8* CurrReadPos = BinaryData->GetData();

    // read the header
    memcpy(&m_model.header, CurrReadPos, sizeof(m_model.header) );

    // verify version
    if (m_model.header.ident != MODEL_IDENT || m_model.header.version != MODEL_VERSION) return false;

    // allocate memory
    m_model.skins = new md2_skin_t[m_model.header.num_skins]; //skins
    m_model.texcoords = new md2_texCoord_t[m_model.header.num_st]; //texture coords
    m_model.triangles = new md2_triangle_t[m_model.header.num_tris]; //triangles
    m_model.frames = new md2_frame_t[m_model.header.num_frames]; //frames
    m_model.glcmds = new int[m_model.header.num_glcmds]; //opengl commands

    // read model data
    CurrReadPos = BinaryData->GetData() + m_model.header.offset_skins;
    memcpy( m_model.skins, CurrReadPos, m_model.header.num_skins * sizeof(md2_skin_t) );

    CurrReadPos = BinaryData->GetData() + m_model.header.offset_st;
    memcpy( m_model.texcoords, CurrReadPos, m_model.header.num_st * sizeof(md2_texCoord_t) );

    CurrReadPos = BinaryData->GetData() + m_model.header.offset_tris;
    memcpy( m_model.triangles, CurrReadPos, m_model.header.num_tris * sizeof(md2_triangle_t) );

    CurrReadPos = BinaryData->GetData() + m_model.header.offset_glcmds;
    memcpy( m_model.glcmds, CurrReadPos, m_model.header.num_glcmds * sizeof(int) );

    // read frames
    CurrReadPos = BinaryData->GetData() + m_model.header.offset_frames;

    uint32 i;
    for (i = 0; i < (uint32)m_model.header.num_frames; i++)
    {
        // allocate verts for this frame
        m_model.frames[i].verts = new md2_vertex_t[m_model.header.num_vertices];

        // read frame data
        memcpy(m_model.frames[i].scale, CurrReadPos, sizeof(vec3_t) );
        CurrReadPos += sizeof(vec3_t);

        memcpy(m_model.frames[i].translate, CurrReadPos, sizeof(vec3_t));
        CurrReadPos += sizeof(vec3_t);

        memcpy(m_model.frames[i].name, CurrReadPos, sizeof(char) * 16);
        CurrReadPos += sizeof(char) * 16;

        memcpy(m_model.frames[i].verts, CurrReadPos, sizeof(md2_vertex_t) * m_model.header.num_vertices );
        CurrReadPos += sizeof(md2_vertex_t) * m_model.header.num_vertices;
    }

    // todo:
    //Load_Textures(modelDir);

    return true;
}

void UMD2Asset::UnLoad(void)
{
    uint32 i;

    //remove verts 
    for (i = 0; i < (uint32)m_model.header.num_frames; i++)
    {
        delete[] m_model.frames[i].verts;
    }

    // remove the skins
    if (m_skin_data.p_tex != nullptr)
    {
        for (i = 0; i < (uint32)m_model.header.num_skins; i++)
        {
            delete[] m_skin_data.p_tex[i];
        }

        delete[] m_skin_data.p_tex;
    }

    // cleanup the rest
    if (m_model.glcmds != nullptr)
    {
        delete[] m_model.glcmds;
    }

    if (m_model.frames != nullptr)
    {
        delete[] m_model.frames;
    }

    if (m_model.triangles != nullptr)
    {
        delete[] m_model.triangles;
    }

    if (m_model.texcoords != nullptr)
    {
        delete[] m_model.texcoords;
    }

    if (m_model.skins != nullptr)
    {
        delete[] m_model.skins;
    }

    memset(&m_model, 0, sizeof(m_model));
}

void UMD2Asset::Convert(FRawMesh& OutRawMesh)
{
    FVector3f EmptyVector = FVector3f(0, 0, 0);
    FColor WhiteVertex = FColor(255, 255, 255, 255);

    OutRawMesh.VertexPositions.Reserve( m_model.header.num_vertices );
    OutRawMesh.WedgeIndices.Reserve( m_model.header.num_tris * 3 );
    OutRawMesh.WedgeColors.Reserve(m_model.header.num_tris * 3 );
    /*OutRawMesh.WedgeTangentX.Reserve(m_model.header.num_tris * 3 );
    OutRawMesh.WedgeTangentY.Reserve(m_model.header.num_tris * 3 );
    OutRawMesh.WedgeTangentZ.Reserve(m_model.header.num_tris * 3 );*/
    OutRawMesh.FaceMaterialIndices.Reserve(m_model.header.num_tris);
    OutRawMesh.FaceSmoothingMasks.Reserve(m_model.header.num_tris);

    for (int i = 0; i < MAX_MESH_TEXTURE_COORDS; i++)
    {
        OutRawMesh.WedgeTexCoords[i].Reserve(m_model.header.num_tris * 3);
    }

    // for now just use the first frame of animation

    // the actual vert data is stored in the frame
    for (int i = 0; i < m_model.header.num_vertices; i++)
    {
        md2_vertex_t& vert = m_model.frames[0].verts[i];
        
        //todo: Y and Z flipped?
        FVector3f vertPos( (vert.v[0] * m_model.frames->scale[0]) + m_model.frames[0].translate[0],
                           (vert.v[1] * m_model.frames->scale[1]) + m_model.frames[0].translate[1],
                           (vert.v[2] * m_model.frames->scale[2]) + m_model.frames[0].translate[2] );
        
        OutRawMesh.VertexPositions.Emplace( vertPos );
    }

    for (int i = 0; i < m_model.header.num_tris; i++)
    {
        OutRawMesh.WedgeIndices.Emplace(m_model.triangles[i].vertex[0]);
        OutRawMesh.WedgeIndices.Emplace(m_model.triangles[i].vertex[1]);
        OutRawMesh.WedgeIndices.Emplace(m_model.triangles[i].vertex[2]);

        OutRawMesh.WedgeColors.Emplace(WhiteVertex);
        OutRawMesh.WedgeColors.Emplace(WhiteVertex);
        OutRawMesh.WedgeColors.Emplace(WhiteVertex);

        /*OutRawMesh.WedgeTangentX.Emplace(EmptyVector);
        OutRawMesh.WedgeTangentX.Emplace(EmptyVector);
        OutRawMesh.WedgeTangentX.Emplace(EmptyVector);

        OutRawMesh.WedgeTangentY.Emplace(EmptyVector);
        OutRawMesh.WedgeTangentY.Emplace(EmptyVector);
        OutRawMesh.WedgeTangentY.Emplace(EmptyVector);

        OutRawMesh.WedgeTangentZ.Emplace(EmptyVector);
        OutRawMesh.WedgeTangentZ.Emplace(EmptyVector);
        OutRawMesh.WedgeTangentZ.Emplace(EmptyVector);*/

        OutRawMesh.FaceMaterialIndices.Emplace(0);
        OutRawMesh.FaceSmoothingMasks.Emplace(0xFFFFFFFF); // Phong ?

        for (int UVIndex = 0; UVIndex < MAX_MESH_TEXTURE_COORDS; UVIndex++)
        {
            OutRawMesh.WedgeTexCoords[UVIndex].Emplace(FVector2f(0.0f, 0.0f));
            OutRawMesh.WedgeTexCoords[UVIndex].Emplace(FVector2f(0.0f, 0.0f));
            OutRawMesh.WedgeTexCoords[UVIndex].Emplace(FVector2f(0.0f, 0.0f));
        }
    }
}
