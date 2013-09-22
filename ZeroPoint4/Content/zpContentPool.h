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
	void destroy( T* obj );

	zp_uint size() const;

protected:
	void markUsed( zp_uint index, zp_bool used );
	zp_bool isUsed( zp_uint index ) const;
	T* at( zp_uint index );

	zpFlag8 m_isUsed[ ( Count / 8 ) + 1 ];
	zp_byte m_pool[ Count * sizeof( T ) ];
};

#include "zpContentPool.inl"

#endif
