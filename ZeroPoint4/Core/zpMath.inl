
namespace zpMath
{
	ZP_FORCE_INLINE zp_int ZP_VECTORCALL ScalarCmp0( zpScalarParamF a )
	{
		return ScalarCmp( a, Scalar( 0.f ) );
	}

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarDegToRad( zpScalarParamF a )
	{
		return ScalarMul( a, Scalar( ZP_PIOVER180 ) );
	}
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarRadToDeg( zpScalarParamF a )
	{
		return ScalarMul( a, Scalar( ZP_180OVERPI ) );
	}

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Perpendicular3( zpVector4fParamF a )
	{
		zpVector4f c = Vector4Cross3( a, Vector4( 1, 0, 0, 0 ) );
		zpScalar l = Vector4LengthSquared3( c );
		zpScalar e = Scalar( ZP_EPSILON );
		zpScalar ne = ScalarNeg( e );

		if( ScalarCmp( l, e ) < 0 && ScalarCmp( l, ne ) > 0 )
		{
			c = Vector4Cross3( a, Vector4( 0, 1, 0, 0 ) );
		}

		return c;
	}

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Madd( zpVector4fParamF a, zpVector4fParamF b, zpScalarParamF c )
	{
		return Vector4Add( a, Vector4Scale( b, c ) );
	}

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4MulAdd( zpVector4fParamF a, zpVector4fParamF b, zpVector4fParamF c )
	{
		return Vector4Add( Vector4Mul( a, b ), c );
	}
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4ScaleAdd( zpVector4fParamF a, zpScalarParamF b, zpVector4fParamF c )
	{
		return Vector4Add( Vector4Scale( a, b ), c );
	}

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared2( zpVector4fParamF a )
	{
		return Vector4Dot2( a, a );
	}
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared3( zpVector4fParamF a )
	{
		return Vector4Dot3( a, a );
	}
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared4( zpVector4fParamF a )
	{
		return Vector4Dot4( a, a );
	}

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length2( zpVector4fParamF a )
	{
		return ScalarSqrt( Vector4Dot2( a, a ) );
	}
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length3( zpVector4fParamF a )
	{
		return ScalarSqrt( Vector4Dot3( a, a ) );
	}
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length4( zpVector4fParamF a )
	{
		return ScalarSqrt( Vector4Dot4( a, a ) );
	}
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionLength4( zpQuaternion4fParamF a )
	{
		return ScalarSqrt( Scalar( 0 ) );
	}

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize2( zpVector4fParamF a )
	{
		zpScalar l = Vector4Length2( a );
		return ScalarDiv( a, l );
	}
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize3( zpVector4fParamF a )
	{
		zpScalar l = Vector4Length3( a );
		return ScalarDiv( a, l );
	}
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize4( zpVector4fParamF a )
	{
		zpScalar l = Vector4Length4( a );
		return ScalarDiv( a, l );
	}
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionNormalize4( zpQuaternion4fParamF a )
	{
		zpScalar l = QuaternionLength4( a );
		return ScalarDiv( a, l );
	}

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL MatrixTransform( zpMatrix4fParamF a, zpVector4fParamC b )
	{
		zpVector4f col;

		col = Vector4Scale( a.m_m1, Vector4GetX( b ) );
		col = Vector4Madd( col, a.m_m2, Vector4GetY( b ) );
		col = Vector4Madd( col, a.m_m3, Vector4GetZ( b ) );
		col = Vector4Madd( col, a.m_m4, Vector4GetW( b ) );

		return col;
	}
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixMul( zpMatrix4fParamF a, zpMatrix4fParamC b )
	{
		zpMatrix4f s;

		zpVector4f col1 = Vector4Scale( b.m_m1, Vector4GetX( a.m_m1 ) );
		zpVector4f col2 = Vector4Scale( b.m_m1, Vector4GetX( a.m_m2 ) );
		zpVector4f col3 = Vector4Scale( b.m_m1, Vector4GetX( a.m_m3 ) );
		zpVector4f col4 = Vector4Scale( b.m_m1, Vector4GetX( a.m_m4 ) );

		col1 = Vector4Madd( col1, b.m_m2, Vector4GetY( a.m_m1 ) );
		col2 = Vector4Madd( col2, b.m_m2, Vector4GetY( a.m_m2 ) );
		col3 = Vector4Madd( col3, b.m_m2, Vector4GetY( a.m_m3 ) );
		col4 = Vector4Madd( col4, b.m_m2, Vector4GetY( a.m_m4 ) );

		col1 = Vector4Madd( col1, b.m_m3, Vector4GetZ( a.m_m1 ) );
		col2 = Vector4Madd( col2, b.m_m3, Vector4GetZ( a.m_m2 ) );
		col3 = Vector4Madd( col3, b.m_m3, Vector4GetZ( a.m_m3 ) );
		col4 = Vector4Madd( col4, b.m_m3, Vector4GetZ( a.m_m4 ) );

		s.m_m1 = Vector4Madd( col1, b.m_m4, Vector4GetW( a.m_m1 ) );
		s.m_m2 = Vector4Madd( col2, b.m_m4, Vector4GetW( a.m_m2 ) );
		s.m_m3 = Vector4Madd( col3, b.m_m4, Vector4GetW( a.m_m3 ) );
		s.m_m4 = Vector4Madd( col4, b.m_m4, Vector4GetW( a.m_m4 ) );

		return s;

	}
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixScale( zpMatrix4fParamF a, zpScalarParamC b )
	{
		zpMatrix4f s;

		s.m_m1 = Vector4Scale( a.m_m1, b );
		s.m_m2 = Vector4Scale( a.m_m2, b );
		s.m_m3 = Vector4Scale( a.m_m3, b );
		s.m_m4 = Vector4Scale( a.m_m1, b );

		return s;
	}


	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL LookAtLH( zpVector4fParamF eye, zpVector4fParamF direction, zpVector4fParamF up )
	{
		zpVector4f x, y, z;
		zpScalar ex, ey, ez;
		zpMatrix4f s;

		z = Vector4Normalize3( direction );
		x = Vector4Cross3( up, z );
		x = Vector4Normalize3( x );
		y = Vector4Cross3( z, x );

		ex = Vector4Dot3( x, eye );
		ey = Vector4Dot3( y, eye );
		ez = Vector4Dot3( z, eye );

		ex = Vector4Neg( ex );
		ey = Vector4Neg( ey );
		ez = Vector4Neg( ez );

		s.m_m1 = x;
		s.m_m2 = y;
		s.m_m3 = z;
		s.m_m4 = Vector4( 0, 0, 0, 0 );

		s = Transpose( s );

		s.m_m4 = Vector4( ex, ey, ez, Scalar( 1.0f ) );

		return s;
	}
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL LookAtRH(  zpVector4fParamF eye, zpVector4fParamF direction, zpVector4fParamF up )
	{
		zpVector4f x, y, z;
		zpScalar ex, ey, ez;
		zpMatrix4f s;

		z = Vector4Neg( direction );
		z = Vector4Normalize3( z );
		x = Vector4Cross3( up, z );
		x = Vector4Normalize3( x );
		y = Vector4Cross3( z, x );

		ex = Vector4Dot3( x, eye );
		ey = Vector4Dot3( y, eye );
		ez = Vector4Dot3( z, eye );

		s.m_m1 = x;
		s.m_m2 = y;
		s.m_m3 = z;
		s.m_m4 = Vector4( 0, 0, 0, 0 );

		s = Transpose( s );

		s.m_m4 = Vector4( ex, ey, ez, Scalar( 1.0f ) );

		return s;
	}

	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveLH( zpScalarParamF fovy, zpScalarParamF aspectRatio, zpScalarParamF zNear, zpScalarParamG zFar )
	{
		zpScalar yScale, xScale, z, fn, r22, r32, o;
		z = Scalar( 0.f );
		o = Scalar( 1.f );

		yScale = ScalarDegToRad( fovy );
		yScale = ScalarMul( yScale, Scalar( 0.5f ) );
		yScale = ScalarTan( yScale );
		yScale = ScalarRcp( yScale );

		xScale = ScalarDiv( yScale, aspectRatio );

		fn = ScalarSub( zFar, zNear );

		r22 = ScalarDiv( zFar, fn );

		r32 = ScalarMul( zNear, r22 );
		r32 = ScalarNeg( r32 );

		zpMatrix4f s;
		s.m_m1 = Vector4( xScale, z,      z,   z );
		s.m_m2 = Vector4( z,      yScale, z,   z );
		s.m_m3 = Vector4( z,      z,      r22, o );
		s.m_m4 = Vector4( z,      z,      r32, z );

		return s;
	}
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveRH( zpScalarParamF fovy, zpScalarParamF aspectRatio, zpScalarParamF zNear, zpScalarParamG zFar )
	{
		zpScalar yScale, xScale, z, nf, r22, r32, no;
		z = Scalar( 0.f );
		no = Scalar( -1.f );

		yScale = ScalarDegToRad( fovy );
		yScale = ScalarMul( yScale, Scalar( 0.5f ) );
		yScale = ScalarTan( yScale );
		yScale = ScalarRcp( yScale );

		xScale = ScalarDiv( yScale, aspectRatio );

		nf = ScalarSub( zNear, zFar );

		r22 = ScalarDiv( zFar, nf );

		r32 = ScalarMul( zNear, r22 );

		zpMatrix4f s;
		s.m_m1 = Vector4( xScale, z,      z,   z  );
		s.m_m2 = Vector4( z,      yScale, z,   z  );
		s.m_m3 = Vector4( z,      z,      r22, no );
		s.m_m4 = Vector4( z,      z,      r32, z  );

		return s;
	}
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoLH( zpScalarParamF l, zpScalarParamF r, zpScalarParamF t, zpScalarParamG b, zpScalarParamH zNear, zpScalarParamH zFar )
	{
		zpScalar rml, lmr, rpl, tmb, bmt, tpb, fmn, nmf, h, z, o;
		zpScalar m00, m11, m22, m30, m31, m32;
		h = Scalar( 2.f );
		z = Scalar( 0.f );
		o = Scalar( 1.f );

		rml = ScalarSub( r, l );
		lmr = ScalarSub( l, r );
		tmb = ScalarSub( t, b );
		bmt = ScalarSub( b, t );
		fmn = ScalarSub( zFar, zNear );
		nmf = ScalarSub( zNear, zFar );

		rpl = ScalarAdd( r, l );
		tpb = ScalarAdd( t, b );

		// scale
		m00 = ScalarDiv( h, rml );
		m11 = ScalarDiv( h, tmb );
		m22 = ScalarRcp( fmn );

		// translate
		m30 = ScalarDiv( rpl, lmr );
		m31 = ScalarDiv( tpb, bmt );
		m32 = ScalarDiv( zNear, nmf );

		zpMatrix4f s;
		s.m_m1 = Vector4( m00, z,   z,   z );
		s.m_m2 = Vector4( z,   m11, z,   z );
		s.m_m3 = Vector4( z,   z,   m22, z );
		s.m_m4 = Vector4( m30, m31, m32, o );

		return s;
	}
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoRH( zpScalarParamF l, zpScalarParamF r, zpScalarParamF t, zpScalarParamG b, zpScalarParamH zNear, zpScalarParamH zFar )
	{
		zpScalar rml, lmr, rpl, tmb, bmt, tpb, nmf, h, z, o;
		zpScalar m00, m11, m22, m30, m31, m32;
		h = Scalar( 2.f );
		z = Scalar( 0.f );
		o = Scalar( 1.f );

		rml = ScalarSub( r, l );
		lmr = ScalarSub( l, r );
		tmb = ScalarSub( t, b );
		bmt = ScalarSub( b, t );
		nmf = ScalarSub( zNear, zFar );

		rpl = ScalarAdd( r, l );
		tpb = ScalarAdd( t, b );

		// scale
		m00 = ScalarDiv( h, rml );
		m11 = ScalarDiv( h, tmb );
		m22 = ScalarRcp( nmf );

		// translate
		m30 = ScalarDiv( rpl, lmr );
		m31 = ScalarDiv( tpb, bmt );
		m32 = ScalarDiv( zNear, nmf );

		zpMatrix4f s;
		s.m_m1 = Vector4( m00, z,   z,   z );
		s.m_m2 = Vector4( z,   m11, z,   z );
		s.m_m3 = Vector4( z,   z,   m22, z );
		s.m_m4 = Vector4( m30, m31, m32, o );

		return s;
	}

	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL Transpose( zpMatrix4fParamF a )
	{
		zpMatrix4f s = a;
		_MM_TRANSPOSE4_PS( s.m_m1, s.m_m2, s.m_m3, s.m_m4 );

		return s;
	}
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Determinant( zpMatrix4fParamF a )
	{
		ZP_ALIGN16 zp_float matrix[4][4];
		Vector4Store4( a.m_m1, matrix[0] );
		Vector4Store4( a.m_m2, matrix[1] );
		Vector4Store4( a.m_m3, matrix[2] );
		Vector4Store4( a.m_m4, matrix[3] );

		zp_float d = 
			matrix[0][3]*matrix[1][2]*matrix[2][1]*matrix[3][0] - matrix[0][2]*matrix[1][3]*matrix[2][1]*matrix[3][0] - matrix[0][3]*matrix[1][1]*matrix[2][2]*matrix[3][0] + matrix[0][1]*matrix[1][3]*matrix[2][2]*matrix[3][0]+
			matrix[0][2]*matrix[1][1]*matrix[2][3]*matrix[3][0] - matrix[0][1]*matrix[1][2]*matrix[2][3]*matrix[3][0] - matrix[0][3]*matrix[1][2]*matrix[2][0]*matrix[3][1] + matrix[0][2]*matrix[1][3]*matrix[2][0]*matrix[3][1]+
			matrix[0][3]*matrix[1][0]*matrix[2][2]*matrix[3][1] - matrix[0][0]*matrix[1][3]*matrix[2][2]*matrix[3][1] - matrix[0][2]*matrix[1][0]*matrix[2][3]*matrix[3][1] + matrix[0][0]*matrix[1][2]*matrix[2][3]*matrix[3][1]+
			matrix[0][3]*matrix[1][1]*matrix[2][0]*matrix[3][2] - matrix[0][1]*matrix[1][3]*matrix[2][0]*matrix[3][2] - matrix[0][3]*matrix[1][0]*matrix[2][1]*matrix[3][2] + matrix[0][0]*matrix[1][3]*matrix[2][1]*matrix[3][2]+
			matrix[0][1]*matrix[1][0]*matrix[2][3]*matrix[3][2] - matrix[0][0]*matrix[1][1]*matrix[2][3]*matrix[3][2] - matrix[0][2]*matrix[1][1]*matrix[2][0]*matrix[3][3] + matrix[0][1]*matrix[1][2]*matrix[2][0]*matrix[3][3]+
			matrix[0][2]*matrix[1][0]*matrix[2][1]*matrix[3][3] - matrix[0][0]*matrix[1][2]*matrix[2][1]*matrix[3][3] - matrix[0][1]*matrix[1][0]*matrix[2][2]*matrix[3][3] + matrix[0][0]*matrix[1][1]*matrix[2][2]*matrix[3][3];

		return Scalar( d );
	}
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL Invert( zpMatrix4fParamF a )
	{
		zpScalar d = Determinant( a );
		d = ScalarRcp( d );

		ZP_ALIGN16 zp_float matrix[4][4];
		Vector4Store4( a.m_m1, matrix[0] );
		Vector4Store4( a.m_m2, matrix[1] );
		Vector4Store4( a.m_m3, matrix[2] );
		Vector4Store4( a.m_m4, matrix[3] );

		ZP_ALIGN16 zp_float data[16];
		data[0]  = matrix[1][2]*matrix[2][3]*matrix[3][1] - matrix[1][3]*matrix[2][2]*matrix[3][1] + matrix[1][3]*matrix[2][1]*matrix[3][2] - matrix[1][1]*matrix[2][3]*matrix[3][2] - matrix[1][2]*matrix[2][1]*matrix[3][3] + matrix[1][1]*matrix[2][2]*matrix[3][3];
		data[1]  = matrix[0][3]*matrix[2][2]*matrix[3][1] - matrix[0][2]*matrix[2][3]*matrix[3][1] - matrix[0][3]*matrix[2][1]*matrix[3][2] + matrix[0][1]*matrix[2][3]*matrix[3][2] + matrix[0][2]*matrix[2][1]*matrix[3][3] - matrix[0][1]*matrix[2][2]*matrix[3][3];
		data[2]  = matrix[0][2]*matrix[1][3]*matrix[3][1] - matrix[0][3]*matrix[1][2]*matrix[3][1] + matrix[0][3]*matrix[1][1]*matrix[3][2] - matrix[0][1]*matrix[1][3]*matrix[3][2] - matrix[0][2]*matrix[1][1]*matrix[3][3] + matrix[0][1]*matrix[1][2]*matrix[3][3];
		data[3]  = matrix[0][3]*matrix[1][2]*matrix[2][1] - matrix[0][2]*matrix[1][3]*matrix[2][1] - matrix[0][3]*matrix[1][1]*matrix[2][2] + matrix[0][1]*matrix[1][3]*matrix[2][2] + matrix[0][2]*matrix[1][1]*matrix[2][3] - matrix[0][1]*matrix[1][2]*matrix[2][3];
		data[4]  = matrix[1][3]*matrix[2][2]*matrix[3][0] - matrix[1][2]*matrix[2][3]*matrix[3][0] - matrix[1][3]*matrix[2][0]*matrix[3][2] + matrix[1][0]*matrix[2][3]*matrix[3][2] + matrix[1][2]*matrix[2][0]*matrix[3][3] - matrix[1][0]*matrix[2][2]*matrix[3][3];
		data[5]  = matrix[0][2]*matrix[2][3]*matrix[3][0] - matrix[0][3]*matrix[2][2]*matrix[3][0] + matrix[0][3]*matrix[2][0]*matrix[3][2] - matrix[0][0]*matrix[2][3]*matrix[3][2] - matrix[0][2]*matrix[2][0]*matrix[3][3] + matrix[0][0]*matrix[2][2]*matrix[3][3];
		data[6]  = matrix[0][3]*matrix[1][2]*matrix[3][0] - matrix[0][2]*matrix[1][3]*matrix[3][0] - matrix[0][3]*matrix[1][0]*matrix[3][2] + matrix[0][0]*matrix[1][3]*matrix[3][2] + matrix[0][2]*matrix[1][0]*matrix[3][3] - matrix[0][0]*matrix[1][2]*matrix[3][3];
		data[7]  = matrix[0][2]*matrix[1][3]*matrix[2][0] - matrix[0][3]*matrix[1][2]*matrix[2][0] + matrix[0][3]*matrix[1][0]*matrix[2][2] - matrix[0][0]*matrix[1][3]*matrix[2][2] - matrix[0][2]*matrix[1][0]*matrix[2][3] + matrix[0][0]*matrix[1][2]*matrix[2][3];
		data[8]  = matrix[1][1]*matrix[2][3]*matrix[3][0] - matrix[1][3]*matrix[2][1]*matrix[3][0] + matrix[1][3]*matrix[2][0]*matrix[3][1] - matrix[1][0]*matrix[2][3]*matrix[3][1] - matrix[1][1]*matrix[2][0]*matrix[3][3] + matrix[1][0]*matrix[2][1]*matrix[3][3];
		data[9]  = matrix[0][3]*matrix[2][1]*matrix[3][0] - matrix[0][1]*matrix[2][3]*matrix[3][0] - matrix[0][3]*matrix[2][0]*matrix[3][1] + matrix[0][0]*matrix[2][3]*matrix[3][1] + matrix[0][1]*matrix[2][0]*matrix[3][3] - matrix[0][0]*matrix[2][1]*matrix[3][3];
		data[10] = matrix[0][1]*matrix[1][3]*matrix[3][0] - matrix[0][3]*matrix[1][1]*matrix[3][0] + matrix[0][3]*matrix[1][0]*matrix[3][1] - matrix[0][0]*matrix[1][3]*matrix[3][1] - matrix[0][1]*matrix[1][0]*matrix[3][3] + matrix[0][0]*matrix[1][1]*matrix[3][3];
		data[11] = matrix[0][3]*matrix[1][1]*matrix[2][0] - matrix[0][1]*matrix[1][3]*matrix[2][0] - matrix[0][3]*matrix[1][0]*matrix[2][1] + matrix[0][0]*matrix[1][3]*matrix[2][1] + matrix[0][1]*matrix[1][0]*matrix[2][3] - matrix[0][0]*matrix[1][1]*matrix[2][3];
		data[12] = matrix[1][2]*matrix[2][1]*matrix[3][0] - matrix[1][1]*matrix[2][2]*matrix[3][0] - matrix[1][2]*matrix[2][0]*matrix[3][1] + matrix[1][0]*matrix[2][2]*matrix[3][1] + matrix[1][1]*matrix[2][0]*matrix[3][2] - matrix[1][0]*matrix[2][1]*matrix[3][2];
		data[13] = matrix[0][1]*matrix[2][2]*matrix[3][0] - matrix[0][2]*matrix[2][1]*matrix[3][0] + matrix[0][2]*matrix[2][0]*matrix[3][1] - matrix[0][0]*matrix[2][2]*matrix[3][1] - matrix[0][1]*matrix[2][0]*matrix[3][2] + matrix[0][0]*matrix[2][1]*matrix[3][2];
		data[14] = matrix[0][2]*matrix[1][1]*matrix[3][0] - matrix[0][1]*matrix[1][2]*matrix[3][0] - matrix[0][2]*matrix[1][0]*matrix[3][1] + matrix[0][0]*matrix[1][2]*matrix[3][1] + matrix[0][1]*matrix[1][0]*matrix[3][2] - matrix[0][0]*matrix[1][1]*matrix[3][2];
		data[15] = matrix[0][1]*matrix[1][2]*matrix[2][0] - matrix[0][2]*matrix[1][1]*matrix[2][0] + matrix[0][2]*matrix[1][0]*matrix[2][1] - matrix[0][0]*matrix[1][2]*matrix[2][1] - matrix[0][1]*matrix[1][0]*matrix[2][2] + matrix[0][0]*matrix[1][1]*matrix[2][2];

		zpMatrix4f s;
		s.m_m1 = Vector4Load4( &data[0] );
		s.m_m2 = Vector4Load4( &data[4] );
		s.m_m3 = Vector4Load4( &data[8] );
		s.m_m4 = Vector4Load4( &data[12] );

		s = MatrixScale( s, d );

		return s;
	}

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Lerp( zpVector4fParamF a, zpVector4fParamF b, zpScalarParamF alpha )
	{
		return Vector4Madd( a, Vector4Sub( b, a ), alpha );
	}
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarLerp( zpScalarParamF a, zpScalarParamF b, zpScalarParamF alpha )
	{
		return Vector4Madd( a, ScalarSub( b, a ), alpha );
	}

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4RotateX( zpVector4fParamF a, zpScalarParamF rad )
	{
		zpScalar y, z, co, si, t0, t1;

		si = ScalarSin( rad );
		co = ScalarCos( rad );

		t0 = Vector4Scale( Vector4GetY( a ), co );
		t1 = Vector4Scale( Vector4GetZ( a ), si );
		y =  Vector4Sub( t0, t1 );

		t0 = Vector4Scale( Vector4GetY( a ), si );
		t1 = Vector4Scale( Vector4GetZ( a ), co );
		z =  Vector4Add( t0, t1 );

		return Vector4( Scalar( 0.f ), y, z, Scalar( 0.f ) );
	}
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4RotateY( zpVector4fParamF a, zpScalarParamF rad )
	{
		zpScalar x, z, co, si, t0, t1, nsi;

		si = ScalarSin( rad );
		co = ScalarCos( rad );
		nsi = ScalarNeg( si );

		t0 = Vector4Scale( Vector4GetX( a ), co );
		t1 = Vector4Scale( Vector4GetZ( a ), si );
		x = Vector4Add( t0, t1 );

		t0 = Vector4Scale( Vector4GetX( a ), nsi );
		t1 = Vector4Scale( Vector4GetZ( a ), co );
		z = Vector4Add( t0, t1 );

		return Vector4( x, Scalar( 0.f ), z, Scalar( 0.f ) );
	}
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4RotateZ( zpVector4fParamF a, zpScalarParamF rad )
	{
		zpScalar y, x, co, si, t0, t1;

		si = ScalarSin( rad );
		co = ScalarCos( rad );

		t0 = Vector4Scale( Vector4GetX( a ), co );
		t1 = Vector4Scale( Vector4GetY( a ), si );
		y = Vector4Sub( t0, t1 );

		t0 = Vector4Scale( Vector4GetX( a ), si );
		t1 = Vector4Scale( Vector4GetY( a ), co );
		x = Vector4Add( t0, t1 );

		return Vector4( x, y, Scalar( 0.f ), Scalar( 0.f ) );
	}

	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionFromAxisAngle( zpVector4fParamF a, zpScalarParamF b )
	{
		zpScalar d, si, co, halfB;
		halfB = ScalarMul( b, Scalar( 0.5f ) );

		d = Vector4Length4( a );
		d = ScalarRcp( d );

		si = ScalarSin( halfB );
		co = ScalarCos( halfB );

		zpVector4f t;
		zpVector4f dv = Vector4( d, d, d, Scalar( 1.f ) );
		zpVector4f av = Vector4( si, si, si, co );

		t = Vector4Mul( a, dv );
		t = Vector4Mul( t, av );

		zp_float buff[4];
		Vector4Store4( t, buff );
		
		return Quaternion( buff[0], buff[1], buff[2], buff[3] );
	}

	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionFromEulerAngle( zpScalarParamF yaw, zpScalarParamF pitch, zpScalarParamF roll )
	{
		zp_float y, p, r;
		y = AsFloat( yaw );
		p = AsFloat( pitch );
		r = AsFloat( roll );

		zp_float hr = r * 0.5f;
		zp_float shr = zp_sin(hr);
		zp_float chr = zp_cos(hr);
		zp_float hp = p * 0.5f;
		zp_float shp = zp_sin(hp);
		zp_float chp = zp_cos(hp);
		zp_float hy = y * 0.5f;
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

		return zpMath::Quaternion( sx, sy, sz, sw );
	}

	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL TRS( zpVector4fParamF p, zpQuaternion4fParamF r, zpVector4fParamF s )
	{
		zpMatrix4f pm = MatrixIdentity();
		pm.m_m4 = p;

		zpMatrix4f rm = QuaternionToMatrix( r );

		zpMatrix4f sm = MatrixIdentity();
		sm.m_m1 = Vector4Mul( sm.m_m1, s );
		sm.m_m2 = Vector4Mul( sm.m_m2, s );
		sm.m_m3 = Vector4Mul( sm.m_m3, s );

		zpMatrix4f m;
		m = MatrixMul( sm, rm );
		m = MatrixMul( m, pm );

		return m;
	}

	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixIdentity()
	{
		zpMatrix4f s;
		s.m_m1 = Vector4( 1, 0, 0, 0 );
		s.m_m2 = Vector4( 0, 1, 0, 0 );
		s.m_m3 = Vector4( 0, 0, 1, 0 );
		s.m_m4 = Vector4( 0, 0, 0, 1 );

		return s;
	}

	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixLoadOpenGL( const zp_float* matrix )
	{
		zpMatrix4f s;
		s.r[ 0 ] = Vector4Load4( matrix + 0 );
		s.r[ 1 ] = Vector4Load4( matrix + 4 );
		s.r[ 2 ] = Vector4Load4( matrix + 8 );
		s.r[ 3 ] = Vector4Load4( matrix + 12 );

		return s;
	}
	ZP_FORCE_INLINE void ZP_VECTORCALL MatrixStoreOpenGL( zpMatrix4fParamF a, zp_float* matrix )
	{
		Vector4Store4( a.r[ 0 ], matrix + 0 );
		Vector4Store4( a.r[ 1 ], matrix + 4 );
		Vector4Store4( a.r[ 2 ], matrix + 8 );
		Vector4Store4( a.r[ 3 ], matrix + 12 );
	}

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Reflect( zpVector4fParamF a, zpVector4fParamF n )
	{
		zpScalar d = ScalarMul( Vector4Dot3( a, n ), Scalar( -2.0f ) );
		return Vector4Madd( a, d, n );
	}
};
