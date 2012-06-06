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

	static void destroyRenderingEngine( zpRenderingEngine* engine );

private:
	static zpRenderingEngine* createRenderingEngine( zpRenderingEngineType type );

	zpRenderingFactory();
};

#endif