#pragma once
#ifndef ZP_CAMERA_H
#define ZP_CAMERA_H

enum zpCameraProjection : zp_byte
{
	ZP_CAMERA_PROJECTION_ORTHO,
	ZP_CAMERA_PROJECTION_ORTHO_CENTERED,
	ZP_CAMERA_PROJECTION_PERSPECTIVE,

	zpCameraProjection_Count,
};
enum zpCameraClearMode : zp_byte
{
	ZP_CAMERA_CLEAR_MODE_NONE =				0,
	ZP_CAMERA_CLEAR_MODE_COLOR =			( 1 << 0 ),
	ZP_CAMERA_CLEAR_MODE_DEPTH =			( 1 << 1 ),
	ZP_CAMERA_CLEAR_MODE_STENCIL =			( 1 << 2 ),
	ZP_CAMERA_CLEAR_MODE_BEFORE_RENDER =	( 1 << 3 ),
	
	ZP_CAMERA_CLEAR_MODE_DEFAULT = ZP_CAMERA_CLEAR_MODE_COLOR | ZP_CAMERA_CLEAR_MODE_DEPTH | ZP_CAMERA_CLEAR_MODE_STENCIL | ZP_CAMERA_CLEAR_MODE_BEFORE_RENDER,
};

class zpCamera
{
public:
	zpCamera();
	~zpCamera();

	zp_bool update();

	void setActive( zp_bool active );
	zp_bool isActive() const;

	void setCameraType( zpCameraType type );
	zpCameraType getCameraType() const;

	void setProjectionType( zpCameraProjection type );

	void setOrthoRect( const zpRecti& rect );

	void setNearFar( zp_float nearDist, zp_float farDist );
	void setFovy( zp_float fovy );
	void setAspectRatio( zp_float aspectRatio );

	void setPosition( zpVector4fParamF position );
	void setLookTo( zpVector4fParamF lookTo );
	void setLookAt( zpVector4fParamF lookAt );
	void setUp( zpVector4fParamF up );

	zp_float getAspectRation() const;
	zp_float getFovy() const;
	zp_float getNear() const;
	zp_float getFar() const;
	zp_float getNearFarDistance() const;

	zpVector4f getPosition() const;
	zpVector4f getLookTo() const;
	zpVector4f getLookAt() const;
	zpVector4f getUp() const;

	const zpFrustum& getFrustum() const;

	zpMatrix4f getView() const;
	zpMatrix4f getProjection() const;
	zpMatrix4f getViewProjection() const;
	zpMatrix4f getInvViewProjection() const;
	const zpCameraBufferData& getCameraBufferData() const;

	const zpViewport& getViewport() const;
	void setViewport( const zpViewport& viewport );

	const zpRecti& getClipRect() const;
	void setClipRect( const zpRecti& clip );

	void generateRay( const zpVector2i& screenSize, const zpVector2i& windowPosition, zpRay& outRay ) const;

	void setRenderLayers( zp_uint layers );
	void setRenderLayer( zp_bool isMarked, zp_uint layerIndex );
	zp_bool isRenderLayerSet( zp_uint layerIndex ) const;
	const zpFlag32& getRenderLayers() const;

	const zpFlag8& getClearMode() const;
	void setClearMode( zp_byte clearMode );

	const zpColor4f& getClearColor() const;
	void setClearColor( const zpColor4f& color );

	zp_float getDepthClear() const;
	void setDepthClear( zp_float depthClear );

	zp_uint getStencilClear() const;
	void setStencilClear( zp_uint stencilClear );

	zp_uint getNumRenderTargets() const;
	zpTexture* const* getRenderTargets() const;
	void setRenderTarget( zp_uint index, zpTexture* target );
	void removeRenderTargets();

	zpDepthStencilBuffer* getDepthStencilBuffer() const;
	void setDepthStencilBuffer( zpDepthStencilBuffer* depthStencil );

	void setOrder( zp_int order );
	zp_int getOrder() const;

	void cloneCamera( zpCamera* camera );

private:
	zpCameraProjection m_projectionType;
	zpCameraType m_type;

	zpFixedArrayList< zpTexture*, 4 > m_renderTargets;
	zpDepthStencilBuffer* m_depthStencil;

	zp_bool m_isActive;
	zp_bool m_isViewDirty;
	zp_bool m_isProjectionDirty;

	zpRecti m_orthoRect;
	zpRecti m_clipRect;
	zpViewport m_viewport;

	zpVector4f m_lookAt;

	zpColor4f m_clearColor;
	zp_float m_clearDepth;
	zp_uint m_clearStencil;

	zpFrustum m_frustum;

	zpMatrix4f m_view;
	zpMatrix4f m_projection;

	zpFlag8 m_clearMode;
	zpFlag32 m_layers;

	zp_int m_order;

	zpCameraBufferData m_bufferData;
};

#endif