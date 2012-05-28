/*
ZP_FORCE_INLINE void* zpMatrix4::operator new( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
ZP_FORCE_INLINE void* zpMatrix4::operator new[]( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
ZP_FORCE_INLINE void zpMatrix4::operator delete( void* ptr ) {
	_aligned_free( ptr );
}
*/
ZP_FORCE_INLINE zpMatrix4::zpMatrix4() :
	m_m1( 1, 0, 0, 0 ),
	m_m2( 0, 1, 0, 0 ),
	m_m3( 0, 0, 1, 0 ),
	m_m4( 0, 0, 0, 1 )
{}
#define MM( r, c )	matrix[( (r) + (c) * 4 )]
#define MM_( r, c )	( (r) + (c) * 4 )
ZP_FORCE_INLINE zpMatrix4::zpMatrix4( const zp_float* matrix ) :
	m_m1( MM( 0, 0 ), MM( 0, 1 ), MM( 0, 2 ), MM( 0, 3 ) ),
	m_m2( MM( 1, 0 ), MM( 1, 1 ), MM( 1, 2 ), MM( 1, 3 ) ),
	m_m3( MM( 2, 0 ), MM( 2, 1 ), MM( 2, 2 ), MM( 2, 3 ) ),
	m_m4( MM( 3, 0 ), MM( 3, 1 ), MM( 3, 2 ), MM( 3, 3 ) )
{}

ZP_FORCE_INLINE zpMatrix4::zpMatrix4( const zp_vec4* matrix ) :
	m_m1( matrix[0] ),
	m_m2( matrix[1] ),
	m_m3( matrix[2] ),
	m_m4( matrix[3] )
{}
ZP_FORCE_INLINE zpMatrix4::zpMatrix4( const zpMatrix4& matrix ) :
	m_m1( matrix.m_m1 ),
	m_m2( matrix.m_m2 ),
	m_m3( matrix.m_m3 ),
	m_m4( matrix.m_m4 )
{}
ZP_FORCE_INLINE zpMatrix4::zpMatrix4( zpMatrix4&& matrix ) :
	m_m1( matrix.m_m1 ),
	m_m2( matrix.m_m2 ),
	m_m3( matrix.m_m3 ),
	m_m4( matrix.m_m4 )
{}
ZP_FORCE_INLINE zpMatrix4::~zpMatrix4() {}

ZP_FORCE_INLINE zpMatrix4& zpMatrix4::operator=( const zpMatrix4& matrix ) {
	m_m1 = matrix.m_m1;
	m_m2 = matrix.m_m2;
	m_m3 = matrix.m_m3;
	m_m4 = matrix.m_m4;
}
ZP_FORCE_INLINE zpMatrix4& zpMatrix4::operator=( zpMatrix4&& matrix ) {
	m_m1 = matrix.m_m1;
	m_m2 = matrix.m_m2;
	m_m3 = matrix.m_m3;
	m_m4 = matrix.m_m4;
}
ZP_FORCE_INLINE zpMatrix4& zpMatrix4::operator=( const zp_float* matrix ) {
	m_m1 = zp_vec4( MM( 0, 0 ), MM( 0, 1 ), MM( 0, 2 ), MM( 0, 3 ) );
	m_m2 = zp_vec4( MM( 1, 0 ), MM( 1, 1 ), MM( 1, 2 ), MM( 1, 3 ) );
	m_m3 = zp_vec4( MM( 2, 0 ), MM( 2, 1 ), MM( 2, 2 ), MM( 2, 3 ) );
	m_m4 = zp_vec4( MM( 3, 0 ), MM( 3, 1 ), MM( 3, 2 ), MM( 3, 3 ) );

	return (*this);
}

ZP_FORCE_INLINE zp_float& zpMatrix4::operator()( zp_uint row, zp_uint col ) {
	return m_data[ MM_( row, col ) ];
}
ZP_FORCE_INLINE zp_float zpMatrix4::operator()( zp_uint row, zp_uint col ) const {
	return m_data[ MM_( row, col ) ];
}

ZP_FORCE_INLINE zpMatrix4::operator zp_float*() {
	return m_data;
}
ZP_FORCE_INLINE zpMatrix4::operator const zp_float*() const {
	return m_data;
}

ZP_FORCE_INLINE void zpMatrix4::operator*=( const zpMatrix4& matrix ) {
	float sum;
	int x, y, j;
	float mat[4][4];

	for( x = 0; x < 4; ++x ) {
		for( y = 0; y < 4; ++y ) {
			sum = 0;

			for( j = 0; j < 4; ++j ) {
				//sum += m_matrix[x][j] * matrix.m_matrix[j][y];
				sum += m_data[ j + x * 4 ] * matrix.m_data[ y + j * 4 ];
			}

			mat[x][y] = sum;
		}
	}
	(*this) = &mat[0][0];
}

#undef MM_
#undef MM