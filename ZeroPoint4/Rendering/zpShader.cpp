#include "zpRendering.h"
#include "zpRenderingImpl.inl"
#include "Common/zpCommon.h"

zpShader::zpShader()
	: m_shader( ZP_NULL )
{}
zpShader::~zpShader()
{
	m_shader = ZP_NULL;
}

zpShaderImpl* zpShader::getShaderImpl() const
{
	return m_shader;
}


zp_bool zpShaderResource::load( const zp_char* filename, zpRenderingEngine* engine )
{
	zp_bool ok;
	m_filename = filename;

	zpBison shaderData;
	if( m_filename.endsWith( ".json" ) )
	{
		zpJson json;
		zpJsonParser parser;
		ok = parser.parseFile( m_filename, json );
		ZP_ASSERT( ok, "" );

		zpDataBuffer data;
		zpBison::compileToBuffer( data, json );

		ok = shaderData.readFromBuffer( data );
	}
	else
	{
		ok = shaderData.readFromFile( m_filename );
	}

	ZP_ASSERT( ok, "Failed to read shader '%s'", getFilename().str() );

	ok = engine->createShader( m_resource );
	ZP_ASSERT( ok, "Failed to create shader '%s'", getFilename().str() );

	ok = engine->loadShader( m_resource, shaderData.root() );
	ZP_ASSERT( ok, "Failed to build shader '%s'", getFilename().str() );

	return ok;
}
void zpShaderResource::unload( zpRenderingEngine* engine )
{
	zp_bool ok;
	ok = engine->destroyShader( m_resource );
	ZP_ASSERT( ok, "Failed to destroy shader '%s'", getFilename().str() );
}


zp_bool zpShaderContentManager::createResource( zpShaderResource* res, const zp_char* filename )
{
	return res->load( filename, getApplication()->getRenderPipeline()->getRenderingEngine() );
}
void zpShaderContentManager::destroyResource( zpShaderResource* res )
{
	res->unload( getApplication()->getRenderPipeline()->getRenderingEngine() );
}
