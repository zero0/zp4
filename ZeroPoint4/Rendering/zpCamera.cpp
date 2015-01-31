#include "zpRendering.h"

zpCamera::zpCamera()
	: m_projectionType( ZP_CAMERA_PROJECTION_PERSPECTIVE )
	, m_type( ZP_CAMERA_TYPE_MAIN )
	, m_depthStencil( ZP_NULL )
	, m_isActive( false )
	, m_isViewDirty( true )
	, m_isProjectionDirty( true )
	, m_orthoRect()
	, m_clipRect()
	, m_viewport()
	, m_lookAt( 0, 0, 0, 1 )
	, m_clearColor( 0, 0, 0, 0 )
	, m_clearDepth( 1.0f )
	, m_clearStencil( 0 )
	, m_frustum()
	, m_view()
	, m_projection()
	, m_clearMode( ZP_CAMERA_CLEAR_MODE_DEFAULT )
	, m_layers( ZP_RENDERING_LAYER_DEFAULT )
	, m_bufferData()
	, m_order( 0 )
{}
zpCamera::~zpCamera()
{}

zp_bool zpCamera::update()
{
	zp_bool isViewProjectionDirty = false;

	if( m_isProjectionDirty )
	{
		switch( m_projectionType )
		{
		case ZP_CAMERA_PROJECTION_ORTHO:
			{
				zpScalar l( (zp_float)m_orthoRect.getLeft() );
				zpScalar r( (zp_float)m_orthoRect.getRight() );
				zpScalar t( (zp_float)m_orthoRect.getTop() );
				zpScalar b( (zp_float)m_orthoRect.getBottom() );
				zpScalar n( m_bufferData.zNear );
				zpScalar f( m_bufferData.zFar );
				
				zpMath::OrthoLH( m_projection, l, r, t, b, n, f );
			}
			break;
		case ZP_CAMERA_PROJECTION_ORTHO_CENTERED:
			{
				zpScalar l( m_orthoRect.getSize().getX() * -0.5f );
				zpScalar r( m_orthoRect.getSize().getX() *  0.5f );
				zpScalar t( m_orthoRect.getSize().getY() *  0.5f );
				zpScalar b( m_orthoRect.getSize().getY() * -0.5f );
				zpScalar n( m_bufferData.zNear );
				zpScalar f( m_bufferData.zFar );

				zpMath::OrthoLH( m_projection, l, r, t, b, n, f );
			}
			break;
		case ZP_CAMERA_PROJECTION_PERSPECTIVE:
			{
				zpScalar fovy( m_bufferData.fovy );
				zpScalar aspectRatio( m_bufferData.aspectRatio );
				zpScalar n( m_bufferData.zNear );
				zpScalar f( m_bufferData.zFar );

				zpMath::PerspectiveLH( m_projection, fovy, aspectRatio, n, f );
			}
			break;
		}

		isViewProjectionDirty = true;
		m_isProjectionDirty = false;
	}

	if( m_isViewDirty )
	{
		zpMath::LookAtLH( m_view, m_bufferData.position, m_bufferData.lookTo, m_bufferData.up );

		isViewProjectionDirty = true;
		m_isViewDirty = false;
	}

	if( isViewProjectionDirty )
	{
		m_frustum.setLookTo( m_bufferData.position, m_bufferData.lookTo, m_bufferData.up, m_bufferData.aspectRatio, m_bufferData.fovy, m_bufferData.zNear, m_bufferData.zFar );

		zpMath::Mul( m_bufferData.viewProjection, m_view, m_projection );
		zpMath::Invert( m_bufferData.invViewProjection, m_bufferData.viewProjection );
	}

	return isViewProjectionDirty;
}

void zpCamera::setActive( zp_bool active )
{
	m_isActive = active;
}
zp_bool zpCamera::isActive() const
{
	return m_isActive;
}

void zpCamera::setCameraType( zpCameraType type )
{
	m_type = type;
}
zpCameraType zpCamera::getCameraType() const
{
	return m_type;
}

void zpCamera::setProjectionType( zpCameraProjection type )
{
	m_projectionType = type;
	m_isProjectionDirty = true;
}

void zpCamera::setNearFar( zp_float nearDist, zp_float farDist )
{
	m_bufferData.zNear = nearDist;
	m_bufferData.zFar = farDist;
	m_isProjectionDirty = true;
}
void zpCamera::setFovy( zp_float fovy )
{
	m_bufferData.fovy = fovy;
	m_isProjectionDirty = true;
}
void zpCamera::setAspectRatio( zp_float aspectRatio )
{
	m_bufferData.aspectRatio = aspectRatio;
	m_isProjectionDirty = true;
}

void zpCamera::setPosition( const zpVector4f& position )
{
	m_bufferData.position = position;
	m_isViewDirty = true;
}
void zpCamera::setLookTo( const zpVector4f& lookTo )
{
	// set look to
	zpMath::Normalize3( m_bufferData.lookTo, lookTo );

	// calc new look at
	zpVector4f len;
	zpScalar d;
	zpMath::Sub( len, m_lookAt, m_bufferData.position );
	zpMath::Length3( d, len );
	zpMath::Madd( m_lookAt, m_bufferData.position, m_bufferData.lookTo, d );

	m_isViewDirty = true;
}
void zpCamera::setLookAt( const zpVector4f& lookAt )
{
	// set look at
	m_lookAt = lookAt;

	// calc new look to
	zpVector4f dir;
	zpMath::Sub( dir, lookAt, m_bufferData.position );
	zpMath::Normalize3( m_bufferData.lookTo, dir );

	m_isViewDirty = true;
}
void zpCamera::setUp( const zpVector4f& up )
{
	zpMath::Normalize3( m_bufferData.up, up );
	m_isViewDirty = true;
}

