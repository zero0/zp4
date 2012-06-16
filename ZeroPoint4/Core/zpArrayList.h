#pragma once
#ifndef ZP_ARRAY_LIST_H
#define ZP_ARRAY_LIST_H

#define ZP_ARRAY_DEFAULT_SIZE	10

template<typename T>
class zpArrayList {
public:
	zpArrayList();
	zpArrayList( zp_uint size );
	zpArrayList( T* arr, zp_uint size, zp_uint capacity );
	zpArrayList( const zpArrayList& arr );
	zpArrayList( zpArrayList&& arr );
	~zpArrayList();

	void operator=( const zpArrayList& arr );
	void operator=( zpArrayList&& arr );
	
	T& operator[]( zp_uint index );
	const T& operator[]( zp_uint index ) const;
	
	const T& at( zp_uint index ) const;

	zp_uint size() const;
	zp_uint capacity() const;
	zp_bool isEmpty() const;
	zp_bool isFull() const;

	void pushBack( const T& val );
	void pushBack( const T* arr, zp_uint count );
	template<zp_uint Count>
	void pushBack( const T (&arr)[Count] );

	void pushFront( const T& val );
	void pushFront( const T* arr, zp_uint count );
	template<zp_uint Count>
	void pushFront( const T (&arr)[Count] );

	void popBack( zp_uint numToPop = 1 );
	void popFront( zp_uint numToPop = 1 );

	void insert( zp_uint index, const T& val );
	
	void remove( zp_uint index, T* outVal = ZP_NULL );
	zp_bool removeFirst( const T& val, T* outVal = ZP_NULL );
	zp_bool removeLast( const T& val, T* outVal = ZP_NULL );
	zp_uint removeAll( const T& val );

	void erase( zp_uint index );
	zp_bool eraseFirst( const T& val );
	zp_bool eraseLast( const T& val );
	zp_uint eraseAll( const T& val );

	void clear();

	zp_uint indexOf( const T& val ) const;
	zp_uint lastIndexOf( const T& val ) const;

	void ensureCapacity( zp_uint size );
	void shrinkToFit( zp_uint padding = 0 );

	T& front();
	T& back();

	const T& front() const;
	const T& back() const;

	T* begin();
	T* end();

	const T* begin() const;
	const T* end() const;

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
	zp_uint m_capacity;
};

#include "zpArrayList.inl"

#endif