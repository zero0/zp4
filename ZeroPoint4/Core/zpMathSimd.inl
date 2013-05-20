
ZP_FORCE_INLINE void zpMath::DegToRad( zpScalar& s, const zpScalar& a )
{
	//s.m_x = ZP_DEG_TO_RAD( a.m_x );
}
ZP_FORCE_INLINE void zpMath::RadToDeg( zpScalar& s, const zpScalar& a )
{
	//s.m_x = ZP_RAD_TO_DEG( a.m_x );
}

ZP_FORCE_INLINE void zpMath::Sin( zpScalar& s, const zpScalar& a )
{
	//s.m_x = zp_sin( a.m_x );
}
ZP_FORCE_INLINE void zpMath::Cos( zpScalar& s, const zpScalar& a )
{
	//s.m_x = zp_cos( a.m_x );
}
ZP_FORCE_INLINE void zpMath::Tan( zpScalar& s, const zpScalar& a )
{
	//s.m_x = zp_tan( a.m_x );
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

ZP_FORCE_INLINE void zpMath::LengthSquared2( zpScalar& s, const zpVector4f& a )
{
	zpMath::Dot2( s, a, a );
}
ZP_FORCE_INLINE void zpMath::LengthSquared3( zpScalar& s, const zpVector4f& a )
{
	zpMath::Dot3( s, a, a );
}

ZP_FORCE_INLINE void zpMath::Length2( zpScalar& s, const zpVector4f& a )
{
	zpMath::Dot2( s, a, a );
	zpMath::Sqrt( s, s );
}
ZP_FORCE_INLINE void zpMath::Length3( zpScalar& s, const zpVector4f& a )
{
	zpMath::Dot3( s, a, a );
	zpMath::Sqrt( s, s );
}

ZP_FORCE_INLINE void zpMath::Normalize2( zpVector4f& s, const zpVector4f& a )
{
	zpScalar l;
	zpMath::Length2( l, a );
	zpMath::Div( s, a, l );
}
ZP_FORCE_INLINE void zpMath::Normalize3( zpVector4f& s, const zpVector4f& a )
{
	zpScalar l;
	zpMath::Length3( l, a );
	zpMath::Div( s, a, l );
}

ZP_FORCE_INLINE void zpMath::Mul( zpVector4f& s, const zpVector4f& a, const zpMatrix4f& b )
{
	zpVector4f col;

	zpMath::Mul( col, a.getX(), b.getRow( 0 ) );
	zpMath::Madd( col, col, a.getY(), b.getRow( 1 ) );
	zpMath::Madd( col, col, a.getZ(), b.getRow( 2 ) );
	zpMath::Madd( col, col, a.getW(), b.getRow( 3 ) );

	s = col;
}
ZP_FORCE_INLINE void zpMath::Mul( zpMatrix4f& s, const zpMatrix4f& a, const zpMatrix4f& b )
{
	zpVector4f col1, col2, col3, col4;

	zpMath::Mul( col1, a.getRow( 0 ).getX(), b.getRow( 0 ) );
	zpMath::Mul( col2, a.getRow( 1 ).getX(), b.getRow( 0 ) );
	zpMath::Mul( col3, a.getRow( 2 ).getX(), b.getRow( 0 ) );
	zpMath::Mul( col4, a.getRow( 3 ).getX(), b.getRow( 0 ) );

	zpMath::Madd( col1, col1, a.getRow( 0 ).getY(), b.getRow( 1 ) );
	zpMath::Madd( col2, col2, a.getRow( 1 ).getY(), b.getRow( 1 ) );
	zpMath::Madd( col3, col3, a.getRow( 2 ).getY(), b.getRow( 1 ) );
	zpMath::Madd( col4, col4, a.getRow( 3 ).getY(), b.getRow( 1 ) );

	zpMath::Madd( col1, col1, a.getRow( 0 ).getZ(), b.getRow( 2 ) );
	zpMath::Madd( col2, col2, a.getRow( 1 ).getZ(), b.getRow( 2 ) );
	zpMath::Madd( col3, col3, a.getRow( 2 ).getZ(), b.getRow( 2 ) );
	zpMath::Madd( col4, col4, a.getRow( 3 ).getZ(), b.getRow( 2 ) );

	zpMath::Madd( col1, col1, a.getRow( 0 ).getW(), b.getRow( 3 ) );
	zpMath::Madd( col2, col2, a.getRow( 1 ).getW(), b.getRow( 3 ) );
	zpMath::Madd( col3, col3, a.getRow( 2 ).getW(), b.getRow( 3 ) );
	zpMath::Madd( col4, col4, a.getRow( 3 ).getW(), b.getRow( 3 ) );

	s.setRow( 0, col1 );
	s.setRow( 1, col2 );
	s.setRow( 2, col3 );
	s.setRow( 3, col4 );
}
ZP_FORCE_INLINE void zpMath::Mul( zpMatrix4f& s, const zpScalar& a, const zpMatrix4f& b )
{
	zpVector4f row1, row2, row3, row4;

	zpMath::Mul( row1, a, b.getRow( 0 ) );
	zpMath::Mul( row2, a, b.getRow( 1 ) );
	zpMath::Mul( row3, a, b.getRow( 2 ) );
	zpMath::Mul( row4, a, b.getRow( 3 ) );

	s.setRow( 0, row1 );
	s.setRow( 1, row2 );
	s.setRow( 2, row3 );
	s.setRow( 3, row4 );
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
	zpMath::Mul( s, a, zpScalar( -1.0f ) );
}
ZP_FORCE_INLINE void zpMath::Neg( zpVector4f& s, const zpVector4f& a )
{
	zpMath::Mul( s, a, zpScalar( -1.0f ) );
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
ZP_FORCE_INLINE void zpMath::Cmp0( zp_int& s, const zpScalar& a )
{
	zpMath::Cmp( s, a, zpScalar( 0.0f ) );
}

ZP_FORCE_INLINE void zpMath::Cmp( zp_int& s, const zpVector4f& a, const zpVector4f& b )
{

}

ZP_FORCE_INLINE void zpMath::LookAtLH( zpMatrix4f& s, const zpVector4f& eye, const zpVector4f& direction, const zpVector4f& up )
{
	zpVector4f x, y, z, e;
	zpScalar ex, ey, ez;

	zpMath::Normalize3( z, direction );
	zpMath::Cross3( x, up, z );
	zpMath::Cross3( y, z, x );
	zpMath::Neg( e, eye );
	zpMath::Neg( x, x );

	zpMath::Dot3( ex, e, x );
	zpMath::Dot3( ey, e, y );
	zpMath::Dot3( ez, e, z );

	s.setRow( 0, x );
	s.setRow( 1, y );
	s.setRow( 2, z );
	s.setRow( 3, zpVector4f( ex, ey, ez, zpScalar( 1.0f ) ) );

	zpMath::Transpose( s, s );
}

ZP_FORCE_INLINE void zpMath::PerspectiveLH( zpMatrix4f& s, const zpScalar& fovy, const zpScalar& aspectRatio, const zpScalar& zNear, const zpScalar& zFar )
{
	zpScalar f, fa, z( 0.0f ), o( 1.0f ), fn, nf, r22, r23;
	zpMath::DegToRad( f, f );
	zpMath::Mul( f, f, zpScalar( 0.5f ) );
	zpMath::Tan( f, f );
	zpMath::Rcp( f, f );

	zpMath::Div( fa, f, aspectRatio );
	zpMath::Add( fn, zFar, zNear );
	zpMath::Neg( fn, fn );

	zpMath::Sub( nf, zNear, zFar );

	zpMath::Div( r22, fn, nf );

	zpMath::Mul( r23, zpScalar( 2.0f ), zNear );
	zpMath::Mul( r23, r23, zFar );
	zpMath::Div( r23, r23, nf );

	s.setRow( 0, zpVector4f( fa, z, z,   z ) );
	s.setRow( 1, zpVector4f( z,  f, z,   z ) );
	s.setRow( 2, zpVector4f( z,  z, r22, r23 ) );
	s.setRow( 3, zpVector4f( z,  z, o,   z ) );
}

ZP_FORCE_INLINE void zpMath::Transpose( zpMatrix4f& s, const zpMatrix4f& a )
{
	zp_float m[4][4];
	m[0][0] = a.m_matrix[0][0];
	m[0][1] = a.m_matrix[1][0];
	m[0][2] = a.m_matrix[2][0];
	m[0][3] = a.m_matrix[3][0];

	m[1][0] = a.m_matrix[0][1];
	m[1][1] = a.m_matrix[1][1];
	m[1][2] = a.m_matrix[2][1];
	m[1][3] = a.m_matrix[3][1];

	m[2][0] = a.m_matrix[0][2];
	m[2][1] = a.m_matrix[1][2];
	m[2][2] = a.m_matrix[2][2];
	m[2][3] = a.m_matrix[3][2];

	m[3][0] = a.m_matrix[0][3];
	m[3][1] = a.m_matrix[1][3];
	m[3][2] = a.m_matrix[2][3];
	m[3][3] = a.m_matrix[3][3];

	zp_memcpy( s.m_data, sizeof( s.m_data ), m, sizeof( s.m_data ) );
}
ZP_FORCE_INLINE void zpMath::Determinant( zpScalar& s, const zpMatrix4f& a )
{
	s = zpScalar(
		a.m_14*a.m_23*a.m_32*a.m_41 - a.m_13*a.m_24*a.m_32*a.m_41 - a.m_14*a.m_22*a.m_33*a.m_41 + a.m_12*a.m_24*a.m_33*a.m_41+
		a.m_13*a.m_22*a.m_34*a.m_41 - a.m_12*a.m_23*a.m_34*a.m_41 - a.m_14*a.m_23*a.m_31*a.m_42 + a.m_13*a.m_24*a.m_31*a.m_42+
		a.m_14*a.m_21*a.m_33*a.m_42 - a.m_11*a.m_24*a.m_33*a.m_42 - a.m_13*a.m_21*a.m_34*a.m_42 + a.m_11*a.m_23*a.m_34*a.m_42+
		a.m_14*a.m_22*a.m_31*a.m_43 - a.m_12*a.m_24*a.m_31*a.m_43 - a.m_14*a.m_21*a.m_32*a.m_43 + a.m_11*a.m_24*a.m_32*a.m_43+
		a.m_12*a.m_21*a.m_34*a.m_43 - a.m_11*a.m_22*a.m_34*a.m_43 - a.m_13*a.m_22*a.m_31*a.m_44 + a.m_12*a.m_23*a.m_31*a.m_44+
		a.m_13*a.m_21*a.m_32*a.m_44 - a.m_11*a.m_23*a.m_32*a.m_44 - a.m_12*a.m_21*a.m_33*a.m_44 + a.m_11*a.m_22*a.m_33*a.m_44
		);
}
ZP_FORCE_INLINE void zpMath::Invert( zpMatrix4f& s, const zpMatrix4f& a )
{
	zpScalar d;
	zpMath::Determinant( d, a );
	zpMath::Rcp( d, d );

	s.m_data[0]  = a.m_23*a.m_34*a.m_42 - a.m_24*a.m_33*a.m_42 + a.m_24*a.m_32*a.m_43 - a.m_22*a.m_34*a.m_43 - a.m_23*a.m_32*a.m_44 + a.m_22*a.m_33*a.m_44;
	s.m_data[1]  = a.m_14*a.m_33*a.m_42 - a.m_13*a.m_34*a.m_42 - a.m_14*a.m_32*a.m_43 + a.m_12*a.m_34*a.m_43 + a.m_13*a.m_32*a.m_44 - a.m_12*a.m_33*a.m_44;
	s.m_data[2]  = a.m_13*a.m_24*a.m_42 - a.m_14*a.m_23*a.m_42 + a.m_14*a.m_22*a.m_43 - a.m_12*a.m_24*a.m_43 - a.m_13*a.m_22*a.m_44 + a.m_12*a.m_23*a.m_44;
	s.m_data[3]  = a.m_14*a.m_23*a.m_32 - a.m_13*a.m_24*a.m_32 - a.m_14*a.m_22*a.m_33 + a.m_12*a.m_24*a.m_33 + a.m_13*a.m_22*a.m_34 - a.m_12*a.m_23*a.m_34;
	s.m_data[4]  = a.m_24*a.m_33*a.m_41 - a.m_23*a.m_34*a.m_41 - a.m_24*a.m_31*a.m_43 + a.m_21*a.m_34*a.m_43 + a.m_23*a.m_31*a.m_44 - a.m_21*a.m_33*a.m_44;
	s.m_data[5]  = a.m_13*a.m_34*a.m_41 - a.m_14*a.m_33*a.m_41 + a.m_14*a.m_31*a.m_43 - a.m_11*a.m_34*a.m_43 - a.m_13*a.m_31*a.m_44 + a.m_11*a.m_33*a.m_44;
	s.m_data[6]  = a.m_14*a.m_23*a.m_41 - a.m_13*a.m_24*a.m_41 - a.m_14*a.m_21*a.m_43 + a.m_11*a.m_24*a.m_43 + a.m_13*a.m_21*a.m_44 - a.m_11*a.m_23*a.m_44;
	s.m_data[7]  = a.m_13*a.m_24*a.m_31 - a.m_14*a.m_23*a.m_31 + a.m_14*a.m_21*a.m_33 - a.m_11*a.m_24*a.m_33 - a.m_13*a.m_21*a.m_34 + a.m_11*a.m_23*a.m_34;
	s.m_data[8]  = a.m_22*a.m_34*a.m_41 - a.m_24*a.m_32*a.m_41 + a.m_24*a.m_31*a.m_42 - a.m_21*a.m_34*a.m_42 - a.m_22*a.m_31*a.m_44 + a.m_21*a.m_32*a.m_44;
	s.m_data[9]  = a.m_14*a.m_32*a.m_41 - a.m_12*a.m_34*a.m_41 - a.m_14*a.m_31*a.m_42 + a.m_11*a.m_34*a.m_42 + a.m_12*a.m_31*a.m_44 - a.m_11*a.m_32*a.m_44;
	s.m_data[10] = a.m_12*a.m_24*a.m_41 - a.m_14*a.m_22*a.m_41 + a.m_14*a.m_21*a.m_42 - a.m_11*a.m_24*a.m_42 - a.m_12*a.m_21*a.m_44 + a.m_11*a.m_22*a.m_44;
	s.m_data[11] = a.m_14*a.m_22*a.m_31 - a.m_12*a.m_24*a.m_31 - a.m_14*a.m_21*a.m_32 + a.m_11*a.m_24*a.m_32 + a.m_12*a.m_21*a.m_34 - a.m_11*a.m_22*a.m_34;
	s.m_data[12] = a.m_23*a.m_32*a.m_41 - a.m_22*a.m_33*a.m_41 - a.m_23*a.m_31*a.m_42 + a.m_21*a.m_33*a.m_42 + a.m_22*a.m_31*a.m_43 - a.m_21*a.m_32*a.m_43;
	s.m_data[13] = a.m_12*a.m_33*a.m_41 - a.m_13*a.m_32*a.m_41 + a.m_13*a.m_31*a.m_42 - a.m_11*a.m_33*a.m_42 - a.m_12*a.m_31*a.m_43 + a.m_11*a.m_32*a.m_43;
	s.m_data[14] = a.m_13*a.m_22*a.m_41 - a.m_12*a.m_23*a.m_41 - a.m_13*a.m_21*a.m_42 + a.m_11*a.m_23*a.m_42 + a.m_12*a.m_21*a.m_43 - a.m_11*a.m_22*a.m_43;
	s.m_data[15] = a.m_12*a.m_23*a.m_31 - a.m_13*a.m_22*a.m_31 + a.m_13*a.m_21*a.m_32 - a.m_11*a.m_23*a.m_32 - a.m_12*a.m_21*a.m_33 + a.m_11*a.m_22*a.m_33;

	zpMath::Mul( s, d, s );
}
