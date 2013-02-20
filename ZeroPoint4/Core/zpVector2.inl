
template<typename T>
zpVector2<T>::zpVector2() : m_x( 0 ), m_y( 0 ) {}
template<typename T>
zpVector2<T>::zpVector2( T x, T y ) : m_x( x ), m_y( y ) {}
template<typename T>
zpVector2<T>::zpVector2( T* xy ) : m_x( xy[0] ), m_y( xy[1] ) {}
template<typename T> template<typename F>
zpVector2<T>::zpVector2( const zpVector2<F>& vector ) : m_x( (T)vector.m_x ), m_y( (T)vector.m_y ) {}
template<typename T> template<typename F>
zpVector2<T>::zpVector2( zpVector2<F>&& vector ) : m_x( (T)vector.m_x ), m_y( (T)vector.m_y ) {}
template<typename T>
zpVector2<T>::~zpVector2() {}

template<typename T>
void zpVector2<T>::operator=( T* xy ) {
	m_x = xy[0];
	m_y = xy[1];
}
template<typename T> template<typename F>
void zpVector2<T>::operator=( const zpVector2<F>& vector ) {
	m_x = (T)vector.m_x;
	m_y = (T)vector.m_y;
}
template<typename T> template<typename F>
void zpVector2<T>::operator=( zpVector2<F>&& vector ) {
	m_x = (T)vector.m_x;
	m_y = (T)vector.m_y;
}

template<typename T>
zpVector2<T> zpVector2<T>::operator-() const {
	return zpVector2<T>( -m_x, -m_y );
}

template<typename T> template<typename F>
zpVector2<T> zpVector2<T>::operator+( const zpVector2<F>& vector ) const {
	return zpVector2<T>( m_x + vector.m_x, m_y + vector.m_y );
}
template<typename T> template<typename F>
zpVector2<T> zpVector2<T>::operator-( const zpVector2<F>& vector ) const {
	return zpVector2<T>( m_x - vector.m_x, m_y - vector.m_y );
}
template<typename T> template<typename F>
zpVector2<T> zpVector2<T>::operator*( const zpVector2<F>& vector ) const {
	return zpVector2<T>( m_x * vector.m_x, m_y * vector.m_y );
}
template<typename T> template<typename F>
zpVector2<T> zpVector2<T>::operator/( const zpVector2<F>& vector ) const {
	return zpVector2<T>( m_x / vector.m_x, m_y / vector.m_y );
}

template<typename T>
zpVector2<T> zpVector2<T>::operator*( zp_float scale ) const {
	return zpVector2<T>( m_x * scale, m_y * scale );
}
template<typename T>
zpVector2<T> zpVector2<T>::operator/( zp_float scale ) const {
	return zpVector2<T>( m_x / scale, m_y / scale );
}

template<typename T> template<typename F>
void zpVector2<T>::operator+=( const zpVector2<F>& vector ) {
	m_x += vector.m_x;
	m_y += vector.m_y;
}
template<typename T> template<typename F>
void zpVector2<T>::operator-=( const zpVector2<F>& vector ) {
	m_x -= vector.m_x;
	m_y -= vector.m_y;
}
template<typename T> template<typename F>
void zpVector2<T>::operator*=( const zpVector2<F>& vector ) {
	m_x *= vector.m_x;
	m_y *= vector.m_y;
}
template<typename T> template<typename F>
void zpVector2<T>::operator/=( const zpVector2<F>& vector ) {
	m_x /= vector.m_x;
	m_y /= vector.m_y;
}

template<typename T>
void zpVector2<T>::operator*=( zp_float scale ) {
	m_x *= scale;
	m_y *= scale;
}
template<typename T> 
void zpVector2<T>::operator/=( zp_float scale ) {
	const zp_float invScale = 1.f / scale;
	m_x *= scale;
	m_y *= scale;
}

template<typename T>
void zpVector2<T>::setX( T x ) {
	m_x = x;
}
template<typename T>
T zpVector2<T>::getX() const {
	return m_x;
}

template<typename T>
void zpVector2<T>::setY( T y ) {
	m_y = y;
}
template<typename T>
T zpVector2<T>::getY() const {
	return m_y;
}

template<typename T>
void zpVector2<T>::set( T x, T y ) {
	m_x = x;
	m_y = y;
}

template<typename T>
void zpVector2<T>::zero() {
	m_x = (T)0;
	m_y = (T)0;
}
template<typename T>
zp_bool zpVector2<T>::isZero() const {
	return m_x == (T)0 && m_y == (T)0;
}

