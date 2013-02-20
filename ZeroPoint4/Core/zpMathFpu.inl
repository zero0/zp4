
ZP_FORCE_INLINE void zpMath::Add( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x + b.m_x;
}
ZP_FORCE_INLINE void zpMath::Sub( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x + b.m_x;
}
ZP_FORCE_INLINE void zpMath::Mul( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x + b.m_x;
}
ZP_FORCE_INLINE void zpMath::Div( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x + b.m_x;
}

ZP_FORCE_INLINE void zpMath::Add( zpVector4f& s, const zpVector4f& a, const zpScalar& b )
{
	s.m_xyzw.x = a.m_xyzw.x + b.m_x;
	s.m_xyzw.y = a.m_xyzw.y + b.m_x;
	s.m_xyzw.z = a.m_xyzw.z + b.m_x;
	s.m_xyzw.w = a.m_xyzw.w + b.m_x;
}
ZP_FORCE_INLINE void zpMath::Sub( zpVector4f& s, const zpVector4f& a, const zpScalar& b )
{
	s.m_xyzw.x = a.m_xyzw.x - b.m_x;
	s.m_xyzw.y = a.m_xyzw.y - b.m_x;
	s.m_xyzw.z = a.m_xyzw.z - b.m_x;
	s.m_xyzw.w = a.m_xyzw.w - b.m_x;
}
ZP_FORCE_INLINE void zpMath::Mul( zpVector4f& s, const zpVector4f& a, const zpScalar& b )
{
	s.m_xyzw.x = a.m_xyzw.x * b.m_x;
	s.m_xyzw.y = a.m_xyzw.y * b.m_x;
	s.m_xyzw.z = a.m_xyzw.z * b.m_x;
	s.m_xyzw.w = a.m_xyzw.w * b.m_x;
}
ZP_FORCE_INLINE void zpMath::Div( zpVector4f& s, const zpVector4f& a, const zpScalar& b )
{
	s.m_xyzw.x = a.m_xyzw.x / b.m_x;
	s.m_xyzw.y = a.m_xyzw.y / b.m_x;
	s.m_xyzw.z = a.m_xyzw.z / b.m_x;
	s.m_xyzw.w = a.m_xyzw.w / b.m_x;
}
																						 
