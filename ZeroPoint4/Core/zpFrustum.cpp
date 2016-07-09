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
    //    matrix(0, 3) + matrix(0, 0),
    //    matrix(1, 3) + matrix(1, 0),
    //    matrix(2, 3) + matrix(2, 0),
    //    matrix(3, 3) + matrix(3, 0)
    //    );
    //// Right clipping plane
    //m_right.set(
    //    matrix(0, 3) - matrix(0, 0),
    //    matrix(1, 3) - matrix(1, 0),
    //    matrix(2, 3) - matrix(2, 0),
    //    matrix(3, 3) - matrix(3, 0)
    //    );
    //// Top clipping plane
    //m_top.set(
    //    matrix(0, 3) - matrix(0, 1),
    //    matrix(1, 3) - matrix(1, 1),
    //    matrix(2, 3) - matrix(2, 1),
    //    matrix(3, 3) - matrix(3, 1)
    //    );
    //// Bottom clipping plane
    //m_bottom.set(
    //    matrix(0, 3) + matrix(0, 1),
    //    matrix(1, 3) + matrix(1, 1),
    //    matrix(2, 3) + matrix(2, 1),
    //    matrix(3, 3) + matrix(3, 1)
    //    );
    //// Near clipping plane
    //m_near.set(
    //    matrix(0, 2),
    //    matrix(1, 2),
    //    matrix(2, 2),
    //    matrix(3, 2)
    //    );
    //// Far clipping plane
    //m_far.set(
    //    matrix(0, 3) - matrix(0, 2),
    //    matrix(1, 3) - matrix(1, 2),
    //    matrix(2, 3) - matrix(2, 2),
    //    matrix(3, 3) - matrix(3, 2)
    //    );
}
void zpFrustum::setLookAt( const zpVector4f& eye, const zpVector4f& lookAt, const zpVector4f& up, zp_float ratio, zp_float fovy, zp_float zNear, zp_float zFar )
{
    zpVector4f dir;
    dir = zpMath::Vector4Sub( lookAt, eye );

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

    zpScalar zNearS = zpMath::Scalar( zNear );
    zpScalar zFarS  = zpMath::Scalar( zFar );
    zpScalar r      = zpMath::Scalar( ratio );

    zpScalar tang = zpMath::Scalar( fovy );
    tang = zpMath::ScalarDegToRad( tang );
    tang = zpMath::ScalarMul( tang, zpMath::Scalar( 0.5f ) );
    tang = zpMath::ScalarTan( tang );

    zpScalar nh;//( zNear * tang );
    zpScalar nw;//( zNear * tang * ratio );
    zpScalar fh;//( zFar * tang );
    zpScalar fw;//( zFar * tang * ratio );

    nh = zpMath::Vector4Mul( zNearS, tang );
    nw = zpMath::Vector4Mul( nh, r );

    fh = zpMath::Vector4Mul( zFarS, tang );
    fw = zpMath::Vector4Mul( fh, r );

    nc = zpMath::Vector4Madd( eye, direction, zNearS );
    fc = zpMath::Vector4Madd( eye, direction, zFarS );

    zpVector4f x;
    x = zpMath::Vector4Cross3( up, direction );
    x = zpMath::Vector4Normalize3( x );

    zpVector4f y;
    y = zpMath::Vector4Cross3( direction, x );

    zpVector4f yNh, yFh;
    zpVector4f xNw, xFw;
    yNh = zpMath::Vector4Mul( y, nh );
    xNw = zpMath::Vector4Mul( x, nw );
    yFh = zpMath::Vector4Mul( y, fh );
    xFw = zpMath::Vector4Mul( x, fw );

    // near
    m_ntl = zpMath::Vector4Add( nc, yNh );
    m_ntl = zpMath::Vector4Sub( m_ntl, xNw );

    m_ntr = zpMath::Vector4Add( nc, yNh );
    m_ntr = zpMath::Vector4Add( m_ntr, xNw );

    m_nbl = zpMath::Vector4Sub( nc, yNh );
    m_nbl = zpMath::Vector4Sub( m_nbl, xNw );

    m_nbr = zpMath::Vector4Sub( nc, yNh );
    m_nbr = zpMath::Vector4Add( m_nbr, xNw );

    // far
    m_ftl = zpMath::Vector4Add( fc, yFh );
    m_ftl = zpMath::Vector4Sub( m_ftl, xFw );

    m_ftr = zpMath::Vector4Add( fc, yFh );
    m_ftr = zpMath::Vector4Add( m_ftr, xFw );

    m_fbl = zpMath::Vector4Sub( fc, yFh );
    m_fbl = zpMath::Vector4Sub( m_fbl, xFw );

    m_fbr = zpMath::Vector4Sub( fc, yFh );
    m_fbr = zpMath::Vector4Add( m_fbr, xFw );

    //m_top.set(        m_ntl, m_ftl, m_ntr );
    //m_bottom.set(    m_nbr, m_fbr, m_nbl );
    //m_left.set(        m_fbl, m_ftl, m_nbl );
    //m_right.set(    m_ntr, m_ftr, m_nbr );
    //m_near.set(        m_nbl, m_ntl, m_nbr );
    //m_far.set(        m_ftl, m_ftr, m_fbl );

    m_top.        set(m_ntr,m_ntl,m_ftl);
    m_bottom.    set(m_nbl,m_nbr,m_fbr);
    m_left.        set(m_ntl,m_nbl,m_fbl);
    m_right.    set(m_nbr,m_ntr,m_fbr);
    m_near.        set(m_ntl,m_ntr,m_nbr);
    m_far.        set(m_ftr,m_ftl,m_fbl);
}

const zpPlane& zpFrustum::getPlane( zpFrustumPlane plane ) const
{
    return m_planes[ plane ];
}
const zpVector4f& zpFrustum::getPoint( zpFrustumPoint point ) const
{
    return m_points[ point ];
}
