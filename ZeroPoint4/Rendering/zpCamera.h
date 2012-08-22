#pragma once
#ifndef ZP_CAMERA_H
#define ZP_CAMERA_H

enum zpCameraProjection {
	ZP_CAMERA_PROJECTION_ORTHO,
	ZP_CAMERA_PROJECTION_PERSPECTIVE
};

class zpCamera {
public:
	zpCamera();
	~zpCamera();

	void update();

	void setProjectionType( zpCameraProjection type );

	void setNearFar( zp_float nearDist, zp_float farDist );
	void setFovy( zp_float fovy );
	void setAspectRatio( zp_float aspectRatio );

	void setPosition( const zpVector4f& position );
	void setLookAt( const zpVector4f& lookAt );
	void setUp( const zpVector4f& up );
	void set( const zpVector4f& position, const zpVector4f& lookAt, const zpVector4f& up );

	zp_float getAspectRation() const;
	zp_float getFovy() const;

	const zpVector4f& getPosition() const;
	const zpVector4f& getLookAt() const;
	const zpVector4f& getUp() const;

	const zpFrustum& getFrustum() const;

	const zpMatrix4f& getView() const;
	const zpMatrix4f& getProjection() const;
	const zpMatrix4f& getViewProjection() const;
	const zpMatrix4f& getInvViewProjection() const;

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

	zpFrustum m_frustum;

	zpMatrix4f m_view;
	zpMatrix4f m_projection;
	zpMatrix4f m_viewProjection;
	zpMatrix4f m_invViewProjection;
};

#endif