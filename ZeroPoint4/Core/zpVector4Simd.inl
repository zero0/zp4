/*
ZP_FORCE_INLINE void* zpVector4f::operator new( zp_uint size ) {
    return _aligned_malloc( size, 16 );
};
ZP_FORCE_INLINE void* zpVector4f::operator new[]( zp_uint size ) {
    return _aligned_malloc( size, 16 );
}
ZP_FORCE_INLINE void zpVector4f::operator delete( void* ptr ) {
    _aligned_free( ptr );
}
*/
#if 0

ZP_FORCE_INLINE void zpVector4f::store3( zp_float* xyz ) const
{
    const __m128& q = m_xyzw;
    xyz[0] = q.m128_f32[0];
    xyz[1] = q.m128_f32[1];
    xyz[2] = q.m128_f32[2];
}
ZP_FORCE_INLINE void zpVector4f::store4( zp_float* xyzw ) const
{
    const __m128& q = m_xyzw;
    xyzw[0] = q.m128_f32[0];
    xyzw[1] = q.m128_f32[1];
    xyzw[2] = q.m128_f32[2];
    xyzw[3] = q.m128_f32[3];
}

ZP_FORCE_INLINE void zpVector4f::load3( const zp_float* xyz )
{
    const __m128& q = m_xyzw;
    m_xyzw = zp_vec4( xyz[0], xyz[1], xyz[2], q.m128_f32[3] );
}
ZP_FORCE_INLINE void zpVector4f::load4( const zp_float* xyzw )
{
    m_xyzw = zp_vec4( xyzw[0], xyzw[1], xyzw[2], xyzw[3] );
}


ZP_FORCE_INLINE zpVector4f::zpVector4f( zp_float x, zp_float y, zp_float z, zp_float w ) :
    m_xyzw( x, y, z, w )
{}
    
ZP_FORCE_INLINE zpVector4f zpVector4f::operator-() const {
    return zpVector4f( zpScalarNeg( m_xyzw ) );
}

ZP_FORCE_INLINE void zpVector4f::set3( const zpScalar& x, const zpScalar& y, const zpScalar& z ) {
    m_xyzw = _mm_set_ps( 
        zpScalarToFloat( x ),
        zpScalarToFloat( y ), 
        zpScalarToFloat( z ), 
        m_xyzw.v.m128_f32[3] 
    );
}
ZP_FORCE_INLINE void zpVector4f::set4( const zpScalar& x, const zpScalar& y, const zpScalar& z, const zpScalar& w ) {
    m_xyzw = _mm_set_ps( 
        zpScalarToFloat( x ),
        zpScalarToFloat( y ), 
        zpScalarToFloat( z ), 
        zpScalarToFloat( w )
    );
}
ZP_FORCE_INLINE void zpVector4f::set3( const zpVector4f& vector ) {
    m_xyzw.v.m128_f32[0] = vector.m_xyzw.v.m128_f32[0];
    m_xyzw.v.m128_f32[1] = vector.m_xyzw.v.m128_f32[1];
    m_xyzw.v.m128_f32[2] = vector.m_xyzw.v.m128_f32[2];
}
ZP_FORCE_INLINE void zpVector4f::set4( const zpVector4f& vector ) {
    m_xyzw = vector.m_xyzw;
}

ZP_FORCE_INLINE void zpVector4f::setX( const zpScalar& x ) {
    m_xyzw.v.m128_f32[0] = zpScalarToFloat( x );
}
ZP_FORCE_INLINE void zpVector4f::setY( const zpScalar& y ) {
    m_xyzw.v.m128_f32[1] = zpScalarToFloat( y );
}
ZP_FORCE_INLINE void zpVector4f::setZ( const zpScalar& z ) {
    m_xyzw.v.m128_f32[2] = zpScalarToFloat( z );
}
ZP_FORCE_INLINE void zpVector4f::setW( const zpScalar& w ) {
    m_xyzw.v.m128_f32[3] = zpScalarToFloat( w );
}

ZP_FORCE_INLINE void zpVector4f::store3( zp_float* xyz ) const {
    xyz[0] = m_xyzw.v.m128_f32[0];
    xyz[1] = m_xyzw.v.m128_f32[1];
    xyz[2] = m_xyzw.v.m128_f32[2];
}
ZP_FORCE_INLINE void zpVector4f::store4( zp_float* xyzw ) const {
    xyzw[0] = m_xyzw.v.m128_f32[0];
    xyzw[1] = m_xyzw.v.m128_f32[1];
    xyzw[2] = m_xyzw.v.m128_f32[2];
    xyzw[3] = m_xyzw.v.m128_f32[3];
}

