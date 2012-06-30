#include <math.h>
/*
ZP_FORCE_INLINE zpReal::zpReal() : m_real( 0 ) {}
ZP_FORCE_INLINE zpReal::zpReal( zp_real real ) : m_real( real ) {}
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
ZP_FORCE_INLINE void zpReal::abs( const zpReal& real ) {
	m_real = fabs( real );
}

ZP_FORCE_INLINE void zpReal::mul( const zpReal& real ) {
	m_real *= real;
}
ZP_FORCE_INLINE void zpReal::div( const zpReal& real ) {
	m_real /= real;
}
ZP_FORCE_INLINE void zpReal::add( const zpReal& real ) {
	m_real += real;
}
ZP_FORCE_INLINE void zpReal::sub( const zpReal& real ) {
	m_real -= real;
}

ZP_FORCE_INLINE const zpReal zpReal::operator-() const {
	return -m_real;
}
ZP_FORCE_INLINE const zpReal zpReal::operator*( const zpReal& real ) const {
	return m_real * real;
}
ZP_FORCE_INLINE const zpReal zpReal::operator/( const zpReal& real ) const {
	return m_real / real;
}
ZP_FORCE_INLINE const zpReal zpReal::operator+( const zpReal& real ) const {
	return m_real + real;
}
ZP_FORCE_INLINE const zpReal zpReal::operator-( const zpReal& real ) const {
	return m_real - real;
}

ZP_FORCE_INLINE void zpReal::setMin( const zpReal& a, const zpReal& b ) {
	m_real = ZP_MIN( a, b );
}
ZP_FORCE_INLINE void zpReal::setMax( const zpReal& a, const zpReal& b ) {
	m_real = ZP_MAX( a, b );
}

ZP_FORCE_INLINE const zpReal zpReal::sqrt() const {
	return sqrtf( m_real );
}
ZP_FORCE_INLINE void zpReal::sqrt( const zpReal& real ) {
	m_real = sqrtf( real );
}
ZP_FORCE_INLINE const zpReal zpReal::invsqrt() const {
	return 1.f / sqrtf( m_real );
}
ZP_FORCE_INLINE void zpReal::invsqrt( const zpReal& real ) {
	m_real = 1.f / sqrtf( real );
}

ZP_FORCE_INLINE void zpReal::zero() {
	m_real = 0;
}
ZP_FORCE_INLINE zp_bool zpReal::isZero() const {
	return m_real == 0;
}

ZP_FORCE_INLINE void zpReal::clamp01() {
	m_real = zp_clamp01( m_real );
}
ZP_FORCE_INLINE void zpReal::clamp01( const zpReal& real ) {
	m_real = zp_clamp01( real );
}

ZP_FORCE_INLINE void zpReal::clamp( const zpReal& low, const zpReal& high ) {
	zpReal l = low;
	zpReal h = high;
	m_real = zp_clamp( m_real, l.m_real, h.m_real );
}
ZP_FORCE_INLINE void zpReal::clamp( const zpReal& real, const zpReal& low, const zpReal& high ) {
	m_real = zp_clamp( real, low, high );
}

ZP_FORCE_INLINE void zpReal::sin() {
	m_real = sinf( m_real );
}
ZP_FORCE_INLINE void zpReal::sin( const zpReal& real ) {
	m_real = sinf( real );
}
ZP_FORCE_INLINE void zpReal::asin() {
	m_real = asinf( m_real );
}
ZP_FORCE_INLINE void zpReal::asin( const zpReal& real ) {
	m_real = asinf( real );
}

ZP_FORCE_INLINE void zpReal::cos() {
	m_real = cosf( m_real );
}
ZP_FORCE_INLINE void zpReal::cos( const zpReal& real ) {
	m_real = cosf( real );
}
ZP_FORCE_INLINE void zpReal::acos() {
	m_real = acosf( m_real );
}
ZP_FORCE_INLINE void zpReal::acos( const zpReal& real ) {
	m_real = acosf( real );
}

ZP_FORCE_INLINE void zpReal::tan() {
	m_real = tanf( m_real );
}
ZP_FORCE_INLINE void zpReal::tan( const zpReal& real ) {
	m_real = tanf( real );
}
ZP_FORCE_INLINE void zpReal::atan() {
	m_real = atanf( m_real );
}
ZP_FORCE_INLINE void zpReal::atan( const zpReal& real ) {
	m_real = atanf( real );
}
*/

