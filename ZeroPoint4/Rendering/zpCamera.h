#pragma once
#ifndef ZP_CAMERA_H
#define ZP_CAMERA_H

enum zpCameraProjection {
	ZP_CAMERA_PROJECTION_ORTHO,
	ZP_CAMERA_PROJECTION_PERSPECTIVE
};

class zpCamera : public zpComponent, public zpRenderable {
public:
	zpCamera();
	~zpCamera();

	void render();

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
	zpCameraProjection m_projectionType;

	zp_bool m_isViewDirty;
	zp_bool m_isProjectionDirty;
	
	zp_float m_near;
	zp_float m_far;
	zp_float m_fovy;
	zp_float m_aspectRatio;

	zpVector4f m_up;
	zpVector4f m_lookAt;
	zpVector4f m_position;

	zpMatrix4f m_view;
	zpMatrix4f m_projection;
	zpMatrix4f m_viewProjection;
	zpMatrix4f m_invViewProjection;
};

#endif