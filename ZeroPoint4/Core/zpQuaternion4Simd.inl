
#if 0
ZP_FORCE_INLINE void zpQuaternion4f::store3( zp_float* xyz ) const
{
	const __m128& q = m_xyzw;
	xyz[0] = q.m128_f32[0];
	xyz[1] = q.m128_f32[1];
	xyz[2] = q.m128_f32[2];
}
ZP_FORCE_INLINE void zpQuaternion4f::store4( zp_float* xyzw ) const
{
	const __m128& q = m_xyzw;
	xyzw[0] = q.m128_f32[0];
	xyzw[1] = q.m128_f32[1];
	xyzw[2] = q.m128_f32[2];
	xyzw[3] = q.m128_f32[3];
}

ZP_FORCE_INLINE void zpQuaternion4f::load3( const zp_float* xyz )
{
	const __m128& q = m_xyzw;
	m_xyzw = zp_vec4( xyz[0], xyz[1], xyz[2], q.m128_f32[3] );
}
ZP_FORCE_INLINE void zpQuaternion4f::load4( const zp_float* xyzw )
{
	m_xyzw = zp_vec4( xyzw[0], xyzw[1], xyzw[2], xyzw[3] );
}
#endif
