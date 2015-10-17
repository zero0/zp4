#pragma once
#ifndef ZP_ARRAY_LIST_H
#define ZP_ARRAY_LIST_H

template<typename T>
class zpArrayList
{
public:
	enum : zp_size_t
	{
		npos = (zp_size_t)-1,
	};

	zpArrayList();
	zpArrayList( const zpArrayList& arr );
	zpArrayList( zpArrayList&& arr );
	~zpArrayList();

	void operator=( const zpArrayList& arr );
	void operator=( zpArrayList&& arr );
	
	T& operator[]( zp_size_t index );
	const T& operator[]( zp_size_t index ) const;
	
	const T& at( zp_size_t index ) const;

	zp_size_t size() const;
	zp_bool isEmpty() const;
	zp_bool isFixed() const;

	void pushBack( const T& val );
	T& pushBackEmpty();

	void pushFront( const T& val );
	T& pushFrontEmpty();

	void popBack();
	void popFront();

	void erase( zp_size_t index );
	zp_size_t eraseAll( const T& val );

	void clear();
	void reset();
	void reserve( zp_size_t size );
	void resize( zp_size_t size );
	void destroy();

	zp_size_t indexOf( const T& val ) const;
	zp_size_t lastIndexOf( const T& val ) const;

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
	zp_bool findIndexIf( Func func, zp_size_t& index ) const;
	
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

	template<typename Func>
	void sort( Func func );

protected:
	zpArrayList( T* fixedArray, zp_size_t count );

private:
	void ensureCapacity( zp_size_t size );

	T* m_array;
	zp_size_t m_size;
	zp_size_t m_capacity;
	zp_bool m_isFixed;
};


template<typename T, zp_size_t Size>
class zpFixedArrayList : public zpArrayList<T>
{
public:
	zpFixedArrayList();

private:
	T m_fixedArray[ Size ];
};

#include "zpArrayList.inl"

#endif