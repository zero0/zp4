#pragma once
#ifndef ZP_XML_PARSER_H
#define ZP_XML_PARSER_H

enum zpXmlNodeType
{
	ZP_XML_NODE_TYPE_NONE = 0,
	ZP_XML_NODE_TYPE_ELEMENT,
	ZP_XML_NODE_TYPE_COMMENT,
	ZP_XML_NODE_TYPE_CDATA,
	ZP_XML_NODE_TYPE_DOCUMENT,
	ZP_XML_NODE_TYPE_PROCESS_INSTRUCTION,
};

struct zpXmlAttribute
{
	zpString name;
	zpString value;
};

struct zpXmlAttributes
{
	zpArrayList< zpXmlAttribute > attributes;

	void setString( const zpString& name, const zpString& value )
	{
		zpXmlAttribute& attr = attributes.pushBackEmpty();
		attr.name = name;
		attr.value = value;
	}

	const zpString& getString( const zp_char* key ) const;

	zp_int getInt( const zp_char* key ) const;

	zp_float getFloat( const zp_char* key ) const;

	template<typename Func>
	void foreach( Func func ) const
	{
		const zpXmlAttribute* b = attributes.begin();
		const zpXmlAttribute* e = attributes.end();
		for( ; b != e; ++b )
		{
			func( b->name, b->value );
		}
	}
};

struct zpXmlNode
{
	zpXmlNodeType type;
	zpXmlNode* parent;
	zpXmlNode* nextSibling;
	zpXmlNode* prevSibling;

	zpString name;
	zpString value;

	zpXmlAttributes attributes;
	zpArrayList< zpXmlNode* > children;

	zpXmlNode();
	~zpXmlNode();
};

class zpXmlParser
{
public:
	zpXmlParser();
	~zpXmlParser();

	zp_bool parseFile( const zpString& filename, zp_bool includeSiblings = false );
	zp_bool parseNode( zpXmlNode* root, zp_bool includeSiblings = false );

	static zp_bool writeToFile( zpXmlNode* root, const zpString& filename );
	static void writeToBuffer( zpXmlNode* root, zpStringBuffer& buffer );

	void setRootNode( zpXmlNode* root, zp_bool ownsRoot );
	zpXmlNode* getRootNode() const;
	zpXmlNode* getCurrentNode() const;

	zp_bool push( const zp_char* nodeName );
	zp_bool next();
	zp_bool pop();

private:
	zp_bool m_ownsRoot;

	zpXmlNode* m_root;
	zpXmlNode* m_current;

	zpArrayList<zpString> m_nodeNameStack;
	zpArrayList<zpXmlNode*> m_nodeStack;
};

#endif