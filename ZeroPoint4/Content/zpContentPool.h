#pragma once
#ifndef ZP_CONTENT_POOL_H
#define ZP_CONTENT_POOL_H

template<typename T, zp_uint Count>
class zpContentPool
{
public:
	zpContentPool();
	~zpContentPool();

	template<typename R>
	T* create( const R& param );

	template<typename R0, typename R1>
	T* create( const R0& param0, const R1& param1 );

	void destroy( T* obj );

	zp_uint size() const;

protected:
	zp_byte m_pool[ Count * sizeof( T ) ];

	zpFixedArrayList< T*, Count > m_used;
	zpFixedArrayList< T*, Count > m_free;
};

#include "zpContentPool.inl"

#endif
