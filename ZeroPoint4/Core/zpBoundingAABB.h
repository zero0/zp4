#pragma once
#ifndef ZP_BOUNDING_AABB_H
#define ZP_BOUNDING_AABB_H

class zpBoundingSphere;

class zpBoundingAABB {
public:
	zpBoundingAABB();
	zpBoundingAABB( zp_float width, zp_float height, zp_float depth );
	zpBoundingAABB( const zpBoundingAABB& box );
	zpBoundingAABB( zpBoundingAABB&& box );
	~zpBoundingAABB();

	void operator=( const zpBoundingAABB& box );
	void operator=( zpBoundingAABB&& box );

	void reset();

	zpVector4f getMin() const;
	zpVector4f getMax() const;
	void setMin( const zpVector4f& min );
	void setMax( const zpVector4f& max );
	void setMinMax( const zpVector4f& min, const zpVector4f& max );

	const zpVector4f& getCenter() const;
	const zpVector4f& getExtents() const;

	void setCenter( const zpVector4f& center );
	void setExtents( const zpVector4f& extents );

	void generateBoundingSphere( zpBoundingSphere& sphere ) const;

	zpScalar getWidth() const;
	zpScalar getHeight() const;
	zpScalar getDepth() const;
	zpVector4f getSize() const;

	void translate( const zpVector4f& translate );
	void scale( const zpScalar& scale );
	void scale( const zpVector4f& scale );
	void pad( const zpScalar& padding );
	void pad( const zpVector4f& padding );

	void add( zp_float x, zp_float y, zp_float z );
	void add( const zpScalar& x, const zpScalar& y, const zpScalar& z );
	void add( const zpVector4f& point );
	void add( const zpBoundingAABB& box );
	void add( const zpBoundingSphere& sphere );

private:
	zpVector4f m_center;
	zpVector4f m_extent;
};

#endif
