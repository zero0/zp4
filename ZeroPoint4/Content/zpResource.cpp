#include "zpContent.h"

zpResource::zpResource() : m_manager( ZP_NULL ) {}
zpResource::~zpResource() {
	m_manager = ZP_NULL;
}

zpContentManager* zpResource::getContentManager() const {
	return m_manager;
}
void zpResource::setContentManager( zpContentManager* manager ) {
	manager = m_manager;
}

const zpString& zpResource::getFilename() const {
	return m_filename;
}
void zpResource::setFilename( const zpString& filename ) {
	m_filename = filename;
}