#include "zpContent.h"
#include <stdlib.h>

zpXmlSerializedInput::zpXmlSerializedInput() :
	m_rootNode( ZP_NULL ),
	m_currentNode( ZP_NULL )
{}
zpXmlSerializedInput::zpXmlSerializedInput( zpXmlNode* root, const zpProperties& properties ) :
	m_rootNode( root ),
	m_currentNode( root ),
	m_properties( properties )
{}
zpXmlSerializedInput::~zpXmlSerializedInput() {
	m_nodeStack.clear();
	m_siblingStack.clear();
	m_properties.clear();
}

zp_bool zpXmlSerializedInput::readBoolean( zp_bool* value, const zp_char* name ) {
	zpString out;

	readValueOrProperty( name, out );

	if( out.isEmpty() ) return false;
	*value = out.compareTo( "true" ) == 0;
	return true;
}

zp_bool zpXmlSerializedInput::readByte( zp_byte* value, const zp_char* name ) {
	zpString out;

	readValueOrProperty( name, out );

	if( out.isEmpty() ) return false;
	*value = (zp_byte)atoi( out.c_str() );
	return true;
}
zp_bool zpXmlSerializedInput::readChar( zp_char* value, const zp_char* name ) {
	zpString out;

	readValueOrProperty( name, out );

	if( out.isEmpty() ) return false;
	*value = out[ 0 ];
	return true;
}
zp_bool zpXmlSerializedInput::readShort( zp_short* value, const zp_char* name ) {
	zpString out;

	readValueOrProperty( name, out );

	if( out.isEmpty() ) return false;
	*value = (zp_short)atoi( out.c_str() );
	return true;
}
zp_bool zpXmlSerializedInput::readInt( zp_int* value, const zp_char* name ) {
	zpString out;

	readValueOrProperty( name, out );

	if( out.isEmpty() ) return false;
	*value = (zp_int)atoi( out.c_str() );
	return true;
}
zp_bool zpXmlSerializedInput::readLong( zp_long* value, const zp_char* name ) {
	zpString out;

	readValueOrProperty( name, out );

	if( out.isEmpty() ) return false;
	*value = (zp_long)atol( out.c_str() );
	return true;
}

zp_bool zpXmlSerializedInput::readFloat( zp_float* value, const zp_char* name ) {
	zpString out;

	readValueOrProperty( name, out );

	if( out.isEmpty() ) return false;
	*value = (zp_float)atof( out.c_str() );
	return true;
}
zp_bool zpXmlSerializedInput::readDouble(  zp_double* value, const zp_char* name ) {
	zpString out;

	readValueOrProperty( name, out );

	if( out.isEmpty() ) return false;
	*value = (zp_double)atof( out.c_str() );
	return true;
}

zp_bool zpXmlSerializedInput::readString( zpString* value, const zp_char* name ) {
	zpString out;

	readValueOrProperty( name, out );

	if( out.isEmpty() ) return false;
	*value = out;
	return true;
}

zp_bool zpXmlSerializedInput::readSerializable( zpSerializable** value, const zp_char* name ) {
	zpSerializable* obj;
	if( name ) {
		obj = zpRegisterSerializable::createSerializable( name );
	} else {
		zpString n;
		zp_bool usePrefab = false;
		zp_bool useAttr = false;

		if( !m_currentNode->children.isEmpty() ) {
			zpXmlNode* prefab = m_currentNode->children[ 0 ];
			if( prefab->name == "Prefab" ) {
				n = prefab->attributes[ "name" ];

				if( prefab->children.isEmpty() ) {
					*value = zpPrefabs::getInstance()->loadPrefab( n );
					return true;
				} else {
					zpProperties overrides;

					prefab->children.foreach( [ &overrides ]( zpXmlNode* node ) {
						if( node->name == "Override" ) {
							overrides[ node->attributes[ "key" ] ] = node->attributes[ "value" ];
						} else if( node->name == "File" ) {
							overrides.load( node->attributes[ "name" ] );
						}
					} );

					*value = zpPrefabs::getInstance()->loadPrefabWithOverrides( n, overrides );
					return true;
				}
			}
		}
	}

	if( obj ) obj->deserialize( this );
	*value = obj;
	
	return obj != ZP_NULL;
}

zp_bool zpXmlSerializedInput::readBlock( const zp_char* name ) {
	if( m_siblingStack.isEmpty() || m_siblingStack.front() != name ) {

		m_nodeStack.pushFront( m_currentNode );
		m_siblingStack.pushFront( name );

		zpXmlNode* element = 0;
	} else {

	}
	return m_currentNode != ZP_NULL;
}
zp_bool zpXmlSerializedInput::endBlock() {
	return true;
}

void zpXmlSerializedInput::readValueOrProperty( const zp_char* name, zpString& outString ) {
	if( name == ZP_NULL ) {
		outString = m_currentNode->value;
	} else if( name[ 0 ] == '@' ) {
		m_currentNode->attributes.find( &name[ 1 ], &outString );
	} else {
		zpXmlNode* node = ZP_NULL;
		zpXmlNode* current = m_currentNode;
		if( m_currentNode->parent->children.findIf( [ &name, &current ]( zpXmlNode* n ){
			return n != current && n->name == name;
		}, &node ) )
		{
			outString = node->name;
		}
	}

	zpString n( name ), def;
	if( isProperty( n, def ) ) {
		if( !m_properties.find( n, &outString ) ) {
			outString = def;
		}
	}
}
zp_bool zpXmlSerializedInput::isProperty( zpString& name, zpString& outDefaultValue ) {
	zp_uint length = name.length();
	if( length < 2 ) return false;

	zp_bool isProp = name[ 0 ] == '{' && name[ length - 1 ] == '}';
	if( isProp ) {
		name = name.substring( 1, length - 2 );

		zp_uint def = name.lastIndexOf( '|' );
		if( def != zpString::npos ) {
			outDefaultValue = name.substring( def + 1 );
			name = name.substring( 0, def );
		}
	}

	return isProp;
}
