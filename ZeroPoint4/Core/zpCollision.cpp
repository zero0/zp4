#include "zpCore.h"

zpCollision::zpCollision() {}
zpCollision::~zpCollision() {}

zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpVector4f& b )
{
	zpScalar x = zpMath::Vector4GetX( b );
	zpScalar y = zpMath::Vector4GetY( b );
	zpScalar z = zpMath::Vector4GetZ( b );
	
	zpVector4f min = a.getMin();
	zpVector4f max = a.getMax();

	return
		zpMath::ScalarCmp( x, zpMath::Vector4GetX( min ) ) > 0 &&
		zpMath::ScalarCmp( y, zpMath::Vector4GetY( min ) ) > 0 &&
		zpMath::ScalarCmp( z, zpMath::Vector4GetZ( min ) ) > 0 &&
		zpMath::ScalarCmp( x, zpMath::Vector4GetX( max ) ) < 0 &&
		zpMath::ScalarCmp( y, zpMath::Vector4GetY( max ) ) < 0 &&
		zpMath::ScalarCmp( z, zpMath::Vector4GetZ( max ) ) < 0 ? ZP_COLLISION_TYPE_CONTAINS : ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpRay& b )
{
	zpVector4f inv;
	inv = zpMath::Vector4Rcp( b.getDirection() );

	zpVector4f t1, t2, tmin, tmax;
	t1 = zpMath::Vector4Sub( a.getMin(), b.getOrigin() );
	t1 = zpMath::Vector4Mul( t1, inv );

	t2 = zpMath::Vector4Sub( a.getMax(), b.getOrigin() );
	t2 = zpMath::Vector4Mul( t2, inv );

	tmin = zpMath::Vector4Min( t1, t2 );
	tmax = zpMath::Vector4Max( t1, t2 );

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

	dist = zpMath::Vector4Sub( b, a.getCenter() );
	d = zpMath::Vector4LengthSquared3( dist );
	r = zpMath::ScalarMul( r, r );

	zp_int c;
	c = zpMath::ScalarCmp( d, r );

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
	r = zpMath::ScalarMul( r, r );

	v = zpMath::Vector4Sub( a.getCenter(), b.getOrigin() );

	bb = zpMath::Vector4Dot3( b.getDirection(), v );
	bb = zpMath::Vector4Mul( bb, zpMath::Scalar( 2.0f ) );

	c = zpMath::Vector4Dot3( v, v );
	c = zpMath::ScalarSub( c, r );
	c = zpMath::ScalarMul( c, zpMath::Scalar( 4.0f ) );

	disc = zpMath::Vector4Mul( bb, bb );
	disc = zpMath::Vector4Sub( disc, c );

	cmp = zpMath::ScalarCmp0( disc );
	if( cmp < 0 )
	{
		return ZP_COLLISION_TYPE_NONE;
	}

	disc = zpMath::ScalarSqrt( disc );
	bb = zpMath::Vector4Neg( bb );

	t = zpMath::ScalarSub( bb, disc );
	t = zpMath::ScalarMul( t, zpMath::Scalar( 0.5f ) );
	cmp = zpMath::ScalarCmp0( t );
	if( cmp < 0 )
	{
		t = zpMath::ScalarAdd( bb, disc );
		t = zpMath::ScalarMul( t, zpMath::Scalar( 0.5f ) );
	}

	cmp = zpMath::ScalarCmp0( t );
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
	dist = zpMath::Vector4Sub( b.getCenter(), a.getCenter() );
	d = zpMath::Vector4LengthSquared3( dist );

	r = zpMath::ScalarMul( r, r );
	g = zpMath::ScalarMul( g, g );
	r = zpMath::ScalarMul( r, g );

	zp_int c;
	c = zpMath::ScalarCmp( d, r );
	
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
		absP = zpMath::Vector4Abs( p );
		
		d = zpMath::Vector4Dot3( center, p );
		r = zpMath::Vector4Dot3( extence, absP );
		
		nd = zpMath::ScalarNeg( zpMath::Vector4GetW( p ) );
		
		dpr = zpMath::ScalarAdd( d, r );
		dmr = zpMath::ScalarSub( d, r );

		c = zpMath::ScalarCmp( dpr, nd );
		if( c < 0 )
		{
			type = ZP_COLLISION_TYPE_NONE;
			break;
		}
		
		c = zpMath::ScalarCmp( dmr, nd );
		if( c < 0 )
		{
			type = ZP_COLLISION_TYPE_INTERSECT;
		}
	}

	return type;
}
zpCollisionType zpCollision::testCollision( const zpFrustum& a, const zpBoundingSphere& b )
{
	zpScalar z = zpMath::Scalar( 0.0f );
	zpScalar d;
	zp_int c;
	zpCollisionType type = ZP_COLLISION_TYPE_CONTAINS;

	for( zp_uint i = 0; i < zpFrustumPlane_Count; ++i )
	{
		d = zpMath::Vector4Dot3( a.getPlane( (zpFrustumPlane)i ).getVector(), b.getCenter() );
		d = zpMath::ScalarAdd( d, b.getRadius() );
		c = zpMath::ScalarCmp( d, z );

		if( c < 0 )
		{
			type = ZP_COLLISION_TYPE_NONE;
			break;
		}
	}

	return type;
}
