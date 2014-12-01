#include "zpCore.h"

zpCollision::zpCollision() {}
zpCollision::~zpCollision() {}

zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpVector4f& b )
{
	zpScalar x( b.getX() );
	zpScalar y( b.getY() );
	zpScalar z( b.getZ() );
	
	return
		zpMath::Cmp( x, a.getMin().getX() ) > 0 &&
		zpMath::Cmp( y, a.getMin().getY() ) > 0 &&
		zpMath::Cmp( z, a.getMin().getZ() ) > 0 &&
		zpMath::Cmp( x, a.getMax().getX() ) < 0 &&
		zpMath::Cmp( y, a.getMax().getY() ) < 0 &&
		zpMath::Cmp( z, a.getMax().getZ() ) < 0 ? ZP_COLLISION_TYPE_CONTAINS : ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpRay& b )
{
	zpVector4f inv;
	zpMath::Rcp( inv, b.getDirection() );

	zpVector4f t1, t2, tmin, tmax;
	zpMath::Sub( t1, a.getMin(), b.getOrigin() );
	zpMath::Mul( t1, t1, inv );

	zpMath::Sub( t2, a.getMax(), b.getOrigin() );
	zpMath::Mul( t2, t2, inv );

	zpMath::Min( tmin, t1, t2 );
	zpMath::Max( tmax, t1, t2 );

	//zpVector4f inv( 1, 1, 1, 1 );
	//inv.div4( b.getDirection() );
	//
	//zpScalar tx1( ( a.getMin().getX() - b.getOrigin().getX() ) * inv.getX() );
	//zpScalar tx2( ( a.getMax().getX() - b.getOrigin().getX() ) * inv.getX() );
	//
	//zpScalar txmin;
	//zpScalar txmax;
	//zpScalarMin( txmin, tx1, tx2 );
	//zpScalarMax( txmax, tx1, tx2 );
	//
	//zpScalar ty1( ( a.getMin().getY() - b.getOrigin().getY() ) * inv.getY() );
	//zpScalar ty2( ( a.getMax().getY() - b.getOrigin().getY() ) * inv.getY() );
	//
	//zpScalar tymin;
	//zpScalar tymax;
	//zpScalarMin( tymin, ty1, ty2 );
	//zpScalarMax( tymax, ty1, ty2 );
	//
	//if( zpScalarGt( txmin, tymax ) || zpScalarGt( tymin, txmax ) ) return ZP_COLLISION_TYPE_NONE;
	//if( zpScalarGt( tymin, txmin ) ) txmin = tymin;
	//if( zpScalarLt( tymax, txmax ) ) txmax = tymax;
	//
	//zpScalar tz1( ( a.getMin().getZ() - b.getOrigin().getZ() ) * inv.getZ() );
	//zpScalar tz2( ( a.getMax().getZ() - b.getOrigin().getZ() ) * inv.getZ() );
	//
	//zpScalar tzmin;
	//zpScalar tzmax;
	//zpScalarMin( tzmin, tz1, tz2 );
	//zpScalarMax( tzmax, tz1, tz2 );
	//
	//if( zpScalarGt( txmin, tzmax ) || zpScalarGt( tzmin, txmax ) ) return ZP_COLLISION_TYPE_NONE;
	//if( zpScalarGt( tzmin, txmin ) ) txmin = tzmin;
	//if( zpScalarLt( tzmax, txmax ) ) txmax = tzmax;
	//
	//if( zpScalarGte( txmax, txmin ) ) {
	//	if( testCollision( a, b.getOrigin() ) != ZP_COLLISION_TYPE_NONE ) return ZP_COLLISION_TYPE_CONTAINS;
	//	return ZP_COLLISION_TYPE_INTERSECT;
	//}
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
zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpBoundingAABB& b )
{
	return ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpBoundingSphere& b )
{
	return ZP_COLLISION_TYPE_NONE;
}

