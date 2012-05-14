
ZP_FORCE_INLINE zpInteger::zpInteger() {}
ZP_FORCE_INLINE zpInteger::zpInteger( zp_int integer ) : m_integer( integer ) {}
ZP_FORCE_INLINE zpInteger::zpInteger( const zpInteger& integer ) : m_integer( integer.m_integer ) {}
ZP_FORCE_INLINE zpInteger::zpInteger( zpInteger&& integer ) : m_integer( integer.m_integer ) {}
ZP_FORCE_INLINE zpInteger::~zpInteger() {}

ZP_FORCE_INLINE void zpInteger::operator=( const zpInteger& integer ) {
	m_integer = integer.m_integer;
}
ZP_FORCE_INLINE void zpInteger::operator=( zpInteger&& integer ) {
	m_integer = integer.m_integer;
}

ZP_FORCE_INLINE zp_int zpInteger::getInteger() const {
	return m_integer;
}

ZP_FORCE_INLINE void zpInteger::abs() {
	m_integer = ::abs( m_integer );
}
ZP_FORCE_INLINE void zpInteger::abs( const zp_integer& integer ) {
	m_integer = ::abs( integer );
}

ZP_FORCE_INLINE void zpInteger::mul( const zp_integer& integer ) {
	m_integer *= integer;
}
ZP_FORCE_INLINE void zpInteger::div( const zp_integer& integer ) {
	m_integer /= integer;
}
ZP_FORCE_INLINE void zpInteger::add( const zp_integer& integer ) {
	m_integer += integer;
}
ZP_FORCE_INLINE void zpInteger::sub( const zp_integer& integer ) {
	m_integer -= integer;
}

ZP_FORCE_INLINE const zp_integer zpInteger::operator-() const {
	return -m_integer;
}
ZP_FORCE_INLINE const zp_integer zpInteger::operator*( const zp_integer& integer ) const {
	return m_integer * integer;
}
ZP_FORCE_INLINE const zp_integer zpInteger::operator/( const zp_integer& integer ) const {
	return m_integer / integer;
}
ZP_FORCE_INLINE const zp_integer zpInteger::operator+( const zp_integer& integer ) const {
	return m_integer + integer;
}
ZP_FORCE_INLINE const zp_integer zpInteger::operator-( const zp_integer& integer ) const {
	return m_integer - integer;
}

ZP_FORCE_INLINE void zpInteger::setMin( const zp_integer& a, const zp_integer& b ) {
	m_integer = ZP_MIN( a, b );
}
ZP_FORCE_INLINE void zpInteger::setMax( const zp_integer& a, const zp_integer& b ) {
	m_integer = ZP_MAX( a, b );
}

ZP_FORCE_INLINE const zp_integer zpInteger::sqrt() const {
	return (zp_integer)sqrtf( m_integer );
}
ZP_FORCE_INLINE const zp_integer zpInteger::invsqrt() const {
	return (zp_integer)( 1.f / sqrtf( m_integer ) );
}

ZP_FORCE_INLINE void zpInteger::zero() {
	m_integer = 0;
}
ZP_FORCE_INLINE zp_bool zpInteger::isZero() const {
	return m_integer == 0;
}