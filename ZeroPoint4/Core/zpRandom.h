#pragma once
#ifndef ZP_RANDOM_H
#define ZP_RANDOM_H

class zpRandom {
public:
	~zpRandom();

	static zpRandom* getInstance();

	zp_int randomInt();
	zp_int randomInt( zp_int low, zp_int high );

	zp_uint randomUInt();
	zp_uint randomUInt( zp_uint low, zp_uint high );

	zp_float randomFloat();
	zp_float randomFloat( zp_float low, zp_float high );

	zpScalar randomScalar( const zpScalar& low, const zpScalar& high );

	zp_bool randomBoolean();

	zp_float noise( zp_float x, zp_float y, zp_float z );

	zpVector4f randomVector( const zpVector4f& low, const zpVector4f& high );
	zpVector4f randomUnitSphere( zp_float w = 1.0f );
	zpVector4f randomUnitCircle( zp_float z = 0.0f, zp_float w = 1.0f );
	zpColor4f randomColor( zp_float alpha = 1.0f );

private:
	zpRandom();

};

#endif