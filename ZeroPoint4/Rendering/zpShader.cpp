#include "zpRendering.h"

zpShader::zpShader( zpShaderImpl* impl )
	: m_shader( impl )
{}
zpShader::~zpShader()
{
	ZP_SAFE_DELETE( m_shader );
}

zpShaderImpl* zpShader::getShaderImpl() const
{
	return m_shader;
}


zp_bool zpShaderResource::load( const zp_char* filename )
{
	zpRenderingEngine* engine = zpRenderingFactory::getRenderingEngine();
	if( m_resource == ZP_NULL )
	{
		m_resource = engine->createShader();
	}

	zp_bool ok;
	m_filename = filename;
	ok = m_shaderData.readFromFile( m_filename );
	ZP_ASSERT( ok, "" );

	ok = engine->loadShader( m_resource, m_shaderData );
	ZP_ASSERT( ok, "" );

	return ok;
}
void zpShaderResource::unload()
{
	if( m_resource != ZP_NULL )
	{
		zpRenderingEngine* engine = zpRenderingFactory::getRenderingEngine();
		engine->destroyShader( m_resource );
	}
}


zp_bool zpShaderContentManager::createResource( zpShaderResource* res, const zp_char* filename )
{
	return res->load( filename );
}
void zpShaderContentManager::destroyResource( zpShaderResource* res )
{
	res->unload();
}
