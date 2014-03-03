#include "zpRendering.h"

zp_bool zpFontResource::load( const zp_char* filename )
{
	m_filename = filename;

	zpBison font;
	zp_bool loaded = false;
	loaded = font.readFromFile( m_filename );
	if( loaded )
	{
		const zpBison::Value& root = font.root();

		// info
		m_resource.face = root[ "Face" ].asCString();
		m_resource.size = root[ "Size" ].asInt();
		m_resource.isBold = root[ "Bold" ].asBool();
		m_resource.isItalic = root[ "Italic" ].asBool();
		m_resource.charset = root[ "Charset" ].asCString();
		m_resource.isUnicode = root[ "Unicode" ].asBool();
		m_resource.stretchH = root[ "StretchH" ].asInt();
		m_resource.isSmooth = root[ "Smooth" ].asBool();
		m_resource.aa = root[ "AA" ].asInt();

		const zpBison::Value& padding = root[ "Padding" ];
		m_resource.padding = zpRecti( padding[0].asInt(), padding[1].asInt(), padding[2].asInt(), padding[3].asInt() );

		const zpBison::Value& spacing = root[ "Spacing" ];
		m_resource.spacing.set( spacing[0].asInt(), spacing[1].asInt() );

		m_resource.outline = root[ "Outline" ].asInt();

		// common
		m_resource.lineHeight = root[ "LineHeight" ].asInt();
		m_resource.base = root[ "Base" ].asInt();
		m_resource.scaleW = root[ "ScaleW" ].asInt();
		m_resource.scaleH = root[ "ScaleH" ].asInt();
		m_resource.isPacked = root[ "Packed" ].asBool();
		m_resource.alphaChannel = root[ "AlphaChannel" ].asInt();
		m_resource.redChannel = root[ "RedChannel" ].asInt();
		m_resource.greenChannel = root[ "GreenChannel" ].asInt();
		m_resource.blueChannel = root[ "BlueChannel" ].asInt();

		// pages
		const zpBison::Value& pages = root[ "Pages" ];
		m_resource.pages.reserve( pages.size() );
		pages.foreachArray( [ this ]( const zpBison::Value& page )
		{
			m_resource.pages.pushBack( page.asString() );
		} );

		// chars
		const zpBison::Value& chars = root[ "Chars" ];
		m_resource.glyphs.resize( 256 );
		chars.foreachArray( [ this ]( const zpBison::Value& c )
		{
			zp_int id = c[ "Id" ].asInt();
			zpFontGlyph& glyph = m_resource.glyphs[ id ];
			glyph.character = (zp_char)id;
			glyph.x = c[ "X" ].asInt();
			glyph.y = c[ "Y" ].asInt();
			glyph.width = c[ "Width" ].asInt();
			glyph.height = c[ "Height" ].asInt();
			glyph.xOffset = c[ "XOffset" ].asInt();
			glyph.yOffset = c[ "YOffset" ].asInt();
			glyph.xAdvance = c[ "XAdvance" ].asInt();
			glyph.page = c[ "Page" ].asInt();
			glyph.channel = c[ "Channel" ].asInt();
		} );

		// kernings
		const zpBison::Value& kernings = root[ "Kernings" ];
		m_resource.kernings.reserve( kernings.size() );
		kernings.foreachArray( [ this ]( const zpBison::Value& kern )
		{
			zpFontGlyphKerning& k = m_resource.kernings.pushBackEmpty();
			k.first = kern[ "First" ].asInt();
			k.second = kern[ "Second" ].asInt();
			k.amount = kern[ "Amount" ].asInt();
		} );
	}

	return true;
}
void zpFontResource::unload()
{
	m_resource.glyphs.clear();
	m_resource.pages.clear();
	m_resource.kernings.clear();
}


zp_bool zpFontContentManager::createResource( zpFontResource* res, const zp_char* filename )
{
	return res->load( filename );
}
void zpFontContentManager::destroyResource( zpFontResource* res )
{
	res->unload();
}
