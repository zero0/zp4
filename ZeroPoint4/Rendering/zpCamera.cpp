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
	, m_viewProjection()
	, m_invViewProjection()
{}
zpCamera::~zpCamera() {}

void zpCamera::update() {
	zp_bool isViewProjectionDirty = false;

	if( m_isProjectionDirty ) {
		switch( m_projectionType ) {
		case ZP_CAMERA_PROJECTION_ORTHO:
			m_projection.orthoOffset(
				m_orthoRect.getPosition().getX(), 
				m_orthoRect.getSize().getX() - m_orthoRect.getPosition().getX(), 
				m_orthoRect.getSize().getY() - m_orthoRect.getPosition().getY(), 
				m_orthoRect.getPosition().getY(), 
				m_near, m_far );
			break;
		case ZP_CAMERA_PROJECTION_PERSPECTIVE:
			m_projection.perspective( m_fovy, m_aspectRatio, m_near, m_far );
			break;
		}

		isViewProjectionDirty = true;
		m_isProjectionDirty = false;
	}

	if( m_isViewDirty ) {
		m_view.lookTo( m_position, m_lookTo, m_up );

		isViewProjectionDirty = true;
		m_isViewDirty = false;
	}

	if( isViewProjectionDirty ) {
		m_frustum.setLookTo( m_position, m_lookTo, m_up, m_aspectRatio, m_fovy, m_near, m_far );

		m_view.mul( m_projection, m_viewProjection );

		m_invViewProjection = m_viewProjection;
		m_invViewProjection.invert();
		m_isDirty = true;
	}
}

void zpCamera::setProjectionType( zpCameraProjection type ) {
	m_projectionType = type;
	m_isProjectionDirty = true;
}

void zpCamera::setNearFar( zp_float nearDist, zp_float farDist ) {
	m_near = nearDist;
	m_far = farDist;
	m_isProjectionDirty = true;
}
void zpCamera::setFovy( zp_float fovy ) {
	m_fovy = fovy;
	m_isProjectionDirty = true;
}
void zpCamera::setAspectRatio( zp_float aspectRatio ) {
	m_aspectRatio = aspectRatio;
	m_isProjectionDirty = true;
}

void zpCamera::setPosition( const zpVector4f& position ) {
	m_position = position;
	m_isViewDirty = true;
}
void zpCamera::setLookTo( const zpVector4f& lookTo ) {
	m_lookTo = lookTo;
	m_lookTo.normalize3();
	m_isViewDirty = true;
}
void zpCamera::setLookAt( const zpVector4f& lookAt ) {
	setLookTo( lookAt - m_position );
}
void zpCamera::setUp( const zpVector4f& up ) {
	m_up;
	m_isViewDirty = true;
}

zp_float zpCamera::getAspectRation() const {
	return m_aspectRatio;
}
zp_float zpCamera::getFovy() const {
	return m_fovy;
}

const zpVector4f& zpCamera::getPosition() const {
	return m_position;
}
const zpVector4f& zpCamera::getLookTo() const {
	return m_lookTo;
}
const zpVector4f& zpCamera::getUp() const {
	return m_up;
}

const zpFrustum& zpCamera::getFrustum() const {
	return m_frustum;
}

const zpMatrix4f& zpCamera::getView() const {
	return m_view;
}
const zpMatrix4f& zpCamera::getProjection() const {
	return m_projection;
}
const zpMatrix4f& zpCamera::getViewProjection() const {
	return m_viewProjection;
}
const zpMatrix4f& zpCamera::getInvViewProjection() const {
	return m_invViewProjection;
}

zp_bool zpCamera::isDirty() const {
	return m_isDirty;
}
void zpCamera::unmarkDirty() {
	m_isDirty = false;
}

zpRay zpCamera::generateRay( const zpVector2i& windowPosition ) const {
	const zpVector2i& screenSize = zpRenderingFactory::getRenderingEngine()->getWindow()->getScreenSize();

	zp_float x = ( ( 2.f * (zp_float)windowPosition.getX() ) / (zp_float)screenSize.getX() ) - 1.f;
	zp_float y = -( ( ( 2.f * (zp_float)windowPosition.getY() ) / (zp_float)screenSize.getY() ) - 1.f );

	zpVector4f nearZ( x, y, 0.f, 1.f );
	zpVector4f farZ( x, y, 1.f, 1.f );

	m_invViewProjection.mul( nearZ, nearZ );
	nearZ.homogenize4();
	
	m_invViewProjection.mul( farZ, farZ );
	farZ.homogenize4();

	return zpRay( nearZ, farZ - nearZ );
}
