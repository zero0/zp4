#include "zpCore.h"

zpCollision::zpCollision() {}
zpCollision::~zpCollision() {}

zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpVector4f& b ) {
	zp_real x = b.getX();
	zp_real y = b.getY();
	zp_real z = b.getZ();

	if(	zp_real_gt( x, a.getMin().getX() ) &&
		zp_real_gt( y, a.getMin().getY() ) &&
		zp_real_gt( z, a.getMin().getZ() ) &&
		zp_real_lt( x, a.getMax().getX() ) &&
		zp_real_lt( y, a.getMax().getY() ) &&
		zp_real_lt( z, a.getMax().getZ() ) ) return ZP_COLLISION_TYPE_CONTAINS;
	return ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingAABB& a, const zpRay& b ) {
	zpVector4f inv( 1, 1, 1, 1 );
	inv.div4( b.getDirection() );

	zp_real tx1 = zp_real_mul( ( zp_real_sub( a.getMin().getX(), b.getOrigin().getX() ) ), inv.getX() );
	zp_real tx2 = zp_real_mul( ( zp_real_sub( a.getMax().getX(), b.getOrigin().getX() ) ), inv.getX() );

	zp_real txmin = zp_real_min( tx1, tx2 );
	zp_real txmax = zp_real_max( tx1, tx2 );

	zp_real ty1 = zp_real_mul( ( zp_real_sub( a.getMin().getY(), b.getOrigin().getY() ) ), inv.getY() );
	zp_real ty2 = zp_real_mul( ( zp_real_sub( a.getMax().getY(), b.getOrigin().getY() ) ), inv.getY() );

	zp_real tymin = zp_real_min( ty1, ty2 );
	zp_real tymax = zp_real_max( ty1, ty2 );

	if( zp_real_gt( txmin, tymax ) || zp_real_gt( tymin, txmax ) ) return ZP_COLLISION_TYPE_NONE;
	if( zp_real_gt( tymin, txmin ) ) txmin = tymin;
	if( zp_real_lt( tymax, txmax ) ) txmax = tymax;

	zp_real tz1 = zp_real_mul( ( zp_real_sub( a.getMin().getZ(), b.getOrigin().getZ() ) ), inv.getZ() );
	zp_real tz2 = zp_real_mul( ( zp_real_sub( a.getMax().getZ(), b.getOrigin().getZ() ) ), inv.getZ() );

	zp_real tzmin = zp_real_min( tz1, tz2 );
	zp_real tzmax = zp_real_max( tz1, tz2 );

	if( zp_real_gt( txmin, tzmax ) || zp_real_gt( tzmin, txmax ) ) return ZP_COLLISION_TYPE_NONE;
	if( zp_real_gt( tzmin, txmin ) ) txmin = tzmin;
	if( zp_real_lt( tzmax, txmax ) ) txmax = tzmax;

	if( zp_real_gte( txmax, txmin ) ) {
		if( testCollision( a, b.getOrigin() ) != ZP_COLLISION_TYPE_NONE ) return ZP_COLLISION_TYPE_CONTAINS;
		return ZP_COLLISION_TYPE_INTERSECT;
	}

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
	zp_real d = dist.magnitudeSquared3();
	zp_real r = a.getCenter().getW();
	r = zp_real_mul( r, r );

	return zp_real_eq( d, r ) ? ZP_COLLISION_TYPE_INTERSECT : zp_real_lt( d, r ) ? ZP_COLLISION_TYPE_CONTAINS : ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingSphere& a, const zpRay& b ) {
	if( testCollision( a, b.getOrigin() ) != ZP_COLLISION_TYPE_NONE ) return ZP_COLLISION_TYPE_CONTAINS;

	zpVector4f v( a.getCenter() );
	v.sub3( b.getOrigin() );
	zp_real bb = ( zp_real_from_float( 2.f ) * b.getDirection().dot3( v ) );
	zp_real c = v.dot3( v ) - ( a.getCenter().getW() * a.getCenter().getW() );

	zp_real disc = ( bb * bb ) - ( zp_real_from_float( 4.f ) * c );

	if( zp_real_lt( disc, zp_real_zero() ) ) return ZP_COLLISION_TYPE_NONE;

	disc = zp_real_sqrt( disc );

	zp_real t = ( zp_real_neg( bb ) - disc ) / zp_real_from_float( 2.f );
	if( zp_real_lte( t, zp_real_zero() ) ) {
		t = ( zp_real_neg( bb ) + disc ) / zp_real_from_float( 2.f );
	}

	return zp_real_lt( t, zp_real_zero() ) ? ZP_COLLISION_TYPE_NONE : ZP_COLLISION_TYPE_INTERSECT;
}
zpCollisionType zpCollision::testCollision( const zpBoundingSphere& a, const zpBoundingAABB& b ) {
	return ZP_COLLISION_TYPE_NONE;
}
zpCollisionType zpCollision::testCollision( const zpBoundingSphere& a, const zpBoundingSphere& b ) {
	zpVector4f dist( b.getCenter() );
	dist.sub3( a.getCenter() );
	zp_real d = dist.magnitudeSquared3();
	zp_real r = a.getCenter().getW();
	zp_real g = b.getCenter().getW();
	r = zp_real_mul( r, r );
	g = zp_real_mul( g, g );
	r = zp_real_add( r, g );

	return zp_real_eq( d, r ) ? ZP_COLLISION_TYPE_INTERSECT : zp_real_lt( d, r ) ? ZP_COLLISION_TYPE_CONTAINS : ZP_COLLISION_TYPE_NONE;
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

		zp_real d = center.dot3( p );
		zp_real r = extence.dot3( absP );

		zp_real n = zp_real_neg( p.getW() );
		if( zp_real_lt( zp_real_add( d, r ), n ) ) {
			return ZP_COLLISION_TYPE_NONE;
		} else if( zp_real_lt( zp_real_sub( d, r ), n ) ) {
			type = ZP_COLLISION_TYPE_INTERSECT;
		}
	}
	return type;
}
zpCollisionType zpCollision::testCollision( const zpFrustum& a, const zpBoundingSphere& b ) {
	return ZP_COLLISION_TYPE_NONE;
}