ZP_FORCE_INLINE void zpMath::Add( zpVector4f& s, const zpScalar& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_x + b.m_xyzw.x;
	s.m_xyzw.y = a.m_x + b.m_xyzw.y;
	s.m_xyzw.z = a.m_x + b.m_xyzw.z;
	s.m_xyzw.w = a.m_x + b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Sub( zpVector4f& s, const zpScalar& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_x - b.m_xyzw.x;
	s.m_xyzw.y = a.m_x - b.m_xyzw.y;
	s.m_xyzw.z = a.m_x - b.m_xyzw.z;
	s.m_xyzw.w = a.m_x - b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Mul( zpVector4f& s, const zpScalar& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_x * b.m_xyzw.x;
	s.m_xyzw.y = a.m_x * b.m_xyzw.y;
	s.m_xyzw.z = a.m_x * b.m_xyzw.z;
	s.m_xyzw.w = a.m_x * b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Div( zpVector4f& s, const zpScalar& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_x / b.m_xyzw.x;
	s.m_xyzw.y = a.m_x / b.m_xyzw.y;
	s.m_xyzw.z = a.m_x / b.m_xyzw.z;
	s.m_xyzw.w = a.m_x / b.m_xyzw.w;
}

ZP_FORCE_INLINE void zpMath::Add( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x + b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y + b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z + b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w + b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Sub( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x - b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y - b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z - b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w - b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Mul( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x * b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y * b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z * b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w * b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Div( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x / b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y / b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z / b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w / b.m_xyzw.w;
}

ZP_FORCE_INLINE void zpMath::Madd( zpVector4f& s, const zpVector4f& a, const zpVector4f& b, const zpScalar& c )
{
	s.m_xyzw.x = a.m_xyzw.x + ( b.m_xyzw.x * c.m_x );
	s.m_xyzw.y = a.m_xyzw.y + ( b.m_xyzw.y * c.m_x );
	s.m_xyzw.z = a.m_xyzw.z + ( b.m_xyzw.z * c.m_x );
	s.m_xyzw.w = a.m_xyzw.w + ( b.m_xyzw.w * c.m_x );
}
ZP_FORCE_INLINE void zpMath::Madd( zpVector4f& s, const zpVector4f& a, const zpScalar& b, const zpVector4f& c )
{
	s.m_xyzw.x = a.m_xyzw.x + ( b.m_x * c.m_xyzw.x );
	s.m_xyzw.y = a.m_xyzw.y + ( b.m_x * c.m_xyzw.y );
	s.m_xyzw.z = a.m_xyzw.z + ( b.m_x * c.m_xyzw.z );
	s.m_xyzw.w = a.m_xyzw.w + ( b.m_x * c.m_xyzw.w );
}


ZP_FORCE_INLINE void zpMath::Dot2( zpScalar& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_x = a.m_xyzw.x * b.m_xyzw.x + a.m_xyzw.y * b.m_xyzw.y;
}
ZP_FORCE_INLINE void zpMath::Dot3( zpScalar& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_x = a.m_xyzw.x * b.m_xyzw.x + a.m_xyzw.y * b.m_xyzw.y + a.m_xyzw.z * b.m_xyzw.z;
}
ZP_FORCE_INLINE void zpMath::Dot4( zpScalar& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_x = a.m_xyzw.x * b.m_xyzw.x + a.m_xyzw.y * b.m_xyzw.y + a.m_xyzw.z * b.m_xyzw.z + a.m_xyzw.w * b.m_xyzw.w;
}

ZP_FORCE_INLINE void zpMath::Cross3( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_xyzw.y * b.m_xyzw.z - a.m_xyzw.z * b.m_xyzw.y;
	s.m_xyzw.y = a.m_xyzw.z * b.m_xyzw.x - a.m_xyzw.x * b.m_xyzw.z;
	s.m_xyzw.z = a.m_xyzw.x * b.m_xyzw.y - a.m_xyzw.y * b.m_xyzw.x;
	s.m_xyzw.w = 0.0f;
}

ZP_FORCE_INLINE void zpMath::LengthSquared2( zpScalar& s, const zpVector4f& a )
{
	s.m_x = a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y;
}
ZP_FORCE_INLINE void zpMath::LengthSquared3( zpScalar& s, const zpVector4f& a )
{
	s.m_x = a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y + a.m_xyzw.z * a.m_xyzw.z;
}

ZP_FORCE_INLINE void zpMath::Length2( zpScalar& s, const zpVector4f& a )
{
	s.m_x = zp_sqrt( a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y );
}
ZP_FORCE_INLINE void zpMath::Length3( zpScalar& s, const zpVector4f& a )
{
	s.m_x = zp_sqrt( a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y + a.m_xyzw.z * a.m_xyzw.z );
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

	s.m_m1 = col1;
	s.m_m2 = col2;
	s.m_m3 = col3;
	s.m_m4 = col4;
}

ZP_FORCE_INLINE void zpMath::Abs( zpScalar& s, const zpScalar& a )
{
	s.m_x = zp_abs( a.m_x );
}
ZP_FORCE_INLINE void zpMath::Abs( zpVector4f& s, const zpVector4f& a )
{
	s.m_xyzw.x = zp_abs( a.m_xyzw.x );
	s.m_xyzw.y = zp_abs( a.m_xyzw.y );
	s.m_xyzw.z = zp_abs( a.m_xyzw.z );
	s.m_xyzw.w = zp_abs( a.m_xyzw.w );
}

ZP_FORCE_INLINE void zpMath::Neg( zpScalar& s, const zpScalar& a )
{
	s.m_x = -a.m_x;
}
ZP_FORCE_INLINE void zpMath::Neg( zpVector4f& s, const zpVector4f& a )
{
	s.m_xyzw.x = -a.m_xyzw.x;
	s.m_xyzw.y = -a.m_xyzw.y;
	s.m_xyzw.z = -a.m_xyzw.z;
	s.m_xyzw.w = -a.m_xyzw.w;
}

ZP_FORCE_INLINE void zpMath::Rcp( zpScalar& s, const zpScalar& a )
{
	s.m_x = 1.0f / a.m_x;
}
ZP_FORCE_INLINE void zpMath::Rcp( zpVector4f& s, const zpVector4f& a )
{
	s.m_xyzw.x = 1.0f / a.m_xyzw.x;
	s.m_xyzw.y = 1.0f / a.m_xyzw.y;
	s.m_xyzw.z = 1.0f / a.m_xyzw.z;
	s.m_xyzw.w = 1.0f / a.m_xyzw.w;
}

ZP_FORCE_INLINE void zpMath::Max( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = ZP_MAX( a.m_x, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Max( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw.x = ZP_MAX( a.m_xyzw.x, b.m_xyzw.x );
	s.m_xyzw.y = ZP_MAX( a.m_xyzw.y, b.m_xyzw.y );
	s.m_xyzw.z = ZP_MAX( a.m_xyzw.z, b.m_xyzw.z );
	s.m_xyzw.w = ZP_MAX( a.m_xyzw.w, b.m_xyzw.w );
}

ZP_FORCE_INLINE void zpMath::Min( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = ZP_MIN( a.m_x, b.m_x );
}
ZP_FORCE_INLINE void zpMath::Min( zpVector4f& s, const zpVector4f& a, const zpVector4f& b )
{
	s.m_xyzw.x = ZP_MIN( a.m_xyzw.x, b.m_xyzw.x );
	s.m_xyzw.y = ZP_MIN( a.m_xyzw.y, b.m_xyzw.y );
	s.m_xyzw.z = ZP_MIN( a.m_xyzw.z, b.m_xyzw.z );
	s.m_xyzw.w = ZP_MIN( a.m_xyzw.w, b.m_xyzw.w );
}

ZP_FORCE_INLINE void zpMath::Cmp( zp_int& s, const zpScalar& a, const zpScalar& b )
{
	s = a.m_x > b.m_x ? 1 : a.m_x < b.m_x ? -1 : 0;
}
ZP_FORCE_INLINE void zpMath::Cmp( zp_int& s, const zpVector4f& a, const zpVector4f& b )
{

}
