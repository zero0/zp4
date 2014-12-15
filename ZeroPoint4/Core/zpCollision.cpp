#include "zpCore.h"

zpCollision::zpCollision() {}
zpCollision::~zpCollision() {}

zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpVector4f& b )
{
	zpScalar x( b.getX() );
	zpScalar y( b.getY() );
	zpScalar z( b.getZ() );
	
	zpVector4f min = a.getMin();
	zpVector4f max = a.getMax();

	return
		zpMath::Cmp( x, min.getX() ) > 0 &&
		zpMath::Cmp( y, min.getY() ) > 0 &&
		zpMath::Cmp( z, min.getZ() ) > 0 &&
		zpMath::Cmp( x, max.getX() ) < 0 &&
		zpMath::Cmp( y, max.getY() ) < 0 &&
		zpMath::Cmp( z, max.getZ() ) < 0 ? ZP_COLLISION_TYPE_CONTAINS : ZP_COLLISION_TYPE_NONE;
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
	const zpVector4f& center = b.getCenter();
	const zpVector4f& extence = b.getExtents();

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
