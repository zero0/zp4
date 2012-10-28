#include "zpCommon.h"

zpEditorCameraComponent::zpEditorCameraComponent()
	: m_isActive( false )
{}
zpEditorCameraComponent::~zpEditorCameraComponent() {}

void zpEditorCameraComponent::receiveMessage( const zpMessage& message ) {}

void zpEditorCameraComponent::serialize( zpSerializedOutput* out ) {}
void zpEditorCameraComponent::deserialize( zpSerializedInput* in ) {
	in->readBlock( ZP_SERIALIZE_TYPE_THIS );

	in->endBlock();
}

void zpEditorCameraComponent::onKeyDown( zpKeyCode key ) {}
void zpEditorCameraComponent::onKeyRepeat( zpKeyCode key ) {}
void zpEditorCameraComponent::onKeyUp( zpKeyCode key ) {
	zp_bool wasActive = m_isActive;

	//if( ZP_KEY_CODE_IS_MOD_DOWN( CTRL, key ) ) {
		switch( ZP_KEY_CODE_STRIP_MOD( key ) ) {
		case ZP_KEY_CODE_E:
			m_isActive = !m_isActive;
			break;
		}
	//}

	if( m_isActive && !wasActive ) {
		m_prevCamera = m_rendering->getCamera();

		m_editorCamera.setPosition( m_prevCamera->getPosition() );
		m_editorCamera.setLookTo( m_prevCamera->getLookTo() );
		m_editorCamera.setUp( m_prevCamera->getUp() );
		m_editorCamera.setAspectRatio( m_prevCamera->getAspectRation() );
		m_editorCamera.setFovy( m_prevCamera->getFovy() );
		m_editorCamera.update();

		m_rendering->setCamera( &m_editorCamera );
	} else if( wasActive && !m_isActive ) {
		m_rendering->setCamera( m_prevCamera );
	}
}

void zpEditorCameraComponent::onMouseButtonDown( zpMouseButton button ) {}
void zpEditorCameraComponent::onMouseButtonRepeat( zpMouseButton button ) {}
void zpEditorCameraComponent::onMouseButtonUp( zpMouseButton button ) {}

void zpEditorCameraComponent::onMouseScroll( zp_int scrollDelta ) {
	if( m_isActive ) {
		zpVector4f pos = m_editorCamera.getPosition();
		pos.add3( m_editorCamera.getLookTo() * (zp_float)scrollDelta );
		m_editorCamera.setPosition( pos );
	}
}

void zpEditorCameraComponent::onMouseMove( const zpVector2i& location ) {}
void zpEditorCameraComponent::onMouseChange( const zpVector2i& delta ) {}

void zpEditorCameraComponent::onCreate() {
	m_input = getGameManagerOfType<zpInputManager>();
	m_rendering = getGameManagerOfType<zpRenderingManager>();

	m_input->getKeyboard()->addListener( this );
	m_input->getMouse()->addListener( this );
}
void zpEditorCameraComponent::onDestroy() {
	m_input->getKeyboard()->removeListener( this );
	m_input->getMouse()->removeListener( this );
}

void zpEditorCameraComponent::onUpdate() {
	if( m_isActive ) {
		if( m_input->getMouse()->isButtonDown( ZP_MOUSE_BUTTON_LEFT ) ) {
			zpVector2i d = m_input->getMouse()->getDelta();
			if( !d.isZero() ) {
				m_editorCamera.setPosition( m_editorCamera.getPosition() + zpVector4f( (zp_float)d.getX(), (zp_float)d.getY(), 0.f ) * zpTime::getInstance()->getDeltaSeconds() * zpTime::getInstance()->getInterpolation());
			}
		}
		m_editorCamera.update();
	}
}

void zpEditorCameraComponent::onEnabled() {}
void zpEditorCameraComponent::onDisabled() {}
