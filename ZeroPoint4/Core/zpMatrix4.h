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
	ZP_FORCE_INLINE zpMatrix4( zp_float* matrix );
	ZP_FORCE_INLINE zpMatrix4( zp_vec4* matrix );
	ZP_FORCE_INLINE zpMatrix4( const zpMatrix4& matrix );
	ZP_FORCE_INLINE zpMatrix4( zpMatrix4&& matrix );
	ZP_FORCE_INLINE ~zpMatrix4();

private:
	zp_vec4 m_m1;
	zp_vec4 m_m2;
	zp_vec4 m_m3;
	zp_vec4 m_m4;
};

#endif