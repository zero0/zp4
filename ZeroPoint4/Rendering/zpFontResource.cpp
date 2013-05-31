#include "zpRendering.h"

zpFontGlyph::zpFontGlyph()
	: character( '\0' )
	, x( 0 )
	, y( 0 )
	, width( 0 )
	, height( 0 )
	, xOffset( 0 )
	, yOffset( 0 )
	, xAdvance( 0 )
	, kerning( ZP_NULL )
{}
zpFontGlyph::~zpFontGlyph() {
	if( kerning ) {
		kerning->clear();
		delete kerning;
		kerning = ZP_NULL;
	}
}

zpFontSet::zpFontSet()
	: face()
	, file()
	, size( 0 )
	, lineHeight( 0 )
	, base( 0 )
	, width( 0 )
	, height( 0 )
	, isBold( false )
	, isItalic( false )
{}
zpFontSet::~zpFontSet() {
	glyphs.clear();
}

zpFontResource::zpFontResource() {}
zpFontResource::~zpFontResource() {}

zp_bool zpFontResource::load() {
#if 0
	zpXmlParser xml;
	if( !xml.parseFile( getFilename(), true ) ) return false;
	//xml.writeToFile( xml.getRootNode(), "" );
	if( xml.push( "font" ) ) {
		if( xml.push( "info" ) ) {
			
			m_fontSet.face = xml.getCurrentNode()->attributes.getString( "face" );
			m_fontSet.size = xml.getCurrentNode()->attributes.getInt( "size" );
			m_fontSet.isBold = xml.getCurrentNode()->attributes.getInt( "bold" ) == 1;
			m_fontSet.isItalic = xml.getCurrentNode()->attributes.getInt( "italic" ) == 1;
			
			xml.pop();
		}

		if( xml.push( "common" ) ) {

			m_fontSet.lineHeight = xml.getCurrentNode()->attributes.getInt( "lineHeight" );
			m_fontSet.base = xml.getCurrentNode()->attributes.getInt( "base" );
			m_fontSet.width = xml.getCurrentNode()->attributes.getInt( "scaleW" );
			m_fontSet.height = xml.getCurrentNode()->attributes.getInt( "scaleH" );

			xml.pop();
		}

		if( xml.push( "pages" ) ) {

			if( xml.push( "page" ) ) {

				m_fontSet.file = xml.getCurrentNode()->attributes.getString( "file" );

				xml.pop();
			}

			xml.pop();
		}

		if( xml.push( "chars" ) ) {

			zp_int count = xml.getCurrentNode()->attributes.getString( "count" );
			
			if( xml.push( "char" ) ) {
				zpFontGlyph* glyph = ZP_NULL;
				do {
					zp_int id = xml.getCurrentNode()->attributes.getInt( "id" );
					glyph = &m_fontSet.glyphs[ id ];

					glyph->character = (zp_char)id;
					glyph->x = xml.getCurrentNode()->attributes.getInt( "x" );
					glyph->y = xml.getCurrentNode()->attributes.getInt( "y" );
					glyph->width = xml.getCurrentNode()->attributes.getInt( "width" );
					glyph->height = xml.getCurrentNode()->attributes.getInt( "height" );
					glyph->xOffset = glyph->x = xml.getCurrentNode()->attributes.getInt( "xoffset" );
					glyph->yOffset = xml.getCurrentNode()->attributes.getInt( "yoffset" );
					glyph->xAdvance = xml.getCurrentNode()->attributes.getInt( "xadvance" );

				} while( xml.next() );

				xml.pop();
			}

			xml.pop();
		}
		/*
		if( xml.push( "kernings" ) ) {

			if( xml.push( "kerning" ) ) {

				zpFontGlyph* glyph = ZP_NULL;
				do {
					zp_int first = xml.getCurrentNode()->attributes.getInt( "first" );
					zp_int second = xml.getCurrentNode()->attributes.getInt( "second" );
					zp_int amount = xml.getCurrentNode()->attributes.getInt( "amount" );

					glyph = &m_fontSet.glyphs[ first ];
					if( glyph->kerning == ZP_NULL ) {
						glyph->kerning = new zpArrayList<zpPair<zp_char, zp_int>>( 5 );
					}
					glyph->kerning->pushBack( zpPair<zp_char, zp_int>( second, amount ) );

				} while( xml.next() );

				xml.pop();
			}

			xml.pop();
		}
		*/
		xml.pop();
	}

	//if( !getContentManager()->loadResource( m_fontSet.file, m_fontSet.file ) ) return false;

	m_fontTexture = getContentManager()->createInstanceOfResource<zpTextureResource>( m_fontSet.file );
#endif
	return true;
}
void zpFontResource::unload() {
	m_fontSet.glyphs.clear();
}

const zpFontSet& zpFontResource::getFontSet() const {
	return m_fontSet;
}
//const zpResourceInstance<zpTextureResource>& zpFontResource::getTexture() const {
//	return m_fontTexture;
//}