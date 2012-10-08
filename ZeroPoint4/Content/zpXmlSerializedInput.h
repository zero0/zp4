#pragma once
#ifndef ZP_XML_SERIALIZED_INPUT_H
#define ZP_XML_SERIALIZED_INPUT_H

class zpXmlSerializedInput : public zpSerializedInput {
public:
	zpXmlSerializedInput();
	zpXmlSerializedInput( zpXmlNode* root, const zpProperties& properties );
	virtual ~zpXmlSerializedInput();

	zp_bool readBoolean( zp_bool* value, const zp_char* name );

	zp_bool readByte( zp_byte* value, const zp_char* name );
	zp_bool readChar( zp_char* value, const zp_char* name );
	zp_bool readShort( zp_short* value, const zp_char* name );
	zp_bool readInt( zp_int* value, const zp_char* name );
	zp_bool readLong( zp_long* value, const zp_char* name );

	zp_bool readFloat( zp_float* value, const zp_char* name );
	zp_bool readDouble(  zp_double* value, const zp_char* name );

	zp_bool readString( zpString* value, const zp_char* name );

	zp_bool readSerializable( zpSerializable** value, const zp_char* name );

	zp_bool readBlock( const zp_char* name );
	zp_bool endBlock();

private:
	void readValueOrProperty( const zp_char* name, zpString& outString );
	zp_bool isProperty( zpString& name, zpString& outDefaultValue );

	zpXmlNode* m_rootNode;
	zpXmlNode* m_currentNode;

	zpArrayList<zpXmlNode*> m_nodeStack;
	zpProperties m_properties;
};

#endif