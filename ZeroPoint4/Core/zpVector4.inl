
ZP_FORCE_INLINE zpVector4f::zpVector4f() : 
	m_xyzw()
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( const zp_real& x, const zp_real& y, const zp_real& z, const zp_real& w ) :
	m_xyzw(
		zp_real_to_float( x ),
		zp_real_to_float( y ),
		zp_real_to_float( z ),
		zp_real_to_float( w )
	)
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( const zpVector4f& vector ) : 
	m_xyzw( vector.m_xyzw ) 
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( zpVector4f&& vector ) : 
	m_xyzw( vector.m_xyzw ) 
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( const zp_vec4&  vector ) : 
	m_xyzw( vector ) 
{}
ZP_FORCE_INLINE zpVector4f::~zpVector4f() {}

ZP_FORCE_INLINE void zpVector4f::operator=( const zpVector4f& vector ) {
	m_xyzw = vector.m_xyzw;
}
ZP_FORCE_INLINE void zpVector4f::operator=( zpVector4f&& vector ) {
	m_xyzw = vector.m_xyzw;
}
ZP_FORCE_INLINE void zpVector4f::operator=( const zp_vec4& vector ) {
	m_xyzw = vector;
}

ZP_FORCE_INLINE zp_vec4 zpVector4f::toVec4() const {
	return m_xyzw;
}

ZP_FORCE_INLINE zp_real zpVector4f::getX() const {
	return m_xyzw.getX();
}
ZP_FORCE_INLINE zp_real zpVector4f::getY() const {
	return m_xyzw.getY();
}
ZP_FORCE_INLINE zp_real zpVector4f::getZ() const {
	return m_xyzw.getZ();
}
ZP_FORCE_INLINE zp_real zpVector4f::getW() const {
	return m_xyzw.getW();
}


ZP_FORCE_INLINE zp_real zpVector4f::magnitude3() const {
	return zp_real_sqrt( dot3( (*this) ) );
}
ZP_FORCE_INLINE zp_real zpVector4f::magnitude4() const {
	return zp_real_sqrt( dot4( (*this) ) );
}
ZP_FORCE_INLINE zp_real zpVector4f::magnitudeSquared3() const {
	return dot3( (*this) );
}
ZP_FORCE_INLINE zp_real zpVector4f::magnitudeSquared4() const {
	return dot4( (*this) );
}

ZP_FORCE_INLINE zpVector4f zpVector4f::normalize3() const {
	zp_real mag = magnitude3();
	zpVector4f v( m_xyzw );
	if( zp_real_eq( mag, zp_real_zero() ) ) return v;
	v.div3( mag );
	return v;
}
ZP_FORCE_INLINE void zpVector4f::normalize3() {
	zp_real mag = magnitude3();
	if( zp_real_neq( mag, zp_real_zero() ) ) div3( mag );
}
ZP_FORCE_INLINE zpVector4f zpVector4f::normalize4() const {
	zp_real mag = magnitude4();
	zpVector4f v( m_xyzw );
	if( zp_real_eq( mag, zp_real_zero() ) ) return v;
	v.div4( mag );
	return v;
}
ZP_FORCE_INLINE void zpVector4f::normalize4() {
	zp_real mag = magnitude4();
	if( zp_real_neq( mag, zp_real_zero() ) ) div4( mag );
}

ZP_FORCE_INLINE zpVector4f zpVector4f::homogenize4() const {
	zp_real w = getW();
	if( zp_real_eq( w, zp_real_zero() ) ) {
		return zpVector4f();
	} else {
		zpVector4f v( m_xyzw );
		v.div4( w );
		return v;
	}
}
ZP_FORCE_INLINE void zpVector4f::homogenize4() {
	zp_real w = getW();
	if( zp_real_eq( w, zp_real_zero() ) ) {
		zero4();
	} else {
		div4( w );
	}
}


ZP_FORCE_INLINE zp_real zpVector4f::angleBetween3( const zpVector4f& vector ) const {
	return zp_real_acos( dot3( vector ) / ( magnitude3() * vector.magnitude3() ) );
}
ZP_FORCE_INLINE zp_real zpVector4f::angleBetween4( const zpVector4f& vector ) const {
	return zp_real_acos( dot4( vector ) / ( magnitude4() * vector.magnitude4() ) );
}

