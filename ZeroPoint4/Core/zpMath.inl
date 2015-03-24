
namespace zpMath
{
	ZP_FORCE_INLINE zp_int Cmp0( zpScalar a )
	{
		return Cmp( a, Scalar( 0.f ) );
	}

	ZP_FORCE_INLINE zpScalar DegToRad( zpScalar a )
	{
		return Mul( a, Scalar( ZP_PIOVER180 ) );
	}
	ZP_FORCE_INLINE zpScalar RadToDeg( zpScalar a )
	{
		return Mul( a, Scalar( ZP_180OVERPI ) );
	}

	ZP_FORCE_INLINE zpVector4f Perpendicular3( zpVector4f a )
	{
		zpVector4f c = Cross3( a, Vector4( 1, 0, 0, 0 ) );
		zpScalar l = LengthSquared3( c );

		if( Cmp( l, Scalar( ZP_EPSILON ) ) < 0 )
		{
			c = Cross3( a, Vector4( 0, 1, 0, 0 ) );
		}

		return c;
	}

	ZP_FORCE_INLINE zpVector4f Madd( zpVector4f a, zpVector4f b, zpScalar c )
	{
		return Add( a, Mul( b, c ) );
	}
	ZP_FORCE_INLINE zpVector4f Madd( zpVector4f a, zpScalar b, zpVector4f c )
	{
		return Add( a, Mul( b, c ) );
	}

	ZP_FORCE_INLINE zpScalar LengthSquared2( zpVector4f a )
	{
		return Dot2( a, a );
	}
	ZP_FORCE_INLINE zpScalar LengthSquared3( zpVector4f a )
	{
		return Dot3( a, a );
	}
	ZP_FORCE_INLINE zpScalar LengthSquared4( zpVector4f a )
	{
		return Dot4( a, a );
	}

	ZP_FORCE_INLINE zpScalar Length2( zpVector4f a )
	{
		return Sqrt( Dot2( a, a ) );
	}
	ZP_FORCE_INLINE zpScalar Length3( zpVector4f a )
	{
		return Sqrt( Dot3( a, a ) );
	}
	ZP_FORCE_INLINE zpScalar Length4( zpVector4f a )
	{
		return Sqrt( Dot4( a, a ) );
	}
	ZP_FORCE_INLINE zpScalar Length4( zpQuaternion4f a )
	{
		return Sqrt( Dot4( a, a ) );
	}

	ZP_FORCE_INLINE zpVector4f Normalize2( zpVector4f a )
	{
		zpScalar l = Length2( a );
		return Div( a, l );
	}
	ZP_FORCE_INLINE zpVector4f Normalize3( zpVector4f a )
	{
		zpScalar l = Length3( a );
		return Div( a, l );
	}
	ZP_FORCE_INLINE zpVector4f Normalize4( zpVector4f a )
	{
		zpScalar l = Length4( a );
		return Div( a, l );
	}
	ZP_FORCE_INLINE zpQuaternion4f Normalize4( zpQuaternion4f a )
	{
		zpScalar l = Length4( a );
		return Div( a, l );
	}

