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
	} else if( filename.endsWith( ".obj" ) ) {
		resource = new zpOBJStaticMeshResource;
	} else if( filename.endsWith( ".fnt" ) ) {
		resource = new zpFontResource;
	} else {
		zpLog::warning() << "Unknown file extension for file: " << filename << zpLog::endl;
	}

	return resource;
}
