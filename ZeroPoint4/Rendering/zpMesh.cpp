#include "zpRendering.h"
#include "Common/zpCommon.h"

zp_bool zpMeshResource::load( const zp_char* filename, zpRenderingPipeline* pipeline )
{
    zp_bool ok;
    m_filename = filename;

    zpBison mesh;
    ok = mesh.readFromFile( m_filename );
    
    if( ok )
    {
        zpRenderingEngine* engine = pipeline->getRenderingEngine();

        const zpBison::Value& root = mesh.root();

        const zpBison::Value format = root[ "Format" ];
        const zp_char* formatStr = format.asCString();
        const zp_uint formatStrLen = format.size();

        zp_uint stride;
        zp_bool found = engine->getInputLayoutFormatAndStride( formatStr, m_resource.m_format, stride );
        ZP_ASSERT( found, "Unable to find Input Layout %s for mesh %s", formatStr, filename );

        const zpBison::Value& vertex = root[ "Vertex" ];
        const zpBison::Value& index = root[ "Index" ];

        //m_resource.m_vertexData.writeBulk( vertex.asData(), vertex.size() );
        //m_resource.m_indexData.writeBulk( index.asData(), index.size() );

        engine->createBuffer( m_resource.m_vertex, ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_IMMUTABLE, vertex.size(), stride, vertex.asData() );
        engine->createBuffer( m_resource.m_index, ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_IMMUTABLE, index.size(), sizeof( zp_ushort ), index.asData() );

        const zpBison::Value parts = root[ "Parts" ];
        m_resource.m_parts.reserve( parts.size() );

        for( zp_uint i = 0; i < parts.size(); ++i )
        {
            zpMeshPart& mp = m_resource.m_parts.pushBackEmpty();

            const zpBison::Value v = parts[ i ];
            mp.m_indexOffset = v[ "IndexOffset" ].asInt();
            mp.m_indexCount= v[ "IndexCount" ].asInt();
            mp.m_vertexOffset = v[ "VertexOffset" ].asInt();
            mp.m_vertexCount = v[ "VertexCount" ].asInt();

            zpBoundingAABB* box = (zpBoundingAABB*)v[ "BoundingBox" ].asData();
            if( box )
            {
                //mp.m_boundingBox = *box;
            }
        }
    }

    return ok;
}
void zpMeshResource::unload( zpRenderingEngine* engine )
{
    engine->destroyBuffer( m_resource.m_vertex );
    engine->destroyBuffer( m_resource.m_index );

    m_resource.m_format = ZP_VERTEX_FORMAT_VERTEX_COLOR;
    //m_resource.m_vertex = ZP_NULL;
    //m_resource.m_index = ZP_NULL;
    m_resource.m_parts.clear();
}

zp_bool zpMeshContentManager::createResource( zpMeshResource* res, const zp_char* filename )
{
    return res->load( filename, getApplication()->getRenderPipeline() );
}
void zpMeshContentManager::destroyResource( zpMeshResource* res )
{
    res->unload( getApplication()->getRenderPipeline()->getRenderingEngine() );
}
