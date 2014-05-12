#pragma once
#ifndef ZP_STRING_BUFFER_H
#define ZP_STRING_BUFFER_H

class zpStringBuffer {
public:
	static const zp_int npos = -1;

	zpStringBuffer();
	zpStringBuffer( const zpString& str );
	zpStringBuffer( const zpStringBuffer& buff );
	//zpStringBuffer( zpStringBuffer&& buff );
	~zpStringBuffer();

	void operator=( const zpStringBuffer& buff );
	//void operator=( zpStringBuffer&& buff );

	zp_char& operator[]( zp_uint index );
	zp_char operator[]( zp_uint index ) const;

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
	zp_uint length() const;
	zp_uint capacity() const;
	zp_uint size() const;

	void append( zp_char value );
	void append( const zp_char* value, zp_int length = npos );
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

	void erase( zp_uint start, zp_int length = 0 );
	void clear();

	zp_int indexOf( zp_char ch, zp_uint fromIndex = 0 ) const;
	zp_int indexOf( const zpString& string, zp_uint fromIndex = 0 ) const;

	zpString toString() const;
	const zp_char* str() const;

	void reserve( zp_uint size );

protected:
	zpStringBuffer( zp_char* buffer, zp_uint size );

private:
	void ensureCapacity( zp_uint size );

	zp_char* m_buffer;
	zp_uint m_length;
	zp_uint m_capacity;
	zp_bool m_isFixed;
};

template< zp_uint Size >
class zpFixedStringBuffer : public zpStringBuffer
{
	ZP_NON_COPYABLE( zpFixedStringBuffer );
public:
	zpFixedStringBuffer();
	virtual ~zpFixedStringBuffer();
private:
	zp_char m_fixedBuffer[ Size ];
};

template< zp_uint Size >
zpFixedStringBuffer<Size>::zpFixedStringBuffer()
	: zpStringBuffer( m_fixedBuffer, Size )
{}
template< zp_uint Size >
zpFixedStringBuffer<Size>::~zpFixedStringBuffer()
{
	clear();
}

#endif
