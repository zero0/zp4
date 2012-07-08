#pragma once
#ifndef ZP_RENDERING_FACTORY_H
#define ZP_RENDERING_FACTORY_H

class zpRenderingFactory {
public:
	~zpRenderingFactory();

	template<zpRenderingEngineType type>
	static zpRenderingEngine* createRenderingEngine() {
		return createRenderingEngine( type );
	}
	static zpRenderingEngine* createRenderingEngine( zpRenderingEngineType type );

	static zpRenderingEngine* getRenderingEngine();

	static void destroyRenderingEngine();

private:
	static zpRenderingEngine* s_renderingEngine;

	zpRenderingFactory();
};

#endif