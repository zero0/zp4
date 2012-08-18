#include "zpCore.h"

zpFrustum::zpFrustum() {}
zpFrustum::zpFrustum( const zpMatrix4f& matrix ) {
	set( matrix );
}
zpFrustum::~zpFrustum() {}
  
void zpFrustum::set( const zpMatrix4f& matrix ) {
	// Left clipping plane
	m_left.set(
		matrix(0, 3) + matrix(0, 0),
		matrix(1, 3) + matrix(1, 0),
		matrix(2, 3) + matrix(2, 0),
		matrix(3, 3) + matrix(3, 0)
		);
	// Right clipping plane
	m_right.set(
		matrix(0, 3) - matrix(0, 0),
		matrix(1, 3) - matrix(1, 0),
		matrix(2, 3) - matrix(2, 0),
		matrix(3, 3) - matrix(3, 0)
		);
	// Top clipping plane
	m_top.set(
		matrix(0, 3) - matrix(0, 1),
		matrix(1, 3) - matrix(1, 1),
		matrix(2, 3) - matrix(2, 1),
		matrix(3, 3) - matrix(3, 1)
		);
	// Bottom clipping plane
	m_bottom.set(
		matrix(0, 3) + matrix(0, 1),
		matrix(1, 3) + matrix(1, 1),
		matrix(2, 3) + matrix(2, 1),
		matrix(3, 3) + matrix(3, 1)
		);
	// Near clipping plane
	m_near.set(
		matrix(0, 2),
		matrix(1, 2),
		matrix(2, 2),
		matrix(3, 2)
		);
	// Far clipping plane
	m_far.set(
		matrix(0, 3) - matrix(0, 2),
		matrix(1, 3) - matrix(1, 2),
		matrix(2, 3) - matrix(2, 2),
		matrix(3, 3) - matrix(3, 2)
		);
}
void zpFrustum::set( const zpVector4f& eye, const zpVector4f& lookAt, const zpVector4f& up, zp_float ratio, zp_float fovy, zp_float zNear, zp_float zFar ) {
	zp_float tang = zp_tan( fovy * 0.5f );
	zp_float nh = zNear * tang;
	zp_float nw = nh * ratio;
	zp_float fh = zFar * tang;
	zp_float fw = fh * ratio;

	//Vector3f p( eye );
	//Vector3f z = (eye.vectorTo( lookAt )).normalize();
	//Vector3f x = ( up ^ z ).normalize();
	//Vector3f y = x ^ z;

	//Point3f nc = eye - z * zNear;
	//Point3f fc = eye - z * zFar;

	zpVector4f ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;

	zpVector4f z( lookAt - eye );
	z.normalize3();

	zpVector4f x( up );
	x.cross3( z );
	x.normalize3();

	zpVector4f y( x );
	y.cross3( z );

	zpVector4f nc = eye - z * zNear;
	zpVector4f fc = eye - z * zFar;

	ntl = nc + y * nh - x * nw;
	ntr = nc + y * nh + x * nw;
	nbl = nc - y * nh - x * nw;
	nbr = nc - y * nh + x * nw;

	ftl = fc + y * fh - x * fw;
	ftr = fc + y * fh + x * fw;
	fbl = fc - y * fh - x * fw;
	fbr = fc - y * fh + x * fw;

	m_top.set( ntl, ftl, ntr );
	m_bottom.set( nbr, fbr, nbl );
	m_left.set( fbl, ftl, nbl );
	m_right.set( ntr, ftr, nbr );
	m_near.set( nbl, ntl, nbr );
	m_far.set( ftl, ftr, fbl );
}

const zpPlane& zpFrustum::getPlane( zpFrustumPlane plane ) const {
	return m_planes[ plane ];
}


zp_int zpFrustum::intersection( const zpVector4f& point ) const {
	zp_uint intersect = 2;
	zp_int side = 0;
	for( zp_uint i = 6; i --> 0; ) {
		side = m_planes[ i ].getSideOfPlane( point );
		if( side < 0 ) {
			return 0;
		} else if( side == 0 ) {
			intersect = 1;
		}
	}

	return intersect;
}