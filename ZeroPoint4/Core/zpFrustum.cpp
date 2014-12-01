#include "zpCore.h"

zpFrustum::zpFrustum()
{}
zpFrustum::zpFrustum( const zpMatrix4f& matrix )
{
	setMatrix( matrix );
}
zpFrustum::~zpFrustum()
{}
  
void zpFrustum::setMatrix( const zpMatrix4f& matrix )
{
	//// Left clipping plane
	//m_left.set(
	//	matrix(0, 3) + matrix(0, 0),
	//	matrix(1, 3) + matrix(1, 0),
	//	matrix(2, 3) + matrix(2, 0),
	//	matrix(3, 3) + matrix(3, 0)
	//	);
	//// Right clipping plane
	//m_right.set(
	//	matrix(0, 3) - matrix(0, 0),
	//	matrix(1, 3) - matrix(1, 0),
	//	matrix(2, 3) - matrix(2, 0),
	//	matrix(3, 3) - matrix(3, 0)
	//	);
	//// Top clipping plane
	//m_top.set(
	//	matrix(0, 3) - matrix(0, 1),
	//	matrix(1, 3) - matrix(1, 1),
	//	matrix(2, 3) - matrix(2, 1),
	//	matrix(3, 3) - matrix(3, 1)
	//	);
	//// Bottom clipping plane
	//m_bottom.set(
	//	matrix(0, 3) + matrix(0, 1),
	//	matrix(1, 3) + matrix(1, 1),
	//	matrix(2, 3) + matrix(2, 1),
	//	matrix(3, 3) + matrix(3, 1)
	//	);
	//// Near clipping plane
	//m_near.set(
	//	matrix(0, 2),
	//	matrix(1, 2),
	//	matrix(2, 2),
	//	matrix(3, 2)
	//	);
	//// Far clipping plane
	//m_far.set(
	//	matrix(0, 3) - matrix(0, 2),
	//	matrix(1, 3) - matrix(1, 2),
	//	matrix(2, 3) - matrix(2, 2),
	//	matrix(3, 3) - matrix(3, 2)
	//	);
}
void zpFrustum::setLookAt( const zpVector4f& eye, const zpVector4f& lookAt, const zpVector4f& up, zp_float ratio, zp_float fovy, zp_float zNear, zp_float zFar )
{
	zpVector4f dir;
	zpMath::Sub( dir, lookAt, eye );

	setLookTo( eye, dir, up, ratio, fovy, zNear, zFar );
}
void zpFrustum::setLookTo( const zpVector4f& eye, const zpVector4f& direction, const zpVector4f& up, zp_float ratio, zp_float fovy, zp_float zNear, zp_float zFar )
{
	/*
	zp_float tang = zp_tan( fovy * 0.5f );
	zpScalar nh( zNear * tang );
	zpScalar nw( zNear * tang * ratio );
	zpScalar fh( zFar * tang );
	zpScalar fw( zFar * tang * ratio );
	zpScalar zNearS( zNear );
	zpScalar zFarS( zFar );

	zpVector4f z;
	zpMath::Normalize3( z, direction );

	zpVector4f x;
	zpMath::Cross3( x, up, z );
	zpMath::Normalize3( x, x );

	zpVector4f y;
	zpMath::Cross3( y, y, z );

	zpVector4f nc;
	zpVector4f fc;

	zpMath::Mul( nc, z, zNearS );
	zpMath::Mul( fc, z, zFarS );

	zpMath::Sub( nc, eye, nc );
	zpMath::Sub( fc, eye, fc );

	zpVector4f yh, xw, yhPxw, yhMxw;
	// near
	zpMath::Mul( yh, y, nh );
	zpMath::Mul( xw, x, nw );

	zpMath::Add( yhPxw, yh, xw );
	zpMath::Sub( yhMxw, yh, xw );

	zpMath::Add( m_ntl, nc, yhMxw );
	zpMath::Add( m_ntr, nc, yhPxw );
	zpMath::Sub( m_nbl, nc, yhMxw );
	zpMath::Sub( m_nbr, nc, yhPxw );

	// far
	zpMath::Mul( yh, y, fh );
	zpMath::Mul( xw, x, fw );

	zpMath::Add( yhPxw, yh, xw );
	zpMath::Sub( yhMxw, yh, xw );

	zpMath::Add( m_ftl, fc, yhMxw );
	zpMath::Add( m_ftr, fc, yhPxw );
	zpMath::Sub( m_fbl, fc, yhMxw );
	zpMath::Sub( m_fbr, fc, yhPxw );
	*/
	//zpVector4f z( direction );
	//z.normalize3();
	//
	//zpVector4f x( up );
	//x.cross3( z );
	//x.normalize3();
	//
	//zpVector4f y( x );
	//y.cross3( z );
	//
	//zpVector4f nc = eye - z * zNear;
	//zpVector4f fc = eye - z * zFar;
	//
	//m_ntl = nc + y * nh - x * nw;
	//m_ntr = nc + y * nh + x * nw;
	//m_nbl = nc - y * nh - x * nw;
	//m_nbr = nc - y * nh + x * nw;
	//
	//m_ftl = fc + y * fh - x * fw;
	//m_ftr = fc + y * fh + x * fw;
	//m_fbl = fc - y * fh - x * fw;
	//m_fbr = fc - y * fh + x * fw;

	zpVector4f fc;
	zpVector4f nc;

	zpScalar zNearS( zNear );
	zpScalar zFarS( zFar );
	zpScalar r( ratio );

	zpScalar tang( fovy );
	zpMath::DegToRad( tang, tang );
	zpMath::Mul( tang, tang, zpScalar( 0.5f ) );
	zpMath::Tan( tang, tang );

	zpScalar nh;//( zNear * tang );
	zpScalar nw;//( zNear * tang * ratio );
	zpScalar fh;//( zFar * tang );
	zpScalar fw;//( zFar * tang * ratio );

	zpMath::Mul( nh, zNearS, tang );
	zpMath::Mul( nw, nh, r );

	zpMath::Mul( fh, zFarS, tang );
	zpMath::Mul( fw, fh, r );

	zpMath::Madd( nc, eye, direction, zNearS );
	zpMath::Madd( fc, eye, direction, zFarS );

	zpVector4f x;
	zpMath::Cross3( x, up, direction );
	zpMath::Normalize3( x, x );

	zpVector4f y;
	zpMath::Cross3( y, direction, x );

	zpVector4f yNh, yFh;
	zpVector4f xNw, xFw;
	zpMath::Mul( yNh, y, nh );
	zpMath::Mul( xNw, x, nw );
	zpMath::Mul( yFh, y, fh );
	zpMath::Mul( xFw, x, fw );

	// near
	zpMath::Add( m_ntl, nc, yNh );
	zpMath::Sub( m_ntl, m_ntl, xNw );

	zpMath::Add( m_ntr, nc, yNh );
	zpMath::Add( m_ntr, m_ntr, xNw );

	zpMath::Sub( m_nbl, nc, yNh );
	zpMath::Sub( m_nbl, m_nbl, xNw );

	zpMath::Sub( m_nbr, nc, yNh );
	zpMath::Add( m_nbr, m_nbr, xNw );

	// far
	zpMath::Add( m_ftl, fc, yFh );
	zpMath::Sub( m_ftl, m_ftl, xFw );

	zpMath::Add( m_ftr, fc, yFh );
	zpMath::Add( m_ftr, m_ftr, xFw );

	zpMath::Sub( m_fbl, fc, yFh );
	zpMath::Sub( m_fbl, m_fbl, xFw );

	zpMath::Sub( m_fbr, fc, yFh );
	zpMath::Add( m_fbr, m_fbr, xFw );

	//m_top.set(		m_ntl, m_ftl, m_ntr );
	//m_bottom.set(	m_nbr, m_fbr, m_nbl );
	//m_left.set(		m_fbl, m_ftl, m_nbl );
	//m_right.set(	m_ntr, m_ftr, m_nbr );
	//m_near.set(		m_nbl, m_ntl, m_nbr );
	//m_far.set(		m_ftl, m_ftr, m_fbl );

	m_top.		set(m_ntr,m_ntl,m_ftl);
	m_bottom.	set(m_nbl,m_nbr,m_fbr);
	m_left.		set(m_ntl,m_nbl,m_fbl);
	m_right.	set(m_nbr,m_ntr,m_fbr);
	m_near.		set(m_ntl,m_ntr,m_nbr);
	m_far.		set(m_ftr,m_ftl,m_fbl);
}

const zpPlane& zpFrustum::getPlane( zpFrustumPlane plane ) const
{
	return m_planes[ plane ];
}
const zpVector4f& zpFrustum::getPoint( zpFrustumPoint point ) const
{
	return m_points[ point ];
}
