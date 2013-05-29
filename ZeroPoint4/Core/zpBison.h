#pragma once
#ifndef ZP_BISON_H
#define ZP_BISON_H

enum zpBisonType : zp_byte
{
	ZP_BISON_TYPE_NULL =	0,
	ZP_BISON_TYPE_BOOL,
	ZP_BISON_TYPE_INT,
	ZP_BISON_TYPE_FLOAT,
	ZP_BISON_TYPE_STRING,
	ZP_BISON_TYPE_ARRAY,
	ZP_BISON_TYPE_OBJECT,

	zpBisonType_Count,
};

struct zpBisonHeader
{
	zp_uint bison;
	zp_uint fileVersion;
	zp_uint stringHashSalt;
};

class zpBison
{
public:
	zpBison();
	~zpBison();

	zp_bool readFromFile( const zpString& filename );
	zp_bool readFromBuffer( const zpDataBuffer& buffer );

	class Value
	{
	public:
		Value();
		~Value();

		zpBisonType type() const;
		zp_uint size() const;
		zp_bool isEmpty() const;

		zp_bool isNull() const;
		zp_bool isBool() const;
		zp_bool isInt() const;
		zp_bool isFloat() const;
		zp_bool isString() const;
		zp_bool isArray() const;
		zp_bool isObject() const;

		zp_bool isIntegral() const;
		zp_bool isReal() const;
		zp_bool isNumeric() const;

		zp_bool asBool() const;
		zp_int asInt() const;
		zp_float asFloat() const;
		zpString asString() const;
		const zp_char* asCString() const;
		zp_hash asHash() const;

		const Value operator[]( zp_int index ) const;
		const Value operator[]( const zp_char* key ) const;
		const Value operator[]( const zpString& key ) const;

		void memberNames( zpArrayList< zpBison::Value >& names ) const;

	private:
		Value( const zp_byte* data, zp_uint offset );

		zpBisonType m_type;
		const zp_byte* m_data;
		zp_uint m_offset;

		friend class zpBison;
	};

	const zpBison::Value& root() const;

	static const zpBison::Value null;

	static zp_bool compileToFile( const zpString& filename, const zpJson& json );
	static zp_bool compileToBuffer( zpDataBuffer& buffer, const zpJson& json );

private:
	static void compileStringTable( zpHashMap< zpString, zp_uint >& stringTable, const zpJson& json );
	static zp_bool compileToBufferInternal( zpDataBuffer& buffer, const zpHashMap< zpString, zp_uint >& stringTable, const zpJson& json );

	void parseData();

	zpDataBuffer m_buffer;
	zpBison::Value m_root;
};

class zpBisonWriter
{
public:
	zpBisonWriter();
	~zpBisonWriter();

	const zp_char* fastWrite( const zpBison& bison );
	const zp_char* styleWrite( const zpBison& bison );

private:
	void writeBison( zpStringBuffer& buffer, const zpBison::Value& bison, zp_int indent );
	void writeIndent( zpStringBuffer& buffer, zp_int indent );
	void writeNewLine( zpStringBuffer& buffer, zp_int indent );
	void writeObjectSeperator( zpStringBuffer& buffer, zp_int indent );

	zpStringBuffer m_string;
};

#endif