	ZP_FORCE_INLINE zpVector4f Mul( zpVector4f a, zpMatrix4f b )
	{
		zpVector4f col = Mul( GetX( a ), b.m_m1 );
		col = Madd( col, GetY( a ), b.m_m2 );
		col = Madd( col, GetZ( a ), b.m_m3 );
		return Madd( col, GetW( a ), b.m_m4 );
	}
	ZP_FORCE_INLINE zpMatrix4f Mul( zpMatrix4f a, zpMatrix4f b )
	{
		zpMatrix4f s;

		zpVector4f col1 = Mul( GetX( a.m_m1 ), b.m_m1 );
		zpVector4f col2 = Mul( GetX( a.m_m2 ), b.m_m1 );
		zpVector4f col3 = Mul( GetX( a.m_m3 ), b.m_m1 );
		zpVector4f col4 = Mul( GetX( a.m_m4 ), b.m_m1 );

		col1 = Madd( col1, GetY( a.m_m1 ), b.m_m2 );
		col2 = Madd( col2, GetY( a.m_m2 ), b.m_m2 );
		col3 = Madd( col3, GetY( a.m_m3 ), b.m_m2 );
		col4 = Madd( col4, GetY( a.m_m4 ), b.m_m2 );

		col1 = Madd( col1, GetZ( a.m_m1 ), b.m_m3 );
		col2 = Madd( col2, GetZ( a.m_m2 ), b.m_m3 );
		col3 = Madd( col3, GetZ( a.m_m3 ), b.m_m3 );
		col4 = Madd( col4, GetZ( a.m_m4 ), b.m_m3 );

		s.m_m1 = Madd( col1, GetW( a.m_m1 ), b.m_m4 );
		s.m_m2 = Madd( col2, GetW( a.m_m2 ), b.m_m4 );
		s.m_m3 = Madd( col3, GetW( a.m_m3 ), b.m_m4 );
		s.m_m4 = Madd( col4, GetW( a.m_m4 ), b.m_m4 );

		return s;

	}
	ZP_FORCE_INLINE zpMatrix4f Mul( zpScalar a, zpMatrix4f b )
	{
		zpMatrix4f s;
		s.m_m1 = Mul( a, b.m_m1 );
		s.m_m2 = Mul( a, b.m_m2 );
		s.m_m3 = Mul( a, b.m_m3 );
		s.m_m4 = Mul( a, b.m_m1 );
		return s;
	}


	ZP_FORCE_INLINE zpMatrix4f LookAtLH( zpVector4f eye, zpVector4f direction, zpVector4f up )
	{
		zpVector4f x, y, z;
		zpScalar ex, ey, ez;
		zpMatrix4f s;

		z = Normalize3( direction );
		x = Cross3( up, z );
		x = Normalize3( x );
		y = Cross3( z, x );

		ex = Dot3( x, eye );
		ey = Dot3( y, eye );
		ez = Dot3( z, eye );

		ex = Neg( ex );
		ey = Neg( ey );
		ez = Neg( ez );

		s.m_m1 = x;
		s.m_m2 = y;
		s.m_m3 = z;
		s.m_m4 = Vector4( 0, 0, 0, 0 );

		s = Transpose( s );

		s.m_m4 = Vector4( ex, ey, ez, Scalar( 1.0f ) );

		return s;
	}
	ZP_FORCE_INLINE zpMatrix4f LookAtRH(  zpVector4f eye, zpVector4f direction, zpVector4f up )
	{
		zpVector4f x, y, z;
		zpScalar ex, ey, ez;
		zpMatrix4f s;

		z = Neg( direction );
		z = Normalize3( z );
		x = Cross3( up, z );
		x = Normalize3( x );
		y = Cross3( z, x );

		ex = Dot3( x, eye );
		ey = Dot3( y, eye );
		ez = Dot3( z, eye );

		s.m_m1 = x;
		s.m_m2 = y;
		s.m_m3 = z;
		s.m_m4 = Vector4( 0, 0, 0, 0 );

		s = Transpose( s );

		s.m_m4 = Vector4( ex, ey, ez, Scalar( 1.0f ) );

		return s;
	}

