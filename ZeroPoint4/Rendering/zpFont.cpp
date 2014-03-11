#include "zpRendering.h"

zp_bool zpFontResource::load( const zp_char* filename, zpRenderingPipeline* pipeline )
{
	m_filename = filename;

	zpBison font;
	zp_bool loaded = false;
	loaded = font.readFromFile( m_filename );
	if( loaded )
	{
		const zpBison::Value& root = font.root();

		// info
		m_resource.face = root[ "face" ].asCString();
		m_resource.size = root[ "size" ].asInt();
		m_resource.isBold = root[ "bold" ].asBool();
		m_resource.isItalic = root[ "italic" ].asBool();
		m_resource.charset = root[ "charset" ].asCString();
		m_resource.isUnicode = root[ "unicode" ].asBool();
		m_resource.stretchH = root[ "stretchh" ].asInt();
		m_resource.isSmooth = root[ "smooth" ].asBool();
		m_resource.aa = root[ "aa" ].asInt();

		const zpBison::Value& padding = root[ "padding" ];
		m_resource.padding = zpRecti( padding[0].asInt(), padding[1].asInt(), padding[2].asInt(), padding[3].asInt() );

		const zpBison::Value& spacing = root[ "spacing" ];
		m_resource.spacing.set( spacing[0].asInt(), spacing[1].asInt() );

		m_resource.outline = root[ "outline" ].asInt();

		// common
		m_resource.lineHeight = root[ "lineheight" ].asInt();
		m_resource.base = root[ "base" ].asInt();
		m_resource.scaleW = root[ "scalew" ].asInt();
		m_resource.scaleH = root[ "scaleh" ].asInt();
		m_resource.isPacked = root[ "packed" ].asBool();
		m_resource.alphaChannel = root[ "alphachnl" ].asInt();
		m_resource.redChannel = root[ "redchnl" ].asInt();
		m_resource.greenChannel = root[ "greenchnl" ].asInt();
		m_resource.blueChannel = root[ "bluechnl" ].asInt();

		// pages
		const zpBison::Value& pages = root[ "pages" ];
		m_resource.pages.reserve( pages.size() );
		pages.foreachArray( [ this, pipeline ]( const zpBison::Value& page )
		{
			pipeline->getTextureContentManager()->getResource( page.asCString(), m_resource.pages.pushBackEmpty() );
		} );

		// chars
		const zpBison::Value& chars = root[ "chars" ];
		m_resource.glyphs.resize( 256 );
		chars.foreachArray( [ this ]( const zpBison::Value& c )
		{
			zp_int id = c[ "id" ].asInt();
			zpFontGlyph& glyph = m_resource.glyphs[ id ];
			glyph.character = (zp_char)id;
			glyph.x = c[ "x" ].asInt();
			glyph.y = c[ "y" ].asInt();
			glyph.width = c[ "width" ].asInt();
			glyph.height = c[ "height" ].asInt();
			glyph.xOffset = c[ "xoffset" ].asInt();
			glyph.yOffset = c[ "yoffset" ].asInt();
			glyph.xAdvance = c[ "xadvance" ].asInt();
			glyph.page = c[ "page" ].asInt();
			glyph.channel = c[ "channel" ].asInt();
		} );

		// kernings
		const zpBison::Value& kernings = root[ "kernings" ];
		m_resource.kernings.reserve( kernings.size() );
		kernings.foreachArray( [ this ]( const zpBison::Value& kern )
		{
			zpFontGlyphKerning& k = m_resource.kernings.pushBackEmpty();
			k.first = kern[ "first" ].asInt();
			k.second = kern[ "second" ].asInt();
			k.amount = kern[ "amount" ].asInt();
		} );
	}

	return true;
}
void zpFontResource::unload( zpRenderingPipeline* pipeline)
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
