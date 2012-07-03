/*
ZP_FORCE_INLINE void* zpMatrix4f::operator new( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
ZP_FORCE_INLINE void* zpMatrix4f::operator new[]( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
ZP_FORCE_INLINE void zpMatrix4f::operator delete( void* ptr ) {
	_aligned_free( ptr );
}
*/
ZP_FORCE_INLINE zpMatrix4f::zpMatrix4f() :
	m_m1( 1, 0, 0, 0 ),
	m_m2( 0, 1, 0, 0 ),
	m_m3( 0, 0, 1, 0 ),
	m_m4( 0, 0, 0, 1 )
{}
#define MM( r, c )	matrix[( (r) + (c) * 4 )]
#define MM_( r, c )	( (r) + (c) * 4 )
ZP_FORCE_INLINE zpMatrix4f::zpMatrix4f( const zp_float* matrix ) :
	m_m1( MM( 0, 0 ), MM( 0, 1 ), MM( 0, 2 ), MM( 0, 3 ) ),
	m_m2( MM( 1, 0 ), MM( 1, 1 ), MM( 1, 2 ), MM( 1, 3 ) ),
	m_m3( MM( 2, 0 ), MM( 2, 1 ), MM( 2, 2 ), MM( 2, 3 ) ),
	m_m4( MM( 3, 0 ), MM( 3, 1 ), MM( 3, 2 ), MM( 3, 3 ) )
{}

ZP_FORCE_INLINE zpMatrix4f::zpMatrix4f( const zp_vec4* matrix ) : 
	m_m1( matrix[0] ),
	m_m2( matrix[1] ),
	m_m3( matrix[2] ),
	m_m4( matrix[3] )
{}
ZP_FORCE_INLINE zpMatrix4f::zpMatrix4f( const zpMatrix4f& matrix ) :
	m_m1( matrix.m_m1 ),
	m_m2( matrix.m_m2 ),
	m_m3( matrix.m_m3 ),
	m_m4( matrix.m_m4 )
{}
ZP_FORCE_INLINE zpMatrix4f::zpMatrix4f( zpMatrix4f&& matrix ) :
	m_m1( matrix.m_m1 ),
	m_m2( matrix.m_m2 ),
	m_m3( matrix.m_m3 ),
	m_m4( matrix.m_m4 )
{}
ZP_FORCE_INLINE zpMatrix4f::~zpMatrix4f() {}

ZP_FORCE_INLINE void zpMatrix4f::operator=( const zpMatrix4f& matrix ) {
	m_m1 = matrix.m_m1;
	m_m2 = matrix.m_m2;
	m_m3 = matrix.m_m3;
	m_m4 = matrix.m_m4;
}
ZP_FORCE_INLINE void zpMatrix4f::operator=( zpMatrix4f&& matrix ) {
	m_m1 = matrix.m_m1;
	m_m2 = matrix.m_m2;
	m_m3 = matrix.m_m3;
	m_m4 = matrix.m_m4;
}
ZP_FORCE_INLINE void zpMatrix4f::operator=( const zp_float* matrix ) {
	m_m1 = zp_vec4( MM( 0, 0 ), MM( 0, 1 ), MM( 0, 2 ), MM( 0, 3 ) );
	m_m2 = zp_vec4( MM( 1, 0 ), MM( 1, 1 ), MM( 1, 2 ), MM( 1, 3 ) );
	m_m3 = zp_vec4( MM( 2, 0 ), MM( 2, 1 ), MM( 2, 2 ), MM( 2, 3 ) );
	m_m4 = zp_vec4( MM( 3, 0 ), MM( 3, 1 ), MM( 3, 2 ), MM( 3, 3 ) );
}

ZP_FORCE_INLINE zp_float& zpMatrix4f::operator()( zp_uint row, zp_uint col ) {
	return m_data[ MM_( row, col ) ];
}
ZP_FORCE_INLINE zp_float zpMatrix4f::operator()( zp_uint row, zp_uint col ) const {
	return m_data[ MM_( row, col ) ];
}

ZP_FORCE_INLINE zpMatrix4f::operator zp_float*() {
	return m_data;
}
ZP_FORCE_INLINE zpMatrix4f::operator const zp_float*() const {
	return m_data;
}

