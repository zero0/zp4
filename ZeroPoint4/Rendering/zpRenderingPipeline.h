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

	zpCamera* getCamera( zp_int cameraIndex );

private:
	void useCamera( zpRenderingContext* i, zpCamera* camera, zpBuffer* cameraBuffer );

	zpRenderingEngine* m_engine;

	zpMaterialContentManager m_materialContent;
	zpShaderContentManager m_shaderContent;
	zpTextureContentManager m_textureContent;
	zpMeshContentManager m_meshContent;

	zpBuffer m_cameraBuffer;

	zpMaterialResourceInstance m_mat;
	zpMeshResourceInstance m_mesh;

	zpRasterState m_raster;

	zpFixedArrayList< zpCamera, ZP_RENDERING_MAX_CAMERAS > m_cameras;
};

#endif
