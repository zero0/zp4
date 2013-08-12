#pragma once
#ifndef ZP_JSON_H
#define ZP_JSON_H

enum zpJsonType
{
	ZP_JSON_TYPE_NULL =	0,
	ZP_JSON_TYPE_BOOL,
	ZP_JSON_TYPE_INT,
	ZP_JSON_TYPE_UINT,
	ZP_JSON_TYPE_LONG,
	ZP_JSON_TYPE_ULONG,
	ZP_JSON_TYPE_FLOAT,
	ZP_JSON_TYPE_DOUBLE,
	ZP_JSON_TYPE_STRING,
	ZP_JSON_TYPE_ARRAY,
	ZP_JSON_TYPE_OBJECT,
	ZP_JSON_TYPE_DATA,

	zpJsonType_Count,
	zpJsonType_Force32 = ZP_FORECE_32BIT,
};

class zpJson
{
public:
	zpJson();
	explicit zpJson( zpJsonType type );
	explicit zpJson( zp_bool value );
	explicit zpJson( zp_int value );
	explicit zpJson( zp_uint value );
	explicit zpJson( zp_long value );
	explicit zpJson( zp_ulong value );
	explicit zpJson( zp_float value );
	explicit zpJson( zp_double value );
	explicit zpJson( const zp_char* value, zp_bool isData = false );
	explicit zpJson( const zpString& value, zp_bool isData = false );
	explicit zpJson( const void* data, zp_uint size );
	explicit zpJson( const zpDataBuffer& data );
	zpJson( const zpJson& json );
	zpJson( zpJson&& json );
	~zpJson();

	void operator=( const zpJson& json );
	void operator=( zpJson&& json );

	zpJsonType type() const;
	zp_uint size() const;
	zp_bool isEmpty() const;

	zp_bool isNull() const;
	zp_bool isBool() const;
	zp_bool isInt() const;
	zp_bool isUInt() const;
	zp_bool isLong() const;
	zp_bool isULong() const;
	zp_bool isFloat() const;
	zp_bool isDouble() const;
	zp_bool isString() const;
	zp_bool isArray() const;
	zp_bool isObject() const;
	zp_bool isData() const;

	zp_bool isIntegral() const;
	zp_bool isReal() const;
	zp_bool isNumeric() const;

	zp_bool asBool() const;
	zp_int asInt() const;
	zp_uint asUInt() const;
	zp_long asLong() const;
	zp_ulong asULong() const;
	zp_float asFloat() const;
	zp_double asDouble() const;
	zpString asString() const;
	zpString asData() const;
	const zp_char* asCString() const;

	zpJson& operator[]( zp_uint index );
	const zpJson& operator[]( zp_uint index ) const;
	
	zpJson& operator[]( const zp_char* key );
	const zpJson& operator[]( const zp_char* key ) const;

	zpJson& operator[]( const zpString& key );
	const zpJson& operator[]( const zpString& key ) const;

	void memberNames( zpArrayList< zpString >& names ) const;

	static const zpJson null;

	template<typename Func>
	void foreachArray( Func func ) const
	{
		if( isArray() && !isNull() && m_array != ZP_NULL )
		{
			m_array->foreach( func );
		}
	}

	template<typename Func>
	void foreachObject( Func func ) const
	{
		if( isObject() && !isNull() && m_object != ZP_NULL )
		{
			m_object->foreach( func );
		}
	}

private:
	zpJsonType m_type;
	union
	{
		zp_bool m_bool;
		zp_int m_int;
		zp_uint m_uint;
		zp_long m_long;
		zp_ulong m_ulong;
		zp_float m_float;
		zp_double m_double;
		zpString* m_string;
		zpArrayList< zpJson >* m_array;
		zpHashMap< zpString, zpJson >* m_object;

		zp_lptr m_data;
	};
};

class zpJsonParser
{
	ZP_NON_COPYABLE( zpJsonParser );
public:
	zpJsonParser();
	~zpJsonParser();

	zp_bool parseFile( const zpString& filename, zpJson& outJson );

	zp_bool writeToFile( const zpJson& json, const zpString& filename );
	zp_bool writeToBuffer( const zpJson& json, const zpStringBuffer& buffer );

private:

	struct zpJsonToken
	{
		enum zpJsonTokenType
		{
			ZP_JSON_TOKEN_EOF = 0,
			ZP_JSON_TOKEN_OBJECT_OPEN,
			ZP_JSON_TOKEN_OBJECT_CLOSE,
			ZP_JSON_TOKEN_ARRAY_OPEN,
			ZP_JSON_TOKEN_ARRAY_CLOSE,
			ZP_JSON_TOKEN_DATA_OPEN,
			ZP_JSON_TOKEN_DATA_CLOSE,
			ZP_JSON_TOKEN_STRING,
			ZP_JSON_TOKEN_NUMBER,
			ZP_JSON_TOKEN_TRUE,
			ZP_JSON_TOKEN_FALSE,
			ZP_JSON_TOKEN_NULL,
			ZP_JSON_TOKEN_MEMBER_SEP,
			ZP_JSON_TOKEN_ARRAY_SEP,
			ZP_JSON_TOKEN_COMMENT,
			ZP_JSON_TOKEN_ERROR,
		};

		zpJsonTokenType type;
		const zp_char* start;
		const zp_char* end;
	};

	zp_bool readToken( zpJsonToken& token );
	zp_bool readValue();
	zp_bool readObject();
	zp_bool readArray();
	zp_bool readString();
	zp_bool readNumber();
	zp_bool readBool();
	zp_bool readData();

	void skipWhitespace();
	void skipComments();

	template< zp_uint Size >
	zp_bool matches( const zp_char (&str)[Size] )
	{
		return matches( str, Size - 1 );
	}
	zp_bool matches( const zp_char* pattern, zp_int length );

	void tokenToString( const zpJsonToken& token, zpStringBuffer& str );
	void tokenToNumber( const zpJsonToken& token );
	void tokenToDouble( const zpJsonToken& token );

	zpJson& currentValue();
	zp_char nextChar();

	const zp_char* m_current;
	const zp_char* m_start;
	const zp_char* m_end;
	zpArrayList< zpJson* > m_nodes;
};

class zpJsonWriter
{
public:
	zpJsonWriter();
	~zpJsonWriter();

	const zp_char* fastWrite( const zpJson& json );
	const zp_char* styleWrite( const zpJson& json );

private:
	void writeJson( zpStringBuffer& buffer, const zpJson& json, zp_int indent );
	void writeIndent( zpStringBuffer& buffer, zp_int indent );
	void writeNewLine( zpStringBuffer& buffer, zp_int indent );
	void writeObjectSeperator( zpStringBuffer& buffer, zp_int ind );

	zpStringBuffer m_string;
};
#endif