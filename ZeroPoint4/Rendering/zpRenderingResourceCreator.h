#pragma once
#ifndef ZP_RENDERING_RESOURCE_CREATOR_H
#define ZP_RENDERING_RESOURCE_CREATOR_H

class zpRenderingResourceCreator : public zpResourceCreator {
public:
	zpRenderingResourceCreator();
	virtual ~zpRenderingResourceCreator();

	zpResource* createResource( const zpString& filename );
};

#endif