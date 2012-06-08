#pragma once
#ifndef ZP_SERIALIZABLE_H
#define ZP_SERIALIZABLE_H

ZP_PURE_INTERFACE zpSerializable;

ZP_PURE_INTERFACE zpSerializedInput {
	virtual zp_bool readBoolean( const zp_char* name = 0 ) = 0;

	virtual zp_byte readByte( const zp_char* name = 0 ) = 0;
	virtual zp_char readChar( const char* zp_name = 0 ) = 0;
	virtual zp_short readShort( const char* zp_name = 0 ) = 0;
	virtual zp_int readInt( const char* zp_name = 0 ) = 0;
	virtual zp_long readLong( const char* zp_name = 0 ) = 0;

	virtual zp_float readFloat( const char* zp_name = 0 ) = 0;
	virtual zp_double readDouble( const char* zp_name = 0 ) = 0;

	virtual const zpString readString( const char* zp_name = 0 ) = 0;

	virtual zpSerializable* readSerializable( const char* zp_name = 0 ) = 0;

	virtual zp_bool readBlock( const char* zp_name = 0 ) = 0;
	virtual zp_bool endBlock() = 0;
};

ZP_PURE_INTERFACE zpSerializedOutput {
	virtual void writeBoolean( bool value, const char* name = 0 ) = 0;

	virtual void writeByte( zp_byte value, const zp_char* name = 0 ) = 0;
	virtual void writeChar( zp_char value, const zp_char* name = 0 ) = 0;
	virtual void writeShort( zp_short value, const zp_char* name = 0 ) = 0;
	virtual void writeInt( zp_int value, const zp_char* name = 0 ) = 0;
	virtual void writeLong( zp_long value, const zp_char* name = 0 ) = 0;

	virtual void writeFloat( zp_float value, const zp_char* name = 0 ) = 0;
	virtual void writeDouble( zp_double value, const zp_char* name = 0 ) = 0;

	virtual void writeString( const zpString& value, const zp_char* name = 0 ) = 0;

	virtual void writeSerializable( zpSerializable* value, const zp_char* name = 0 ) = 0;

	virtual void writeBlock( const zp_char* name = 0 ) = 0;
	virtual void endBlock() = 0;
};

ZP_PURE_INTERFACE zpSerializable {
	virtual void serialize( zpSerializedOutput* out ) = 0;
	virtual void deserialize( zpSerializedInput* in ) = 0;
};

#endif