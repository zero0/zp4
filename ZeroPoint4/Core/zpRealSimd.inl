
zpReal::zpReal() : m_real( _mm_setzero_ps() ) {}
zpReal::zpReal( zp_float real ) {}
zpReal::zpReal( const zpReal& real ) {}
zpReal::zpReal( zpReal&& real ) {}
zpReal::~zpReal() {}

void zpReal::operator=( const zpReal& real ) {}
void zpReal::operator=( zpReal&& real ) {}

zp_float zpReal::getReal() const {
	zp_float v;
	_mm_store_ss( &v, m_real );
	return v;
}

void zpReal::abs() {
	
}

void zpReal::abs( const zp_real& real ) {}

void zpReal::mul( const zp_real& real ) {
	m_real = _mm_mul_ps( m_real, real );
}
void zpReal::div( const zp_real& real ) {
	m_real = _mm_div_ps( m_real, real );
}
void zpReal::add( const zp_real& real ) {
	m_real = _mm_add_ps( m_real, real );
}
void zpReal::sub( const zp_real& real ) {
	m_real = _mm_sub_ps( m_real, real );
}

const zp_real zpReal::operator-() const {
	zp_real zero = _mm_setzero_ps();
	return _mm_sub_ps( zero, m_real );
}
const zp_real zpReal::operator*( const zp_real& real ) const {
	return _mm_mul_ps( m_real, real );
}
const zp_real zpReal::operator/( const zp_real& real ) const {
	return _mm_div_ps( m_real, real );
}
const zp_real zpReal::operator+( const zp_real& real ) const {
	return _mm_add_ps( m_real, real );
}
const zp_real zpReal::operator-( const zp_real& real ) const {
	return _mm_sub_ps( m_real, real );
}

void zpReal::setMin( const zp_real& a, const zp_real& b ) {
	m_real = _mm_min_ps( a, b );
}
void zpReal::setMax( const zp_real& a, const zp_real& b ) {
	m_real = _mm_max_ps( a, b );
}

const zp_real zpReal::sqrt() const {
	return _mm_sqrt_ps( m_real );
}
const zp_real zpReal::invsqrt() const {
	return m_real;
}

void zpReal::zero() {
	m_real = _mm_setzero_ps();
}
zp_bool zpReal::isZero() const {
	zp_real zero = _mm_setzero_ps();
	zp_real cmp = _mm_cmpeq_ps( zero, m_real );
	float v;
	_mm_store_ss( &v, cmp );
	return v != 0;
}