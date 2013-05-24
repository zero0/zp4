#pragma once
#ifndef ZP_POOLED_OBJECT_H
#define ZP_POOLED_OBJECT_H

template<typename T, zp_uint Count>
class zpPooledObject
{
public:
	~zpPooledObject();

	T* aquire();
	void release( T* obj );

	static zpPooledObject<T, Count>* getInstance();

private:
	zpPooledObject();

	void markUsed( zp_uint index, zp_bool used );
	zp_bool isUsed( zp_uint index ) const;

	zpFlag8 m_isUsed[ Count / 8 ];
	zp_byte m_pool[ Count * sizeof( T ) ];
};

#include "zpPooledObject.inl"

#endif