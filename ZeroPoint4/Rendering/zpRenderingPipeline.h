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

private:
	void pushCameraState( zpCameraType type, zpCameraState* state );

	void useCamera( zpRenderingContext* i, zpCamera* camera, zpBuffer* cameraBuffer );
	void renderCamera( zpRenderingContext* i, zpCamera* camera );

	zpApplication* m_application;
	zpRenderingEngine* m_engine;

	zpMaterialContentManager m_materialContent;
	zpShaderContentManager m_shaderContent;
	zpTextureContentManager m_textureContent;
	zpMeshContentManager m_meshContent;
	zpFontContentManager m_fontContent;

	zpBuffer m_cameraBuffer;
	zpBuffer m_perFrameBuffer;

	zpMaterialResourceInstance m_mat;
	zpMeshResourceInstance m_mesh;

	zpRasterState m_raster;
	zpBlendState m_blend;

	zpFontResourceInstance m_debugFont;

	zpCamera* m_prevCamera;
	zpFixedArrayList< zpCamera, zpCameraType_Count > m_cameras;
	zpFixedArrayList< zpArrayList< zpCameraState* >, zpCameraType_Count > m_cameraStack;
};

#endif
