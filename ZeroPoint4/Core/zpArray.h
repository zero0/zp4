#pragma once
#ifndef ZP_ARRAY_H
#define ZP_ARRAY_H

template<typename T>
class zpArray {
public:
	zpArray();
	zpArray( zp_uint size );
	zpArray( const zpArray& arr );
	zpArray( zpArray&& arr );
	~zpArray();

	void operator=( const zpArray& arr );
	void operator=( zpArray&& arr );

	T& operator[]( zp_uint index );
	const T& operator[]( zp_uint index ) const;

	operator T*();
	operator const T*() const;

	const T& at( zp_uint index ) const;

	zp_uint size() const;

	void remove( zp_uint index, T* outVal = ZP_NULL );
	void erase( zp_uint index );

	void clear();

	zp_uint indexOf( const T& val ) const;
	zp_uint lastIndexOf( const T& val ) const;

	template<typename Func>
	void foreach( Func func ) const;
	template<typename Func>
	void foreachIf( Func func ) const;

	template<typename Func>
	void foreachIndexed( Func func ) const;
	template<typename Func>
	void foreachIndexedIf( Func func ) const;

	template<typename Func>
	void map( Func func ) const;
	
	
	static const zp_uint npos;

private:
	T* m_array;
	zp_uint m_size;
};

#include "zpArray.inl"

#endif