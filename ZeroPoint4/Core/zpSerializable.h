#pragma once
#ifndef ZP_SERIALIZABLE_H
#define ZP_SERIALIZABLE_H

const zp_char* __zp_serialize_type( const void* type );

template<typename T>
const zp_char* zp_serialize_type( const T* type = ZP_NULL ) {
	return __zp_serialize_type( &typeid( T ) );
};

#define ZP_SERIALIZE_TYPE_THIS		( zp_serialize_type( this ) )
#define ZP_SERIALIZE_TYPE( t )		( zp_serialize_type<t>() )

ZP_PURE_INTERFACE zpSerializable;

ZP_PURE_INTERFACE zpSerializedInput {
	virtual zp_bool readBoolean( const zp_char* name = 0 ) = 0;

	virtual zp_byte readByte( const zp_char* name = 0 ) = 0;
	virtual zp_char readChar( const zp_char* name = 0 ) = 0;
	virtual zp_short readShort( const zp_char* name = 0 ) = 0;
	virtual zp_int readInt( const zp_char* name = 0 ) = 0;
	virtual zp_long readLong( const zp_char* name = 0 ) = 0;

	virtual zp_float readFloat( const zp_char* name = 0 ) = 0;
	virtual zp_double readDouble( const zp_char* name = 0 ) = 0;

	virtual const zpString readString( const zp_char* name = 0 ) = 0;

	virtual zpSerializable* readSerializable( const zp_char* name = 0 ) = 0;

	virtual zp_bool readBlock( const zp_char* name = 0 ) = 0;
	virtual zp_bool endBlock() = 0;

	template<typename Func>
	void readEach( const zp_char* name, Func func ) {
		bool hasMore = true;
		while( hasMore ) {
			if( readBlock( name ) ) {
				func();
			}
			hasMore = endBlock();
		}
	}

	template<typename T>
	T* readSerializableType( const zp_char* name = 0 ) {
		return (T*)readSerializable( name );
	}
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