#pragma once
#ifndef ZP_XML_PARSER_H
#define ZP_XML_PARSER_H

enum zpXmlNodeType {
	ZP_XML_NODE_TYPE_NONE,
	ZP_XML_NODE_TYPE_ELEMENT,
	ZP_XML_NODE_TYPE_COMMENT,
	ZP_XML_NODE_TYPE_CDATA,
	ZP_XML_NODE_TYPE_DOCUMENT,
	ZP_XML_NODE_TYPE_PROCESS_INSTRUCTION,
};

struct zpXmlNode {
	zpXmlNodeType type;
	zpXmlNode* parent;

	zpString name;
	zpString value;

	zpHashMap<zpString, zpString> attributes;
	zpArrayList<zpXmlNode*> children;

	zpXmlNode();
	~zpXmlNode();
};

class zpXmlParser {
public:
	zpXmlParser();
	~zpXmlParser();

	static zpXmlNode* parseFile( const zpString& filename );
	
private:
};

#endif