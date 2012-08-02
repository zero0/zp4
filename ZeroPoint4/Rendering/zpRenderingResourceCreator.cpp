#include "zpRendering.h"

zpRenderingResourceCreator::zpRenderingResourceCreator() {}
zpRenderingResourceCreator::~zpRenderingResourceCreator() {}

zpResource* zpRenderingResourceCreator::createResource( const zpString& filename ) {
	zpResource* resource = ZP_NULL;
	zpRenderingEngine* engine = zpRenderingFactory::getRenderingEngine();
	
	if( filename.endsWith( ".png" ) || filename.endsWith( ".jpg" ) ) {
		resource = engine->createTextureResource();
	} else if( filename.endsWith( ".shader" ) ) {
		resource = engine->createShaderResource();
	}

	return resource;
}
