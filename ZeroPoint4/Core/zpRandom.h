#pragma once
#ifndef ZP_RANDOM_H
#define ZP_RANDOM_H

class zpRandom {
public:
	~zpRandom();

	static zpRandom* getInstance();

	zp_int randomInt();
	zp_int randomInt( zp_int low, zp_int high );

	zp_float randomFloat();
	zp_float randomFloat( zp_float low, zp_float high );

	zp_bool randomBoolean();

	zp_float noise( zp_float x, zp_float y, zp_float z );

	template<typename T, zp_uint Size>
	T& randomElement( const zpArray<T, Size>& arr ) {
		return arr[ randomInt( 0, Size ) ];
	}

	template<typename T>
	T& randomElement( const zpArrayList<T>& arr ) {
		return arr[ randomInt( 0, arr.size() ) ];
	}

private:
	zpRandom();

};

#endif