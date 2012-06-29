#pragma once
#ifndef ZP_MATRIX4F_H
#define ZP_MATRIX4F_H

ZP_ALIGN16 class zpMatrix4f {
public:
	/*
	ZP_FORCE_INLINE void* operator new( zp_uint size );
	ZP_FORCE_INLINE void* operator new[]( zp_uint size );
	ZP_FORCE_INLINE void operator delete( void* ptr );
	*/
	ZP_FORCE_INLINE zpMatrix4f();
	ZP_FORCE_INLINE zpMatrix4f( const zp_float* matrix );
	ZP_FORCE_INLINE zpMatrix4f( const zp_vec4* matrix );
	ZP_FORCE_INLINE zpMatrix4f( const zpMatrix4f& matrix );
	ZP_FORCE_INLINE zpMatrix4f( zpMatrix4f&& matrix );
	ZP_FORCE_INLINE ~zpMatrix4f();

	ZP_FORCE_INLINE void operator=( const zpMatrix4f& matrix );
	ZP_FORCE_INLINE void operator=( zpMatrix4f&& matrix );
	ZP_FORCE_INLINE void operator=( const zp_float* matrix );

	ZP_FORCE_INLINE zp_float& operator()( zp_uint row, zp_uint col );
	ZP_FORCE_INLINE zp_float operator()( zp_uint row, zp_uint col ) const;

	ZP_FORCE_INLINE operator zp_float*();
	ZP_FORCE_INLINE operator const zp_float*() const;

	ZP_FORCE_INLINE void operator*=( const zpMatrix4f& matrix );

	ZP_FORCE_INLINE zpMatrix4f operator*( const zpMatrix4f& matrix ) const;

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