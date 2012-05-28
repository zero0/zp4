#pragma once
#ifndef ZP_MATRIX4_H
#define ZP_MATRIX4_H

ZP_ALIGN16 class zpMatrix4 {
public:
	/*
	ZP_FORCE_INLINE void* operator new( zp_uint size );
	ZP_FORCE_INLINE void* operator new[]( zp_uint size );
	ZP_FORCE_INLINE void operator delete( void* ptr );
	*/
	ZP_FORCE_INLINE zpMatrix4();
	ZP_FORCE_INLINE zpMatrix4( const zp_float* matrix );
	ZP_FORCE_INLINE zpMatrix4( const zp_vec4* matrix );
	ZP_FORCE_INLINE zpMatrix4( const zpMatrix4& matrix );
	ZP_FORCE_INLINE zpMatrix4( zpMatrix4&& matrix );
	ZP_FORCE_INLINE ~zpMatrix4();

	ZP_FORCE_INLINE zpMatrix4& operator=( const zpMatrix4& matrix );
	ZP_FORCE_INLINE zpMatrix4& operator=( zpMatrix4&& matrix );
	ZP_FORCE_INLINE zpMatrix4& operator=( const zp_float* matrix );

	ZP_FORCE_INLINE zp_float& operator()( zp_uint row, zp_uint col );
	ZP_FORCE_INLINE zp_float operator()( zp_uint row, zp_uint col ) const;

	ZP_FORCE_INLINE operator zp_float*();
	ZP_FORCE_INLINE operator const zp_float*() const;

	ZP_FORCE_INLINE void operator*=( const zpMatrix4& matrix );

private:
	union {
		zp_float m_data[16];
		struct {
			zp_vec4 m_m1;
			zp_vec4 m_m2;
			zp_vec4 m_m3;
			zp_vec4 m_m4;
		};
	};
};

#endif