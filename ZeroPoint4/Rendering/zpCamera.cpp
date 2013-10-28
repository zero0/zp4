#include "zpRendering.h"

zpCamera::zpCamera()
	: m_projectionType( ZP_CAMERA_PROJECTION_PERSPECTIVE )
	, m_isViewDirty( true )
	, m_isProjectionDirty( true )
	, m_orthoRect()
	, m_clipRect()
	, m_viewport()
	, m_frustum()
	, m_view()
	, m_projection()
	, m_bufferData()
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
	zpMath::Normalize3( m_bufferData.lookTo, lookTo );
	m_isViewDirty = true;
}
void zpCamera::setLookAt( const zpVector4f& lookAt )
{
	zpVector4f dir;
	zpMath::Sub( dir, lookAt, m_bufferData.position );

	setLookTo( dir );
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
