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

enum zpFrustumPoint {
	ZP_FRUSTUM_POINT_NEAR_TOP_LEFT = 0,
	ZP_FRUSTUM_POINT_NEAR_TOP_RIGHT,
	ZP_FRUSTUM_POINT_NEAR_BOTTOM_LEFT,
	ZP_FRUSTUM_POINT_NEAR_BOTTOM_RIGHT,

	ZP_FRUSTUM_POINT_FAR_TOP_LEFT,
	ZP_FRUSTUM_POINT_FAR_TOP_RIGHT,
	ZP_FRUSTUM_POINT_FAR_BOTTOM_LEFT,
	ZP_FRUSTUM_POINT_FAR_BOTTOM_RIGHT,
};

class zpFrustum {
public:
	zpFrustum();
	zpFrustum( const zpMatrix4f& matrix );
	~zpFrustum();

	void setMatrix( const zpMatrix4f& matrix );
	void setLookAt( const zpVector4f& eye, const zpVector4f& lookAt, const zpVector4f& up, zp_float ratio, zp_float fovy, zp_float zNear, zp_float zFar );
	void setLookTo( const zpVector4f& eye, const zpVector4f& direction, const zpVector4f& up, zp_float ratio, zp_float fovy, zp_float zNear, zp_float zFar );

	const zpPlane& getPlane( zpFrustumPlane plane ) const;
	const zpVector4f& getPoint( zpFrustumPoint point ) const;
	
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

	union {
		struct {
			zpVector4f m_ntl, m_ntr, m_nbl, m_nbr;
			zpVector4f m_ftl, m_ftr, m_fbl, m_fbr;
		};
		struct {
			zpVector4f m_points[8];
		};
	};
};

#endif