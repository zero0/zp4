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

enum zpScreenshotType
{
	ZP_SCREENSHOT_TYPE_NONE,

	ZP_SCREENSHOT_TYPE_ALL,
	ZP_SCREENSHOT_TYPE_NO_UI,

	zpScreenshotType_Count,

	ZP_SCREENSHOT_TYPE_PROCESSING,
};

class zpRenderingPipeline
{
public:
	zpRenderingPipeline();
	~zpRenderingPipeline();

	const zpRenderingEngine* getRenderingEngine() const;
	zpRenderingEngine* getRenderingEngine();

	void initialize();
	void setup();
	void teardown();
	void shutdown();

	void update();

	void beginFrame( zpRenderingContext* i, zpTime* time );
	void submitRendering( zpRenderingContext* i );
	void endFrame( zpRenderingContext* i );
	void present();

	zpMaterialContentManager* getMaterialContentManager();
	zpShaderContentManager* getShaderContentManager();
	zpTextureContentManager* getTextureContentManager();
	zpMeshContentManager* getMeshContentManager();
	zpSkeletonContentManager* getSkeletonContentManager();
	zpAnimationContentManager* getAnimationContentManager();
	zpFontContentManager* getFontContentManager();

	void onFocusGained();
	void onFocusLost();

	void generateSamplerStateDesc( const zpBison::Value& sampler, zpSamplerStateDesc& outSamplerDesc );
	void generateRasterStateDesc( const zpBison::Value& raster, zpRasterStateDesc& outRasterDesc );
	void generateBlendStateDesc( const zpBison::Value& blend, zpBlendStateDesc& outBlendStateDesc );
	void generateDepthStencilStateDesc( const zpBison::Value& depthStencil, zpDepthStencilStateDesc& outDepthStencilStateDesc );

	void enterEditMode();
	void leaveEditMode();

	zp_size_t getNumCameras( zpCameraType type ) const;
	const zpArrayList< zpCamera* >& getUsedCameras( zpCameraType type ) const;

	zpCamera* getCamera( zpCameraType type );
	void releaseCamera( zpCamera* camera );

	void setApplication( zpApplication* app ) { m_application = app; };
	zpApplication* getApplication() const { return m_application; };

	zp_bool takeScreenshot( zpScreenshotType type, const zp_char* directoryPath, zp_long time );

	zpLightBufferData* getLight( zpLightType type );
	void releaseLight( zpLightBufferData* light );

	void processRenderingQueue( zpRenderingQueue layer, zp_bool useLighting );

private:
	void useCamera( zpRenderingContext* i, zpCamera* camera, zpBuffer* cameraBuffer );

	zp_bool performScreenshot();

	zpApplication* m_application;
	zpRenderingEngine m_engine;

	zpMaterialContentManager m_materialContent;
	zpShaderContentManager m_shaderContent;
	zpTextureContentManager m_textureContent;
	zpMeshContentManager m_meshContent;
	zpSkeletonContentManager m_skeletonContent;
	zpAnimationContentManager m_animationContent;
	zpFontContentManager m_fontContent;

	zpBuffer m_constantBuffers[ zpConstantBufferSlot_Count ];

	zpRasterState m_raster;
	zpBlendState m_alphaBlend;

	zpFontResourceInstance m_debugFont;

	zpCamera* m_prevCamera;
	zpCamera* m_debugCamera;
	zpCamera* m_debugUICamera;

	zpFixedArrayList< zpCamera, 8 > m_cameras;
	zpFixedArrayList< zpCamera*, 8 > m_freeCameras;
	zpArrayList< zpCamera* > m_usedCameras[ zpCameraType_Count ];

	zpFixedArrayList< zpLightBufferData, 64 > m_lights;
	zpFixedArrayList< zpLightBufferData*, 64 > m_freeLights;
	zpArrayList< zpLightBufferData* > m_usedLights[ zpLightType_Count ];

	zpLightBufferData* m_dirLight;

	zpScreenshotType m_screenshotType;
	zpString m_screenshotFilename;
};

#endif
