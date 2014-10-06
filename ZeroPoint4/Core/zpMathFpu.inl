
ZP_FORCE_INLINE void zpMath::DegToRad( zpScalar& s, const zpScalar& a )
{
	s.m_x = ZP_DEG_TO_RAD( a.m_x );
}
ZP_FORCE_INLINE void zpMath::RadToDeg( zpScalar& s, const zpScalar& a )
{
	s.m_x = ZP_RAD_TO_DEG( a.m_x );
}

ZP_FORCE_INLINE void zpMath::Sin( zpScalar& s, const zpScalar& a )
{
	s.m_x = zp_sin( a.m_x );
}
ZP_FORCE_INLINE void zpMath::Cos( zpScalar& s, const zpScalar& a )
{
	s.m_x = zp_cos( a.m_x );
}
ZP_FORCE_INLINE void zpMath::Tan( zpScalar& s, const zpScalar& a )
{
	s.m_x = zp_tan( a.m_x );
}

ZP_FORCE_INLINE void zpMath::Add( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x + b.m_x;
}
ZP_FORCE_INLINE void zpMath::Sub( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x - b.m_x;
}
ZP_FORCE_INLINE void zpMath::Mul( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x * b.m_x;
}
ZP_FORCE_INLINE void zpMath::Div( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x / b.m_x;
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

ZP_FORCE_INLINE void zpMath::Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b )
{
	s.m_xyzw.x = a.m_xyzw.x + b.m_x;
	s.m_xyzw.y = a.m_xyzw.y + b.m_x;
	s.m_xyzw.z = a.m_xyzw.z + b.m_x;
	s.m_xyzw.w = a.m_xyzw.w + b.m_x;
}
ZP_FORCE_INLINE void zpMath::Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b )
{
	s.m_xyzw.x = a.m_xyzw.x - b.m_x;
	s.m_xyzw.y = a.m_xyzw.y - b.m_x;
	s.m_xyzw.z = a.m_xyzw.z - b.m_x;
	s.m_xyzw.w = a.m_xyzw.w - b.m_x;
}
ZP_FORCE_INLINE void zpMath::Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b )
{
	s.m_xyzw.x = a.m_xyzw.x * b.m_x;
	s.m_xyzw.y = a.m_xyzw.y * b.m_x;
	s.m_xyzw.z = a.m_xyzw.z * b.m_x;
	s.m_xyzw.w = a.m_xyzw.w * b.m_x;
}
ZP_FORCE_INLINE void zpMath::Div( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b )
{
	s.m_xyzw.x = a.m_xyzw.x / b.m_x;
	s.m_xyzw.y = a.m_xyzw.y / b.m_x;
	s.m_xyzw.z = a.m_xyzw.z / b.m_x;
	s.m_xyzw.w = a.m_xyzw.w / b.m_x;
}

