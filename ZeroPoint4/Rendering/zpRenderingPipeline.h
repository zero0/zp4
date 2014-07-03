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
};

class zpRenderingPipeline
{
public:
	zpRenderingPipeline();
	~zpRenderingPipeline();

	zpRenderingEngine* getRenderingEngine() const;

	void initialize();
	void destroy();

	void update();

	void beginFrame( zpRenderingContext* i );
	void submitRendering( zpRenderingContext* i );
	void endFrame( zpRenderingContext* i );
	void present();

	zpMaterialContentManager* getMaterialContentManager();
	zpShaderContentManager* getShaderContentManager();
	zpTextureContentManager* getTextureContentManager();
	zpMeshContentManager* getMeshContentManager();
	zpFontContentManager* getFontContentManager();

	void onFocusGained();
	void onFocusLost();

	void generateSamplerStateDesc( const zpBison::Value& sampler, zpSamplerStateDesc& outSamplerDesc );
	void generateRasterStateDesc( const zpBison::Value& raster, zpRasterStateDesc& outRasterDesc );
	void generateBlendStateDesc( const zpBison::Value& blend, zpBlendStateDesc& outBlendStateDesc );
	void generateDepthStencilStateDesc( const zpBison::Value& depthStencil, zpDepthStencilStateDesc& outDepthStencilStateDesc );

	void enterEditMode();
	void leaveEditMode();

	zp_uint getNumCameras() const { return m_cameras.size(); }
	zpCamera* getCamera( zpCameraType type );
	void setCameraActive( zpCameraType type, zp_bool active );

	template< typename T >
	T* pushCameraState( zpCameraType type )
	{
		T* state = new T;
		pushCameraState( type, state );
		return state;
	};
	void popCameraState( zpCameraType type );

	void setApplication( zpApplication* app ) { m_application = app; };
	zpApplication* getApplication() const { return m_application; };

	zp_bool takeScreenshot( zpScreenshotType type, const zp_char* directoryPath );

	zpLightBufferData* getLight( zpLightType type );
	void releaseLight( zpLightBufferData* light );

	void processRenderingQueue( zpRenderingQueue layer, zp_bool useLighting );

private:
	void pushCameraState( zpCameraType type, zpCameraState* state );

	void useCamera( zpRenderingContext* i, zpCamera* camera, zpBuffer* cameraBuffer );
	void renderCamera( zpRenderingContext* i, zpCamera* camera );

	zp_bool performScreenshot();

	zpApplication* m_application;
	zpRenderingEngine* m_engine;

	zpMaterialContentManager m_materialContent;
	zpShaderContentManager m_shaderContent;
	zpTextureContentManager m_textureContent;
	zpMeshContentManager m_meshContent;
	zpFontContentManager m_fontContent;

	zpBuffer m_cameraBuffer;
	zpBuffer m_perFrameBuffer;
	zpBuffer m_perDrawCallBuffer;
	zpBuffer m_lightBuffer;

	zpRasterState m_raster;
	zpBlendState m_alphaBlend;

	zpFontResourceInstance m_debugFont;

	zpCamera* m_prevCamera;
	zpFixedArrayList< zpCamera, zpCameraType_Count > m_cameras;
	zpFixedArrayList< zpArrayList< zpCameraState* >, zpCameraType_Count > m_cameraStack;

	zpFixedArrayList< zpLightBufferData, 64 > m_lights;
	zpFixedArrayList< zpLightBufferData*, 64 > m_freeLights;
	zpArrayList< zpLightBufferData* > m_usedLights[ zpLightType_Count ];

	zpLightBufferData* m_dirLight;

	zp_bool m_screenshotTaken;
	zpScreenshotType m_screenshotType;
	zpString m_screenshotFilename;
};

#endif
