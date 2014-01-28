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

	void setProjectionType( zpCameraProjection type );

	void setOrthoRect( const zpRecti& rect ) { m_orthoRect = rect; }

	void setNearFar( zp_float nearDist, zp_float farDist );
	void setFovy( zp_float fovy );
	void setAspectRatio( zp_float aspectRatio );

	void setPosition( const zpVector4f& position );
	void setLookTo( const zpVector4f& lookTo );
	void setLookAt( const zpVector4f& lookAt );
	void setUp( const zpVector4f& up );

	zp_float getAspectRation() const;
	zp_float getFovy() const;
	zp_float getNear() const { return m_bufferData.zNear; }
	zp_float getFar() const { return m_bufferData.zFar; }
	zp_float getNearFarDistance() const;

	const zpVector4f& getPosition() const;
	const zpVector4f& getLookTo() const;
	const zpVector4f& getUp() const;

	const zpFrustum& getFrustum() const;

	const zpMatrix4f& getView() const;
	const zpMatrix4f& getProjection() const;
	const zpMatrix4f& getViewProjection() const;
	const zpMatrix4f& getInvViewProjection() const;
	const zpCameraBufferData& getCameraBufferData() const;

	const zpViewport& getViewport() const { return m_viewport; }
	void setViewport( const zpViewport& viewport ) { m_viewport = viewport; }

	const zpRecti& getClipRect() const { return m_clipRect; }
	void setClipRect( const zpRecti& clip ) { m_clipRect = clip; }

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

private:
	zpCameraProjection m_projectionType;

	zpFixedArrayList< zpTexture*, 4 > m_renderTargets;
	zpDepthStencilBuffer* m_depthStencil;

	zp_bool m_isViewDirty;
	zp_bool m_isProjectionDirty;

	zpRecti m_orthoRect;
	zpRecti m_clipRect;
	zpViewport m_viewport;

	zpColor4f m_clearColor;
	zp_float m_clearDepth;
	zp_uint m_clearStencil;

	zpFrustum m_frustum;

	zpMatrix4f m_view;
	zpMatrix4f m_projection;

	zpFlag8 m_clearMode;
	zpFlag32 m_layers;

	zpCameraBufferData m_bufferData;
};

#endif