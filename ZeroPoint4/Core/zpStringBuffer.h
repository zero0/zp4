#pragma once
#ifndef ZP_STRING_BUFFER_H
#define ZP_STRING_BUFFER_H

class zpStringBuffer {
public:
	static const zp_uint npos = -1;

	zpStringBuffer();
	zpStringBuffer( zp_uint size );
	zpStringBuffer( const zpStringBuffer& buff );
	zpStringBuffer( zpStringBuffer&& buff );
	~zpStringBuffer();

	void operator=( const zpStringBuffer& buff );
	void operator=( zpStringBuffer&& buff );

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
	void append( const zp_char* value, zp_uint length = npos );
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

	void prepend( zp_char value );
	void prepend( const zp_char* value );

	void prepend( zp_sbyte value );
	void prepend( zp_short value );
	void prepend( zp_int value );
	void prepend( zp_long value );

	void prepend( zp_byte value );
	void prepend( zp_ushort value );
	void prepend( zp_uint value );
	void prepend( zp_ulong value );

	void prepend( zp_float value );
	void prepend( zp_double value );

	void erase( zp_uint start, zp_uint end = npos );
	void clear();

	zp_uint indexOf( zp_char ch, zp_uint fromIndex = 0 ) const;
	zp_uint indexOf( const zpString& string, zp_uint fromIndex = 0 ) const;

	zpString toString() const;
	const zp_char* getChars() const;

private:
	void ensureCapacity( zp_uint size );

	zp_char* m_buffer;
	zp_uint m_length;
	zp_uint m_capacity;
};

#endif