#pragma once
#ifndef ZP_FONT_H
#define ZP_FONT_H

enum zpFontAlignment
{
	ZP_FONT_ALIGNMENT_LEFT,
	ZP_FONT_ALIGNMENT_CENTER,
	ZP_FONT_ALIGNMENT_RIGHT,

	zpFontAlignment_Count,
};

struct zpFontGlyphKerning
{
	zp_char first;
	zp_char second;
	zp_int amount;
};

struct zpFontGlyph
{
	zp_char character;
	zp_int x;
	zp_int y;
	zp_int width;
	zp_int height;
	zp_int xOffset;
	zp_int yOffset;
	zp_int xAdvance;
	zp_int page;
	zp_int channel;
};

struct zpFontSet
{
	zpString face;
	zpString charset;

	zp_int size;
	zp_int stretchH;
	zp_int width;
	zp_int height;
	zp_int aa;
	zp_int outline;

	zpRecti padding;
	zpVector2i spacing;

	zp_bool isBold;
	zp_bool isItalic;
	zp_bool isUnicode;
	zp_bool isSmooth;

	zp_int lineHeight;
	zp_int base;
	zp_int scaleW;
	zp_int scaleH;

	zp_bool isPacked;
	zp_int alphaChannel;
	zp_int redChannel;
	zp_int greenChannel;
	zp_int blueChannel;

	zpFixedArrayList< zpFontGlyph, 256 > glyphs;
	zpArrayList< zpString > pages;
	zpArrayList< zpFontGlyphKerning > kernings;
};

class zpFontResource;
class zpFontResourceInstance;
class zpFontContentManager;

class zpFontResource : public zpResource< zpFontSet >
{
private:
	zp_bool load( const zp_char* filename );
	void unload();

	friend class zpFontContentManager;
};


class zpFontResourceInstance : public zpResourceInstance< zpFontResource >
{};


class zpFontContentManager : public zpContentManager< zpFontResource, zpFontResourceInstance, zpFontContentManager, 4 >
{
private:
	zp_bool createResource( zpFontResource* res, const zp_char* filename );
	void destroyResource( zpFontResource* res );
	void initializeInstance( zpFontResourceInstance& instance ) {}

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