ZP_FORCE_INLINE void zpVector4f::load3( const zp_float* xyz ) {
    m_xyzw.v.m128_f32[0] = xyz[0];
    m_xyzw.v.m128_f32[1] = xyz[1];
    m_xyzw.v.m128_f32[2] = xyz[2];
}
ZP_FORCE_INLINE void zpVector4f::load4( const zp_float* xyzw ) {
    m_xyzw.v.m128_f32[0] = xyzw[0];
    m_xyzw.v.m128_f32[1] = xyzw[1];
    m_xyzw.v.m128_f32[2] = xyzw[2];
    m_xyzw.v.m128_f32[3] = xyzw[3];
}

ZP_FORCE_INLINE void zpVector4f::mul3( const zpVector4f& vector ) {
    zp_float w = m_xyzw.v.m128_f32[3];
    m_xyzw = _mm_mul_ps( m_xyzw, vector.m_xyzw );
    m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::mul4( const zpVector4f& vector ) {
    m_xyzw = _mm_mul_ps( m_xyzw, vector.m_xyzw );
}
ZP_FORCE_INLINE void zpVector4f::div3( const zpVector4f& vector ) {
    zp_float w = m_xyzw.v.m128_f32[3];
    m_xyzw = _mm_div_ps( m_xyzw, vector.m_xyzw );
    m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::div4( const zpVector4f& vector ) {
    m_xyzw = _mm_div_ps( m_xyzw, vector.m_xyzw );
}
ZP_FORCE_INLINE void zpVector4f::add3( const zpVector4f& vector ) {
    zp_float w = m_xyzw.v.m128_f32[3];
    m_xyzw = _mm_add_ps( m_xyzw, vector.m_xyzw );
    m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::add4( const zpVector4f& vector ) {
    m_xyzw = _mm_add_ps( m_xyzw, vector.m_xyzw );
}
ZP_FORCE_INLINE void zpVector4f::sub3( const zpVector4f& vector ) {
    zp_float w = m_xyzw.v.m128_f32[3];
    m_xyzw = _mm_sub_ps( m_xyzw, vector.m_xyzw );
    m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::sub4( const zpVector4f& vector ) {
    m_xyzw = _mm_sub_ps( m_xyzw, vector.m_xyzw );
}

ZP_FORCE_INLINE void zpVector4f::mul3( const zpScalar& r ) {
    zp_float w = m_xyzw.v.m128_f32[3];
    m_xyzw = _mm_mul_ps( m_xyzw, r );
    m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::mul4( const zpScalar& r ) {
    m_xyzw = _mm_mul_ps( m_xyzw, r );
}
ZP_FORCE_INLINE void zpVector4f::div3( const zpScalar& r ) {
    zp_float w = m_xyzw.v.m128_f32[3];
    m_xyzw = _mm_div_ps( m_xyzw, r );
    m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::div4( const zpScalar& r ) {
    m_xyzw = _mm_div_ps( m_xyzw, r );
}
ZP_FORCE_INLINE void zpVector4f::add3( const zpScalar& r ) {
    zp_float w = m_xyzw.v.m128_f32[3];
    m_xyzw = _mm_add_ps( m_xyzw, r );
    m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::add4( const zpScalar& r ) {
    m_xyzw = _mm_add_ps( m_xyzw, r );
}
ZP_FORCE_INLINE void zpVector4f::sub3( const zpScalar& r ) {
    zp_float w = m_xyzw.v.m128_f32[3];
    m_xyzw = _mm_sub_ps( m_xyzw, r );
    m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::sub4( const zpScalar& r ) {
    m_xyzw = _mm_sub_ps( m_xyzw, r );
}

ZP_FORCE_INLINE void zpVector4f::zero3() {
    zp_float w = m_xyzw.v.m128_f32[3];
    m_xyzw = _mm_setzero_ps();
    m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::zero4() {
    m_xyzw = _mm_setzero_ps();
}
ZP_FORCE_INLINE zp_bool zpVector4f::isZero3() const {
    zpScalar zero = _mm_setzero_ps();
    zpScalar cmp = _mm_cmpeq_ps( m_xyzw, zero );
    return cmp.m128_i32[0] && cmp.m128_i32[1] && cmp.m128_i32[2];
}
ZP_FORCE_INLINE zp_bool zpVector4f::isZero4() const {
    zpScalar zero = _mm_setzero_ps();
    zpScalar cmp = _mm_cmpeq_ps( m_xyzw, zero );
    return cmp.m128_i32[0] && cmp.m128_i32[1] && cmp.m128_i32[2] && cmp.m128_i32[3];
}

ZP_FORCE_INLINE zpScalar zpVector4f::dot3( const zpVector4f& vector ) const {
    zpScalar m = _mm_mul_ps( m_xyzw, vector.m_xyzw );
    zpScalar x = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 0, 0, 0, 0 ) );
    zpScalar y = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 1, 1, 1, 1 ) );
    zpScalar z = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 2, 2, 2, 2 ) );
    return _mm_add_ps( x, _mm_add_ps( y, z ) );
}
ZP_FORCE_INLINE zpScalar zpVector4f::dot4( const zpVector4f& vector ) const {
    zpScalar t0 = _mm_mul_ps( m_xyzw, vector.m_xyzw );
    zpScalar t1 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 1, 0, 3, 2 ) );
    zpScalar t2 = _mm_add_ps( t0, t1 );
    zp_vec4 t3 = _mm_shuffle_ps( t2, t2, _MM_SHUFFLE( 2, 3, 0, 1 ) );
    return _mm_add_ps( t3, t2 );
}

ZP_FORCE_INLINE zpVector4f zpVector4f::cross3( const zpVector4f& vector ) const {
    zpScalar s0 = _mm_shuffle_ps( m_xyzw, m_xyzw, _MM_SHUFFLE( 3, 0, 2, 1 ) );
    zpScalar s1 = _mm_shuffle_ps( vector.m_xyzw, vector.m_xyzw, _MM_SHUFFLE( 3, 1, 0, 2 ) );
    zpScalar s2 = _mm_mul_ps( s0, s1 );

    zpScalar s3 = _mm_shuffle_ps( m_xyzw, m_xyzw, _MM_SHUFFLE( 3, 1, 0, 2 ) );
    zpScalar s4 = _mm_shuffle_ps( vector.m_xyzw, vector.m_xyzw, _MM_SHUFFLE( 3, 0, 2, 1 ) );
    zpScalar s5 = _mm_mul_ps( s3, s4 );

    return zpVector4f( _mm_sub_ps( s2, s5 ) );
}
ZP_FORCE_INLINE zpVector4f zpVector4f::cross4( const zpVector4f& vector ) const {
    // hmmm...
    return zpVector4f();
}

ZP_FORCE_INLINE void zpVector4f::abs3() {
    zp_float w = m_xyzw.v.m128_f32[3];
    m_xyzw = _mm_and_ps( m_xyzw, *(zp_vec4*)&_mm_set1_epi32( 0x7fffffff ) );
    m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::abs4() {
    m_xyzw = _mm_and_ps( m_xyzw, *(zp_vec4*)&_mm_set1_epi32( 0x7fffffff ) );
}
ZP_FORCE_INLINE zpVector4f zpVector4f::abs3() const {
    zp_float w = m_xyzw.v.m128_f32[3];
    zp_vec4 v = _mm_and_ps( m_xyzw, *(zp_vec4*)&_mm_set1_epi32( 0x7fffffff ) );
    v.v.m128_f32[3] = w;

    return zpVector4f( v );
}
ZP_FORCE_INLINE zpVector4f zpVector4f::abs4() const {
    return zpVector4f( _mm_and_ps( m_xyzw, *(zp_vec4*)&_mm_set1_epi32( 0x7fffffff ) ) );
}

ZP_FORCE_INLINE zp_bool zpVector4f::equals3( const zpVector4f& vector ) const {
    zpScalar cmp = _mm_cmpeq_ps( m_xyzw, vector.m_xyzw );
    return cmp.m128_i32[0] && cmp.m128_i32[1] && cmp.m128_i32[2];
}
ZP_FORCE_INLINE zp_bool zpVector4f::equals4( const zpVector4f& vector ) const {
    zpScalar cmp = _mm_cmpeq_ps( m_xyzw, vector.m_xyzw );
    return cmp.m128_i32[0] && cmp.m128_i32[1] && cmp.m128_i32[2] && cmp.m128_i32[3];
}

ZP_FORCE_INLINE zp_bool zpVector4f::operator==( const zpVector4f& vector ) const {
    return equals4( vector );
}
ZP_FORCE_INLINE zp_bool zpVector4f::operator!=( const zpVector4f& vector ) const {
    zpScalar cmp = _mm_cmpneq_ps( m_xyzw, vector.m_xyzw );
    return cmp.m128_i32[0] && cmp.m128_i32[1] && cmp.m128_i32[2] && cmp.m128_i32[3];
}
#endif
