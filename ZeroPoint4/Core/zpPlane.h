#pragma once
#ifndef ZP_PLANE_H
#define ZP_PLANE_H

class zpPlane {
public:
	zpPlane();
	zpPlane( const zpVector4f& normal );
	zpPlane( zp_float a, zp_float b, zp_float c, zp_float d );
	zpPlane( const zpPlane& plane );
	zpPlane( zpPlane&& plane );
	~zpPlane();

	void operator=( const zpPlane& plane );
	void operator=( zpPlane&& plane );

	zp_int getSideOfPlane( const zpVector4f& point ) const;
	zp_float getDistanceToPoint( const zpVector4f& point ) const;

	void set( zp_float a, zp_float b, zp_float c, zp_float d );
	void set( const zpVector4f& p0, const zpVector4f& p1, const zpVector4f& p2 );

private:
	zpVector4f m_plane;
};

#endif