template<typename T> template<typename F>
zp_float zpVector2<T>::dot( const zpVector2<F>& vector ) const {
	return m_x * vector.m_x + m_y * vector.m_y;
}
template<typename T> template<typename F>
zp_float zpVector2<T>::cross( const zpVector2<F>& vector ) const {
	return m_x * vector.m_y - my * vector.m_x;
}

template<typename T>
zp_float zpVector2<T>::magnitudeSquared() const {
	return m_x * m_x + m_y * m_y;
}
template<typename T>
zp_float zpVector2<T>::magnitude() const {
	return zp_sqrt( magnitudeSquared() );
}
template<typename T>
void zpVector2<T>::normalize() {
	zp_float mag = magnitude();
	if( !mag ) mag = 1.f;
	const zp_float inv = 1.f / mag;
	m_x *= inv;
	m_y *= inv;
}

template<typename T> template<typename F>
zp_float zpVector2<T>::distanceToSquared( const zpVector2<F>& vector ) const {
	const zp_float x = vector.m_x - m_x;
	const zp_float y = vector.m_y - m_y;
	return x * x + y * y;
}
template<typename T> template<typename F>
zp_float zpVector2<T>::distanceTo( const zpVector2<F>& vector ) const {
	return zp_sqrt( distanceToSquared( vector ) );
}

template<typename T> template<typename F>
zpVector2<T> zpVector2<T>::reflect( const zpVector2<F>& normal ) const {
	return (*this) - ( normal * 2.f * normal.dot( normal ) );
}
template<typename T> template<typename F>
zpVector2<T> zpVector2<T>::refract( const zpVector2<F>& normal, zp_float indexOfRefractionIncoming, zp_float indexOfRefractionTransmit ) const {
	const zp_float nOverN = indexOfRefractionIncoming / indexOfRefractionTransmit;

	const float dDotN = dot( normal );

	const zp_float disc = 1.f + ( ( nOverN * nOverN ) * ( ( dDotN * dDotN ) - 1.f ) );
	if( disc < 0.f ) {
		return reflect( normal );
	}
		
	const zp_float beta = ( nOverN * dDotN ) - sqrtf( disc );
	
	return (*this) * nOverN + ( normal * beta );
}
template<typename T> template<typename F>
zpVector2<T> zpVector2<T>::faceForward( const zpVector2<F>& normal ) const {
	return dot( normal ) < 0.f ? -(*this) : (*this);
}
template<typename T> template<typename F>
zp_float zpVector2<T>::angleBetween( const zpVector2<F>& normal, zp_bool inRadians ) const {
	const zp_float angle = acosf( dot( normal ) / ( magnitude() * normal.magnitude() ) );
	return inRadians ? angle : ZP_RAD_TO_DEG( angle );
}

template<typename T> template<typename F>
zpVector2<T> zpVector2<T>::lerp( const zpVector2<F>& vector, zp_float alpha ) const {
	return ( (*this) + ( vector - (*this) ) ) * zp_saturate( alpha );
}
template<typename T> template<typename F>
zpVector2<T> zpVector2<T>::slerp( const zpVector2<F>& vector, zp_float alpha ) const {
	alpha = zp_saturate( alpha );

	const zp_float omega = dot( vector );
	const zp_float somao = sinf( ( 1 - alpha ) * omega );
	const zp_float sao = sinf( alpha * omega );
	const zp_float so = sinf( omega );

	return ( (*this) * ( somao / so ) ) + ( vector * ( sao / so ) );
}

template<typename T>
zpVector2<T> zpVector2<T>::rotateLeft() const {
	return zpVector2<T>( -m_y, m_x );
}
template<typename T>
zpVector2<T> zpVector2<T>::rotateRight() const {
	return zpVector2<T>( m_y, -m_x );
}
template<typename T>
zpVector2<T> zpVector2<T>::rotate( zp_float radians ) const {
	const zp_float c = zp_cos( radians );
	const zp_float s = zp_sin( radians );
	return zpVector2<T>( m_x * c - m_y * s, m_x * s + m_y * c );
}
template<typename T>
zpVector2<T> zpVector2<T>::rotateDeg( zp_float degrees ) const {
	return rotate( ZP_DEG_TO_RAD( degrees ) );
}
template<typename T>
zp_float zpVector2<T>::angle() const {
	return zp_atan2( m_y, m_x );
}

template<typename T> template<typename F>
zp_bool zpVector2<T>::operator==( const zpVector2<F>& vector ) const {
	return m_x == vector.m_x && m_y == vector.m_y;
}
template<typename T> template<typename F>
zp_bool zpVector2<T>::operator!=( const zpVector2<F>& vector ) const {
	return m_x != vector.m_x || m_y != vector.m_y;
}
