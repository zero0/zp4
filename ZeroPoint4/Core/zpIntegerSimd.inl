
zpInteger::zpInteger() : m_integer( _mm_setzero_ps() ) {}
zpInteger::zpInteger( zp_int integer ) : m_integer( _mm_setzero_ps() ) {}
zpInteger::zpInteger( const zpInteger& integer ) : m_integer( integer.m_integer ) {}
zpInteger::zpInteger( zpInteger&& integer ) : m_integer( integer.m_integer ) {}
zpInteger::~zpInteger() {}

zpInteger::operator zp_integer() const {
	return m_integer;
}

void zpInteger::operator=( const zpInteger& integer ) {}
void zpInteger::operator=( zpInteger&& integer ) {}

zp_int zpInteger::getInteger() const {
	//return _mm_cvtsi128_si32( m_integer );
	return 0;
}

void zpInteger::abs() {
	
}

void zpInteger::abs( const zp_integer& integer ) {}

void zpInteger::mul( const zp_integer& integer ) {
	//m_integer = _mm_mul_epu32( m_integer, integer );
}
void zpInteger::div( const zp_integer& integer ) {
	//m_integer = _mm_mu( m_integer, integer );
}
void zpInteger::add( const zp_integer& integer ) {
	//m_integer = _mm_add_epi32( m_integer, integer );
}
void zpInteger::sub( const zp_integer& integer ) {
	//m_integer = _mm_sub_epi32( m_integer, integer );
}

const zp_integer zpInteger::operator-() const {
	//zp_integer zero = _mm_setzero_ps();
	//return _mm_sub_ps( zero, m_integer );
	return m_integer;
}
const zp_integer zpInteger::operator*( const zp_integer& integer ) const {
	//return _mm_mul_ps( m_integer, integer );
	return m_integer;
}
const zp_integer zpInteger::operator/( const zp_integer& integer ) const {
	//return _mm_div_ps( m_integer, integer );
	return m_integer;
}
const zp_integer zpInteger::operator+( const zp_integer& integer ) const {
	//return _mm_add_ps( m_integer, integer );
	return m_integer;
}
const zp_integer zpInteger::operator-( const zp_integer& integer ) const {
	//return _mm_sub_ps( m_integer, integer );
	return m_integer;
}

void zpInteger::setMin( const zp_integer& a, const zp_integer& b ) {
	//m_integer = _mm_min_ps( a, b );
}
void zpInteger::setMax( const zp_integer& a, const zp_integer& b ) {
	//m_integer = _mm_max_ps( a, b );
}

const zp_integer zpInteger::sqrt() const {
	//return _mm_sqrt_ps( m_integer );
	return m_integer;
}
const zp_integer zpInteger::invsqrt() const {
	return m_integer;
}

void zpInteger::zero() {
	//m_integer = _mm_setzero_ps();
}
zp_bool zpInteger::isZero() const {
	//zp_integer zero = _mm_setzero_ps();
	//zp_integer cmp = _mm_cmpeq_ps( zero, m_integer );
	//int v;
	//_mm_store_ss( &v, cmp );
	//return v != 0;
	return false;
}