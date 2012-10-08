#pragma once
#ifndef ZP_XML_SERIALIZED_OUTPUT_H
#define ZP_XML_SERIALIZED_OUTPUT_H

class zpXmlSerializedOutput : public zpSerializedOutput {
public:
	zpXmlSerializedOutput( zpXmlNode* root );
	virtual ~zpXmlSerializedOutput();

	void writeBoolean( bool value, const char* name );

	void writeByte( zp_byte value, const zp_char* name );
	void writeChar( zp_char value, const zp_char* name );
	void writeShort( zp_short value, const zp_char* name );
	void writeInt( zp_int value, const zp_char* name );
	void writeLong( zp_long value, const zp_char* name );

	void writeFloat( zp_float value, const zp_char* name );
	void writeDouble( zp_double value, const zp_char* name );

	void writeString( const zpString& value, const zp_char* name );

	void writeSerializable( zpSerializable* value, const zp_char* name );

	void writeBlock( const zp_char* name );
	void endBlock();

private:
	zpXmlNode* m_root;
	zpXmlNode* m_currentNode;
	zpArrayList<zpXmlNode*> m_nodeStack;
};

#endif