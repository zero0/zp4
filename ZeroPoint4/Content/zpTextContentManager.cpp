#include "zpContent.h"

zp_bool zpTextResource::load( const zp_char* filename )
{
    zp_bool loaded = false;

    m_filename = filename;
    if( m_filename.endsWith( ".json" ) )
    {
        zpJson json;
        zpJsonParser parser;
        if( parser.parseFile( m_filename, json ) )
        {
            zpDataBuffer bisonData;
            if( zpBison::compileToBuffer( bisonData, json ) )
            {
                loaded = m_resource.readFromBuffer( bisonData );
            }
        }
    }
    else
    {
        loaded = m_resource.readFromFile( m_filename );
    }

    return loaded;
}
void zpTextResource::unload()
{
    m_filename.clear();
    m_resource.destroy();
}
zp_bool zpTextContentManager::getResourceWithoutLoadJson( zpTextResourceInstance& outInstance, const zpJson& json )
{
    zpDataBuffer data;
    zpBison::compileToBuffer( data, json );

    zpBison* bison;
    zp_bool ok = getResourceWithoutLoad( outInstance, bison );
    bison->readFromBuffer( data );
    
    return ok;
}

zp_bool zpTextContentManager::createResource( zpTextResource* res, const zp_char* filename )
{
    return res->load( filename );
}
void zpTextContentManager::destroyResource( zpTextResource* res )
{
    res->unload();
}
