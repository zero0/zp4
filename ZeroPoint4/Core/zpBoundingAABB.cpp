#include "zpCore.h"

zpBoundingAABB::zpBoundingAABB() : 
	m_min( -1, -1, -1 ), 
	m_max( 1, 1, 1 )
{}
zpBoundingAABB::zpBoundingAABB( zp_float width, zp_float height, zp_float depth ) : 
	m_min( width * -0.5f, height * -0.5f, depth * -0.5f ), 
	m_max( width * 0.5f, height * 0.5f, depth * 0.5f )
{}
zpBoundingAABB::zpBoundingAABB( const zpVector4f& min, const zpVector4f& max ) : 
	m_min( min ), 
	m_max( max )
{}
zpBoundingAABB::zpBoundingAABB( const zpBoundingAABB& box ) : 
	m_min( box.m_min ), 
	m_max( box.m_max )
{}
zpBoundingAABB::zpBoundingAABB( zpBoundingAABB&& box ) : 
	m_min( box.m_min ), 
	m_max( box.m_max )
{}
zpBoundingAABB::~zpBoundingAABB() {}

void zpBoundingAABB::operator=( const zpBoundingAABB& box ) {
	m_min = box.m_min;
	m_max = box.m_max;
}
void zpBoundingAABB::operator=( zpBoundingAABB&& box ) {
	m_min = box.m_min;
	m_max = box.m_max;
}

const zpVector4f& zpBoundingAABB::getMin() const {
	return m_min;
}
const zpVector4f& zpBoundingAABB::getMax() const {
	return m_max;
}
void zpBoundingAABB::setMin( const zpVector4f& min ) {
	m_min = min;
}
void zpBoundingAABB::setMax( const zpVector4f& max ) {
	m_max = max;
}

zpVector4f zpBoundingAABB::getCenter() const {
	return ( m_min + m_max ) * 0.5f;
}
void zpBoundingAABB::setCenter( const zpVector4f& center ) {
	translate( center - getCenter() );
}

zp_float zpBoundingAABB::getWidth() const {
	return zp_real_to_float( zp_real_sub( m_max.getX(), m_min.getX() ) );
}
zp_float zpBoundingAABB::getHeight() const {
	return zp_real_to_float( zp_real_sub( m_max.getY(), m_min.getY() ) );
}
zp_float zpBoundingAABB::getDepth() const {
	return zp_real_to_float( zp_real_sub( m_max.getZ(), m_min.getZ() ) );
}

void zpBoundingAABB::translate( const zpVector4f& translate ) {
	m_min.add3( translate );
	m_max.add3( translate );
}
void zpBoundingAABB::scale( zp_float scale ) {
	zp_real rscale = zp_real_from_float( scale );
	m_min.mul3( rscale );
	m_max.mul3( rscale );
}
void zpBoundingAABB::scale( const zpVector4f& scale ) {
	m_min.mul3( scale );
	m_max.mul3( scale );
}
void zpBoundingAABB::pad( zp_float padding ) {
	zp_real rpadding = zp_real_from_float( padding );
	m_min.sub3( rpadding );
	m_max.add3( rpadding );
}
void zpBoundingAABB::pad( const zpVector4f& padding ) {
	m_min.sub3( padding );
	m_max.add3( padding );
}

void zpBoundingAABB::add( zp_float x, zp_float y, zp_float z ) {
	zp_real rx = zp_real_from_float( x );
	zp_real ry = zp_real_from_float( y );
	zp_real rz = zp_real_from_float( z );

	if( zp_real_gt( rx, m_max.getX() ) ) m_max.setX( rx );
	if( zp_real_lt( rx, m_min.getX() ) ) m_min.setX( rx );

	if( zp_real_gt( ry, m_max.getY() ) ) m_max.setY( ry );
	if( zp_real_lt( ry, m_min.getY() ) ) m_min.setY( ry );

	if( zp_real_gt( rz, m_max.getZ() ) ) m_max.setZ( rz );
	if( zp_real_lt( rz, m_min.getZ() ) ) m_min.setZ( rz );
}
void zpBoundingAABB::add( const zpVector4f& point ) {
	zp_real rx = point.getX();
	zp_real ry = point.getY();
	zp_real rz = point.getZ();

	if( zp_real_gt( rx, m_max.getX() ) ) m_max.setX( rx );
	if( zp_real_lt( rx, m_min.getX() ) ) m_min.setX( rx );

	if( zp_real_gt( ry, m_max.getY() ) ) m_max.setY( ry );
	if( zp_real_lt( ry, m_min.getY() ) ) m_min.setY( ry );

	if( zp_real_gt( rz, m_max.getZ() ) ) m_max.setZ( rz );
	if( zp_real_lt( rz, m_min.getZ() ) ) m_min.setZ( rz );
}
void zpBoundingAABB::add( const zpBoundingAABB& box ) {
	add( box.m_min );
	add( box.m_max );
}
void zpBoundingAABB::add( const zpBoundingSphere& sphere ) {
	zpVector4f min( sphere.getCenter() );
	zpVector4f max( sphere.getCenter() );

	min.add3( zp_real_neg( min.getW() ) );
	max.add3( max.getW() );

	add( min );
	add( max );
}