ZP_FORCE_INLINE zpVector4f zpVector4f::lerp3( const zpVector4f& vector, zp_real alpha ) const {
	zpVector4f v( m_xyzw );
	v.lerp3( vector, alpha );
	return v;
}
ZP_FORCE_INLINE void zpVector4f::lerp3( const zpVector4f& vector, zp_real alpha ) {
	alpha = zp_real_clamp01( alpha );
	add3( ( vector.m_xyzw - m_xyzw ) * alpha );
}
ZP_FORCE_INLINE zpVector4f zpVector4f::lerp4( const zpVector4f& vector, zp_real alpha ) const {
	zpVector4f v( m_xyzw );
	v.lerp4( vector, alpha );
	return v;
}
ZP_FORCE_INLINE void zpVector4f::lerp4( const zpVector4f& vector, zp_real alpha ) {
	alpha = zp_real_clamp01( alpha );
	add4( ( vector.m_xyzw - m_xyzw ) * alpha );
}

ZP_FORCE_INLINE zpVector4f zpVector4f::slerp3( const zpVector4f& vector, zp_real alpha ) const {
	zpVector4f v( m_xyzw );
	v.slerp3( vector, alpha );
	return v;
}
ZP_FORCE_INLINE void zpVector4f::slerp3( const zpVector4f& vector, zp_real alpha ) {
	alpha = zp_real_clamp01( alpha );

	zp_real omega = dot3( vector );
	zp_real somao = zp_real_sin( ( zp_real_from_float( 1 ) - alpha ) * omega );
	zp_real sao = zp_real_sin( alpha * omega );
	zp_real so = zp_real_sin( omega );

	mul3( somao / so );

	zpVector4f v( vector );
	v.mul3( sao / so );

	add3( v );
}
ZP_FORCE_INLINE zpVector4f zpVector4f::slerp4( const zpVector4f& vector, zp_real alpha ) const {
	zpVector4f v( m_xyzw );
	v.slerp4( vector, alpha );
	return v;
}
ZP_FORCE_INLINE void zpVector4f::slerp4( const zpVector4f& vector, zp_real alpha ) {
	alpha = zp_real_clamp01( alpha );

	zp_real omega = dot4( vector );
	zp_real somao = zp_real_sin( ( zp_real_from_float( 1 ) - alpha ) * omega );
	zp_real sao = zp_real_sin( alpha * omega );
	zp_real so = zp_real_sin( omega );

	mul4( somao / so );

	zpVector4f v( vector );
	v.mul4( sao / so );

	add4( v );
}

ZP_FORCE_INLINE zpVector4f operator+( const zpVector4f& v1, const zpVector4f& v2 ) {
	zpVector4f v( v1 );
	v.add4( v2 );
	return v;
}
ZP_FORCE_INLINE zpVector4f operator-( const zpVector4f& v1, const zpVector4f& v2 ) {
	zpVector4f v( v1 );
	v.sub4( v2 );
	return v;
}
ZP_FORCE_INLINE zpVector4f operator*( const zpVector4f& v1, const zpVector4f& v2 ) {
	zpVector4f v( v1 );
	v.mul4( v2 );
	return v;
}
ZP_FORCE_INLINE zpVector4f operator/( const zpVector4f& v1, const zpVector4f& v2 ) {
	zpVector4f v( v1 );
	v.div4( v2 );
	return v;
}

ZP_FORCE_INLINE zpVector4f operator*( const zpVector4f& v1, const zp_float& v2 ) {
	zpVector4f v( v1 );
	v.mul4( zp_real_from_float( v2 ) );
	return v;
}
ZP_FORCE_INLINE zpVector4f operator*( const zp_float& v1, const zpVector4f& v2 ) {
	zpVector4f v( v2 );
	v.mul4( zp_real_from_float( v1 ) );
	return v;
}

ZP_FORCE_INLINE zpVector4f operator/( const zpVector4f& v1, const zp_float& v2 ) {
	zpVector4f v( v1 );
	v.div4( zp_real_from_float( v2 ) );
	return v;
}
ZP_FORCE_INLINE zpVector4f operator/( const zp_float& v1, const zpVector4f& v2 ) {
	zpVector4f v( v2 );
	v.mul4( zp_real_rcp( zp_real_from_float( v1 ) ) );
	return v;
}
