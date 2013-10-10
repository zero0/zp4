#pragma once
#ifndef ZP_RENDERING_PIPELINE_H
#define ZP_RENDERING_PIPELINE_H

enum zpScreenQuad
{
	ZP_SCREEN_QUAD_FULL,
	ZP_SCREEN_QUAD_UPPER_LEFT,
	ZP_SCREEN_QUAD_UPPER_RIGHT,
	ZP_SCREEN_QUAD_LOWER_RIGHT,
	ZP_SCREEN_QUAD_LOWER_LEFT,
};

class zpRenderingPipeline
{
public:
	zpRenderingPipeline();
	~zpRenderingPipeline();

	zpRenderingEngine* getRenderingEngine() const;

	void initialize();
	void destroy();

	void beginFrame();

	void submitRendering();
	void submitDebugRendering();

	void endFrame();

	zpMaterialContentManager* getMaterialContentManager();
	zpShaderContentManager* getShaderContentManager();
	zpTextureContentManager* getTextureContentManager();
	zpMeshContentManager* getMeshContentManager();

	void onFocusGained();
	void onFocusLost();

	void generateSamplerStateDesc( const zpBison::Value& sampler, zpSamplerStateDesc& outSamplerDesc );
	void generateRasterStateDesc( const zpBison::Value& raster, zpRasterStateDesc& outRasterDesc );

private:
	zpRenderingEngine* m_engine;

	zpMaterialContentManager m_materialContent;
	zpShaderContentManager m_shaderContent;
	zpTextureContentManager m_textureContent;
	zpMeshContentManager m_meshContent;

	zpBuffer m_cameraBuffer;

	zpCamera* m_currentCamera;
	zpViewport m_viewport;

	zpMaterialResourceInstance m_mat;
	zpMeshResourceInstance m_mesh;

	zpRasterState m_raster;
};

#endif
