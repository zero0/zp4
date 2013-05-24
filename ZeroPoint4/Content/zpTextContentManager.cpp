#include "zpContent.h"

zp_bool zpTextResource::load( const zp_char* filename )
{
	zp_bool loaded = false;
	if( !m_resource )
	{
		m_resource = new zpBison;
		m_filename = filename;

		if( m_filename.endsWith( ".json" ) )
		{
			zpJsonParser parser;
			zpJson json;
			if( parser.parseFile( m_filename, json ) )
			{
				zpDataBuffer bisonData;
				if( zpBison::compileToBuffer( bisonData, json ) )
				{
					loaded = m_resource->readFromBuffer( bisonData );
				}
			}
		}
		else
		{
			loaded = m_resource->readFromFile( m_filename );
		}
	}

	return loaded;
}
void zpTextResource::unload()
{
	ZP_SAFE_DELETE( m_resource );
	m_filename.clear();
}

zp_bool zpTextContentManager::createResource( zpTextResource* res, const zp_char* filename )
{
	return res->load( filename );
}
void zpTextContentManager::destroyResource( zpTextResource* res )
{
	res->unload();
}