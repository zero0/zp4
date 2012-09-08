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
	zp_float sum;
	zp_int x, y, j;
	zp_float mat[4][4];

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
	zp_vec4 v = position.toVec4();
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
ZP_FORCE_INLINE void zpMatrix4f::scale( zp_float uniformScale ) {
	zpMatrix4f m;
	m( 0, 0 ) = uniformScale;
	m( 1, 1 ) = uniformScale;
	m( 2, 2 ) = uniformScale;

	(*this) = m * (*this);
}
ZP_FORCE_INLINE void zpMatrix4f::scale( const zpVector4f& scale ) {
	zpMatrix4f m;
	m( 0, 0 ) = scale.getX();
	m( 1, 1 ) = scale.getY();
	m( 2, 2 ) = scale.getZ();

	(*this) = m * (*this);
}

ZP_FORCE_INLINE void zpMatrix4f::mul( const zpVector4f& vector, zpVector4f* outVector ) const {}

ZP_FORCE_INLINE void zpMatrix4f::lookAt( const zpVector4f& eye, const zpVector4f& at, const zpVector4f& up ) {
	if( eye == at ) {
		(*this) = identity;
		return;
	}

	lookTo( eye, at - eye, up );
}
ZP_FORCE_INLINE void zpMatrix4f::lookTo( const zpVector4f& eye, const zpVector4f& direction, const zpVector4f& up ) {
	zpVector4f z( direction );
	z.normalize3();

	zpVector4f x( up );
	x = x.cross3( z );
	x.normalize3();

	zpVector4f y( z );
	y = y.cross3( x );

	zpVector4f e( -eye );

	// after transpose
	m_11 = -x.getX();	m_12 = y.getX();	m_13 = z.getX();	m_14 = x.dot3( e );
	m_21 = -x.getY();	m_22 = y.getY();	m_23 = z.getY();	m_24 = y.dot3( e );
	m_31 = -x.getZ();	m_32 = y.getZ();	m_33 = z.getZ();	m_34 = z.dot3( e );
	m_41 = 0;			m_42 = 0;			m_43 = 0;			m_44 = 1;
}
ZP_FORCE_INLINE void zpMatrix4f::perspective( zp_float fovy, zp_float aspect, zp_float nearDistance, zp_float farDistance ) {
	zp_float f = 1.f / zp_tan( ZP_DEG_TO_RAD( fovy * .5f ) );
	zp_float fRange = ( farDistance ) / ( farDistance - nearDistance );

	m_m1 = zp_vec4( f / aspect, 0, 0, 0 );
	m_m2 = zp_vec4( 0, f, 0, 0 );
	m_m3 = zp_vec4( 0, 0, -( farDistance + nearDistance ) / ( nearDistance - farDistance ), 2.f * nearDistance * farDistance / ( nearDistance - farDistance ) );
	m_m4 = zp_vec4( 0, 0, 1, 0 );
}
ZP_FORCE_INLINE void zpMatrix4f::ortho( zp_float width, zp_float height, zp_float nearDistance, zp_float farDistance ) {
	zp_float fn = farDistance - nearDistance;
	zp_float pfn = farDistance + nearDistance;
	zp_float nf = nearDistance - farDistance;

	m_11 = 2.f / width;	m_12 = 0;			m_13 = 0;			m_14 = 0;
	m_21 = 0;			m_22 = 2.f / height;m_23 = 0;			m_24 = 0;
	m_31 = 0;			m_32 = 0;			m_33 = -2.f / fn;	m_34 = 0;
	m_41 = 0;			m_42 = 0;			m_43 = -(pfn / fn);	m_44 = 1;
}
ZP_FORCE_INLINE void zpMatrix4f::orthoOffset( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance ) {
	m_11 = 2.f / ( right - left );	m_12 = 0;			m_13 = 0;			m_14 = 0;
	m_21 = 0;			m_22 = 2.f / ( top - bottom );m_23 = 0;			m_24 = 0;
	m_31 = 0;			m_32 = 0;			m_33 = 2.f / ( farDistance - nearDistance );	m_34 = 0;
	m_41 = ( left + right ) / ( left - right );			m_42 = ( top + bottom ) / ( bottom - top );			m_43 = nearDistance / ( nearDistance - farDistance );	m_44 = 1;
}
ZP_FORCE_INLINE void zpMatrix4f::frustum( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance ) {}

ZP_FORCE_INLINE void zpMatrix4f::transpose() {
	zp_float m[16] = {
		m_11, m_21, m_31, m_41,
		m_12, m_22, m_32, m_42,
		m_13, m_23, m_33, m_43,
		m_14, m_24, m_34, m_44
	};
	(*this) = m;
}
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