zpCollisionType zpCollision::testCollision( const zpBoundingSphere& a, const zpVector4f& b )
{
	zpVector4f dist;
	zpScalar d;
	zpScalar r( a.getRadius() );

	zpMath::Sub( dist, b, a.getCenter() );
	zpMath::LengthSquared3( d, dist );
	zpMath::Mul( r, r, r );

	zp_int c;
	zpMath::Cmp( c, d, r );

	return c == 0 ? ZP_COLLISION_TYPE_INTERSECT : c < 0 ? ZP_COLLISION_TYPE_CONTAINS : ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingSphere& a, const zpRay& b )
{
	if( testCollision( a, b.getOrigin() ) != ZP_COLLISION_TYPE_NONE )
	{
		return ZP_COLLISION_TYPE_CONTAINS;
	}

	zp_int cmp;
	zpVector4f v;
	zpScalar r( a.getRadius() ), bb, c, disc, t;
	zpMath::Mul( r, r, r );

	zpMath::Sub( v, a.getCenter(), b.getOrigin() );

	zpMath::Dot3( bb, b.getDirection(), v );
	zpMath::Mul( bb, bb, zpScalar( 2.0f ) );

	zpMath::Dot3( c, v, v );
	zpMath::Sub( c, c, r );
	zpMath::Mul( c, c, zpScalar( 4.0f ) );

	zpMath::Mul( disc, bb, bb );
	zpMath::Sub( disc, disc, c );

	zpMath::Cmp0( cmp, disc );
	if( cmp < 0 )
	{
		return ZP_COLLISION_TYPE_NONE;
	}

	zpMath::Sqrt( disc, disc );
	zpMath::Neg( bb, bb );

	zpMath::Sub( t, bb, disc );
	zpMath::Mul( t, t, zpScalar( 0.5f ) );
	zpMath::Cmp0( cmp, t );
	if( cmp < 0 )
	{
		zpMath::Add( t, bb, disc );
		zpMath::Mul( t, t, zpScalar( 0.5f ) );
	}

	zpMath::Cmp0( cmp, t );
	return cmp < 0 ? ZP_COLLISION_TYPE_NONE : ZP_COLLISION_TYPE_INTERSECT;
}
zpCollisionType zpCollision::testCollision( const zpBoundingSphere& a, const zpBoundingAABB& b )
{
	return ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingSphere& a, const zpBoundingSphere& b )
{
	zpScalar d, r( a.getRadius() ), g( b.getRadius() );
	zpVector4f dist;
	zpMath::Sub( dist, b.getCenter(), a.getCenter() );
	zpMath::LengthSquared3( d, dist );

	zpMath::Mul( r, r, r );
	zpMath::Mul( g, g, g );
	zpMath::Mul( r, r, g );

	zp_int c;
	zpMath::Cmp( c, d, r );
	
	return c == 0 ? ZP_COLLISION_TYPE_INTERSECT : c < 0 ? ZP_COLLISION_TYPE_CONTAINS : ZP_COLLISION_TYPE_NONE;
}

zpCollisionType zpCollision::testCollision( const zpFrustum& a, const zpVector4f& b )
{
	zpCollisionType type = ZP_COLLISION_TYPE_CONTAINS;

	for( zp_uint i = 0; i < zpFrustumPlane_Count; ++i )
	{
		zpPlaneSide side = a.getPlane( (zpFrustumPlane)i ).getSideOfPlane( b );
		switch( side )
		{
			case ZP_PLANE_SIDE_NEGATIVE:
				return ZP_COLLISION_TYPE_NONE;
			case ZP_PLANE_SIDE_ON_PLANE:
				type = ZP_COLLISION_TYPE_INTERSECT;
				break;
		}
	}

	return type;
}
zpCollisionType zpCollision::testCollision( const zpFrustum& a, const zpRay& b ) {
	return ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpFrustum& a, const zpBoundingAABB& b )
{
	zpVector4f center;
	zpVector4f extence;
	b.getCenter( center );
	b.getExtents( extence );

	zpVector4f absP;
	zpScalar d, r;
	zpScalar nd;
	zpScalar dpr, dmr;
	zp_int c;

	zpCollisionType type = ZP_COLLISION_TYPE_CONTAINS;
	for( zp_uint i = 0; i < zpFrustumPlane_Count; ++i )
	{
		const zpVector4f& p = a.getPlane( (zpFrustumPlane)i ).getVector();
		zpMath::Abs( absP, p );
		
		zpMath::Dot3( d, center, p );
		zpMath::Dot3( r, extence, absP );
		
		zpMath::Neg( nd, p.getW() );
		
		zpMath::Add( dpr, d, r );
		zpMath::Sub( dmr, d, r );

		zpMath::Cmp( c, dpr, nd );
		if( c < 0 )
		{
			type = ZP_COLLISION_TYPE_NONE;
			break;
		}
		
		zpMath::Cmp( c, dmr, nd );
		if( c < 0 )
		{
			type = ZP_COLLISION_TYPE_INTERSECT;
		}
	}

	return type;
}
zpCollisionType zpCollision::testCollision( const zpFrustum& a, const zpBoundingSphere& b )
{
	zpScalar z( 0.0f );
	zpScalar d;
	zp_int c;
	zpCollisionType type = ZP_COLLISION_TYPE_CONTAINS;

	for( zp_uint i = 0; i < zpFrustumPlane_Count; ++i )
	{
		zpMath::Dot3( d, a.getPlane( (zpFrustumPlane)i ).getVector(), b.getCenter() );
		zpMath::Add( d, d, b.getRadius() );
		zpMath::Cmp( c, d, z );

		if( c < 0 )
		{
			type = ZP_COLLISION_TYPE_NONE;
			break;
		}
	}

	return type;
}
