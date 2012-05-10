
ZP_FORCE_INLINE zpReal::zpReal() {}
ZP_FORCE_INLINE zpReal::zpReal( zp_float real ) : m_real( real ) {}
ZP_FORCE_INLINE zpReal::zpReal( const zpReal& real ) : m_real( real.m_real ) {}
ZP_FORCE_INLINE zpReal::zpReal( zpReal&& real ) : m_real( real.m_real ) {}
ZP_FORCE_INLINE zpReal::~zpReal() {}

ZP_FORCE_INLINE void zpReal::operator=( const zpReal& real ) {
	m_real = real.m_real;
}
ZP_FORCE_INLINE void zpReal::operator=( zpReal&& real ) {
	m_real = real.m_real;
}

ZP_FORCE_INLINE zp_float zpReal::getReal() const {
	return m_real;
}

ZP_FORCE_INLINE void zpReal::abs() {
	m_real = fabs( m_real );
}
ZP_FORCE_INLINE void zpReal::abs( const zp_real& real ) {
	m_real = fabs( real );
}

ZP_FORCE_INLINE void zpReal::mul( const zp_real& real ) {
	m_real *= real;
}
ZP_FORCE_INLINE void zpReal::div( const zp_real& real ) {
	m_real /= real;
}
ZP_FORCE_INLINE void zpReal::add( const zp_real& real ) {
	m_real += real;
}
ZP_FORCE_INLINE void zpReal::sub( const zp_real& real ) {
	m_real -= real;
}

ZP_FORCE_INLINE const zp_real zpReal::operator-() const {
	return -m_real;
}
ZP_FORCE_INLINE const zp_real zpReal::operator*( const zp_real& real ) const {
	return m_real * real;
}
ZP_FORCE_INLINE const zp_real zpReal::operator/( const zp_real& real ) const {
	return m_real / real;
}
ZP_FORCE_INLINE const zp_real zpReal::operator+( const zp_real& real ) const {
	return m_real + real;
}
ZP_FORCE_INLINE const zp_real zpReal::operator-( const zp_real& real ) const {
	return m_real - real;
}

ZP_FORCE_INLINE void zpReal::setMin( const zp_real& a, const zp_real& b ) {
	m_real = ZP_MIN( a, b );
}
ZP_FORCE_INLINE void zpReal::setMax( const zp_real& a, const zp_real& b ) {
	m_real = ZP_MAX( a, b );
}

ZP_FORCE_INLINE const zp_real zpReal::sqrt() const {
	return sqrtf( m_real );
}
ZP_FORCE_INLINE const zp_real zpReal::invsqrt() const {
	return 1.f / sqrtf( m_real );
}

ZP_FORCE_INLINE void zpReal::zero() {
	m_real = 0;
}
ZP_FORCE_INLINE zp_bool zpReal::isZero() const {
	return m_real == 0;
}