ZP_FORCE_INLINE void zpMatrix4f::operator*=( const zpMatrix4f& matrix ) {
	zp_vec4 col1, col2, col3, col4;

	col1 = _mm_mul_ps( matrix.m_m1.get<0>(), m_m1 );
	col2 = _mm_mul_ps( matrix.m_m2.get<0>(), m_m1 );
	col3 = _mm_mul_ps( matrix.m_m3.get<0>(), m_m1 );
	col4 = _mm_mul_ps( matrix.m_m4.get<0>(), m_m1 );

	col1 = _mm_add_ps( col1, _mm_mul_ps( matrix.m_m1.get<1>(), m_m2 ) );
	col2 = _mm_add_ps( col2, _mm_mul_ps( matrix.m_m2.get<1>(), m_m2 ) );
	col3 = _mm_add_ps( col3, _mm_mul_ps( matrix.m_m3.get<1>(), m_m2 ) );
	col4 = _mm_add_ps( col4, _mm_mul_ps( matrix.m_m4.get<1>(), m_m2 ) );

	col1 = _mm_add_ps( col1, _mm_mul_ps( matrix.m_m1.get<2>(), m_m3 ) );
	col2 = _mm_add_ps( col2, _mm_mul_ps( matrix.m_m2.get<2>(), m_m3 ) );
	col3 = _mm_add_ps( col3, _mm_mul_ps( matrix.m_m3.get<2>(), m_m3 ) );
	col4 = _mm_add_ps( col4, _mm_mul_ps( matrix.m_m4.get<2>(), m_m3 ) );

	col1 = _mm_add_ps( col1, _mm_mul_ps( matrix.m_m1.get<3>(), m_m4 ) );
	col2 = _mm_add_ps( col2, _mm_mul_ps( matrix.m_m2.get<3>(), m_m4 ) );
	col3 = _mm_add_ps( col3, _mm_mul_ps( matrix.m_m3.get<3>(), m_m4 ) );
	col4 = _mm_add_ps( col4, _mm_mul_ps( matrix.m_m4.get<3>(), m_m4 ) );

	m_m1 = col1;
	m_m2 = col2;
	m_m3 = col3;
	m_m4 = col4;
}

ZP_FORCE_INLINE void zpMatrix4f::translate( const zpVector4f& position ) {
	zp_real col, x, y, z, w;

	x = position.getX();
	y = position.getY();
	z = position.getZ();
	w = position.getW();

	col = _mm_mul_ps( x, m_m1 );
	col = _mm_add_ps( col, _mm_mul_ps( y, m_m2 ) );
	col = _mm_add_ps( col, _mm_mul_ps( z, m_m3 ) );
	col = _mm_add_ps( col, _mm_mul_ps( w, m_m4 ) );

	m_m4 = col;
}
ZP_FORCE_INLINE void zpMatrix4f::rotate( const zpVector4f& axis, zp_float angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::rotateX( zp_float angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::rotateY( zp_float angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::rotateZ( zp_float angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::scale( zp_float uniformScale ) {}
ZP_FORCE_INLINE void zpMatrix4f::scale( const zpVector4f& scale ) {}

ZP_FORCE_INLINE void zpMatrix4f::mul( const zpVector4f& vector, zpVector4f* outVector ) const {}

ZP_FORCE_INLINE void zpMatrix4f::lookAt( const zpVector4f& eye, const zpVector4f& point, const zpVector4f& up ) {}
ZP_FORCE_INLINE void zpMatrix4f::perspective( zp_float fovy, zp_float aspect, zp_float nearDistance, zp_float farDistance ) {}
ZP_FORCE_INLINE void zpMatrix4f::ortho( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance ) {}
ZP_FORCE_INLINE void zpMatrix4f::orthoOffset( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance ) {}
ZP_FORCE_INLINE void zpMatrix4f::frustum( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance ) {}

ZP_FORCE_INLINE zp_bool zpMatrix4f::operator==( const zpMatrix4f& matrix ) const {
	for( zp_uint i = 16; i --> 0; ) if( m_data[i] != matrix.m_data[i] ) return false;
	return true;
}
ZP_FORCE_INLINE zp_bool zpMatrix4f::operator!=( const zpMatrix4f& matrix ) const {
	for( zp_uint i = 16; i --> 0; ) if( m_data[i] == matrix.m_data[i] ) return false;
	return true;
}
#undef MM_
#undef MM