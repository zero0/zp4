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
	void setLookTo( const zpVector4f& lookTo );
	void setLookAt( const zpVector4f& lookAt );
	void setUp( const zpVector4f& up );

	zp_float getAspectRation() const;
	zp_float getFovy() const;

	const zpVector4f& getPosition() const;
	const zpVector4f& getLookTo() const;
	const zpVector4f& getUp() const;

	const zpFrustum& getFrustum() const;

	const zpMatrix4f& getView() const;
	const zpMatrix4f& getProjection() const;
	const zpMatrix4f& getViewProjection() const;
	const zpMatrix4f& getInvViewProjection() const;
	const zpCameraBufferData& getCameraBufferData() const;

	zp_bool isDirty() const;
	void unmarkDirty();

	zpRay generateRay( const zpVector2i& windowPosition ) const;

private:
	zpCameraProjection m_projectionType;

	zp_bool m_isViewDirty;
	zp_bool m_isProjectionDirty;
	zp_bool m_isDirty;

	zp_float m_near;
	zp_float m_far;
	zp_float m_fovy;
	zp_float m_aspectRatio;

	zpRecti m_orthoRect;

	zpVector4f m_up;
	zpVector4f m_lookTo;
	zpVector4f m_position;

	zpFrustum m_frustum;

	zpMatrix4f m_view;
	zpMatrix4f m_projection;

	zpCameraBufferData m_bufferData;
};

#endif