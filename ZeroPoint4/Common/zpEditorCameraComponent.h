#pragma once
#ifndef ZP_EDITOR_CAMERA_COMPONENT_H
#define ZP_EDITOR_CAMERA_COMPONENT_H

class zpEditorCameraComponent
	: public zpComponent
	, public zpKeyboardListener
	, public zpMouseListener
{
public:
	zpEditorCameraComponent();
	virtual ~zpEditorCameraComponent();

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	void onKeyDown( zpKeyCode key );
	void onKeyRepeat( zpKeyCode key );
	void onKeyUp( zpKeyCode key );

	void onMouseButtonDown( zpMouseButton button );
	void onMouseButtonRepeat( zpMouseButton button );
	void onMouseButtonUp( zpMouseButton button );

	void onMouseScroll( zp_int scrollDelta );

	void onMouseMove( const zpVector2i& location );
	void onMouseChange( const zpVector2i& delta );

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zp_bool m_isActive;
	zpRenderingManager* m_rendering;

	zpCamera* m_prevCamera;
	zpCamera m_editorCamera;
};

#endif