	ZP_FORCE_INLINE zpMatrix4f PerspectiveLH( zpScalar fovy, zpScalar aspectRatio, zpScalar zNear, zpScalar zFar )
	{
		zpScalar yScale, xScale, z, fn, r22, r32, o;
		z = Scalar( 0.f );
		o = Scalar( 1.f );

		yScale = DegToRad( fovy );
		yScale = Mul( yScale, Scalar( 0.5f ) );
		yScale = Tan( yScale );
		yScale = Rcp( yScale );

		xScale = Div( yScale, aspectRatio );

		fn = Sub( zFar, zNear );

		r22 = Div( zFar, fn );

		r32 = Mul( zNear, r22 );
		r32 = Neg( r32 );

		zpMatrix4f s;
		s.m_m1 = Vector4( xScale, z,      z,   z ) );
		s.m_m2 = Vector4( z,      yScale, z,   z ) );
		s.m_m3 = Vector4( z,      z,      r22, o ) );
		s.m_m4 = Vector4( z,      z,      r32, z ) );

		return s;
	}
	ZP_FORCE_INLINE zpMatrix4f PerspectiveRH( zpScalar fovy, zpScalar aspectRatio, zpScalar zNear, zpScalar zFar )
	{
		zpScalar yScale, xScale, z, nf, r22, r32, no;
		z = Scalar( 0.f );
		no = Scalar( -1.f );

		yScale = DegToRad( fovy );
		yScale = Mul( yScale, Scalar( 0.5f ) );
		yScale = Tan( yScale );
		yScale = Rcp( yScale );

		xScale = Div( yScale, aspectRatio );

		nf = Sub( zNear, zFar );

		r22 = Div( zFar, nf );

		r32 = Mul( zNear, r22 );

		zpMatrix4f s;
		s.m_m1 = Vector4( xScale, z,      z,   z  ) );
		s.m_m2 = Vector4( z,      yScale, z,   z  ) );
		s.m_m3 = Vector4( z,      z,      r22, no ) );
		s.m_m4 = Vector4( z,      z,      r32, z  ) );

		return s;
	}
	ZP_FORCE_INLINE zpMatrix4f OrthoLH( zpScalar l, zpScalar r, zpScalar t, zpScalar b, zpScalar zNear, zpScalar zFar )
	{
		zpScalar rml, lmr, rpl, tmb, bmt, tpb, fmn, nmf, h, z, o;
		zpScalar m00, m11, m22, m30, m31, m32;
		h = Scalar( 2.f );
		z = Scalar( 0.f );
		o = Scalar( 1.f );

		rml = Sub( r, l );
		lmr = Sub( l, r );
		tmb = Sub( t, b );
		bmt = Sub( b, t );
		fmn = Sub( zFar, zNear );
		nmf = Sub( zNear, zFar );

		rpl = Add( r, l );
		tpb = Add( t, b );

		// scale
		m00 = Div( h, rml );
		m11 = Div( h, tmb );
		m22 = Rcp( fmn );

		// translate
		m30 = Div( rpl, lmr );
		m31 = Div( tpb, bmt );
		m32 = Div( zNear, nmf );

		zpMatrix4f s;
		s.m_m1 = Vector4( m00, z,   z,   z );
		s.m_m2 = Vector4( z,   m11, z,   z );
		s.m_m3 = Vector4( z,   z,   m22, z );
		s.m_m4 = Vector4( m30, m31, m32, o );

		return s;
	}
	ZP_FORCE_INLINE zpMatrix4f OrthoRH( zpScalar l, zpScalar r, zpScalar t, zpScalar b, zpScalar zNear, zpScalar zFar )
	{
		zpScalar rml, lmr, rpl, tmb, bmt, tpb, nmf, h, z, o;
		zpScalar m00, m11, m22, m30, m31, m32;
		h = Scalar( 2.f );
		z = Scalar( 0.f );
		o = Scalar( 1.f );

		rml = Sub( r, l );
		lmr = Sub( l, r );
		tmb = Sub( t, b );
		bmt = Sub( b, t );
		nmf = Sub( zNear, zFar );

		rpl = Add( r, l );
		tpb = Add( t, b );

		// scale
		m00 = Div( h, rml );
		m11 = Div( h, tmb );
		m22 = Rcp( nmf );

		// translate
		m30 = Div( rpl, lmr );
		m31 = Div( tpb, bmt );
		m32 = Div( zNear, nmf );

		zpMatrix4f s;
		s.m_m1 = Vector4( m00, z,   z,   z );
		s.m_m2 = Vector4( z,   m11, z,   z );
		s.m_m3 = Vector4( z,   z,   m22, z );
		s.m_m4 = Vector4( m30, m31, m32, o );

		return s;
	}

	ZP_FORCE_INLINE zpMatrix4f Transpose( zpMatrix4f a )
	{
		zp_float matrix[4][4];
		Store4( a.m_m1, matrix[0] );
		Store4( a.m_m2, matrix[1] );
		Store4( a.m_m3, matrix[2] );
		Store4( a.m_m4, matrix[3] );

		matrix[0][0] = matrix[0][0];
		matrix[0][1] = matrix[1][0];
		matrix[0][2] = matrix[2][0];
		matrix[0][3] = matrix[3][0];

		matrix[1][0] = matrix[0][1];
		matrix[1][1] = matrix[1][1];
		matrix[1][2] = matrix[2][1];
		matrix[1][3] = matrix[3][1];

		matrix[2][0] = matrix[0][2];
		matrix[2][1] = matrix[1][2];
		matrix[2][2] = matrix[2][2];
		matrix[2][3] = matrix[3][2];

		matrix[3][0] = matrix[0][3];
		matrix[3][1] = matrix[1][3];
		matrix[3][2] = matrix[2][3];
		matrix[3][3] = matrix[3][3];

		zpMatrix4f s;
		s.m_m1 = Load4( matrix[0] );
		s.m_m2 = Load4( matrix[1] );
		s.m_m3 = Load4( matrix[2] );
		s.m_m4 = Load4( matrix[3] );

		return s;
	}
	ZP_FORCE_INLINE zpScalar Determinant( zpMatrix4f a )
	{
		zp_float matrix[4][4];
		Store4( a.m_m1, matrix[0] );
		Store4( a.m_m2, matrix[1] );
		Store4( a.m_m3, matrix[2] );
		Store4( a.m_m4, matrix[3] );

		zp_float d = 
			matrix[0][3]*matrix[1][2]*matrix[2][1]*matrix[3][0] - matrix[0][2]*matrix[1][3]*matrix[2][1]*matrix[3][0] - matrix[0][3]*matrix[1][1]*matrix[2][2]*matrix[3][0] + matrix[0][1]*matrix[1][3]*matrix[2][2]*matrix[3][0]+
			matrix[0][2]*matrix[1][1]*matrix[2][3]*matrix[3][0] - matrix[0][1]*matrix[1][2]*matrix[2][3]*matrix[3][0] - matrix[0][3]*matrix[1][2]*matrix[2][0]*matrix[3][1] + matrix[0][2]*matrix[1][3]*matrix[2][0]*matrix[3][1]+
			matrix[0][3]*matrix[1][0]*matrix[2][2]*matrix[3][1] - matrix[0][0]*matrix[1][3]*matrix[2][2]*matrix[3][1] - matrix[0][2]*matrix[1][0]*matrix[2][3]*matrix[3][1] + matrix[0][0]*matrix[1][2]*matrix[2][3]*matrix[3][1]+
			matrix[0][3]*matrix[1][1]*matrix[2][0]*matrix[3][2] - matrix[0][1]*matrix[1][3]*matrix[2][0]*matrix[3][2] - matrix[0][3]*matrix[1][0]*matrix[2][1]*matrix[3][2] + matrix[0][0]*matrix[1][3]*matrix[2][1]*matrix[3][2]+
			matrix[0][1]*matrix[1][0]*matrix[2][3]*matrix[3][2] - matrix[0][0]*matrix[1][1]*matrix[2][3]*matrix[3][2] - matrix[0][2]*matrix[1][1]*matrix[2][0]*matrix[3][3] + matrix[0][1]*matrix[1][2]*matrix[2][0]*matrix[3][3]+
			matrix[0][2]*matrix[1][0]*matrix[2][1]*matrix[3][3] - matrix[0][0]*matrix[1][2]*matrix[2][1]*matrix[3][3] - matrix[0][1]*matrix[1][0]*matrix[2][2]*matrix[3][3] + matrix[0][0]*matrix[1][1]*matrix[2][2]*matrix[3][3];

		return Scalar( d );
	}
	ZP_FORCE_INLINE zpMatrix4f Invert( zpMatrix4f a )
	{
		zpScalar d = Determinant( a );
		d = Rcp( d );

		zp_float matrix[4][4];
		Store4( a.m_m1, matrix[0] );
		Store4( a.m_m2, matrix[1] );
		Store4( a.m_m3, matrix[2] );
		Store4( a.m_m4, matrix[3] );

		zp_float data[16];
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
		s.m_m1 = Load4( &data[0] );
		s.m_m2 = Load4( &data[4] );
		s.m_m3 = Load4( &data[8] );
		s.m_m4 = Load4( &data[12] );

		s = Mul( d, s );

		return s;
	}

	ZP_FORCE_INLINE zpVector4f Lerp( zpVector4f a, zpVector4f b, zpScalar alpha )
	{
		return Madd( a, Sub( b, a ), alpha );
	}
	ZP_FORCE_INLINE zpScalar Lerp( zpScalar a, zpScalar b, zpScalar alpha )
	{
		return Madd( a, Sub( b, a ), alpha );
	}

	ZP_FORCE_INLINE zpVector4f RotateX( zpVector4f a, zpScalar rad )
	{
		zpScalar angle, y, z, co, si, t0, t1;

		si = Sin( rad );
		co = Cos( rad );

		t0 = Mul( GetY( a ), co );
		t1 = Mul( GetZ( a ), si );
		y = Sub( t0, t1 );

		t0 = Mul( GetY( a ), si );
		t1 = Mul( GetZ( a ), co );
		z = Add( t0, t1 );

		return Vector4( Scalar( 0.f ), y, z, Scalar( 0.f ) );
	}
	ZP_FORCE_INLINE zpVector4f RotateY( zpVector4f a, zpScalar rad )
	{
		zpScalar angle, x, z, co, si, t0, t1, nsi;

		si = Sin( rad );
		co = Cos( rad );
		nsi = Neg( si );

		t0 = Mul( GetX( a ), co );
		t1 = Mul( GetZ( a ), si );
		x = Add( t0, t1 );

		t0 = Mul( GetX( a ), nsi );
		t1 = Mul( GetZ( a ), co );
		z = Add( t0, t1 );

		return Vector4( x, Scalar( 0.f ), z, Scalar( 0.f ) );
	}
	ZP_FORCE_INLINE zpVector4f RotateZ( zpVector4f a, zpScalar rad )
	{
		zpScalar angle, y, x, co, si, t0, t1;

		si = Sin( rad );
		co = Cos( rad );

		t0 = Mul( GetX( a ), co );
		t1 = Mul( GetY( a ), si );
		y = Sub( t0, t1 );

		t0 = Mul( GetX( a ), si );
		t1 = Mul( GetY( a ), co );
		x = Add( t0, t1 );

		return Vector4( x, y, Scalar( 0.f ), Scalar( 0.f ) );
	}

	ZP_FORCE_INLINE zpQuaternion4f FromAxisAngle( zpVector4f a, zpScalar b )
	{
		zpScalar d, si, co, halfB;
		halfB = Mul( b, Scalar( 0.5f ) );

		d = Length4( a );
		d = Rcp( d );

		si = Sin( halfB );
		co = Cos( halfB );

		zpVector4f t;
		zpVector4f dv = Vector4( d, d, d, Scalar( 1.f ) );
		zpVector4f av = Vector4( si, si, si, co );

		t = Mul( a, dv );
		t = Mul( t, av );

		zp_float buff[4];
		Store4( t, buff );
		
		return Quaternion4( buff[0], buff[1], buff[2], buff[3] );
	}

	ZP_FORCE_INLINE zpMatrix4f TRS( zpVector4f p, zpQuaternion4f r, zpVector4f s )
	{
		zpMatrix4f pm = MatrixIdentity();
		pm.m_m4 = p;

		zpMatrix4f rm = ToMatrix( r );

		zpMatrix4f sm = MatrixIdentity();
		sm.m_m1 = Mul( sm.m_m1, s );
		sm.m_m2 = Mul( sm.m_m2, s );
		sm.m_m3 = Mul( sm.m_m3, s );

		zpMatrix4f m;
		m = Mul( sm, rm );
		m = Mul( m, pm );

		return m;
	}

	ZP_FORCE_INLINE zpMatrix4f MatrixIdentity()
	{
		zpMatrix4f s;
		s.m_m1 = Vector4( 1, 0, 0, 0 );
		s.m_m2 = Vector4( 0, 1, 0, 0 );
		s.m_m3 = Vector4( 0, 0, 1, 0 );
		s.m_m4 = Vector4( 0, 0, 0, 1 );

		return s;
	}

	ZP_FORCE_INLINE zpVector4f Reflect( zpVector4f a, zpVector4f n )
	{
		zpScalar d = Mul( Dot3( a, n ), Scalar( -2.0f ) );
		return Madd( a, d, n );
	}
};
