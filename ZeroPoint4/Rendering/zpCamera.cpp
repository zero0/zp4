#include "zpRendering.h"

zpCamera::zpCamera()
	: m_projectionType( ZP_CAMERA_PROJECTION_PERSPECTIVE )
	, m_isViewDirty( true )
	, m_isProjectionDirty( true )
	, m_near( 1.f )
	, m_far( 1000.f )
	, m_fovy( ZP_DEG_TO_RAD( 45.f ) )
	, m_aspectRatio( 4.f / 3.f )
	, m_orthoRect()
	, m_up( 0, 1, 0 )
	, m_lookTo( 0, 0, -1 )
	, m_position( 0, 0, 0 )
	, m_frustum()
	, m_view()
	, m_projection()
	, m_bufferData()
{}
zpCamera::~zpCamera()
{}

void zpCamera::update()
{
	zp_bool isViewProjectionDirty = false;

	if( m_isProjectionDirty )
	{
		switch( m_projectionType )
		{
		case ZP_CAMERA_PROJECTION_ORTHO:
			//m_projection.orthoOffset(
			//	m_orthoRect.getPosition().getX(), 
			//	m_orthoRect.getSize().getX() - m_orthoRect.getPosition().getX(), 
			//	m_orthoRect.getSize().getY() - m_orthoRect.getPosition().getY(), 
			//	m_orthoRect.getPosition().getY(), 
			//	m_near, m_far );
			break;
		case ZP_CAMERA_PROJECTION_PERSPECTIVE:
			{
				zpMath::PerspectiveLH( m_projection, zpScalar( m_fovy ), zpScalar( m_aspectRatio ), zpScalar( m_near ), zpScalar( m_far ) );
			}
			break;
		}

		isViewProjectionDirty = true;
		m_isProjectionDirty = false;
	}

	if( m_isViewDirty )
	{
		zpMath::LookAtLH( m_view, m_position, m_lookTo, m_up );

		isViewProjectionDirty = true;
		m_isViewDirty = false;
	}

	if( isViewProjectionDirty )
	{
		m_frustum.setLookTo( m_position, m_lookTo, m_up, m_aspectRatio, m_fovy, m_near, m_far );

		zpMath::Mul( m_bufferData.viewProjection, m_view, m_projection );
		zpMath::Invert( m_bufferData.invViewProjection, m_bufferData.viewProjection );

		m_isDirty = true;
	}
}

void zpCamera::setProjectionType( zpCameraProjection type )
{
	m_projectionType = type;
	m_isProjectionDirty = true;
}

void zpCamera::setNearFar( zp_float nearDist, zp_float farDist )
{
	m_near = nearDist;
	m_far = farDist;
	m_isProjectionDirty = true;
}
void zpCamera::setFovy( zp_float fovy )
{
	m_fovy = fovy;
	m_isProjectionDirty = true;
}
void zpCamera::setAspectRatio( zp_float aspectRatio )
{
	m_aspectRatio = aspectRatio;
	m_isProjectionDirty = true;
}

void zpCamera::setPosition( const zpVector4f& position )
{
	m_position = position;
	m_isViewDirty = true;
}
void zpCamera::setLookTo( const zpVector4f& lookTo )
{
	zpMath::Normalize3( m_lookTo, lookTo );
	m_isViewDirty = true;
}
void zpCamera::setLookAt( const zpVector4f& lookAt )
{
	zpVector4f dir;
	zpMath::Sub( dir, lookAt, m_position );

	setLookTo( dir );
}
void zpCamera::setUp( const zpVector4f& up )
{
	zpMath::Normalize3( m_up, up );
	m_isViewDirty = true;
}

zp_float zpCamera::getAspectRation() const
{
	return m_aspectRatio;
}
zp_float zpCamera::getFovy() const
{
	return m_fovy;
}

const zpVector4f& zpCamera::getPosition() const
{
	return m_position;
}
const zpVector4f& zpCamera::getLookTo() const
{
	return m_lookTo;
}
const zpVector4f& zpCamera::getUp() const
{
	return m_up;
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

zp_bool zpCamera::isDirty() const
{
	return m_isDirty;
}
void zpCamera::unmarkDirty()
{
	m_isDirty = false;
}

zpRay zpCamera::generateRay( const zpVector2i& windowPosition ) const
{
	const zpVector2i& screenSize = zpRenderingFactory::getRenderingEngine()->getWindow()->getScreenSize();

	zp_float x = ( ( 2.0f * (zp_float)windowPosition.getX() ) / (zp_float)screenSize.getX() ) - 1.0f;
	zp_float y = -( ( ( 2.0f * (zp_float)windowPosition.getY() ) / (zp_float)screenSize.getY() ) - 1.0f );

	zpVector4f nearZ( x, y, 0.0f, 1.0f );
	zpVector4f farZ(  x, y, 1.0f, 1.0f );

	zpMath::Mul( nearZ, nearZ, m_bufferData.invViewProjection );
	zpMath::Mul( farZ, farZ, m_bufferData.invViewProjection );
	
	zpMath::Sub( farZ, farZ, nearZ );

	return zpRay( nearZ, farZ );
}
