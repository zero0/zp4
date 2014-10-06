
ZP_FORCE_INLINE zpQuaternion4f::zpQuaternion4f()
	: m_xyzw()
{}

ZP_FORCE_INLINE zpQuaternion4f::zpQuaternion4f( zp_float xyzw )
	: m_xyzw( xyzw, xyzw, xyzw, xyzw )
{}
ZP_FORCE_INLINE zpQuaternion4f::zpQuaternion4f( zp_float x, zp_float y )
	: m_xyzw( x, y, 0.0f, 1.0f )
{}
ZP_FORCE_INLINE zpQuaternion4f::zpQuaternion4f( zp_float x, zp_float y, zp_float z )
	: m_xyzw( x, y, z, 1.0f )
{}
ZP_FORCE_INLINE zpQuaternion4f::zpQuaternion4f( zp_float x, zp_float y, zp_float z, zp_float w )
	: m_xyzw( x, y, z, w )
{}

ZP_FORCE_INLINE zpQuaternion4f::zpQuaternion4f( const zpScalar& xyzw )
	: m_xyzw( xyzw.getFloat(), xyzw.getFloat(), xyzw.getFloat(), xyzw.getFloat() )
{}
ZP_FORCE_INLINE zpQuaternion4f::zpQuaternion4f( const zpScalar& x, const zpScalar& y )
	: m_xyzw( x.getFloat(), y.getFloat(), 0.0f, 1.0f )
{}
ZP_FORCE_INLINE zpQuaternion4f::zpQuaternion4f( const zpScalar& x, const zpScalar& y, const zpScalar& z )
	: m_xyzw( x.getFloat(), y.getFloat(), z.getFloat(), 1.0f )
{}
ZP_FORCE_INLINE zpQuaternion4f::zpQuaternion4f( const zpScalar& x, const zpScalar& y, const zpScalar& z, const zpScalar& w )
	: m_xyzw( x.getFloat(), y.getFloat(), z.getFloat(), w.getFloat() )
{}

ZP_FORCE_INLINE zpQuaternion4f::zpQuaternion4f( const zpQuaternion4f& quat )
	: m_xyzw( quat.m_xyzw ) 
{}
ZP_FORCE_INLINE zpQuaternion4f::zpQuaternion4f( zpQuaternion4f&& quat )
	: m_xyzw( zp_move( quat.m_xyzw ) ) 
{}
ZP_FORCE_INLINE zpQuaternion4f::~zpQuaternion4f() {}

ZP_FORCE_INLINE void zpQuaternion4f::operator=( const zpQuaternion4f& vector )
{
	m_xyzw = vector.m_xyzw;
}
ZP_FORCE_INLINE void zpQuaternion4f::operator=( zpQuaternion4f&& quat )
{
	m_xyzw = zp_move( quat.m_xyzw );
}

ZP_FORCE_INLINE zpScalar zpQuaternion4f::getX() const
{
	return m_xyzw.getX();
}
ZP_FORCE_INLINE zpScalar zpQuaternion4f::getY() const
{
	return m_xyzw.getY();
}
ZP_FORCE_INLINE zpScalar zpQuaternion4f::getZ() const
{
	return m_xyzw.getZ();
}
ZP_FORCE_INLINE zpScalar zpQuaternion4f::getW() const
{
	return m_xyzw.getW();
}

ZP_FORCE_INLINE void zpQuaternion4f::setX( const zpScalar& x )
{
	m_xyzw.setX( x );
}
ZP_FORCE_INLINE void zpQuaternion4f::setY( const zpScalar& y )
{
	m_xyzw.setY( y );
}
ZP_FORCE_INLINE void zpQuaternion4f::setZ( const zpScalar& z )
{
	m_xyzw.setZ( z );
}
ZP_FORCE_INLINE void zpQuaternion4f::setW( const zpScalar& w )
{
	m_xyzw.setW( w );
}
