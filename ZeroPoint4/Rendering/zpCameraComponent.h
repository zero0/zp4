#pragma once
#ifndef ZP_CAMERA_COMPONENT_H
#define ZP_CAMERA_COMPONENT_H

class zpCameraComponent : public zpComponent {
public:
	zpCameraComponent();
	~zpCameraComponent();

	zp_bool isLocalToGameObject() const;

	zpCamera& getCamera();

	void setRenderLayers( const zpRenderLayer& layers );
	const zpRenderLayer& getRenderLayers() const;

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
	zp_bool m_isLocalToGameObject;
	zpRenderLayer m_renderLayers;
	zpRenderingManager* m_manager;

	zpCamera m_camera;
};

#endif