#include "zpRendering.h"

zpRenderingResourceCreator::zpRenderingResourceCreator() {}
zpRenderingResourceCreator::~zpRenderingResourceCreator() {}

zpResource* zpRenderingResourceCreator::createResource( const zpString& filename ) {
	zpResource* resource = ZP_NULL;
	if( filename.endsWith( ".png" ) || filename.endsWith( ".jpg" ) ) {
		resource = zpRenderingFactory::getRenderingEngine()->createTextureResource();
	}
	return resource;
}