ZP_FORCE_INLINE void zpMath::Add( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b )
{
	s.m_xyzw.x = a.m_x + b.m_xyzw.x;
	s.m_xyzw.y = a.m_x + b.m_xyzw.y;
	s.m_xyzw.z = a.m_x + b.m_xyzw.z;
	s.m_xyzw.w = a.m_x + b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Sub( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b )
{
	s.m_xyzw.x = a.m_x - b.m_xyzw.x;
	s.m_xyzw.y = a.m_x - b.m_xyzw.y;
	s.m_xyzw.z = a.m_x - b.m_xyzw.z;
	s.m_xyzw.w = a.m_x - b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Mul( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b )
{
	s.m_xyzw.x = a.m_x * b.m_xyzw.x;
	s.m_xyzw.y = a.m_x * b.m_xyzw.y;
	s.m_xyzw.z = a.m_x * b.m_xyzw.z;
	s.m_xyzw.w = a.m_x * b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Div( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b )
{
	s.m_xyzw.x = a.m_x / b.m_xyzw.x;
	s.m_xyzw.y = a.m_x / b.m_xyzw.y;
	s.m_xyzw.z = a.m_x / b.m_xyzw.z;
	s.m_xyzw.w = a.m_x / b.m_xyzw.w;
}

ZP_FORCE_INLINE void zpMath::Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x + b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y + b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z + b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w + b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x - b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y - b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z - b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w - b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x * b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y * b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z * b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w * b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Div( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x / b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y / b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z / b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w / b.m_xyzw.w;
}

ZP_FORCE_INLINE void zpMath::Add( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x + b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y + b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z + b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w + b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Sub( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x - b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y - b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z - b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w - b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Mul( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x * b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y * b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z * b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w * b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Div( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x / b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y / b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z / b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w / b.m_xyzw.w;
}

ZP_FORCE_INLINE void zpMath::Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x + b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y + b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z + b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w + b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b )
{
	s.m_xyzw.x = a.m_xyzw.x - b.m_xyzw.x;
	s.m_xyzw.y = a.m_xyzw.y - b.m_xyzw.y;
	s.m_xyzw.z = a.m_xyzw.z - b.m_xyzw.z;
	s.m_xyzw.w = a.m_xyzw.w - b.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b )
{
	zp_float nx = a.m_xyzw.w * b.m_xyzw.x + a.m_xyzw.y * b.m_xyzw.w + a.m_xyzw.y * b.m_xyzw.z - a.m_xyzw.z * b.m_xyzw.y;
	zp_float ny = a.m_xyzw.w * b.m_xyzw.y + a.m_xyzw.x * b.m_xyzw.w + a.m_xyzw.z * b.m_xyzw.x - a.m_xyzw.x * b.m_xyzw.z;
	zp_float nz = a.m_xyzw.w * b.m_xyzw.z + a.m_xyzw.z * b.m_xyzw.w + a.m_xyzw.x * b.m_xyzw.y - a.m_xyzw.y * b.m_xyzw.x;
	zp_float nw = a.m_xyzw.w * b.m_xyzw.w - a.m_xyzw.x * b.m_xyzw.x - a.m_xyzw.y * b.m_xyzw.y - a.m_xyzw.z * b.m_xyzw.z;
	s.m_xyzw.x = nx;
	s.m_xyzw.y = ny;
	s.m_xyzw.z = nz;
	s.m_xyzw.w = nw;
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
ZP_FORCE_INLINE void zpMath::Dot4( zpScalar& s, const zpQuaternion4f& a, const zpQuaternion4f& b )
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

ZP_FORCE_INLINE void zpMath::LengthSquared2( zpScalar& s, const zpVector4f& a )
{
	s.m_x = a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y;
}
ZP_FORCE_INLINE void zpMath::LengthSquared3( zpScalar& s, const zpVector4f& a )
{
	s.m_x = a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y + a.m_xyzw.z * a.m_xyzw.z;
}
ZP_FORCE_INLINE void zpMath::LengthSquared4( zpScalar& s, const zpVector4f& a )
{
	s.m_x = a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y + a.m_xyzw.z * a.m_xyzw.z + a.m_xyzw.w * a.m_xyzw.w;
}
ZP_FORCE_INLINE void zpMath::LengthSquared4( zpScalar& s, const zpQuaternion4f& a )
{
	s.m_x = a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y + a.m_xyzw.z * a.m_xyzw.z + a.m_xyzw.w * a.m_xyzw.w;
}

ZP_FORCE_INLINE void zpMath::Length2( zpScalar& s, const zpVector4f& a )
{
	s.m_x = zp_sqrt( a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y );
}
ZP_FORCE_INLINE void zpMath::Length3( zpScalar& s, const zpVector4f& a )
{
	s.m_x = zp_sqrt( a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y + a.m_xyzw.z * a.m_xyzw.z );
}
ZP_FORCE_INLINE void zpMath::Length4( zpScalar& s, const zpVector4f& a )
{
	s.m_x = zp_sqrt( a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y + a.m_xyzw.z * a.m_xyzw.z + a.m_xyzw.w * a.m_xyzw.w );
}
ZP_FORCE_INLINE void zpMath::Length4( zpScalar& s, const zpQuaternion4f& a )
{
	s.m_x = zp_sqrt( a.m_xyzw.x * a.m_xyzw.x + a.m_xyzw.y * a.m_xyzw.y + a.m_xyzw.z * a.m_xyzw.z + a.m_xyzw.w * a.m_xyzw.w );
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


ZP_FORCE_INLINE void zpMath::Abs( zpScalar& s, const zpScalar& a )
{
	zp_abs( s.m_x, a.m_x );
}
ZP_FORCE_INLINE void zpMath::Abs( zpVector4f& s, const zpVector4f& a )
{
	zp_abs( s.m_xyzw.x, a.m_xyzw.x );
	zp_abs( s.m_xyzw.y, a.m_xyzw.y );
	zp_abs( s.m_xyzw.z, a.m_xyzw.z );
	zp_abs( s.m_xyzw.w, a.m_xyzw.w );
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

ZP_FORCE_INLINE void zpMath::Sqrt( zpScalar& s, const zpScalar& a )
{
	s.m_x = zp_sqrt( a.m_x );
}
ZP_FORCE_INLINE void zpMath::Sqrt( zpVector4f& s, const zpVector4f& a )
{
	s.m_xyzw.x = zp_sqrt( a.m_xyzw.x );
	s.m_xyzw.y = zp_sqrt( a.m_xyzw.y );
	s.m_xyzw.z = zp_sqrt( a.m_xyzw.z );
	s.m_xyzw.w = zp_sqrt( a.m_xyzw.w );
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
ZP_FORCE_INLINE void zpMath::Cmp0( zp_int& s, const zpScalar& a )
{
	s = a.m_x > 0.0f ? 1 : a.m_x < 0.0f ? -1 : 0;
}

ZP_FORCE_INLINE void zpMath::Cmp( zp_int& s, const zpVector4f& a, const zpVector4f& b )
{

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
	s.m_x = 
		a.m_14*a.m_23*a.m_32*a.m_41 - a.m_13*a.m_24*a.m_32*a.m_41 - a.m_14*a.m_22*a.m_33*a.m_41 + a.m_12*a.m_24*a.m_33*a.m_41+
		a.m_13*a.m_22*a.m_34*a.m_41 - a.m_12*a.m_23*a.m_34*a.m_41 - a.m_14*a.m_23*a.m_31*a.m_42 + a.m_13*a.m_24*a.m_31*a.m_42+
		a.m_14*a.m_21*a.m_33*a.m_42 - a.m_11*a.m_24*a.m_33*a.m_42 - a.m_13*a.m_21*a.m_34*a.m_42 + a.m_11*a.m_23*a.m_34*a.m_42+
		a.m_14*a.m_22*a.m_31*a.m_43 - a.m_12*a.m_24*a.m_31*a.m_43 - a.m_14*a.m_21*a.m_32*a.m_43 + a.m_11*a.m_24*a.m_32*a.m_43+
		a.m_12*a.m_21*a.m_34*a.m_43 - a.m_11*a.m_22*a.m_34*a.m_43 - a.m_13*a.m_22*a.m_31*a.m_44 + a.m_12*a.m_23*a.m_31*a.m_44+
		a.m_13*a.m_21*a.m_32*a.m_44 - a.m_11*a.m_23*a.m_32*a.m_44 - a.m_12*a.m_21*a.m_33*a.m_44 + a.m_11*a.m_22*a.m_33*a.m_44;
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


ZP_FORCE_INLINE void zpMath::Lerp( zpVector4f& s, const zpVector4f& a, const zpVector4f& b, const zpScalar& alpha )
{
	zpMath::Sub( s, b, a );
	zpMath::Mul( s, s, alpha );
	zpMath::Add( s, s, a );
}
ZP_FORCE_INLINE void zpMath::Lerp( zpScalar& s, const zpScalar& a, const zpScalar& b, const zpScalar& alpha )
{
	zpMath::Sub( s, b, a );
	zpMath::Mul( s, s, alpha );
	zpMath::Add( s, s, a );
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

	s.m_xyzw.x = (chy_shp * chr) + (shy_chp * shr); // cos(yaw/2) * sin(pitch/2) * cos(roll/2) + sin(yaw/2) * cos(pitch/2) * sin(roll/2)
	s.m_xyzw.y = (shy_chp * chr) - (chy_shp * shr); // sin(yaw/2) * cos(pitch/2) * cos(roll/2) - cos(yaw/2) * sin(pitch/2) * sin(roll/2)
	s.m_xyzw.z = (chy_chp * shr) - (shy_shp * chr); // cos(yaw/2) * cos(pitch/2) * sin(roll/2) - sin(yaw/2) * sin(pitch/2) * cos(roll/2)
	s.m_xyzw.w = (chy_chp * chr) + (shy_shp * shr); // cos(yaw/2) * cos(pitch/2) * cos(roll/2) + sin(yaw/2) * sin(pitch/2) * sin(roll/2)
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
	s.m_xyzw.x = -a.m_xyzw.x;
	s.m_xyzw.y = -a.m_xyzw.y;
	s.m_xyzw.z = -a.m_xyzw.z;
}
void zpMath::Transform( zpVector4f& s, const zpQuaternion4f& a, const zpVector4f& b )
{

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
