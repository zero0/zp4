#pragma once
#ifndef ZP_DATA_BUFFER_H
#define ZP_DATA_BUFFER_H

class zpDataBuffer
{
public:
	zpDataBuffer();
	explicit zpDataBuffer( zp_uint capacity );
	zpDataBuffer( const zpDataBuffer& other );
	~zpDataBuffer();

	void operator=( const zpDataBuffer& other );

	zp_byte* getData();
	const zp_byte* getData() const;

	zp_uint size() const;
	zp_uint capacity() const;

	void reserve( zp_uint size );
	void resize( zp_uint size );

	void clear();
	void reset();
	void destroy();

	template<typename T>
	void read( T& out ) const;

	template<typename T>
	void write( const T& in );

	template<typename T>
	void writeBulk( const T* in, zp_uint count );

	template<typename T>
	void writeFill( const T& in, zp_uint count );

	template<typename T>
	void writeAt( const T& in, zp_uint offset );

	template<typename T>
	void readAt( T& out, zp_uint offset ) const;

	void readIn( const zpDataBuffer& buffer );
	void writeOut( zpDataBuffer& buffer ) const;

	template<typename T, typename Func >
	void map( zp_uint start, zp_uint count, Func func );

protected:
	void ensureCapacity( zp_uint capacity );

	zpDataBuffer( zp_byte* data, zp_uint capacity );

private:
	zp_byte* m_data;

	zp_uint m_capacity;
	mutable zp_uint m_size;
	zp_bool m_isFixed;
};

template<typename T>
void zpDataBuffer::read( T& out ) const
{
	zp_uint newSize = m_size + sizeof( T );
	ZP_ASSERT( newSize < m_capacity, "Buffer overrun" );

	const T* t = (const T*)( m_data + m_size );
	out = *t;

	m_size = newSize;
}

template<typename T>
void zpDataBuffer::write( const T& in )
{
	zp_uint newSize = m_size + sizeof( T );
	ensureCapacity( newSize );

	T* t = (T*)( m_data + m_size );
	*t = in;

	m_size = newSize;
}
template<typename T>
void zpDataBuffer::writeBulk( const T* in, zp_uint count )
{
	zp_uint newSize = m_size + ( sizeof( T ) * count );
	ensureCapacity( newSize );

	T* t = (T*)( m_data + m_size );
	for( zp_uint i = 0; i < count; ++i, ++t, ++in )
	{
		*t = *in;
	}

	m_size = newSize;
}
template<typename T>
void zpDataBuffer::writeFill( const T& in, zp_uint count )
{
	zp_uint newSize = m_size + ( sizeof( T ) * count );
	ensureCapacity( newSize );

	T* t = (T*)( m_data + m_size );
	for( zp_uint i = 0; i < count; ++i, ++t )
	{
		*t = in;
	}

	m_size = newSize;
}
template<typename T>
void zpDataBuffer::writeAt( const T& in, zp_uint offset )
{
	ZP_ASSERT( offset < m_size, "Buffer overrun" );
	*(T*)( m_data + offset ) = in;
}
template<typename T>
void zpDataBuffer::readAt( T& out, zp_uint offset ) const
{
	ZP_ASSERT( offset < m_size, "Buffer overrun" );
	out = *(T*)( m_data + offset );
}


template<typename T, typename Func >
void zpDataBuffer::map( zp_uint start, zp_uint count, Func func )
{
	T* t = (T*)( m_data + start );
	for( zp_uint i = 0; i < count; ++i, ++t )
	{
		*t = func( *t );
	}
}

template< zp_uint Size >
class zpFixedDataBuffer : public zpDataBuffer
{
	ZP_NON_COPYABLE( zpFixedDataBuffer );
public:
	zpFixedDataBuffer();
	~zpFixedDataBuffer();

private:
	zp_byte m_byteData[ Size ];
};

template< zp_uint Size >
zpFixedDataBuffer<Size>::zpFixedDataBuffer()
	: zpDataBuffer( m_byteData, Size )
{}
template< zp_uint Size >
zpFixedDataBuffer<Size>::~zpFixedDataBuffer()
{}

#endif
