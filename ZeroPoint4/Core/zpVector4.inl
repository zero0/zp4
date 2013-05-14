
ZP_FORCE_INLINE zpVector4f::zpVector4f()
	: m_xyzw()
{}

ZP_FORCE_INLINE zpVector4f::zpVector4f( zp_float xyzw )
	: m_xyzw( xyzw, xyzw, xyzw, xyzw )
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( zp_float x, zp_float y )
	: m_xyzw( x, y, 0, 0 )
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( zp_float x, zp_float y, zp_float z )
	: m_xyzw( x, y, z, 0 )
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( zp_float x, zp_float y, zp_float z, zp_float w )
	: m_xyzw( x, y, z, w )
{}

ZP_FORCE_INLINE zpVector4f::zpVector4f( const zpScalar& xyzw )
	: m_xyzw( xyzw.getFloat(), xyzw.getFloat(), xyzw.getFloat(), xyzw.getFloat() )
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( const zpScalar& x, const zpScalar& y )
	: m_xyzw( x.getFloat(), y.getFloat(), 0, 0 )
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( const zpScalar& x, const zpScalar& y, const zpScalar& z )
	: m_xyzw( x.getFloat(), y.getFloat(), z.getFloat(), 0 )
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( const zpScalar& x, const zpScalar& y, const zpScalar& z, const zpScalar& w )
	: m_xyzw( x.getFloat(), y.getFloat(), z.getFloat(), w.getFloat() )
{}

ZP_FORCE_INLINE zpVector4f::zpVector4f( const zpVector4f& vector )
	: m_xyzw( vector.m_xyzw ) 
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( zpVector4f&& vector )
	: m_xyzw( zp_move( vector.m_xyzw ) ) 
{}
ZP_FORCE_INLINE zpVector4f::~zpVector4f() {}

ZP_FORCE_INLINE void zpVector4f::operator=( const zpVector4f& vector )
{
	m_xyzw = vector.m_xyzw;
}
ZP_FORCE_INLINE void zpVector4f::operator=( zpVector4f&& vector )
{
	m_xyzw = zp_move( vector.m_xyzw );
}

ZP_FORCE_INLINE zpScalar zpVector4f::getX() const
{
	return m_xyzw.getX();
}
ZP_FORCE_INLINE zpScalar zpVector4f::getY() const
{
	return m_xyzw.getY();
}
ZP_FORCE_INLINE zpScalar zpVector4f::getZ() const
{
	return m_xyzw.getZ();
}
ZP_FORCE_INLINE zpScalar zpVector4f::getW() const
{
	return m_xyzw.getW();
}

ZP_FORCE_INLINE void zpVector4f::setX( const zpScalar& x )
{
	m_xyzw.setX( x );
}
ZP_FORCE_INLINE void zpVector4f::setY( const zpScalar& y )
{
	m_xyzw.setY( y );
}
ZP_FORCE_INLINE void zpVector4f::setZ( const zpScalar& z )
{
	m_xyzw.setZ( z );
}
ZP_FORCE_INLINE void zpVector4f::setW( const zpScalar& w )
{
	m_xyzw.setW( w );
}

