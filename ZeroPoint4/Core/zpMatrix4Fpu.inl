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

ZP_FORCE_INLINE void zpMatrix4f::operator*=( const zpMatrix4f& matrix ) {
	float sum;
	int x, y, j;
	float mat[4][4];

	for( x = 0; x < 4; ++x ) {
		for( y = 0; y < 4; ++y ) {
			sum = 0;

			for( j = 0; j < 4; ++j ) {
				//sum += m_matrix[x][j] * matrix.m_matrix[j][y];
				sum += m_matrix[j][x] * matrix.m_matrix[y][j];
			}

			mat[x][y] = sum;
		}
	}
	(*this) = &mat[0][0];
}

ZP_FORCE_INLINE void zpMatrix4f::translate( const zpVector4f& position ) {
	zp_vec4 v = position;
	m_41 = m_11 * v.x + m_21 * v.y + m_31 * v.z + m_41;
	m_42 = m_12 * v.x + m_22 * v.y + m_32 * v.z + m_42;
	m_43 = m_13 * v.x + m_23 * v.y + m_33 * v.z + m_43;
	m_44 = m_14 * v.x + m_24 * v.y + m_34 * v.z + m_44;
}
ZP_FORCE_INLINE void zpMatrix4f::rotate( const zpVector4f& axis, zp_float angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::rotateX( zp_real angle ) {
	//left handed
	zp_real c = zp_real_cos( angle );
	zp_real s = zp_real_sin( angle );

	zpMatrix4f rotX;
	rotX.m_22 = c; rotX.m_23 = -s;
	rotX.m_32 = s; rotX.m_33 = c;

	(*this) = rotX * (*this);
}
ZP_FORCE_INLINE void zpMatrix4f::rotateY( zp_real angle ) {
	//left handed
	zp_real c = zp_real_cos( angle );
	zp_real s = zp_real_sin( angle );

	zpMatrix4f rotY;
	rotY.m_11 = c; rotY.m_13 = s;
	rotY.m_31 = -s; rotY.m_33 = c;

	(*this) = rotY * (*this);
}
ZP_FORCE_INLINE void zpMatrix4f::rotateZ( zp_real angle ) {
	//left handed
	zp_real c = zp_real_cos( angle );
	zp_real s = zp_real_sin( angle );

	zpMatrix4f rotZ;
	rotZ.m_11 = c; rotZ.m_12 = -s;
	rotZ.m_21 = s; rotZ.m_22 = c;

	(*this) = rotZ * (*this);
}
ZP_FORCE_INLINE void zpMatrix4f::scale( zp_float uniformScale ) {}
ZP_FORCE_INLINE void zpMatrix4f::scale( const zpVector4f& scale ) {
	zpMatrix4f m;
	m( 0, 0 ) = scale.getX();
	m( 1, 1 ) = scale.getY();
	m( 2, 2 ) = scale.getZ();

	(*this) = m * (*this);
}

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
//ZP_FORCE_INLINE void zpMatrix4f::getRotation( zpVector4f* rotation ) const;
//ZP_FORCE_INLINE void zpMatrix4f::getScale( zpVector4f* rotation ) const;

#undef MM_
#undef MM