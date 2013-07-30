#include "zpRendering.h"
#include "zpRenderingImpl.inl"

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


zp_bool zpShaderResource::load( const zp_char* filename )
{
	zpRenderingEngine* engine = zpRenderingFactory::getRenderingEngine();
	if( m_resource.getShaderImpl() == ZP_NULL )
	{
		engine->createShader( &m_resource );
	}

	zp_bool ok;
	m_filename = filename;

	zpBison shaderData;
	ok = shaderData.readFromFile( m_filename );
	ZP_ASSERT( ok, "" );

	ok = engine->loadShader( &m_resource, shaderData );
	ZP_ASSERT( ok, "" );

	return ok;
}
void zpShaderResource::unload()
{
	//if( m_resource )
	{
		zpRenderingEngine* engine = zpRenderingFactory::getRenderingEngine();
		engine->destroyShader( &m_resource );
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
