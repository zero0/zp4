#pragma once
#ifndef ZP_ARRAY_H
#define ZP_ARRAY_H

template<typename T>
class zpArray {
public:
	zpArray();
	zpArray( zp_uint size );
	zpArray( T* arr, zp_uint size, zp_uint capacity );
	zpArray( const zpArray& arr );
	zpArray( zpArray&& arr );

	zpArray& operator=( const zpArray& arr );
	zpArray& operator=( zpArray&& arr );
	
	T& operator[]( zp_uint index );
	T& operator[]( zp_uint index ) const;
	
	T& at( zp_uint index ) const;

	zp_uint size() const;
	zp_uint capacity() const;
	zp_bool isEmpty() const;
	zp_bool isFull() const;

	void pushBack( T& val );
	void pushBack( T* arr, zp_uint count );

	void pushFront( T& val );
	void pushFront( T* arr, zp_uint count );

	void popBack( zp_uint numToPop = 1 );
	void popFront( zp_uint numToPop = 1 );

	void insert( zp_uint index, T& val );
	T remove( zp_uint index );
	void clear();

	void ensureCapacity( zp_uint size );

	T& front();
	T& back();

	const T& front() const;
	const T& back() const;

	T* begin();
	T* end();

	const T* begin() const;
	const T* end() const;

private:
	T* m_array;
	zp_uint m_size;
	zp_uint m_capacity;
};

#endif