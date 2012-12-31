#include "zpCore.h"

zpCollision::zpCollision() {}
zpCollision::~zpCollision() {}

zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpVector4f& b ) {
	zpScalar x = b.getX();
	zpScalar y = b.getY();
	zpScalar z = b.getZ();

	if(	zpScalarGt( x, a.getMin().getX() ) &&
		zpScalarGt( y, a.getMin().getY() ) &&
		zpScalarGt( z, a.getMin().getZ() ) &&
		zpScalarLt( x, a.getMax().getX() ) &&
		zpScalarLt( y, a.getMax().getY() ) &&
		zpScalarLt( z, a.getMax().getZ() ) ) return ZP_COLLISION_TYPE_CONTAINS;
	return ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpRay& b ) {
	zpVector4f inv( 1, 1, 1, 1 );
	inv.div4( b.getDirection() );
	
	zpScalar tx1( ( a.getMin().getX() - b.getOrigin().getX() ) * inv.getX() );
	zpScalar tx2( ( a.getMax().getX() - b.getOrigin().getX() ) * inv.getX() );
	
	zpScalar txmin;
	zpScalar txmax;
	zpScalarMin( txmin, tx1, tx2 );
	zpScalarMax( txmax, tx1, tx2 );

	zpScalar ty1( ( a.getMin().getY() - b.getOrigin().getY() ) * inv.getY() );
	zpScalar ty2( ( a.getMax().getY() - b.getOrigin().getY() ) * inv.getY() );
	
	zpScalar tymin;
	zpScalar tymax;
	zpScalarMin( tymin, ty1, ty2 );
	zpScalarMax( tymax, ty1, ty2 );
	
	if( zpScalarGt( txmin, tymax ) || zpScalarGt( tymin, txmax ) ) return ZP_COLLISION_TYPE_NONE;
	if( zpScalarGt( tymin, txmin ) ) txmin = tymin;
	if( zpScalarLt( tymax, txmax ) ) txmax = tymax;
	
	zpScalar tz1( ( a.getMin().getZ() - b.getOrigin().getZ() ) * inv.getZ() );
	zpScalar tz2( ( a.getMax().getZ() - b.getOrigin().getZ() ) * inv.getZ() );
	
	zpScalar tzmin;
	zpScalar tzmax;
	zpScalarMin( tzmin, tz1, tz2 );
	zpScalarMax( tzmax, tz1, tz2 );

	if( zpScalarGt( txmin, tzmax ) || zpScalarGt( tzmin, txmax ) ) return ZP_COLLISION_TYPE_NONE;
	if( zpScalarGt( tzmin, txmin ) ) txmin = tzmin;
	if( zpScalarLt( tzmax, txmax ) ) txmax = tzmax;
	
	if( zpScalarGte( txmax, txmin ) ) {
		if( testCollision( a, b.getOrigin() ) != ZP_COLLISION_TYPE_NONE ) return ZP_COLLISION_TYPE_CONTAINS;
		return ZP_COLLISION_TYPE_INTERSECT;
	}
	/*
	zpVector4f inv( 1, 1, 1 ), minDir( a.getMin() ), maxDir( a.getMax() ), t1, t2, tmin, tmax;
	inv.div3( b.getDirection() );
	minDir.sub3( b.getOrigin() );
	maxDir.sub3( b.getOrigin() );

	t1.set3( minDir );
	t1.div3( inv );

	t2.set3( maxDir );
	t2.div3( inv );


	zpVector4fDiv( inv, zpScalar( 1.0f ), r.m_direction );
	zpVector4fSub( minDir, b.m_min, r.m_origin );
	zpVector4fSub( maxDir, b.m_max, r.m_origin );

	zpVector4fMul( t1, minDir, inv );
	zpVector4fMul( t2, maxDir, inv );

	zpVector4fMin( tmin, t1, t2 );
	zpVector4fMax( tmax, t1, t2 );

	// check x and y
	zpVector4f tmin0( tmin.m_y, tmin.m_z, tmin.m_w, tmin.m_x );
	zpVector4f tmax0( tmax.m_y, tmax.m_z, tmax.m_w, tmax.m_x );
	zpVector4fMinX( tmax, tmax, tmax0 );
	zpVector4fMaxX( tmin, tmin, tmin0 );

	// check x and z
	zpVector4f tmin1( tmin.m_z, tmin.m_y, tmin.m_z, tmin.m_y );
	zpVector4f tmax1( tmax.m_z, tmax.m_y, tmax.m_z, tmax.m_y );
	zpVector4fMinX( tmax, tmax, tmax0 );
	zpVector4fMaxX( tmin, tmin, tmin0 );

	tNear.m_x = tmin.m_x;
	tFar.m_x = tmax.m_x;

	return tmax.m_x >= 0.0f && tmax.m_x >= tmin.m_x;
	*/
	return ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpBoundingAABB& b ) {
	return ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpBoundingSphere& b ) {
	return ZP_COLLISION_TYPE_NONE;
}

