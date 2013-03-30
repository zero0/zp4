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

	zpJsonType_Count,
};

class zpJson
{
public:
	zpJson();
	explicit zpJson( zp_bool value );
	explicit zpJson( zp_int value );
	explicit zpJson( zp_uint value );
	explicit zpJson( zp_long value );
	explicit zpJson( zp_ulong value );
	explicit zpJson( zp_float value );
	explicit zpJson( zp_double value );
	explicit zpJson( const zp_char* value );
	explicit zpJson( const zpString& value );
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
	const zp_char* asCString() const;

	zpJson& operator[]( zp_uint index );
	const zpJson& operator[]( zp_uint index ) const;
	
	zpJson& operator[]( const zp_char* key );
	const zpJson& operator[]( const zp_char* key ) const;

	zpJson& operator[]( const zpString& key );
	const zpJson& operator[]( const zpString& key ) const;

	static const zpJson null;

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

#endif