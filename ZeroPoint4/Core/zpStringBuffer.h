#pragma once
#ifndef ZP_STRING_BUFFER_H
#define ZP_STRING_BUFFER_H

class zpStringBuffer
{
public:
	enum : zp_size_t
	{
		npos = -1,
	};

	zpStringBuffer();
	zpStringBuffer( const zpString& str );
	zpStringBuffer( const zpStringBuffer& buff );
	//zpStringBuffer( zpStringBuffer&& buff );
	~zpStringBuffer();

	void operator=( const zpStringBuffer& buff );
	//void operator=( zpStringBuffer&& buff );

	zp_char& operator[]( zp_size_t index );
	zp_char operator[]( zp_size_t index ) const;

	zpStringBuffer& operator<<( zp_char value );
	zpStringBuffer& operator<<( const zp_char* value );
	zpStringBuffer& operator<<( const zpString& value );
	
	zpStringBuffer& operator<<( zp_sbyte value );
	zpStringBuffer& operator<<( zp_short value );
	zpStringBuffer& operator<<( zp_int value );
	zpStringBuffer& operator<<( zp_long value );

	zpStringBuffer& operator<<( zp_byte value );
	zpStringBuffer& operator<<( zp_ushort value );
	zpStringBuffer& operator<<( zp_uint value );
	zpStringBuffer& operator<<( zp_ulong value );

	zpStringBuffer& operator<<( zp_float value );
	zpStringBuffer& operator<<( zp_double value );
	
	zp_bool isEmpty() const;
	zp_size_t length() const;
	zp_size_t capacity() const;
	zp_size_t size() const;

	void append( zp_char value );
	void append( const zp_char* value, zp_size_t length = npos );
	void append( const zpString& value );

	void append( zp_sbyte value );
	void append( zp_short value );
	void append( zp_int value );
	void append( zp_long value );

	void append( zp_byte value );
	void append( zp_ushort value );
	void append( zp_uint value );
	void append( zp_ulong value );

	void append( zp_float value );
	void append( zp_double value );

	void erase( zp_size_t start, zp_size_t length = 0 );
	void clear();

	zp_size_t indexOf( zp_char ch, zp_size_t fromIndex = 0 ) const;
	zp_size_t indexOf( const zpString& string, zp_size_t fromIndex = 0 ) const;

	zpString toString() const;
	const zp_char* str() const;

	void reserve( zp_size_t size );

protected:
	zpStringBuffer( zp_char* buffer, zp_size_t size );

private:
	void ensureCapacity( zp_size_t size );

	zp_char* m_buffer;
	zp_size_t m_length;
	zp_size_t m_capacity;
	zp_bool m_isFixed;
};

template< zp_size_t Size >
class zpFixedStringBuffer : public zpStringBuffer
{
	ZP_NON_COPYABLE( zpFixedStringBuffer );
public:
	zpFixedStringBuffer();
	virtual ~zpFixedStringBuffer();
private:
	zp_char m_fixedBuffer[ Size ];
};

template< zp_size_t Size >
zpFixedStringBuffer<Size>::zpFixedStringBuffer()
	: zpStringBuffer( m_fixedBuffer, Size )
{}
template< zp_size_t Size >
zpFixedStringBuffer<Size>::~zpFixedStringBuffer()
{
	clear();
}

#endif
