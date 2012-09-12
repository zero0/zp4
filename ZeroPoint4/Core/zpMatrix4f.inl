
#define MM( r, c )		matrix[( (r) + (c) * 4 )]
#define MM_( r, c )		( (r) + (c) * 4 )
//#define MM( r, c )		matrix[( (r) * 4 + (c) )]
//#define MM_( r, c )		( (r) * 4 + (c) )

ZP_FORCE_INLINE zpMatrix4f::zpMatrix4f() :
	m_m1( 1, 0, 0, 0 ),
	m_m2( 0, 1, 0, 0 ),
	m_m3( 0, 0, 1, 0 ),
	m_m4( 0, 0, 0, 1 )
{}
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

ZP_FORCE_INLINE void zpMatrix4f::getPosition( zpVector4f& position ) const {
	position = m_m4;
}

ZP_FORCE_INLINE void zpMatrix4f::setPosition( const zpVector4f& position ) {
	m_m4 = position.toVec4();
}

ZP_FORCE_INLINE zp_bool zpMatrix4f::isIdentity() const {
	return (*this) == identity;
}

#undef MM_
#undef MM