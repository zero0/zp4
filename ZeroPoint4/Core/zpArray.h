#pragma once
#ifndef ZP_ARRAY_H
#define ZP_ARRAY_H

template<typename T, zp_uint Size>
class zpArray {
public:
	zpArray();
	zpArray( T (&arr)[ Size ] );
	zpArray( T* data, zp_uint count );
	zpArray( const zpArray& arr );
	zpArray( zpArray&& arr );
	~zpArray();

	void operator=( T (&arr)[ Size ] );
	void operator=( const zpArray& arr );
	void operator=( zpArray&& arr );

	T& operator[]( zp_uint index );
	const T& operator[]( zp_uint index ) const;

	T* data();
	const T* data() const;

	const T& at( zp_uint index ) const;

	zp_uint size() const;

	void remove( zp_uint index, T* outVal = ZP_NULL );
	void erase( zp_uint index );

	void clear();

	zp_uint indexOf( const T& val ) const;
	zp_uint lastIndexOf( const T& val ) const;

	void fill( const T* vals, zp_uint size, zp_uint startIndex = 0 );

	template<typename Func>
	void foreach( Func func ) const;
	template<typename Func>
	void foreachIf( Func func ) const;
	template<typename Func>
	void foreachInRange( zp_uint start, zp_int count, Func func ) const;

	template<typename Func>
	void foreachIndexed( Func func ) const;
	template<typename Func>
	void foreachIndexedIf( Func func ) const;

	template<typename Func>
	void map( Func func );
	
	
	static const zp_uint npos;

private:
	T m_array[ Size ];
};

#include "zpArray.inl"

#endif