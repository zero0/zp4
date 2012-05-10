
ZP_FORCE_INLINE zpVector4::zpVector4() {
	m_xyzw.x = 0;
	m_xyzw.y = 0;
	m_xyzw.z = 0;
	m_xyzw.w = 0;
}
ZP_FORCE_INLINE zpVector4::zpVector4( zp_float x, zp_float y, zp_float z, zp_float w ) {
	m_xyzw.x = x;
	m_xyzw.y = y;
	m_xyzw.z = z;
	m_xyzw.w = w;
}
ZP_FORCE_INLINE zpVector4::zpVector4( const zpVector4& vector ) : m_xyzw( vector.m_xyzw ) {}
ZP_FORCE_INLINE zpVector4::zpVector4( zpVector4&& vector ) : m_xyzw( vector.m_xyzw ) {}
ZP_FORCE_INLINE zpVector4::zpVector4( const zp_vec4& vector ) : m_xyzw( vector ) {}
ZP_FORCE_INLINE zpVector4::~zpVector4() {}
