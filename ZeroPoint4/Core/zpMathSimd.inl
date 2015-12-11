
namespace zpMath
{
	//
	// Constructors
	//
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Scalar( zp_float s )
	{
		return _mm_set1_ps( s );
	}
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4( zp_float x, zp_float y, zp_float z, zp_float w )
	{
		return _mm_setr_ps( x, y, z, w );
	}
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4( zpScalarParamF x, zpScalarParamF y, zpScalarParamF z, zpScalarParamG w )
	{
		return _mm_setr_ps( AsFloat( x ), AsFloat( y ), AsFloat( z ), AsFloat( w ) );
	}
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL Quaternion( zp_float x, zp_float y, zp_float z, zp_float w )
	{
		return _mm_setr_ps( x, y, z, w );
	}
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL Quaternion( zpScalarParamF x, zpScalarParamF y, zpScalarParamF z, zpScalarParamG w )
	{
		return _mm_setr_ps( AsFloat( x ), AsFloat( y ), AsFloat( z ), AsFloat( w ) );
	}
	ZP_FORCE_INLINE zp_float ZP_VECTORCALL AsFloat( zpScalarParamF s )
	{
		zp_float v;
		_mm_store_ss( &v, s );
		return v;
	}

	//
	// Vector getters
	//
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetX( zpVector4fParamF s ) { return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 0, 0, 0, 0 ) ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetY( zpVector4fParamF s ) { return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 1, 1, 1, 1 ) ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetZ( zpVector4fParamF s ) { return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 2, 2, 2, 2 ) ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetW( zpVector4fParamF s ) { return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 3, 3, 3, 3 ) ); }

	//
	// Vector setters
	//
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetX( zpVector4fParamF s, zpScalarParamF x ) { s.m128_f32[0] = AsFloat( x ); return s; }
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetY( zpVector4fParamF s, zpScalarParamF y ) { s.m128_f32[1] = AsFloat( y ); return s; }
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetZ( zpVector4fParamF s, zpScalarParamF z ) { s.m128_f32[2] = AsFloat( z ); return s; }
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetW( zpVector4fParamF s, zpScalarParamF w ) { s.m128_f32[3] = AsFloat( w ); return s; }

	//
	// Vector load and store
	//
	ZP_FORCE_INLINE void ZP_VECTORCALL Vector4Store4( zpVector4fParamF s, zp_float* xyzw )
	{
		_mm_store_ps( xyzw, s );
	}
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Load4( const zp_float* xyzw )
	{
		return _mm_load_ps( xyzw );
	}

	//
	// Quaternion getter
	//
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetX( zpQuaternion4fParamF s ) { return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 0, 0, 0, 0 ) ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetY( zpQuaternion4fParamF s ) { return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 1, 1, 1, 1 ) ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetZ( zpQuaternion4fParamF s ) { return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 2, 2, 2, 2 ) ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetW( zpQuaternion4fParamF s ) { return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 3, 3, 3, 3 ) ); }

	//
	// Quaternion setter
	//
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetX( zpQuaternion4fParamF s, zpScalarParamF x ) { s.m128_f32[0] = AsFloat( x ); return s; }
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetY( zpQuaternion4fParamF s, zpScalarParamF y ) { s.m128_f32[1] = AsFloat( y ); return s; }
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetZ( zpQuaternion4fParamF s, zpScalarParamF z ) { s.m128_f32[2] = AsFloat( z ); return s; }
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetW( zpQuaternion4fParamF s, zpScalarParamF w ) { s.m128_f32[3] = AsFloat( w ); return s; }

	//
	// Trig functions
	//
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSin( zpScalarParamF a ) { zp_float s = AsFloat( a ); return Scalar( zp_sin( s ) ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarCos( zpScalarParamF a ) { zp_float s = AsFloat( a ); return Scalar( zp_cos( s ) ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarTan( zpScalarParamF a ) { zp_float s = AsFloat( a ); return Scalar( zp_tan( s ) ); }

	//
	// Scalar math
	//
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarAdd( zpScalarParamF a, zpScalarParamF b ) { return _mm_add_ps( a, b ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSub( zpScalarParamF a, zpScalarParamF b ) { return _mm_sub_ps( a, b ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMul( zpScalarParamF a, zpScalarParamF b ) { return _mm_mul_ps( a, b ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarDiv( zpScalarParamF a, zpScalarParamF b ) { return _mm_div_ps( a, b ); }

	//
	// Vector / Scalar math
	//
	//ZP_FORCE_INLINE zpVector4f Add( zpVector4f a, zpScalar b ) { return _mm_add_ps( a, b ); }
	//ZP_FORCE_INLINE zpVector4f Sub( zpVector4f a, zpScalar b ) { return _mm_sub_ps( a, b ); }
	//ZP_FORCE_INLINE zpVector4f Mul( zpVector4f a, zpScalar b ) { return _mm_mul_ps( a, b ); }
	//ZP_FORCE_INLINE zpVector4f Div( zpVector4f a, zpScalar b ) { return _mm_div_ps( a, b ); }

	//
	// Scalar / Vector math
	//
	//ZP_FORCE_INLINE zpVector4f Add( zpScalar a, zpVector4f b ) { return _mm_add_ps( a, b ); }
	//ZP_FORCE_INLINE zpVector4f Sub( zpScalar a, zpVector4f b ) { return _mm_sub_ps( a, b ); }
	//ZP_FORCE_INLINE zpVector4f Mul( zpScalar a, zpVector4f b ) { return _mm_mul_ps( a, b ); }
	//ZP_FORCE_INLINE zpVector4f Div( zpScalar a, zpVector4f b ) { return _mm_div_ps( a, b ); }

	//
	// Vector / Vector math
	//
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Add( zpVector4fParamF a, zpVector4fParamF b ) { return _mm_add_ps( a, b ); }
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Sub( zpVector4fParamF a, zpVector4fParamF b ) { return _mm_sub_ps( a, b ); }
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Mul( zpVector4fParamF a, zpVector4fParamF b ) { return _mm_mul_ps( a, b ); }
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Div( zpVector4fParamF a, zpVector4fParamF b ) { return _mm_div_ps( a, b ); }

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Scale( zpVector4fParamF a, zpScalarParamF b ) { return _mm_mul_ps( a, b ); }

	//
	// Quat / Scalar math
	//
	//ZP_FORCE_INLINE zpQuaternion4f Add( zpQuaternion4f a, zpScalar b ) { return _mm_add_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Sub( zpQuaternion4f a, zpScalar b ) { return _mm_sub_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Mul( zpQuaternion4f a, zpScalar b ) { return _mm_mul_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Div( zpQuaternion4f a, zpScalar b ) { return _mm_div_ps( a, b ); }

	//
	// Scalar / Quat math
	//
	//ZP_FORCE_INLINE zpQuaternion4f Add( zpScalar a, zpQuaternion4f b ) { return _mm_add_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Sub( zpScalar a, zpQuaternion4f b ) { return _mm_sub_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Mul( zpScalar a, zpQuaternion4f b ) { return _mm_mul_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Div( zpScalar a, zpQuaternion4f b ) { return _mm_div_ps( a, b ); }

	//
	// Quat / Vector math
	//
	//ZP_FORCE_INLINE zpQuaternion4f Add( zpQuaternion4f a, zpVector4f b ) { return _mm_add_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Sub( zpQuaternion4f a, zpVector4f b ) { return _mm_sub_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Mul( zpQuaternion4f a, zpVector4f b ) { return _mm_mul_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Div( zpQuaternion4f a, zpVector4f b ) { return _mm_div_ps( a, b ); }

	//
	// Vector / Quat math
	//
	//ZP_FORCE_INLINE zpQuaternion4f Add( zpVector4f a, zpQuaternion4f b ) { return _mm_add_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Sub( zpVector4f a, zpQuaternion4f b ) { return _mm_sub_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Mul( zpVector4f a, zpQuaternion4f b ) { return _mm_mul_ps( a, b ); }
	//ZP_FORCE_INLINE zpQuaternion4f Div( zpVector4f a, zpQuaternion4f b ) { return _mm_div_ps( a, b ); }

	//
	// Quat / Quat math
	//
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionAdd( zpQuaternion4fParamF a, zpQuaternion4fParamF b ) { return _mm_add_ps( a, b ); }
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSub( zpQuaternion4fParamF a, zpQuaternion4fParamF b ) { return _mm_sub_ps( a, b ); }
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionMul( zpQuaternion4fParamF a, zpQuaternion4fParamF b )
	{
		zp_float ax = AsFloat( QuaternionGetX( a ) );
		zp_float ay = AsFloat( QuaternionGetY( a ) );
		zp_float az = AsFloat( QuaternionGetZ( a ) );
		zp_float aw = AsFloat( QuaternionGetW( a ) );
		
		zp_float bx = AsFloat( QuaternionGetX( b ) );
		zp_float by = AsFloat( QuaternionGetY( b ) );
		zp_float bz = AsFloat( QuaternionGetZ( b ) );
		zp_float bw = AsFloat( QuaternionGetW( b ) );
		
		zp_float nx = aw * bx + ay * bw + ay * bz - az * by;
		zp_float ny = aw * by + ax * bw + az * bx - ax * bz;
		zp_float nz = aw * bz + az * bw + ax * by - ay * bx;
		zp_float nw = aw * bw - ax * bx - ay * by - az * bz;

		return Quaternion( nx, ny, nz, nw );
	}

	//
	// Quaternion load and store
	//
	ZP_FORCE_INLINE void ZP_VECTORCALL QuaternionStore4( zpQuaternion4fParamF s, zp_float* xyzw )
	{
		_mm_storer_ps( xyzw, s );
	}
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionLoad4( const zp_float* xyzw )
	{
		return _mm_loadr_ps( xyzw );
	}

	//
	// Vector4 Dot
	//
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot2( zpVector4fParamF a, zpVector4fParamF b )
	{
		zpVector4f m = Vector4Mul( a, b );
		return ScalarAdd( Vector4GetX( m ), Vector4GetY( m ) );
	}
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot3( zpVector4fParamF a, zpVector4fParamF b )
	{
		zpVector4f m = Vector4Mul( a, b );
		return ScalarAdd( Vector4GetX( m ), ScalarAdd( Vector4GetY( m ), Vector4GetZ( m ) ) );
	}
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot4( zpVector4fParamF a, zpVector4fParamF b )
	{
		__m128 t0 = _mm_mul_ps( a, b );
		__m128 t1 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 1, 0, 3, 2 ) );
		__m128 t2 = _mm_add_ps( t0, t1 );
		__m128 t3 = _mm_shuffle_ps( t2, t2, _MM_SHUFFLE( 2, 3, 0, 1 ) );

		return _mm_add_ps( t3, t2 );
	}

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Cross3( zpVector4fParamF a, zpVector4fParamF b )
	{
		__m128 s0 = _mm_shuffle_ps( a, a, _MM_SHUFFLE( 3, 0, 2, 1 ) );
		__m128 s1 = _mm_shuffle_ps( b, b, _MM_SHUFFLE( 3, 1, 0, 2 ) );
		__m128 s2 = _mm_mul_ps( s0, s1 );

		__m128 s3 = _mm_shuffle_ps( a, a, _MM_SHUFFLE( 3, 1, 0, 2 ) );
		__m128 s4 = _mm_shuffle_ps( b, b, _MM_SHUFFLE( 3, 0, 2, 1 ) );
		__m128 s5 = _mm_mul_ps( s3, s4 );

		return _mm_sub_ps( s2, s5 );
	}

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarAbs( zpScalarParamF a )  { return _mm_and_ps( a, *(__m128*)&_mm_set1_epi32( 0x7fffffff ) ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarNeg( zpScalarParamF a )  { return _mm_xor_ps( a, *(__m128*)&_mm_set1_epi32( 0x80000000 ) ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarRcp( zpScalarParamF a )  { return _mm_rcp_ps( a ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSqrt( zpScalarParamF a ) { return _mm_sqrt_ps( a ); }

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Abs( zpVector4fParamF a )  { return _mm_and_ps( a, *(__m128*)&_mm_set1_epi32( 0x7fffffff ) ); }
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Neg( zpVector4fParamF a )  { return _mm_xor_ps( a, *(__m128*)&_mm_set1_epi32( 0x80000000 ) ); }
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Rcp( zpVector4fParamF a )  { return _mm_rcp_ps( a ); }
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Sqrt( zpVector4fParamF a ) { return _mm_sqrt_ps( a ); }

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMax( zpScalarParamF a, zpScalarParamF b ) { return _mm_min_ps( a, b ); }
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMin( zpScalarParamF a, zpScalarParamF b ) { return _mm_max_ps( a, b ); }

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Max( zpVector4fParamF a, zpVector4fParamF b ) { return _mm_min_ps( a, b ); }
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Min( zpVector4fParamF a, zpVector4fParamF b ) { return _mm_max_ps( a, b ); }

	ZP_FORCE_INLINE zp_int ZP_VECTORCALL ScalarCmp( zpScalarParamF a, zpScalarParamF b )
	{
		__m128 lt = _mm_cmplt_ss( a, b );
		__m128 gt = _mm_cmpgt_ss( a, b );

		return lt.m128_i32[0] ? -1 : gt.m128_i32[0] ? 1 : 0;
	}
	ZP_FORCE_INLINE zpVector4fCmp ZP_VECTORCALL Vector4Cmp( zpVector4fParamF a, zpVector4fParamF b )
	{
		zpVector4fCmp cmp;

		__m128 lt = _mm_cmplt_ps( a, b );
		__m128 gt = _mm_cmpgt_ps( a, b );

		cmp.cmp[0] = lt.m128_i32[0] ? -1 : gt.m128_i32[0] ? 1 : 0;
		cmp.cmp[1] = lt.m128_i32[1] ? -1 : gt.m128_i32[1] ? 1 : 0;
		cmp.cmp[2] = lt.m128_i32[2] ? -1 : gt.m128_i32[2] ? 1 : 0;
		cmp.cmp[3] = lt.m128_i32[3] ? -1 : gt.m128_i32[3] ? 1 : 0;
		return cmp;
	}


	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL QuaternionToMatrix( zpQuaternion4fParamF a )
	{
		zp_float x = AsFloat( QuaternionGetX( a ) );
		zp_float y = AsFloat( QuaternionGetY( a ) );
		zp_float z = AsFloat( QuaternionGetZ( a ) );
		zp_float w = AsFloat( QuaternionGetW( a ) );

		zp_float xx = x * x;
		zp_float xy = x * y;
		zp_float xz = x * z;
		zp_float xw = x * w;
		zp_float yy = y * y;
		zp_float yz = y * z;
		zp_float yw = y * w;
		zp_float zz = z * z;
		zp_float zw = z * w;

		zpMatrix4f s;
		s.r[ 0 ] = Vector4( 1 - 2 * (yy + zz),     2 * (xy + zw),     2 * (xz - yw), 0 );
		s.r[ 1 ] = Vector4(     2 * (xy - zw), 1 - 2 * (xx + zz),     2 * (yz + xw), 0 );
		s.r[ 2 ] = Vector4(     2 * (xz + yw),     2 * (yz - xw), 1 - 2 * (xx + yy), 0 );
		s.r[ 3 ] = Vector4( 0, 0, 0, 1 );

		return s;
	}
};

#if 0
ZP_FORCE_INLINE void zpMath::Sin( zpScalar& s, const zpScalar& a )
{
	//s.m_x = zp_sin( a.m_x );
	s = zpScalar( zp_sin( a.getFloat() ) );
}
ZP_FORCE_INLINE void zpMath::Cos( zpScalar& s, const zpScalar& a )
{
	//s.m_x = zp_cos( a.m_x );
	s = zpScalar( zp_cos( a.getFloat() ) );
}
ZP_FORCE_INLINE void zpMath::Tan( zpScalar& s, const zpScalar& a )
{
	//s.m_x = zp_tan( a.m_x );
	s = zpScalar( zp_tan( a.getFloat() ) );
}

ZP_FORCE_INLINE void zpMath::Add( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = _mm_add_ps( a.m_x, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Sub( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = _mm_sub_ps( a.m_x, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Mul( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x =_mm_mul_ps( a.m_x, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Div( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = _mm_div_ps( a.m_x, b.m_x );
}

ZP_FORCE_INLINE void zpMath::Add( zpVector4f& s, const zpVector4f& a, const zpScalar& b )
{
	s.m_xyzw = _mm_add_ps( a.m_xyzw, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Sub( zpVector4f& s, const zpVector4f& a, const zpScalar& b )
{
	s.m_xyzw = _mm_sub_ps( a.m_xyzw, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Mul( zpVector4f& s, const zpVector4f& a, const zpScalar& b )
{
	s.m_xyzw = _mm_mul_ps( a.m_xyzw, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Div( zpVector4f& s, const zpVector4f& a, const zpScalar& b )
{
	s.m_xyzw = _mm_div_ps( a.m_xyzw, b.m_x );
}

ZP_FORCE_INLINE void zpMath::Add( zpVector4f& s, const zpScalar& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_add_ps( a.m_x, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Sub( zpVector4f& s, const zpScalar& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_sub_ps( a.m_x, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Mul( zpVector4f& s, const zpScalar& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_mul_ps( a.m_x, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Div( zpVector4f& s, const zpScalar& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_div_ps( a.m_x, b.m_xyzw );
}

ZP_FORCE_INLINE void zpMath::Add( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_add_ps( a.m_xyzw, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Sub( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_sub_ps( a.m_xyzw, b.m_xyzw );

}
ZP_FORCE_INLINE void zpMath::Mul( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_mul_ps( a.m_xyzw, b.m_xyzw );

}
ZP_FORCE_INLINE void zpMath::Div( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_div_ps( a.m_xyzw, b.m_xyzw );
}

ZP_FORCE_INLINE void zpMath::Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b )
{
	s.m_xyzw = _mm_add_ps( a.m_xyzw, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b )
{
	s.m_xyzw = _mm_sub_ps( a.m_xyzw, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b )
{
	s.m_xyzw = _mm_mul_ps( a.m_xyzw, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Div( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b )
{
	s.m_xyzw = _mm_div_ps( a.m_xyzw, b.m_x );
}

ZP_FORCE_INLINE void zpMath::Add( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b )
{
	s.m_xyzw = _mm_add_ps( a.m_x, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Sub( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b )
{
	s.m_xyzw = _mm_sub_ps( a.m_x, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Mul( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b )
{
	s.m_xyzw = _mm_mul_ps( a.m_x, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Div( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b )
{
	s.m_xyzw = _mm_div_ps( a.m_x, b.m_xyzw );
}

ZP_FORCE_INLINE void zpMath::Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_add_ps( a.m_xyzw, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_sub_ps( a.m_xyzw, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_mul_ps( a.m_xyzw, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Div( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_div_ps( a.m_xyzw, b.m_xyzw );
}

ZP_FORCE_INLINE void zpMath::Add( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw = _mm_add_ps( a.m_xyzw, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Sub( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw = _mm_sub_ps( a.m_xyzw, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Mul( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw = _mm_mul_ps( a.m_xyzw, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Div( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw = _mm_div_ps( a.m_xyzw, b.m_xyzw );
}

ZP_FORCE_INLINE void zpMath::Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw = _mm_add_ps( a.m_xyzw, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw = _mm_sub_ps( a.m_xyzw, b.m_xyzw );
}
ZP_FORCE_INLINE void zpMath::Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b )
{
	zp_float ax = a.m_xyzw.getX().getFloat();
	zp_float ay = a.m_xyzw.getY().getFloat();
	zp_float az = a.m_xyzw.getZ().getFloat();
	zp_float aw = a.m_xyzw.getW().getFloat();

	zp_float bx = b.m_xyzw.getX().getFloat();
	zp_float by = b.m_xyzw.getY().getFloat();
	zp_float bz = b.m_xyzw.getZ().getFloat();
	zp_float bw = b.m_xyzw.getW().getFloat();

	zp_float nx = aw * bx + ay * bw + ay * bz - az * by;
	zp_float ny = aw * by + ax * bw + az * bx - ax * bz;
	zp_float nz = aw * bz + az * bw + ax * by - ay * bx;
	zp_float nw = aw * bw - ax * bx - ay * by - az * bz;

	s.m_xyzw = zp_vec4( nx, ny, nz, nw );
}

ZP_FORCE_INLINE void zpMath::Madd( zpVector4f& s, const zpVector4f& a, const zpVector4f& b, const zpScalar& c )
{
	s.m_xyzw = _mm_add_ps( a.m_xyzw, _mm_mul_ps( b.m_xyzw, c.m_x ) );
}
ZP_FORCE_INLINE void zpMath::Madd( zpVector4f& s, const zpVector4f& a, const zpScalar& b, const zpVector4f& c )
{
	s.m_xyzw = _mm_add_ps( a.m_xyzw, _mm_mul_ps( b.m_x, c.m_xyzw ) );
}


ZP_FORCE_INLINE void zpMath::Dot2( zpScalar& s, const zpVector4f& a, const zpVector4f& b )
{
	__m128 m = _mm_mul_ps( a.m_xyzw, b.m_xyzw );
	__m128 x = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 0, 0, 0, 0 ) );
	__m128 y = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	s.m_x = _mm_add_ps( x, y );
}
ZP_FORCE_INLINE void zpMath::Dot3( zpScalar& s, const zpVector4f& a, const zpVector4f& b )
{
	__m128 m = _mm_mul_ps( a.m_xyzw, b.m_xyzw );
	__m128 x = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 0, 0, 0, 0 ) );
	__m128 y = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	__m128 z = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 2, 2, 2, 2 ) );
	s.m_x = _mm_add_ps( x, _mm_add_ps( y, z ) );
}
ZP_FORCE_INLINE void zpMath::Dot4( zpScalar& s, const zpVector4f& a, const zpVector4f& b )
{
	__m128 t0 = _mm_mul_ps( a.m_xyzw, b.m_xyzw );
	__m128 t1 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 1, 0, 3, 2 ) );
	__m128 t2 = _mm_add_ps( t0, t1 );
	__m128 t3 = _mm_shuffle_ps( t2, t2, _MM_SHUFFLE( 2, 3, 0, 1 ) );
	s.m_x = _mm_add_ps( t3, t2 );
}

ZP_FORCE_INLINE void zpMath::Cross3( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	__m128 s0 = _mm_shuffle_ps( a.m_xyzw, a.m_xyzw, _MM_SHUFFLE( 3, 0, 2, 1 ) );
	__m128 s1 = _mm_shuffle_ps( b.m_xyzw, b.m_xyzw, _MM_SHUFFLE( 3, 1, 0, 2 ) );
	__m128 s2 = _mm_mul_ps( s0, s1 );

	__m128 s3 = _mm_shuffle_ps( a.m_xyzw, a.m_xyzw, _MM_SHUFFLE( 3, 1, 0, 2 ) );
	__m128 s4 = _mm_shuffle_ps( b.m_xyzw, b.m_xyzw, _MM_SHUFFLE( 3, 0, 2, 1 ) );
	__m128 s5 = _mm_mul_ps( s3, s4 );

	s.m_xyzw = _mm_sub_ps( s2, s5 );
}

ZP_FORCE_INLINE void zpMath::Abs( zpScalar& s, const zpScalar& a )
{
	s.m_x = _mm_and_ps( a.m_x, *(__m128*)&_mm_set1_epi32( 0x7fffffff ) );
}
ZP_FORCE_INLINE void zpMath::Abs( zpVector4f& s, const zpVector4f& a )
{
	s.m_xyzw = _mm_and_ps( a.m_xyzw, *(__m128*)&_mm_set1_epi32( 0x7fffffff ) );
}

ZP_FORCE_INLINE void zpMath::Neg( zpScalar& s, const zpScalar& a )
{
	s.m_x = _mm_xor_ps( a.m_x, *(__m128*)&_mm_set1_epi32( 0x80000000 ) );
}
ZP_FORCE_INLINE void zpMath::Neg( zpVector4f& s, const zpVector4f& a )
{
	s.m_xyzw = _mm_xor_ps( a.m_xyzw, *(__m128*)&_mm_set1_epi32( 0x80000000 ) );
}

ZP_FORCE_INLINE void zpMath::Rcp( zpScalar& s, const zpScalar& a )
{
	s.m_x = _mm_rcp_ps( a.m_x );
}
ZP_FORCE_INLINE void zpMath::Rcp( zpVector4f& s, const zpVector4f& a )
{
	s.m_xyzw = _mm_rcp_ps( a.m_xyzw );
}

ZP_FORCE_INLINE void zpMath::Sqrt( zpScalar& s, const zpScalar& a )
{
	s.m_x = _mm_sqrt_ps( a.m_x );
}
ZP_FORCE_INLINE void zpMath::Sqrt( zpVector4f& s, const zpVector4f& a )
{
	s.m_xyzw = _mm_sqrt_ps( a.m_xyzw );
}

ZP_FORCE_INLINE void zpMath::Max( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = _mm_max_ps( a.m_x, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Max( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_max_ps( a.m_xyzw, b.m_xyzw );
}

ZP_FORCE_INLINE void zpMath::Min( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = _mm_min_ps( a.m_x, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Min( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw = _mm_min_ps( a.m_xyzw, b.m_xyzw );
}

ZP_FORCE_INLINE void zpMath::Cmp( zp_int& s, const zpScalar& a, const zpScalar& b )
{
	__m128 lt = _mm_cmplt_ps( a.m_x, b.m_x );
	__m128 gt = _mm_cmpgt_ps( a.m_x, b.m_x );
	s = lt.m128_i32[0] ? -1 : gt.m128_i32[0] ? 1 : 0;
}

ZP_FORCE_INLINE void zpMath::Cmp( zp_int& s, const zpVector4f& a, const zpVector4f& b )
{

}



void zpMath::SetEulerAngle( zpQuaternion4f& s, const zpScalar& yaw, const zpScalar& pitch, const zpScalar& roll )
{
	zp_float hr = roll.getFloat() * 0.5f;
	zp_float shr = zp_sin(hr);
	zp_float chr = zp_cos(hr);
	zp_float hp = pitch.getFloat() * 0.5f;
	zp_float shp = zp_sin(hp);
	zp_float chp = zp_cos(hp);
	zp_float hy = yaw.getFloat() * 0.5f;
	zp_float shy = zp_sin(hy);
	zp_float chy = zp_cos(hy);
	zp_float chy_shp = chy * shp;
	zp_float shy_chp = shy * chp;
	zp_float chy_chp = chy * chp;
	zp_float shy_shp = shy * shp;

	zp_float sx = (chy_shp * chr) + (shy_chp * shr); // cos(yaw/2) * sin(pitch/2) * cos(roll/2) + sin(yaw/2) * cos(pitch/2) * sin(roll/2)
	zp_float sy = (shy_chp * chr) - (chy_shp * shr); // sin(yaw/2) * cos(pitch/2) * cos(roll/2) - cos(yaw/2) * sin(pitch/2) * sin(roll/2)
	zp_float sz = (chy_chp * shr) - (shy_shp * chr); // cos(yaw/2) * cos(pitch/2) * sin(roll/2) - sin(yaw/2) * sin(pitch/2) * cos(roll/2)
	zp_float sw = (chy_chp * chr) + (shy_shp * shr); // cos(yaw/2) * cos(pitch/2) * cos(roll/2) + sin(yaw/2) * sin(pitch/2) * sin(roll/2)

	s = zpQuaternion4f( sx, sy, sz, sw );
}

void zpMath::GetRoll( zpScalar& s, const zpQuaternion4f& a )
{

}
void zpMath::GetPitch( zpScalar& s, const zpQuaternion4f& a )
{

}
void zpMath::GetYaw( zpScalar& s, const zpQuaternion4f& a )
{

}

void zpMath::Conjugate( zpQuaternion4f& s, const zpQuaternion4f& a )
{
	zpMath::Mul( s, a, zpVector4f( -1, -1, -1, 1 ) );
}
void zpMath::Transform( zpVector4f& s, const zpQuaternion4f& a, const zpVector4f& b )
{

}


void zpMath::ToMatrix( zpMatrix4f& s, const zpQuaternion4f& a )
{
	zp_float x = a.getX().getFloat();
	zp_float y = a.getY().getFloat();
	zp_float z = a.getZ().getFloat();
	zp_float w = a.getW().getFloat();

	zp_float xx = x * x;
	zp_float xy = x * y;
	zp_float xz = x * z;
	zp_float xw = x * w;
	zp_float yy = y * y;
	zp_float yz = y * z;
	zp_float yw = y * w;
	zp_float zz = z * z;
	zp_float zw = z * w;

	s.m_matrix[0][0] = 1 - 2 * (yy + zz);
	s.m_matrix[0][1] = 2 * (xy - zw);
	s.m_matrix[0][2] = 2 * (xz + yw);
	s.m_matrix[0][3] = 0;
	s.m_matrix[1][0] = 2 * (xy + zw);
	s.m_matrix[1][1] = 1 - 2 * (xx + zz);
	s.m_matrix[1][2] = 2 * (yz - xw);
	s.m_matrix[1][3] = 0;
	s.m_matrix[2][0] = 2 * (xz - yw);
	s.m_matrix[2][1] = 2 * (yz + xw);
	s.m_matrix[2][2] = 1 - 2 * (xx + yy);
	s.m_matrix[2][3] = 0;
	s.m_matrix[3][0] = 0;
	s.m_matrix[3][1] = 0;
	s.m_matrix[3][2] = 0;
	s.m_matrix[3][3] = 1;
}
#endif
