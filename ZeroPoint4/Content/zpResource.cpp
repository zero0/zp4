#include "zpContent.h"

zpResource::zpResource()
	: m_isLoaded( false )
	, m_manager( ZP_NULL )
	, m_filename()
{}
zpResource::~zpResource()
{
	m_manager = ZP_NULL;
}

zpContentManager* zpResource::getContentManager() const
{
	return m_manager;
}
const zpString& zpResource::getFilename() const
{
	return m_filename;
}
zp_bool zpResource::isLoaded() const
{
	return m_isLoaded;
}

void zpResource::setIsLoaded( zp_bool loaded )
{
	m_isLoaded = loaded;
}

void zpResource::setContentManager( zpContentManager* manager )
{
	m_manager = manager;
}
void zpResource::setFilename( const zpString& filename )
{
	m_filename = filename;
}