#pragma once
#ifndef ZP_BOUNDING_SPHERE_H
#define ZP_BOUNDING_SPHERE_H

class zpBoundingAABB;

class zpBoundingSphere {
public:
	zpBoundingSphere();
	zpBoundingSphere( const zpVector4f& center, const zpScalar& radius );
	zpBoundingSphere( const zpBoundingSphere& sphere );
	zpBoundingSphere( zpBoundingSphere&& sphere );
	~zpBoundingSphere();

	void operator=( const zpBoundingSphere& sphere );
	void operator=( zpBoundingSphere&& sphere );

	const zpVector4f& getCenter() const;
	void setCenter( const zpVector4f& center );
	
	const zpScalar& getRadius() const;
	void setRadius( const zpScalar& radius );

	void generateBoundingAABB( zpBoundingAABB& box ) const;

	void translate( const zpVector4f& translate );
	void scale( const zpScalar& scale );
	void pad( const zpScalar& padding );
	
	void add( zp_float x, zp_float y, zp_float z );
	void add( const zpScalar& x, const zpScalar& y, const zpScalar& z );
	void add( const zpVector4f& point );
	void add( const zpBoundingAABB& box );
	void add( const zpBoundingSphere& sphere );

private:
	zpVector4f m_center;
	zpScalar m_radius;
};

#endif