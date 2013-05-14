#pragma once
#ifndef ZP_FONT_RESOURCE_H
#define ZP_FONT_RESOURCE_H

struct zpFontGlyph {
	zp_char character;
	zp_int x;
	zp_int y;
	zp_int width;
	zp_int height;
	zp_int xOffset;
	zp_int yOffset;
	zp_int xAdvance;
	zpArrayList<zpPair<zp_char, zp_int>>* kerning;

	zpFontGlyph();
	~zpFontGlyph();
};

struct zpFontSet {
	zpString face;
	zpString file;

	zp_uint size;
	zp_uint lineHeight;
	zp_uint base;
	zp_uint width;
	zp_uint height;

	zp_bool isBold;
	zp_bool isItalic;

	zpArray<zpFontGlyph, 256> glyphs;

	zpFontSet();
	~zpFontSet();
};

class zpFontResource : public zpResource {
public:
	zpFontResource();
	virtual ~zpFontResource();

	zp_bool load();
	void unload();

	const zpFontSet& getFontSet() const;
	const zpResourceInstance<zpTextureResource>& getTexture() const;

private:
	zpFontSet m_fontSet;
	zpResourceInstance<zpTextureResource> m_fontTexture;
};

//ZP_RESOURCE_INSTANCE_TEMPLATE_START( zpFontResource )
//ZP_RESOURCE_INSTANCE_TEMPLATE_END

#endif