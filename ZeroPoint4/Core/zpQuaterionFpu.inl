
ZP_FORCE_INLINE void zpQuaternion4f::store3( zp_float* xyz ) const {
	xyz[0] = m_xyzw.x;
	xyz[1] = m_xyzw.y;
	xyz[2] = m_xyzw.z;
}
ZP_FORCE_INLINE void zpQuaternion4f::store4( zp_float* xyzw ) const {
	xyzw[0] = m_xyzw.x;
	xyzw[1] = m_xyzw.y;
	xyzw[2] = m_xyzw.z;
	xyzw[3] = m_xyzw.w;
}

ZP_FORCE_INLINE void zpQuaternion4f::load3( const zp_float* xyz ) {
	m_xyzw.x = xyz[0];
	m_xyzw.y = xyz[1];
	m_xyzw.z = xyz[2];
}
ZP_FORCE_INLINE void zpQuaternion4f::load4( const zp_float* xyzw ) {
	m_xyzw.x = xyzw[0];
	m_xyzw.y = xyzw[1];
	m_xyzw.z = xyzw[2];
	m_xyzw.w = xyzw[3];
}