//ZP_FORCE_INLINE zpScalar zpVector4f::magnitude3() const
//{
//	zpScalar mag( dot3( (*this) ) );
//	zpScalarSqrt( mag, mag );
//	return mag;
//}
//ZP_FORCE_INLINE zpScalar zpVector4f::magnitude4() const
//{
//	zpScalar mag( dot4( (*this) ) );
//	zpScalarSqrt( mag, mag );
//	return mag;
//}
//ZP_FORCE_INLINE zpScalar zpVector4f::magnitudeSquared3() const
//{
//	return dot3( (*this) );
//}
//ZP_FORCE_INLINE zpScalar zpVector4f::magnitudeSquared4() const
//{
//	return dot4( (*this) );
//}
//
//ZP_FORCE_INLINE zpVector4f zpVector4f::normalize3() const
//{
//	zpScalar mag( magnitude3() );
//	zpVector4f v( m_xyzw );
//	if( zpScalarEq( mag, zpScalar() ) ) return v;
//	v.div3( mag );
//	return v;
//}
//ZP_FORCE_INLINE void zpVector4f::normalize3()
//{
//	zpScalar mag( magnitude3() );
//	if( zpScalarNeq( mag, zpScalar() ) ) div3( mag );
//}
//ZP_FORCE_INLINE zpVector4f zpVector4f::normalize4() const
//{
//	zpScalar mag( magnitude4() );
//	zpVector4f v( m_xyzw );
//	if( zpScalarEq( mag, zpScalar() ) ) return v;
//	v.div4( mag );
//	return v;
//}
//ZP_FORCE_INLINE void zpVector4f::normalize4()
//{
//	zpScalar mag( magnitude4() );
//	if( zpScalarNeq( mag, zpScalar() ) ) div4( mag );
//}
//
//ZP_FORCE_INLINE zpVector4f zpVector4f::homogenize4() const
//{
//	zpScalar w( getW() );
//	if( zpScalarEq( w, zpScalar() ) ) {
//		return zpVector4f();
//	} else {
//		zpVector4f v( m_xyzw );
//		v.div4( w );
//		return v;
//	}
//}
//ZP_FORCE_INLINE void zpVector4f::homogenize4()
//{
//	zpScalar w( getW() );
//	if( zpScalarEq( w, zpScalar() ) ) {
//		zero4();
//	} else {
//		div4( w );
//	}
//}
//
//ZP_FORCE_INLINE const zp_vec4& zpVector4f::getVec4() const
//{
//	return m_xyzw;
//}
//
//ZP_FORCE_INLINE zpScalar zpVector4f::angleBetween3( const zpVector4f& vector ) const
//{
//	zpScalar angle( dot3( vector ) );
//	zpScalar mag( magnitude3() );
//
//	zpScalarMul( mag, mag, vector.magnitude3() );
//	zpScalarDiv( angle, angle, mag );
//	zpScalarAcos( angle, angle );
//
//	return angle;
//}
//ZP_FORCE_INLINE zpScalar zpVector4f::angleBetween4( const zpVector4f& vector ) const
//{
//	zpScalar angle( dot4( vector ) );
//	zpScalar mag( magnitude4() );
//
//	zpScalarMul( mag, mag, vector.magnitude4() );
//	zpScalarDiv( angle, angle, mag );
//	zpScalarAcos( angle, angle );
//
//	return angle;
//}
//
//ZP_FORCE_INLINE zpVector4f zpVector4f::lerp3( const zpVector4f& vector, zpScalar alpha ) const
//{
//	zpVector4f v( m_xyzw );
//	v.lerp3( vector, alpha );
//	return v;
//}
//ZP_FORCE_INLINE void zpVector4f::lerp3( const zpVector4f& vector, zpScalar alpha )
//{
//	zpScalarSaturate( alpha, alpha );
//	//add3( ( vector.m_xyzw - m_xyzw ) );
//}
//ZP_FORCE_INLINE zpVector4f zpVector4f::lerp4( const zpVector4f& vector, zpScalar alpha ) const
//{
//	zpVector4f v( m_xyzw );
//	v.lerp4( vector, alpha );
//	return v;
//}
//ZP_FORCE_INLINE void zpVector4f::lerp4( const zpVector4f& vector, zpScalar alpha )
//{
//	zpScalarSaturate( alpha, alpha );
//	//add4( ( vector.m_xyzw - m_xyzw ) * alpha );
//}
//
//ZP_FORCE_INLINE zpVector4f zpVector4f::slerp3( const zpVector4f& vector, zpScalar alpha ) const
//{
//	zpVector4f v( m_xyzw );//////
//	v.slerp3( vector, alpha );
//	return v;
//}
//ZP_FORCE_INLINE void zpVector4f::slerp3( const zpVector4f& vector, zpScalar alpha )
//{
//// 	alpha = zpScalarSaturate( alpha );
//// 		
//// 			zpScalar omega = dot3( vector );
//// 		zpScalar somao = zpScalarSin( ( zpScalarFromFloat( 1 ) - alpha ) * omega );
//// 		zpScalar sao = zpScalarSin( alpha * omega );
//// 		zpScalar so = zpScalarSin( omega );
//// 	
//// 		mul3( somao / so );
//// 	
//// 		zpVector4f v( vector );
//// 	v.mul3( sao / so );
//// 
//// 	add3( v );
//}
//ZP_FORCE_INLINE zpVector4f zpVector4f::slerp4( const zpVector4f& vector, zpScalar alpha ) const
//{
//	zpVector4f v( m_xyzw );
//	v.slerp4( vector, alpha );
//	return v;
//}
//ZP_FORCE_INLINE void zpVector4f::slerp4( const zpVector4f& vector, zpScalar alpha )
//{
//// 	alpha = zpScalarSaturate( alpha );
//// 
//// 	zpScalar omega = dot4( vector );
//// 	zpScalar somao = zpScalarSin( ( zpScalarFromFloat( 1 ) - alpha ) * omega );
//// 	zpScalar sao = zpScalarSin( alpha * omega );
//// 	zpScalar so = zpScalarSin( omega );
//// 
//// 	mul4( somao / so );
//// 
//// 	zpVector4f v( vector );
//// 	v.mul4( sao / so );
//// 
//// 	add4( v );
//}
//
//ZP_FORCE_INLINE zpVector4f operator+( const zpVector4f& v1, const zpVector4f& v2 )
//{
//	zpVector4f v( v1 );
//	v.add4( v2 );
//	return v;
//}
//ZP_FORCE_INLINE zpVector4f operator-( const zpVector4f& v1, const zpVector4f& v2 )
//{
//	zpVector4f v( v1 );
//	v.sub4( v2 );
//	return v;
//}
//ZP_FORCE_INLINE zpVector4f operator*( const zpVector4f& v1, const zpVector4f& v2 )
//{
//	zpVector4f v( v1 );
//	v.mul4( v2 );
//	return v;
//}
//ZP_FORCE_INLINE zpVector4f operator/( const zpVector4f& v1, const zpVector4f& v2 )
//{
//	zpVector4f v( v1 );
//	v.div4( v2 );
//	return v;
//}
//
//ZP_FORCE_INLINE zpVector4f operator*( const zpVector4f& v1, const zp_float& v2 )
//{
//	zpVector4f v( v1 );
//	v.mul4( zpScalar( v2 ) );
//	return v;
//}
//ZP_FORCE_INLINE zpVector4f operator*( const zp_float& v1, const zpVector4f& v2 )
//{
//	zpVector4f v( v2 );
//	v.mul4( zpScalar( v1 ) );
//	return v;
//}
//
//ZP_FORCE_INLINE zpVector4f operator/( const zpVector4f& v1, const zp_float& v2 )
//{
//	zpVector4f v( v1 );
//	v.div4( zpScalar( v2 ) );
//	return v;
//}
//ZP_FORCE_INLINE zpVector4f operator/( const zp_float& v1, const zpVector4f& v2 )
//{
//	zpVector4f v( v2 );
//	v.div4( zpScalar( v1 ) );
//	return v;
//}
//
//
//ZP_FORCE_INLINE zpVector4f operator*( const zpVector4f& v1, const zpScalar& v2 )
//{
//	zpVector4f v( v1 );
//	v.mul4( v2 );
//	return v;
//}
//ZP_FORCE_INLINE zpVector4f operator*( const zpScalar& v1, const zpVector4f& v2 )
//{
//	zpVector4f v( v2 );
//	v.mul4( v1 );
//	return v;
//}
//
//ZP_FORCE_INLINE zpVector4f operator/( const zpVector4f& v1, const zpScalar& v2 )
//{
//	zpVector4f v( v1 );
//	v.div4( v2 );
//	return v;
//}
//ZP_FORCE_INLINE zpVector4f operator/( const zpScalar& v1, const zpVector4f& v2 )
//{
//	zpVector4f v( v2 );
//	v.div4( v1 );
//	return v;
//}