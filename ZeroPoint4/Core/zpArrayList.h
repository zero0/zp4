#pragma once
#ifndef ZP_ARRAY_LIST_H
#define ZP_ARRAY_LIST_H

template<typename T>
class zpArrayList {
public:
	enum
	{
		npos = -1,
	};

	zpArrayList();
	zpArrayList( const zpArrayList& arr );
	zpArrayList( zpArrayList&& arr );
	~zpArrayList();

	void operator=( const zpArrayList& arr );
	void operator=( zpArrayList&& arr );
	
	T& operator[]( zp_uint index );
	const T& operator[]( zp_uint index ) const;
	
	const T& at( zp_uint index ) const;

	zp_uint size() const;
	zp_bool isEmpty() const;
	zp_bool isFixed() const;

	void pushBack( const T& val );
	T& pushBackEmpty();

	void pushFront( const T& val );
	T& pushFrontEmpty();

	void popBack();
	void popFront();

	void erase( zp_uint index );
	zp_uint eraseAll( const T& val );

	void clear();
	void reset();
	void reserve( zp_uint size );

	zp_int indexOf( const T& val ) const;
	zp_int lastIndexOf( const T& val ) const;

	T& front();
	T& back();

	const T& front() const;
	const T& back() const;

	T* begin();
	T* end();

	const T* begin() const;
	const T* end() const;

	template<typename Func>
	zp_bool findIf( Func func, const T** found ) const;
	
	template<typename Func>
	zp_bool findIf( Func func, T** found );
	
	template<typename Func>
	void foreach( Func func ) const;
	template<typename Func>
	void foreachIf( Func func ) const;

	template<typename Func>
	void foreachIndexed( Func func ) const;
	template<typename Func>
	void foreachIndexedIf( Func func ) const;

	template<typename Func>
	void eraseIf( Func func );

	template<typename Func>
	void map( Func func );
	
protected:
	zpArrayList( T* fixedArray, zp_uint count );

private:
	void ensureCapacity( zp_uint size );

	T* m_array;
	zp_uint m_size;
	zp_uint m_capacity;
	zp_bool m_isFixed;
};


template<typename T, zp_uint Size>
class zpFixedArrayList : public zpArrayList<T> {
public:
	zpFixedArrayList();
	~zpFixedArrayList();

private:
	T m_fixedArray[ Size ];
};

#include "zpArrayList.inl"

#endif