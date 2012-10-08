#pragma once
#ifndef ZP_SERIALIZABLE_H
#define ZP_SERIALIZABLE_H

const zp_char* __zp_serialize_type( const void* type );

template<typename T>
const zp_char* zp_serialize_type( const T* /*type = ZP_NULL*/ ) {
	return __zp_serialize_type( &typeid( T ) );
};

#define ZP_SERIALIZE_TYPE_THIS		( zp_serialize_type( this ) )
#define ZP_SERIALIZE_TYPE( t )		( zp_serialize_type<t>() )

ZP_PURE_INTERFACE zpSerializable;

ZP_PURE_INTERFACE zpSerializedInput {
public:
	virtual zp_bool readBoolean( zp_bool* value, const zp_char* name = 0 ) = 0;

	virtual zp_bool readByte( zp_byte* value, const zp_char* name = 0 ) = 0;
	virtual zp_bool readChar( zp_char* value, const zp_char* name = 0 ) = 0;
	virtual zp_bool readShort( zp_short* value, const zp_char* name = 0 ) = 0;
	virtual zp_bool readInt( zp_int* value, const zp_char* name = 0 ) = 0;
	virtual zp_bool readLong( zp_long* value, const zp_char* name = 0 ) = 0;

	virtual zp_bool readFloat( zp_float* value, const zp_char* name = 0 ) = 0;
	virtual zp_bool readDouble(  zp_double* value, const zp_char* name = 0 ) = 0;

	virtual zp_bool readString( zpString* value, const zp_char* name = 0 ) = 0;

	virtual zp_bool readSerializable( zpSerializable** value, const zp_char* name = 0 ) = 0;

	virtual zp_bool readBlock( const zp_char* name = 0 ) = 0;
	virtual zp_bool endBlock() = 0;

	template<typename Func>
	void readEachBlock( const zp_char* name, Func func ) {
		do {
			if( readBlock( name ) ) {
				func( this );
			}
		} while( endBlock() );
	}

	template<typename T>
	zp_bool readSerializableOfType( T** value, const zp_char* name = 0 ) {
		return readSerializable( (zpSerializable**)value, name );
	}
};

ZP_PURE_INTERFACE zpSerializedOutput {
public:
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
public:
	virtual void serialize( zpSerializedOutput* out ) = 0;
	virtual void deserialize( zpSerializedInput* in ) = 0;
};

template<typename T>
class zpSerializableObject {
public:
	zpSerializableObject() : m_object() {}
	zpSerializableObject( const T& obj ) : m_object( obj ) {}
	virtual ~zpSerializableObject() {}

	operator T&() { return m_object; }
	operator const T&() const { return m_object; }

	void serialize( zpSerializedOutput* out ) { serializeAs( out, m_object ); }
	void deserialize( zpSerializedInput* in ) { deserializeAs( in, m_object ); }

	static void serializeAs( zpSerializedOutput* out, T& in );
	static void deserializeAs( zpSerializedInput* in, T& out );

private:
	T m_object;
};

#endif