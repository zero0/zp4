#include "zpContent.h"

zpXmlSerializedOutput::zpXmlSerializedOutput( zpXmlNode* root )
	: m_root( root )
	, m_currentNode( root )
{}
zpXmlSerializedOutput::~zpXmlSerializedOutput() {
	m_nodeStack.clear();
}

void zpXmlSerializedOutput::writeBoolean( bool value, const char* name ) {
	writeString( value ? "true" : "false", name );
}

void zpXmlSerializedOutput::writeByte( zp_byte value, const zp_char* name ) {
	zp_char buff[4];
	zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%d", value );

	writeString( buff, name );
}
void zpXmlSerializedOutput::writeChar( zp_char value, const zp_char* name ) {
	writeString( zpString( &value, 1 ), name );
}
void zpXmlSerializedOutput::writeShort( zp_short value, const zp_char* name ) {
	zp_char buff[8];
	zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%d", value );

	writeString( buff, name );
}
void zpXmlSerializedOutput::writeInt( zp_int value, const zp_char* name ) {
	zp_char buff[16];
	zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%d", value );

	writeString( buff, name );
}
void zpXmlSerializedOutput::writeLong( zp_long value, const zp_char* name ) {
	zp_char buff[32];
	zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%dl", value );

	writeString( buff, name );
}

void zpXmlSerializedOutput::writeFloat( zp_float value, const zp_char* name ) {
	zp_char buff[32];
	zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%f", value );

	writeString( buff, name );
}
void zpXmlSerializedOutput::writeDouble( zp_double value, const zp_char* name ) {
	zp_char buff[32];
	zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%fl", value );

	writeString( buff, name );
}

void zpXmlSerializedOutput::writeString( const zpString& value, const zp_char* name ) {
	if( name == ZP_NULL ) {
		m_currentNode->value = value;
	} else if( name[0] == '@' ) {
		m_currentNode->attributes[ &name[1] ] = value;
	} else {
		zpXmlNode* node = new zpXmlNode;
		node->name = name;
		node->value = value;
		node->type = ZP_XML_NODE_TYPE_ELEMENT;

		node->parent = m_currentNode;
		m_currentNode->children.pushBack( node );
	}
}

void zpXmlSerializedOutput::writeSerializable( zpSerializable* value, const zp_char* name ) {
	writeBlock( name );

	value->serialize( this );

	endBlock();
}

void zpXmlSerializedOutput::writeBlock( const zp_char* name ) {
	zpXmlNode* node = new zpXmlNode;
	node->name = name;
	node->type = ZP_XML_NODE_TYPE_ELEMENT;

	node->parent = m_currentNode;
	m_currentNode->children.pushBack( node );

	m_nodeStack.pushBack( m_currentNode );

	m_currentNode = node;
}
void zpXmlSerializedOutput::endBlock() {
	m_currentNode = m_nodeStack.back();
	m_nodeStack.popBack();
}
