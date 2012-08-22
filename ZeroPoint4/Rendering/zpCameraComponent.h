#pragma once
#ifndef ZP_CAMERA_COMPONENT_H
#define ZP_CAMERA_COMPONENT_H

class zpCameraComponent : public zpComponent {
public:
	zpCameraComponent();
	~zpCameraComponent();

	zpCamera& getCamera();
	zpFlag32& getRenderLayers();

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zpFlag32 m_renderLayers;
	zpCamera m_camera;
};

#endif