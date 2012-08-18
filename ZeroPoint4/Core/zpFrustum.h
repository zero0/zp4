#pragma once
#ifndef ZP_FRUSTUM_H
#define ZP_FRUSTUM_H

enum zpFrustumPlane {
	ZP_FRUSTUM_PLANE_LEFT = 0,
	ZP_FRUSTUM_PLANE_RIGHT,
	ZP_FRUSTUM_PLANE_BOTTOM,
	ZP_FRUSTUM_PLANE_TOP,
	ZP_FRUSTUM_PLANE_NEAR,
	ZP_FRUSTUM_PLANE_FAR,
};

class zpFrustum {
public:
	zpFrustum();
	zpFrustum( const zpMatrix4f& matrix );
	~zpFrustum();

	void set( const zpMatrix4f& matrix );
	void set( const zpVector4f& eye, const zpVector4f& lookAt, const zpVector4f& up, zp_float ratio, zp_float fovy, zp_float zNear, zp_float zFar );

	const zpPlane& getPlane( zpFrustumPlane plane ) const;

	zp_int intersection( const zpVector4f& point ) const;

private:
	union {
		struct {
			zpPlane m_left,
					m_right,
					m_bottom,
					m_top,
					m_near,
					m_far;
		};
		struct {
			zpPlane m_planes[6];
		};
	};
};

#endif