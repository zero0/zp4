#include "zpRendering.h"

zpCamera::zpCamera() :
	m_projectionType( ZP_CAMERA_PROJECTION_PERSPECTIVE ),
	m_isViewDirty( true ),
	m_isProjectionDirty( true ),
	m_near( 1.f ),
	m_far( 1000.f ),
	m_up( 0, 1, 0 ),
	m_lookAt( 0, 0, -1 ),
	m_position( 0, 0, 0 ),
	m_frustum(),
	m_view(),
	m_projection(),
	m_viewProjection(),
	m_invViewProjection()
{}
zpCamera::~zpCamera() {}

void zpCamera::update() {
	zp_bool isViewProjectionDirty = false;

	if( m_isProjectionDirty ) {
		switch( m_projectionType ) {
		case ZP_CAMERA_PROJECTION_ORTHO:
			m_projection.ortho( 0, 800, 600, 0, m_near, m_far );
			break;
		case ZP_CAMERA_PROJECTION_PERSPECTIVE:
			m_projection.perspective( ( m_fovy ), m_aspectRatio, m_near, m_far );
			break;
		}

		isViewProjectionDirty = true;
		m_isProjectionDirty = false;
	}

	if( m_isViewDirty ) {
		m_view.lookAt( m_position, m_lookAt, m_up );

		isViewProjectionDirty = true;
		m_isViewDirty = false;
	}

	if( isViewProjectionDirty ) {
		m_frustum.set( m_position, m_lookAt, m_up, m_aspectRatio, m_fovy, m_near, m_far );

		m_viewProjection = m_view * m_projection;
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
void zpCamera::setLookAt( const zpVector4f& lookAt ) {
	m_lookAt = lookAt;
	m_isViewDirty = true;
}
void zpCamera::setUp( const zpVector4f& up ) {
	m_up;
	m_isViewDirty = true;
}
void zpCamera::set( const zpVector4f& position, const zpVector4f& lookAt, const zpVector4f& up ) {
	m_position = position;
	m_lookAt = lookAt;
	m_up = up;
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
const zpVector4f& zpCamera::getLookAt() const {
	return m_lookAt;
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