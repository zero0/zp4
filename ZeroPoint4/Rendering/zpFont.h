#pragma once
#ifndef ZP_FONT_H
#define ZP_FONT_H

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

class zpFontResource;
class zpFontResourceInstance;
class zpFontContentManager;

class zpFontResource : public zpResource< int >
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
public:
	zpFontContentManager();
	virtual ~zpFontContentManager();

private:
	zp_bool createResource( zpFontResource* res, const zp_char* filename );
	void destroyResource( zpFontResource* res );
	void initializeInstance( zpFontResourceInstance& instance );

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