zp_float zpCamera::getAspectRation() const
{
	return m_bufferData.aspectRatio;
}
zp_float zpCamera::getFovy() const
{
	return m_bufferData.fovy;
}
zp_float zpCamera::getNearFarDistance() const
{
	return m_bufferData.zFar - m_bufferData.zNear;
}

const zpVector4f& zpCamera::getPosition() const
{
	return m_bufferData.position;
}
const zpVector4f& zpCamera::getLookTo() const
{
	return m_bufferData.lookTo;
}
const zpVector4f& zpCamera::getLookAt() const
{
	return m_lookAt;
}
const zpVector4f& zpCamera::getUp() const
{
	return m_bufferData.up;
}

const zpFrustum& zpCamera::getFrustum() const
{
	return m_frustum;
}

const zpMatrix4f& zpCamera::getView() const
{
	return m_view;
}
const zpMatrix4f& zpCamera::getProjection() const
{
	return m_projection;
}
const zpMatrix4f& zpCamera::getViewProjection() const
{
	return m_bufferData.viewProjection;
}
const zpMatrix4f& zpCamera::getInvViewProjection() const
{
	return m_bufferData.invViewProjection;
}
const zpCameraBufferData& zpCamera::getCameraBufferData() const
{
	return m_bufferData;
}

void zpCamera::generateRay( const zpVector2i& screenSize, const zpVector2i& windowPosition, zpRay& outRay ) const
{
	zp_float x = ( ( 2.0f * (zp_float)windowPosition.getX() ) / (zp_float)screenSize.getX() ) - 1.0f;
	zp_float y = -( ( ( 2.0f * (zp_float)windowPosition.getY() ) / (zp_float)screenSize.getY() ) - 1.0f );

	zpVector4f nearZ( x, y, 0.0f, 1.0f );
	zpVector4f farZ(  x, y, 1.0f, 1.0f );

	zpMath::Mul( nearZ, nearZ, m_bufferData.invViewProjection );
	zpMath::Mul( farZ, farZ, m_bufferData.invViewProjection );
	
	zpMath::Sub( farZ, farZ, nearZ );

	outRay.setOrigin( nearZ );
	outRay.setDirection( farZ );
}

void zpCamera::setRenderLayers( zp_uint layers )
{
	m_layers.set( layers );
}
void zpCamera::setRenderLayer( zp_bool isMarked, zp_uint layerIndex )
{
	m_layers.setMarked( layerIndex, isMarked );
}
zp_bool zpCamera::isRenderLayerSet( zp_uint layerIndex ) const
{
	return m_layers.isMarked( layerIndex );
}
const zpFlag32& zpCamera::getRenderLayers() const
{
	return m_layers;
}

const zpFlag8& zpCamera::getClearMode() const
{
	return m_clearMode;
}
void zpCamera::setClearMode( zp_byte clearMode )
{
	m_clearMode.set( clearMode );
}

const zpColor4f& zpCamera::getClearColor() const
{
	return m_clearColor;
}
void zpCamera::setClearColor( const zpColor4f& color )
{
	m_clearColor = color;
}

zp_float zpCamera::getDepthClear() const
{
	return m_clearDepth;
}
void zpCamera::setDepthClear( zp_float depthClear )
{
	m_clearDepth = depthClear;
}

zp_uint zpCamera::getStencilClear() const
{
	return m_clearStencil;
}
void zpCamera::setStencilClear( zp_uint stencilClear )
{
	m_clearStencil = stencilClear;
}


zp_uint zpCamera::getNumRenderTargets() const
{
	return m_renderTargets.size();
}
zpTexture* const* zpCamera::getRenderTargets() const
{
	return m_renderTargets.begin();
}
void zpCamera::setRenderTarget( zp_uint index, zpTexture* target )
{
	while( m_renderTargets.size() < index + 1 )
	{
		m_renderTargets.pushBack( ZP_NULL );
	}
	m_renderTargets[ index ] = target;
}
void zpCamera::removeRenderTargets()
{
	m_renderTargets.clear();
}

zpDepthStencilBuffer* zpCamera::getDepthStencilBuffer() const
{
	return m_depthStencil;
}
void zpCamera::setDepthStencilBuffer( zpDepthStencilBuffer* depthStencil )
{
	m_depthStencil = depthStencil;
}

void zpCamera::setOrder( zp_int order )
{
	m_order = order;
}
zp_int zpCamera::getOrder() const
{
	return m_order;
}

void zpCamera::cloneCamera( zpCamera* camera )
{
	m_projectionType = camera->m_projectionType;
	m_type = camera->m_type;

	m_orthoRect = camera->m_orthoRect;
	m_clipRect = camera->m_clipRect;

	m_viewport = camera->m_viewport;
	m_lookAt = camera->m_lookAt;

	m_clearColor = camera->m_clearColor;
	m_clearDepth = camera->m_clearDepth;
	m_clearStencil = camera->m_clearStencil;

	m_view = camera->m_view;
	m_projection = camera->m_projection;

	m_clearMode = camera->m_clearMode;
	m_layers = camera->m_layers;

	m_bufferData = camera->m_bufferData;

	m_isViewDirty = true;
	m_isProjectionDirty = true;
}
