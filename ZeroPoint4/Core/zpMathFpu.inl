

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
