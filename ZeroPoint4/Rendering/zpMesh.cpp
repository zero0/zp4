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
		const zpBison::Value& root = mesh.root();

		const zpBison::Value format = root[ "Format" ];
		const zp_char* formatStr = format.asCString();
		const zp_uint formatStrLen = format.size();

		zp_char formatDesc[ 4 ];
		zp_uint i = 0;
		for( ; i < formatStrLen; ++i )
		{
			formatDesc[ i ] = formatStr[ i ];
		}
		for( ; i < 4; ++i )
		{
			formatDesc[ i ] = '\0';
		}

		zpVertexFormatDesc vertFormat = (zpVertexFormatDesc)ZP_MAKE_UINT( formatDesc[0], formatDesc[1], formatDesc[2], formatDesc[3] );
		zp_uint stride = 0;
		switch( vertFormat )
		{
		case ZP_VERTEX_FORMAT_DESC_VERTEX_COLOR:
			m_resource.m_format = ZP_VERTEX_FORMAT_VERTEX_COLOR;
			stride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_COLOR;
			break;
		case ZP_VERTEX_FORMAT_DESC_VERTEX_UV:
			m_resource.m_format = ZP_VERTEX_FORMAT_VERTEX_UV;
			stride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_UV;
			break;
		case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV:
			m_resource.m_format = ZP_VERTEX_FORMAT_VERTEX_COLOR_UV;
			stride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_COLOR_UV;
			break;
		case ZP_VERTEX_FORMAT_DESC_VERTEX_NORMAL_UV:
			m_resource.m_format = ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV;
			stride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_NORMAL_UV;
			break;
		case ZP_VERTEX_FORMAT_DESC_VERTEX_NORMAL_UV2:
			m_resource.m_format = ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2;
			stride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_NORMAL_UV2;
			break;
		default:
			ZP_ASSERT( false, "" );
			break;
		}

		const zpBison::Value vertex = root[ "Vertex" ];
		const zpBison::Value index = root[ "Index" ];

		//m_resource.m_vertex = pipeline->getRenderingEngine()->createBuffer( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_IMMUTABLE, vertex.size(), 0, vertex.asData() );
		//m_resource.m_index  = pipeline->getRenderingEngine()->createBuffer( ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_IMMUTABLE, index.size(), sizeof( zp_short ), index.asData() );
		pipeline->getRenderingEngine()->createBuffer( m_resource.m_vertex, ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_IMMUTABLE, vertex.size(), 0, vertex.asData() );
		pipeline->getRenderingEngine()->createBuffer( m_resource.m_index, ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_IMMUTABLE, index.size(), sizeof( zp_short ), index.asData() );

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
				mp.m_boundingBox = *box;
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
