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
	m_view(),
	m_projection(),
	m_viewProjection(),
	m_invViewProjection()
{}
zpCamera::~zpCamera() {}

void zpCamera::render() {}

void zpCamera::receiveMessage( const zpMessage& message ) {}

void zpCamera::serialize( zpSerializedOutput* out ) {}
void zpCamera::deserialize( zpSerializedInput* in ) {}

void zpCamera::onCreate() {}
void zpCamera::onDestroy() {}

void zpCamera::onUpdate() {
	zp_bool isViewProjectionDirty = false;

	if( m_isProjectionDirty ) {
		switch( m_projectionType ) {
		case ZP_CAMERA_PROJECTION_ORTHO:
			m_projection.ortho( 0, 0, 0, 0, m_near, m_far );
			break;
		case ZP_CAMERA_PROJECTION_PERSPECTIVE:
			m_projection.perspective( m_fovy, m_aspectRatio, m_near, m_far );
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
		m_viewProjection = m_view * m_projection;
	}
}

void zpCamera::onEnabled() {}
void zpCamera::onDisabled() {}
