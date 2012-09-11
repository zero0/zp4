#include "zpContent.h"

#define XML_START_COMMENT( x, i )	( (x)[ (i) + 1 ] == xml_exclamation && (x)[ (i) + 2 ] == xml_minus && (x)[ (i) + 3 ] == xml_minus )
#define XML_END_COMMENT( x, i )		( (x)[ (i) + 0 ] == xml_minus && (x)[ (i) + 1 ] == xml_minus && (x)[ (i) + 2 ] == xml_close )

#define XML_START_CDATA( x, i )		( \
	(x)[ (i) + 1 ] == xml_exclamation && \
	(x)[ (i) + 2 ] == xml_sqrpo && \
	(x)[ (i) + 3 ] == 'C' && \
	(x)[ (i) + 4 ] == 'D' && \
	(x)[ (i) + 5 ] == 'A' && \
	(x)[ (i) + 6 ] == 'T' && \
	(x)[ (i) + 7 ] == 'A' && \
	(x)[ (i) + 8 ] == xml_sqrpo \
)
#define XML_END_CDATA( x, i )		( (x)[ (i) + 0 ] == xml_sqrpc && (x)[ (i) + 1 ] == xml_sqrpc && (x)[ (i) + 2 ] == xml_close )

zpXmlNode::zpXmlNode() :
	type( ZP_XML_NODE_TYPE_NONE ),
	parent( ZP_NULL ),
	name(),
	value(),
	attributes( 0 ),
	children( 0 )
{}
zpXmlNode::~zpXmlNode() {
	attributes.clear();
	children.clear();
}

zpXmlParser::zpXmlParser() {}
zpXmlParser::~zpXmlParser() {}

zpXmlNode* zpXmlParser::parseFile( const zpString& filename ) {
	zpStringBuffer xmlBuffer;
	zpFile file( filename, ZP_FILE_MODE_READ );

	if( file.open() ) {
		file.readFile( &xmlBuffer );
		file.close();
	}

	if( xmlBuffer.isEmpty() ) return ZP_NULL;

	const zp_char* xml = xmlBuffer.getChars();
	const zp_uint length = xmlBuffer.length();

	enum ops {
		xml_null = '\0',
		xml_open = '<',
		xml_close = '>',
		xml_slash = '/',
		xml_space = ' ',
		xml_quote = '\'',
		xml_dquote = '"',
		xml_equal = '=',
		xml_newline = '\n',
		xml_cret = '\r',
		xml_tab = '\t',
		xml_exclamation = '!',
		xml_minus = '-',
		xml_sqrpo = '[',
		xml_sqrpc = ']',
		xml_quesion = '?',
	};

	zp_uint k = 0;
	zp_bool inCDATA = false;
	zp_bool inProcInst = false;
	zp_char c = xml[ k ];
	zp_char delim = xml_null;
	zpXmlNode* current = ZP_NULL;
	zpStringBuffer buff;
	zpString attrName, attrValue;

	zpXmlNode* rootNode = new zpXmlNode;
	rootNode->type = ZP_XML_NODE_TYPE_DOCUMENT;
	rootNode->name = "XML_DOC";

	current = rootNode;

	while( k < length ) {
		c = xml[ k ];

		if( c == xml_tab || c == xml_newline || c == xml_cret ) {
			k++;
			continue;;
		}

		if( c == xml_open ) {
			// check for comment
			if( XML_START_COMMENT( xml, k ) ) {
				k += 4;
				c = xml[ k ];
					
				buff.clear();
				while( !XML_END_COMMENT( xml, k ) ) {
					buff.append( c );
					c = xml[ ++k ];
				}
				k += 3;
					
				zpXmlNode* node = new zpXmlNode;
				node->type = ZP_XML_NODE_TYPE_COMMENT;
				node->parent = current;
				node->name = "Comment";
				node->value = buff.toString();
					
				current->children.pushBack( node );
				continue;
			}

			// check for CDATA
			if( XML_START_CDATA( xml, k ) ) {
				k += 8;
				c = xml[ k ];

				buff.clear();
				while( !XML_END_CDATA( xml, k ) ) {
					buff.append( c );
					c = xml[ ++k ];
				}
				k += 3;

				zpXmlNode* node = new zpXmlNode;
				node->type = ZP_XML_NODE_TYPE_CDATA;
				node->parent = current;
				node->name = "CDATA";
				node->value = buff.toString();

				current->children.pushBack( node );
				continue;
			}

			// check for closing tag
			if( xml[ k + 1 ] == xml_slash ) {
				current = current->parent;
				k++;
				while( xml[ k++ ] != xml_close );
				k++;
				continue;
			}

			// check for process instructions
			if( xml[ k + 1 ] == xml_quesion ) {
				c = xml[ ++k ];
				inProcInst = true;
			}

			// grab the name of the tag
			buff.clear();
			c = xml[ ++k ];
			while( c != xml_slash && c != xml_space && c != xml_close ) {
				if( c != xml_newline && c != xml_tab && c != xml_cret ) {
					buff.append( c );
				}
				c = xml[ ++k ];
			}

			// add the node
			zpXmlNode* node = new zpXmlNode;
			node->type = ZP_XML_NODE_TYPE_ELEMENT;
			node->parent = current;
			node->name = buff.toString();
			current->children.pushBack( node );
				
			current = node;

			while( c == xml_space ) {
				c = xml[ ++k ];
					
				if( c == xml_slash || ( c == xml_quesion && inProcInst ) ) {
					break;
				}

				buff.clear();
				while( c != xml_equal ) {
					if( c != xml_newline && c != xml_tab && c != xml_cret ) {
						buff.append( c );
					}
					c = xml[ ++k ];
				}
				attrName = buff.toString();

				c = xml[ ++k ];

				if( c == xml_quote || c == xml_dquote ) {
					delim = c;
					c = xml[ ++k ];
				}

				buff.clear();
				while( c != delim && delim != xml_null ) {
					if( c != xml_newline && c != xml_tab && c != xml_cret ) {
						buff.append( c );
					}
					c = xml[ ++k ];
				}
				attrValue = buff.toString();

				delim = xml_null;
				c = xml[ ++k ];

				current->attributes.put( attrName, attrValue );
			}

			if( c == xml_slash ) {
				current = current->parent;
				c = xml[ ++k ];
				while( c != xml_close ) {
					c = xml[ ++k ];
				}
			}

			if( c == xml_quesion && inProcInst ) {
				current->type = ZP_XML_NODE_TYPE_PROCESS_INSTRUCTION;
				current = current->parent;
				inProcInst = false;
				c = xml[ ++k ];
				while( c != xml_close ) {
					c = xml[ ++k ];
				}
			}

			if( c == xml_close ) {
				// should be true as the tag ended
				int f = 0;f++;
			}
		}

		buff.clear();
		if( c != xml_open && c != xml_close && c != xml_slash ) {
			while( c != xml_open ) {
				if( c != xml_newline && c != xml_tab && c != xml_cret ) {
					buff.append( c );
				}
				c = xml[ ++k ];
			}
			current->value = buff.toString();
			continue;
		}
		++k;
	}

	return rootNode;
}
