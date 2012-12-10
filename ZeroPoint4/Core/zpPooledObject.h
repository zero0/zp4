#pragma once
#ifndef ZP_POOLED_OBJECT_H
#define ZP_POOLED_OBJECT_H

template<typename T, zp_uint Count>
class zpPooledObject
{
public:
	~zpPooledObject();

	static zpPooledObject<T, Count>* getInstance();

	T* aquire();
	void release( T* obj );

private:
	zpPooledObject();

	void markUsed( zp_uint index, zp_bool used );
	zp_bool isUsed( zp_uint index ) const;

	zpFlag8 m_isUsed[ Count / 8 ];
	
	T m_pool[ Count ];
	//T* m_pool;
};

#include "zpPooledObject.inl"

#endif