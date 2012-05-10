
zpVector4::zpVector4() : m_xyzw() {}
zpVector4::zpVector4( zp_float x, zp_float y, zp_float z, zp_float w ) {}
zpVector4::zpVector4( const zpVector4& vector ) : m_xyzw( vector.m_xyzw ) {}
zpVector4::zpVector4( zpVector4&& vector ) : m_xyzw( vector.m_xyzw ) {}
zpVector4::zpVector4( const zp_vec4& vector ) : m_xyzw( vector ) {}
zpVector4::~zpVector4() {}
