#pragma once
#ifndef ZP_RENDERING_PIPELINE_H
#define ZP_RENDERING_PIPELINE_H

class zpRenderingPipeline
{
public:
	zpRenderingPipeline();
	~zpRenderingPipeline();

	zpRenderingEngine* getRenderingEngine() const;

	void submitRendering();
	void submitDebugRendering();

	void finalize();

private:
	zpRenderingEngine* m_engine;

	zpShaderContentManager m_shaderContent;
	zpTextureContentManager m_textureContent;
};

#endif