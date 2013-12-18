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

zp_bool zpFontResource::load( const zp_char* filename )
{
	return true;
}
void zpFontResource::unload()
{
}