zpCollisionType zpCollision::testCollision( const zpBoundingSphere& a, const zpVector4f& b ) {
	zpVector4f dist( b );
	dist.sub3( a.getCenter() );
	zpScalar d( dist.magnitudeSquared3() );
	zpScalar r( a.getCenter().getW() );
	zpScalarMul( r, r, r );

	return zpScalarEq( d, r ) ? ZP_COLLISION_TYPE_INTERSECT : zpScalarLt( d, r ) ? ZP_COLLISION_TYPE_CONTAINS : ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingSphere& a, const zpRay& b ) {
	if( testCollision( a, b.getOrigin() ) != ZP_COLLISION_TYPE_NONE ) return ZP_COLLISION_TYPE_CONTAINS;

	zpVector4f v( a.getCenter() );
	zpScalar r( a.getCenter().getW() );
	zpScalarMul( r, r, r );

	v.sub3( b.getOrigin() );
	zpScalar bb( zpScalar( 2.f ) * b.getDirection().dot3( v ) );
	zpScalar c( v.dot3( v ) - r );

	zpScalar disc( ( bb * bb ) - ( zpScalar( 4.f ) * c ) );

	if( zpScalarLt( disc, zpScalar() ) ) return ZP_COLLISION_TYPE_NONE;

	zpScalarSqrt( disc, disc );

	zpScalarNeg( bb, bb );
	zpScalar t = ( bb - disc ) / zpScalar( 2.f );
	if( zpScalarLte( t, zpScalar() ) ) {
		t = ( bb + disc ) / zpScalar( 2.f );
	}

	return zpScalarLt( t, zpScalar() ) ? ZP_COLLISION_TYPE_NONE : ZP_COLLISION_TYPE_INTERSECT;
}
zpCollisionType zpCollision::testCollision( const zpBoundingSphere& a, const zpBoundingAABB& b ) {
	return ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingSphere& a, const zpBoundingSphere& b ) {
	zpVector4f dist( b.getCenter() );
	dist.sub3( a.getCenter() );
	zpScalar d( dist.magnitudeSquared3() );
	zpScalar r( a.getCenter().getW() );
	zpScalar g( b.getCenter().getW() );
	zpScalarMul( r, r, r );
	zpScalarMul( g, g, g );
	zpScalarAdd( r, r, g );

	return zpScalarEq( d, r ) ? ZP_COLLISION_TYPE_INTERSECT : zpScalarLt( d, r ) ? ZP_COLLISION_TYPE_CONTAINS : ZP_COLLISION_TYPE_NONE;
}

zpCollisionType zpCollision::testCollision( const zpFrustum& a, const zpVector4f& b ) {
	zpCollisionType type = ZP_COLLISION_TYPE_CONTAINS;
	for( zp_uint i = 6; i --> 0; ) {
		zpPlaneSide side = a.getPlane( (zpFrustumPlane)i ).getSideOfPlane( b );
		switch( side ) {
		case ZP_PLANE_SIDE_NEGATIVE: return ZP_COLLISION_TYPE_NONE;
		case ZP_PLANE_SIDE_ON_PLANE: type = ZP_COLLISION_TYPE_INTERSECT;
		}
	}
	return type;
}
zpCollisionType zpCollision::testCollision( const zpFrustum& a, const zpRay& b ) {
	return ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpFrustum& a, const zpBoundingAABB& b ) {
	zpVector4f center( b.getCenter() );
	zpVector4f extence( b.getMax() - center );
	zpCollisionType type = ZP_COLLISION_TYPE_CONTAINS;

	for( zp_uint i = 6; i --> 0; ) {
		zpVector4f p( a.getPlane( (zpFrustumPlane)i ).getVector() );
		zpVector4f absP( p );
		absP.abs3();

		zpScalar d( center.dot3( p ) );
		zpScalar r( extence.dot3( absP ) );

		zpScalar n;
		zpScalarNeg( n, p.getW() );
		if( zpScalarLt( ( d + r ), n ) ) {
			return ZP_COLLISION_TYPE_NONE;
		} else if( zpScalarLt( ( d - r ), n ) ) {
			type = ZP_COLLISION_TYPE_INTERSECT;
		}
	}
	return type;
}
zpCollisionType zpCollision::testCollision( const zpFrustum& a, const zpBoundingSphere& b ) {
	zpScalar r( b.getCenter().getW() );
	zpScalar z;
	zpVector4f center( b.getCenter().xyz1() );

	for( zp_uint i = 6; i --> 0; ) {
		zpScalar d( a.getPlane( (zpFrustumPlane)i ).getVector().dot4( center ) );
		zpScalarAdd( d, d, r );
		if( zpScalarLt( d, z ) ) {
			return ZP_COLLISION_TYPE_NONE;
		}
	}

	return ZP_COLLISION_TYPE_CONTAINS;
}
