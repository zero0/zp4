
ZP_FORCE_INLINE void zpMath::DegToRad( zpScalar& s, const zpScalar& a )
{
	zpMath::Mul( s, a, zpScalar( ZP_PIOVER180 ) );
}
ZP_FORCE_INLINE void zpMath::RadToDeg( zpScalar& s, const zpScalar& a )
{
	zpMath::Mul( s, a, zpScalar( ZP_180OVERPI ) );
}

ZP_FORCE_INLINE void zpMath::LengthSquared2( zpScalar& s, const zpVector4f& a )
{
	zpMath::Dot2( s, a, a );
}
ZP_FORCE_INLINE void zpMath::LengthSquared3( zpScalar& s, const zpVector4f& a )
{
	zpMath::Dot3( s, a, a );
}
ZP_FORCE_INLINE void zpMath::LengthSquared4( zpScalar& s, const zpVector4f& a )
{
	zpMath::Dot4( s, a, a );
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
ZP_FORCE_INLINE void zpMath::Length4( zpScalar& s, const zpVector4f& a )
{
	zpMath::Dot4( s, a, a );
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
ZP_FORCE_INLINE void zpMath::Normalize4( zpVector4f& s, const zpVector4f& a )
{
	zpScalar l;
	zpMath::Length4( l, a );
	zpMath::Div( s, a, l );
}
ZP_FORCE_INLINE void zpMath::Normalize4( zpQuaternion4f& s, const zpQuaternion4f& a )
{
	zpScalar l;
	zpMath::Length4( l, a );
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

ZP_FORCE_INLINE void zpMath::Perpendicular3( zpVector4f& s, const zpVector4f& a )
{
	zpScalar l;
	Cross3( s, a, zpVector4f( 1, 0, 0 ) );
	LengthSquared3( l, s );

	if( Cmp( l, zpScalar( ZP_EPSILON ) ) < 0 )
	{
		Cross3( s, a, zpVector4f( 0, 1, 0 ) );
	}
}

ZP_FORCE_INLINE zp_int zpMath::Cmp( const zpScalar& a, const zpScalar& b )
{
	zp_int s;
	Cmp( s, a, b );
	return s;
}
ZP_FORCE_INLINE zp_int zpMath::Cmp0( const zpScalar& a )
{
	zp_int s;
	Cmp0( s, a );
	return s;
}

ZP_FORCE_INLINE zp_int zpMath::Cmp( const zpVector4f& a, const zpVector4f& b )
{
	zp_int s;
	Cmp( s, a, b );
	return s;
}

ZP_FORCE_INLINE void zpMath::Cmp0( zp_int& s, const zpScalar& a )
{
	zpMath::Cmp( s, a, zpScalar( 0.0f ) );
}

ZP_FORCE_INLINE void zpMath::Transpose( zpMatrix4f& s, const zpMatrix4f& a )
{
	zpMatrix4f m;
	m.m_matrix[0][0] = a.m_matrix[0][0];
	m.m_matrix[0][1] = a.m_matrix[1][0];
	m.m_matrix[0][2] = a.m_matrix[2][0];
	m.m_matrix[0][3] = a.m_matrix[3][0];

	m.m_matrix[1][0] = a.m_matrix[0][1];
	m.m_matrix[1][1] = a.m_matrix[1][1];
	m.m_matrix[1][2] = a.m_matrix[2][1];
	m.m_matrix[1][3] = a.m_matrix[3][1];

	m.m_matrix[2][0] = a.m_matrix[0][2];
	m.m_matrix[2][1] = a.m_matrix[1][2];
	m.m_matrix[2][2] = a.m_matrix[2][2];
	m.m_matrix[2][3] = a.m_matrix[3][2];

	m.m_matrix[3][0] = a.m_matrix[0][3];
	m.m_matrix[3][1] = a.m_matrix[1][3];
	m.m_matrix[3][2] = a.m_matrix[2][3];
	m.m_matrix[3][3] = a.m_matrix[3][3];

	s = m;
}
ZP_FORCE_INLINE void zpMath::Determinant( zpScalar& s, const zpMatrix4f& a )
{
	zp_float d = 
		a.m_14*a.m_23*a.m_32*a.m_41 - a.m_13*a.m_24*a.m_32*a.m_41 - a.m_14*a.m_22*a.m_33*a.m_41 + a.m_12*a.m_24*a.m_33*a.m_41+
		a.m_13*a.m_22*a.m_34*a.m_41 - a.m_12*a.m_23*a.m_34*a.m_41 - a.m_14*a.m_23*a.m_31*a.m_42 + a.m_13*a.m_24*a.m_31*a.m_42+
		a.m_14*a.m_21*a.m_33*a.m_42 - a.m_11*a.m_24*a.m_33*a.m_42 - a.m_13*a.m_21*a.m_34*a.m_42 + a.m_11*a.m_23*a.m_34*a.m_42+
		a.m_14*a.m_22*a.m_31*a.m_43 - a.m_12*a.m_24*a.m_31*a.m_43 - a.m_14*a.m_21*a.m_32*a.m_43 + a.m_11*a.m_24*a.m_32*a.m_43+
		a.m_12*a.m_21*a.m_34*a.m_43 - a.m_11*a.m_22*a.m_34*a.m_43 - a.m_13*a.m_22*a.m_31*a.m_44 + a.m_12*a.m_23*a.m_31*a.m_44+
		a.m_13*a.m_21*a.m_32*a.m_44 - a.m_11*a.m_23*a.m_32*a.m_44 - a.m_12*a.m_21*a.m_33*a.m_44 + a.m_11*a.m_22*a.m_33*a.m_44;
	s = zpScalar( d );
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

ZP_FORCE_INLINE void zpMath::LookAtLH( zpMatrix4f& s, const zpVector4f& eye, const zpVector4f& direction, const zpVector4f& up )
{
	zpVector4f x, y, z;
	zpScalar ex, ey, ez;

	zpMath::Normalize3( z, direction );
	zpMath::Cross3( x, up, z );
	zpMath::Normalize3( x, x );
	zpMath::Cross3( y, z, x );

	zpMath::Dot3( ex, x, eye );
	zpMath::Dot3( ey, y, eye );
	zpMath::Dot3( ez, z, eye );

	zpMath::Neg( ex, ex );
	zpMath::Neg( ey, ey );
	zpMath::Neg( ez, ez );

	s.setRow( 0, x );
	s.setRow( 1, y );
	s.setRow( 2, z );
	s.setRow( 3, zpVector4f( 0 ) );
	zpMath::Transpose( s, s );

	s.setRow( 3, zpVector4f( ex, ey, ez, zpScalar( 1.0f ) ) );
}
ZP_FORCE_INLINE void zpMath::LookAtRH( zpMatrix4f& s, const zpVector4f& eye, const zpVector4f& direction, const zpVector4f& up )
{
	zpVector4f x, y, z;
	zpScalar ex, ey, ez;

	zpMath::Neg( z, direction );
	zpMath::Normalize3( z, z );
	zpMath::Cross3( x, up, z );
	zpMath::Normalize3( x, x );
	zpMath::Cross3( y, z, x );

	zpMath::Dot3( ex, x, eye );
	zpMath::Dot3( ey, y, eye );
	zpMath::Dot3( ez, z, eye );

	s.setRow( 0, x );
	s.setRow( 1, y );
	s.setRow( 2, z );
	s.setRow( 3, zpVector4f( 0 ) );
	zpMath::Transpose( s, s );

	s.setRow( 3, zpVector4f( ex, ey, ez, zpScalar( 1.0f ) ) );
}

ZP_FORCE_INLINE void zpMath::PerspectiveLH( zpMatrix4f& s, const zpScalar& fovy, const zpScalar& aspectRatio, const zpScalar& zNear, const zpScalar& zFar )
{
	zpScalar yScale, xScale, z( 0.0f ), fn, r22, r32, o( 1.0f );
	zpMath::DegToRad( yScale, fovy );
	zpMath::Mul( yScale, yScale, zpScalar( 0.5f ) );
	zpMath::Tan( yScale, yScale );
	zpMath::Rcp( yScale, yScale );

	zpMath::Div( xScale, yScale, aspectRatio );

	zpMath::Sub( fn, zFar, zNear );

	zpMath::Div( r22, zFar, fn );

	zpMath::Mul( r32, zNear, r22 );
	zpMath::Neg( r32, r32 );

	s.setRow( 0, zpVector4f( xScale, z,      z,   z ) );
	s.setRow( 1, zpVector4f( z,      yScale, z,   z ) );
	s.setRow( 2, zpVector4f( z,      z,      r22, o ) );
	s.setRow( 3, zpVector4f( z,      z,      r32, z ) );
}
ZP_FORCE_INLINE void zpMath::PerspectiveRH( zpMatrix4f& s, const zpScalar& fovy, const zpScalar& aspectRatio, const zpScalar& zNear, const zpScalar& zFar )
{
	zpScalar yScale, xScale, z( 0.0f ), nf, r22, r32, no( -1.0f );
	zpMath::DegToRad( yScale, fovy );
	zpMath::Mul( yScale, yScale, zpScalar( 0.5f ) );
	zpMath::Tan( yScale, yScale );
	zpMath::Rcp( yScale, yScale );

	zpMath::Div( xScale, yScale, aspectRatio );

	zpMath::Sub( nf, zNear, zFar );

	zpMath::Div( r22, zFar, nf );

	zpMath::Mul( r32, zNear, r22 );

	s.setRow( 0, zpVector4f( xScale, z,      z,   z ) );
	s.setRow( 1, zpVector4f( z,      yScale, z,   z ) );
	s.setRow( 2, zpVector4f( z,      z,      r22, no ) );
	s.setRow( 3, zpVector4f( z,      z,      r32, z ) );
}
ZP_FORCE_INLINE void zpMath::OrthoLH( zpMatrix4f& s, const zpScalar& l, const zpScalar& r, const zpScalar& t, const zpScalar& b, const zpScalar& zNear, const zpScalar& zFar )
{
	zpScalar rml, lmr, rpl, tmb, bmt, tpb, fmn, nmf, h( 2.0f ), z( 0.0f ), o( 1.0f );
	zpScalar m00, m11, m22, m30, m31, m32;

	zpMath::Sub( rml, r, l );
	zpMath::Sub( lmr, l, r );
	zpMath::Sub( tmb, t, b );
	zpMath::Sub( bmt, b, t );
	zpMath::Sub( fmn, zFar, zNear );
	zpMath::Sub( nmf, zNear, zFar );

	zpMath::Add( rpl, r, l );
	zpMath::Add( tpb, t, b );

	// scale
	zpMath::Div( m00, h, rml );
	zpMath::Div( m11, h, tmb );
	zpMath::Rcp( m22, fmn );

	// translate
	zpMath::Div( m30, rpl, lmr );
	zpMath::Div( m31, tpb, bmt );
	zpMath::Div( m32, zNear, nmf );

	s.setRow( 0, zpVector4f( m00, z,   z,   z ) );
	s.setRow( 1, zpVector4f( z,   m11, z,   z ) );
	s.setRow( 2, zpVector4f( z,   z,   m22, z ) );
	s.setRow( 3, zpVector4f( m30, m31, m32, o ) );
}
ZP_FORCE_INLINE void zpMath::OrthoRH( zpMatrix4f& s, const zpScalar& l, const zpScalar& r, const zpScalar& t, const zpScalar& b, const zpScalar& zNear, const zpScalar& zFar )
{
	zpScalar rml, lmr, rpl, tmb, bmt, tpb, nmf, h( 2.0f ), z( 0.0f ), o( 1.0f );
	zpScalar m00, m11, m22, m30, m31, m32;

	zpMath::Sub( rml, r, l );
	zpMath::Sub( lmr, l, r );
	zpMath::Sub( tmb, t, b );
	zpMath::Sub( bmt, b, t );
	zpMath::Sub( nmf, zNear, zFar );

	zpMath::Add( rpl, r, l );
	zpMath::Add( tpb, t, b );

	// scale
	zpMath::Div( m00, h, rml );
	zpMath::Div( m11, h, tmb );
	zpMath::Rcp( m22, nmf );

	// translate
	zpMath::Div( m30, rpl, lmr );
	zpMath::Div( m31, tpb, bmt );
	zpMath::Div( m32, zNear, nmf );

	s.setRow( 0, zpVector4f( m00, z,   z,   z ) );
	s.setRow( 1, zpVector4f( z,   m11, z,   z ) );
	s.setRow( 2, zpVector4f( z,   z,   m22, z ) );
	s.setRow( 3, zpVector4f( m30, m31, m32, o ) );
}

ZP_FORCE_INLINE void zpMath::Lerp( zpVector4f& s, const zpVector4f& a, const zpVector4f& b, const zpScalar& alpha )
{
	zpVector4f l;
	zpMath::Sub( l, b, a );
	zpMath::Mul( l, l, alpha );
	zpMath::Add( s, l, a );
}
ZP_FORCE_INLINE void zpMath::Lerp( zpScalar& s, const zpScalar& a, const zpScalar& b, const zpScalar& alpha )
{
	zpScalar l;
	zpMath::Sub( l, b, a );
	zpMath::Mul( l, l, alpha );
	zpMath::Add( s, l, a );
}

ZP_FORCE_INLINE void zpMath::RotateX( zpVector4f& s, const zpVector4f& a, const zpScalar& rad )
{
	zpScalar angle, y, z, co, si, t0, t1;

	zpMath::Sin( si, rad );
	zpMath::Cos( co, rad );

	zpMath::Mul( t0, a.getY(), co );
	zpMath::Mul( t1, a.getZ(), si );
	zpMath::Sub( y, t0, t1 );

	zpMath::Mul( t0, a.getY(), si );
	zpMath::Mul( t1, a.getZ(), co );
	zpMath::Add( z, t0, t1 );

	s.setY( y );
	s.setZ( z );
}
ZP_FORCE_INLINE void zpMath::RotateY( zpVector4f& s, const zpVector4f& a, const zpScalar& rad )
{
	zpScalar angle, x, z, co, si, t0, t1, nsi;

	zpMath::Sin( si, rad );
	zpMath::Cos( co, rad );
	zpMath::Neg( nsi, si );

	zpMath::Mul( t0, a.getX(), co );
	zpMath::Mul( t1, a.getZ(), si );
	zpMath::Add( x, t0, t1 );

	zpMath::Mul( t0, a.getX(), nsi );
	zpMath::Mul( t1, a.getZ(), co );
	zpMath::Add( z, t0, t1 );

	s.setX( x );
	s.setZ( z );
}
ZP_FORCE_INLINE void zpMath::RotateZ( zpVector4f& s, const zpVector4f& a, const zpScalar& rad )
{
	zpScalar angle, y, x, co, si, t0, t1;

	zpMath::Sin( si, rad );
	zpMath::Cos( co, rad );

	zpMath::Mul( t0, a.getX(), co );
	zpMath::Mul( t1, a.getY(), si );
	zpMath::Sub( y, t0, t1 );

	zpMath::Mul( t0, a.getX(), si );
	zpMath::Mul( t1, a.getY(), co );
	zpMath::Add( x, t0, t1 );

	s.setX( x );
	s.setY( y );
}

void zpMath::AxisAngle( zpQuaternion4f& s, const zpVector4f& a, const zpScalar& b )
{
	zpScalar d, si, co, halfB;
	zpMath::Mul( halfB, b, zpScalar( 0.5f ) );

	zpMath::Length4( d, a );
	zpMath::Rcp( d, d );

	zpMath::Sin( si, halfB );
	zpMath::Cos( co, halfB );

	zpVector4f t;
	zpVector4f dv( d, d, d, zpScalar( 1.f ) );
	zpVector4f av( si, si, si, co );

	zpMath::Mul( t, a, dv );
	zpMath::Mul( t, t, av );

	zp_float buff[4];
	t.store4( buff );
	s.load4( buff );
}


void zpMath::TRS( zpMatrix4f& m, const zpVector4f& p, const zpQuaternion4f& r, const zpVector4f& s )
{
	zpMatrix4f pm;
	pm.setIdentity();
	pm.setRow( 3, p );

	zpMatrix4f rm;
	ToMatrix( rm, r );

	zpMatrix4f sm;
	sm.setIdentity();
	zpMath::Mul( sm.m_m1, sm.m_m1, s );
	zpMath::Mul( sm.m_m2, sm.m_m2, s );
	zpMath::Mul( sm.m_m3, sm.m_m3, s );

	zpMath::Mul( m, sm, rm );
	zpMath::Mul( m, m, pm );
}

void zpMath::Reflect( zpVector4f& s, const zpVector4f& a, const zpVector4f& n )
{
	zpScalar d;
	zpMath::Dot3( d, a, n );
	zpMath::Mul( d, d, zpScalar( -2.0f ) );

	zpMath::Madd( s, a, d, n );
}
