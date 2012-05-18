
ZP_FORCE_INLINE zpReal::zpReal() : m_real( _mm_setzero_ps() ) {}
ZP_FORCE_INLINE zpReal::zpReal( zp_float real ) {}
ZP_FORCE_INLINE zpReal::zpReal( const zpReal& real ) {}
ZP_FORCE_INLINE zpReal::zpReal( zpReal&& real ) {}
ZP_FORCE_INLINE zpReal::~zpReal() {}

ZP_FORCE_INLINE zpReal::operator zp_real() const {
	return m_real;
}

ZP_FORCE_INLINE void zpReal::operator=( const zpReal& real ) {}
ZP_FORCE_INLINE void zpReal::operator=( zpReal&& real ) {}

ZP_FORCE_INLINE zp_float zpReal::getReal() const {
	zp_float v;
	_mm_store_ss( &v, m_real );
	return v;
}

ZP_FORCE_INLINE void zpReal::abs() {
	
}

ZP_FORCE_INLINE void zpReal::abs( const zp_real& real ) {}

ZP_FORCE_INLINE void zpReal::mul( const zp_real& real ) {
	m_real = _mm_mul_ps( m_real, real );
}
ZP_FORCE_INLINE void zpReal::div( const zp_real& real ) {
	m_real = _mm_div_ps( m_real, real );
}
ZP_FORCE_INLINE void zpReal::add( const zp_real& real ) {
	m_real = _mm_add_ps( m_real, real );
}
ZP_FORCE_INLINE void zpReal::sub( const zp_real& real ) {
	m_real = _mm_sub_ps( m_real, real );
}

ZP_FORCE_INLINE const zp_real zpReal::operator-() const {
	zp_real zero = _mm_setzero_ps();
	return _mm_sub_ps( zero, m_real );
}
ZP_FORCE_INLINE const zp_real zpReal::operator*( const zp_real& real ) const {
	return _mm_mul_ps( m_real, real );
}
ZP_FORCE_INLINE const zp_real zpReal::operator/( const zp_real& real ) const {
	return _mm_div_ps( m_real, real );
}
ZP_FORCE_INLINE const zp_real zpReal::operator+( const zp_real& real ) const {
	return _mm_add_ps( m_real, real );
}
ZP_FORCE_INLINE const zp_real zpReal::operator-( const zp_real& real ) const {
	return _mm_sub_ps( m_real, real );
}

ZP_FORCE_INLINE void zpReal::setMin( const zp_real& a, const zp_real& b ) {
	m_real = _mm_min_ps( a, b );
}
ZP_FORCE_INLINE void zpReal::setMax( const zp_real& a, const zp_real& b ) {
	m_real = _mm_max_ps( a, b );
}

ZP_FORCE_INLINE const zp_real zpReal::sqrt() const {
	return _mm_sqrt_ps( m_real );
}
ZP_FORCE_INLINE const zp_real zpReal::invsqrt() const {
	return m_real;
}

ZP_FORCE_INLINE void zpReal::zero() {
	m_real = _mm_setzero_ps();
}
ZP_FORCE_INLINE zp_bool zpReal::isZero() const {
	zp_real zero = _mm_setzero_ps();
	zp_real cmp = _mm_cmpeq_ps( zero, m_real );
	float v;
	_mm_store_ss( &v, cmp );
	return v != 0;
}