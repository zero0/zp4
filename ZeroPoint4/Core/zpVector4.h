#ifndef ZP_VECTOR4_H
#define ZP_VECTOR4_H

class zpVector4 {
public:
	zpVector4();
	zpVector4( zp_float x, zp_float y, zp_float z, zp_float w = 0 );
	zpVector4( zpReal x, zpReal y, zpReal z, zpReal w );
	zpVector4( const zpVector4& vector );
	zpVector4( zpVector4&& vector );
	~zpVector4();

	void operator=( const zpVector4& vector );
	void operator=( zpVector4&& vector );



private:
	zp_vec4 m_quad;
};

#endif