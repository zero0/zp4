#pragma once
#ifndef ZP_BOUNDING_AABB_H
#define ZP_BOUNDING_AABB_H

class zpBoundingSphere;

class zpBoundingAABB {
public:
	zpBoundingAABB();
	zpBoundingAABB( zp_float width, zp_float height, zp_float depth );
	zpBoundingAABB( const zpVector4f& min, const zpVector4f& max );
	zpBoundingAABB( const zpBoundingAABB& box );
	zpBoundingAABB( zpBoundingAABB&& box );
	~zpBoundingAABB();

	void operator=( const zpBoundingAABB& box );
	void operator=( zpBoundingAABB&& box );

	const zpVector4f& getMin() const;
	const zpVector4f& getMax() const;
	void setMin( const zpVector4f& min );
	void setMax( const zpVector4f& max );

	zpVector4f getCenter() const;
	void setCenter( const zpVector4f& center );

	zpBoundingSphere generateBoundingSphere( zp_bool isSphereContained = false ) const;

	zp_float getWidth() const;
	zp_float getHeight() const;
	zp_float getDepth() const;

	void translate( const zpVector4f& translate );
	void scale( zp_float scale );
	void scale( const zpVector4f& scale );
	void pad( zp_float padding );
	void pad( const zpVector4f& padding );

	void add( zp_float x, zp_float y, zp_float z );
	void add( const zpVector4f& point );
	void add( const zpBoundingAABB& box );
	void add( const zpBoundingSphere& sphere );

private:
	zpVector4f m_min;
	zpVector4f m_max;
};

#endif