#pragma once
#ifndef ZP_RENDERING_PIPELINE_H
#define ZP_RENDERING_PIPELINE_H

class zpRenderingPipeline
{
public:
	zpRenderingPipeline();
	~zpRenderingPipeline();

	zpRenderingEngine* getRenderingEngine() const;

	void beginFrame();

	void submitRendering();
	void submitDebugRendering();

	void endFrame();

	zpMaterialContentManager* getMaterialContentManager();
	zpShaderContentManager* getShaderContentManager();
	zpTextureContentManager* getTextureContentManager();

	void onFocusGained();
	void onFocusLost();

private:
	zpRenderingEngine* m_engine;

	zpMaterialContentManager m_materialContent;
	zpShaderContentManager m_shaderContent;
	zpTextureContentManager m_textureContent;
};

#endif
