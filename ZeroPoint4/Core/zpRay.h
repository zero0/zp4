#pragma once
#ifndef ZP_RAY_H
#define ZP_RAY_H

class zpRay {
public:
	zpRay();
	zpRay( const zpVector4f& origin, const zpVector4f& direction );
	zpRay( const zpRay& ray );
	zpRay( zpRay&& ray );
	~zpRay();

	void operator=( const zpRay& ray );
	void operator=( zpRay&& ray );

	const zpVector4f& getOrigin() const;
	void setOrigin( const zpVector4f& origin );

	const zpVector4f& getDirection() const;
	void setDirection( const zpVector4f& direction );

	zpVector4f getPointAt( zp_float t ) const;
	zp_bool isPointOnRay( const zpVector4f& point ) const;

private:
	zpVector4f m_origin;
	zpVector4f m_direction;
};

#endif