ZP_FORCE_INLINE zp_float zp_real_to_float( zp_real a ) {
	return a;
}
ZP_FORCE_INLINE zp_real zp_real_from_float( zp_float a ) {
	return a;
}

ZP_FORCE_INLINE zp_real zp_real_zero() {
	return 0.f;
}

ZP_FORCE_INLINE zp_real zp_real_add( zp_real a, zp_real b ) {
	return a + b;
}
ZP_FORCE_INLINE zp_real zp_real_sub( zp_real a, zp_real b ) {
	return a - b;
}
ZP_FORCE_INLINE zp_real zp_real_mul( zp_real a, zp_real b ) {
	return a * b;
}
ZP_FORCE_INLINE zp_real zp_real_div( zp_real a, zp_real b ) {
	return a / b;
}

ZP_FORCE_INLINE zp_real zp_real_neg( zp_real a ) {
	return -a;
}
ZP_FORCE_INLINE zp_real zp_real_rcp( zp_real a ) {
	return 1.f / a;
}
ZP_FORCE_INLINE zp_real zp_real_abs( zp_real a ) {
	return abs( a );
}

ZP_FORCE_INLINE zp_real zp_real_sqrt( zp_real a ) {
	return sqrtf( a );
}
ZP_FORCE_INLINE zp_real zp_real_rsqrt( zp_real a ) {
	return 1.f / sqrtf( a );
}

ZP_FORCE_INLINE zp_real zp_real_mod( zp_real a, zp_real b ) {
	return fmodf( a, b );
}

ZP_FORCE_INLINE zp_real zp_real_min( zp_real a, zp_real b ) {
	return ZP_MIN( a, b );
}
ZP_FORCE_INLINE zp_real zp_real_max( zp_real a, zp_real b ) {
	return ZP_MAX( a, b );
}

ZP_FORCE_INLINE zp_real zp_real_clamp( zp_real real, zp_real low, zp_real high ) {
	return ZP_MAX( ZP_MIN( real, high ), low );
}
ZP_FORCE_INLINE zp_real zp_real_clamp01( zp_real real ) {
	return ZP_MAX( ZP_MIN( real, 1.f ), 0.f );
}

ZP_FORCE_INLINE zp_real zp_real_sin( zp_real theta ) {
	return sinf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_cos( zp_real theta ) {
	return cosf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_tan( zp_real theta ) {
	return tanf( theta );
}

ZP_FORCE_INLINE zp_real zp_real_asin( zp_real theta ) {
	return asinf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_acos( zp_real theta ) {
	return acosf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_atan( zp_real theta ) {
	return atan( theta );
}

ZP_FORCE_INLINE zp_real zp_real_csc( zp_real theta ) {
	return 1.f / sinf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_sec( zp_real theta ) {
	return 1.f / cosf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_cot( zp_real theta ) {
	return 1.f / tanf( theta );
}

ZP_FORCE_INLINE zp_bool zp_real_eq( zp_real a, zp_real b ) {
	return a == b;
}
ZP_FORCE_INLINE zp_bool zp_real_neq( zp_real a, zp_real b ) {
	return a != b;
}

ZP_FORCE_INLINE zp_bool zp_real_lt( zp_real a, zp_real b ) {
	return a < b;
}
ZP_FORCE_INLINE zp_bool zp_real_lte( zp_real a, zp_real b ) {
	return a <= b;
}

ZP_FORCE_INLINE zp_bool zp_real_gt( zp_real a, zp_real b ) {
	return a > b;
}
ZP_FORCE_INLINE zp_bool zp_real_gte( zp_real a, zp_real b ) {
	return a >= b;
}
