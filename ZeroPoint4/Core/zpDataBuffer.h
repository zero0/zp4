#pragma once
#ifndef ZP_DATA_BUFFER_H
#define ZP_DATA_BUFFER_H

class zpDataBuffer
{
	ZP_NON_COPYABLE( zpDataBuffer );

public:
	explicit zpDataBuffer( zp_uint capacity );
	~zpDataBuffer();

	const void* getData() const;

	zp_uint size() const;
	zp_uint capacity() const;

	void ensureCapacity( zp_uint capacity );

	void clear();
	void reset();

	template<typename T>
	void read( T& out ) const;

	template<typename T>
	void write( const T& in );

protected:
	zpDataBuffer( void* data, zp_uint capacity );

private:
	void* m_data;
	zp_uint m_capacity;
	zp_uint m_offset;
	zp_bool m_isFixed;
};

template<typename T>
void zpDataBuffer::read( T& out ) const
{
	zp_uint newOffset = m_offset + sizeof( T );
	ZP_ASSERT( newOffset < m_capacity, "Buffer overrun" );

	T* t = (T*)( ( (zp_byte*)m_data ) + m_offset );
	out = *t;

	m_offset = newOffset;
}

template<typename T>
void zpDataBuffer::write( const T& in )
{
	zp_uint newOffset = m_offset + sizeof( T );
	ensureCapacity( newOffset );

	T* t = (T*)( ( (zp_byte*)m_data ) + m_offset );
	*t = in;

	m_offset = newOffset;
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
{
	clear();
}

#endif