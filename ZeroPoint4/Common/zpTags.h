#pragma once
#ifndef ZP_TAGS_H
#define ZP_TAGS_H

class zpTags
{
public:
    zpTags();
    ~zpTags();

    void clear();

    zp_int getTag( const zpString& tag );
    zp_int getTag( const zp_char* tag );

    const zpString& getTagName( zp_int tagIndex ) const;

private:
    zpFixedArrayList< zpString, 64 > m_tags;
};

#endif
