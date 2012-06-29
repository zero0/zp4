#include <math.h>

ZP_FORCE_INLINE zpReal::zpReal() : m_real( 0 ) {}
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

ZP_FORCE_INLINE void zpReal::cos() {
	m_real = cosf( m_real );
}
ZP_FORCE_INLINE void zpReal::cos( const zpReal& real ) {
	m_real = cosf( real );
}

ZP_FORCE_INLINE void zpReal::tan() {
	m_real = tanf( m_real );
}
ZP_FORCE_INLINE void zpReal::tan( const zpReal& real ) {
	m_real = tanf( real );
}