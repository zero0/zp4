#include "zpCommon.h"

zpTags::zpTags()
{
}
zpTags::~zpTags()
{
    clear();
}

void zpTags::clear()
{
    m_tags.clear();
}

zp_int zpTags::getTag( const zpString& tag )
{
    zp_size_t index;
    if( !m_tags.findIndexIf( [ &tag ]( const zpString& t ) {
        return tag == t;
    }, index ) )
    {
        index = m_tags.size();
        m_tags.pushBack( tag );
    }

    return (zp_int)index;
}
zp_int zpTags::getTag( const zp_char* tag )
{
    zp_size_t index;
    if( !m_tags.findIndexIf( [ &tag ]( const zpString& t ) {
        return tag == t;
    }, index ) )
    {
        index = m_tags.size();
        m_tags.pushBackEmpty() = tag;
    }

    return (zp_int)index;
}

const zpString& zpTags::getTagName( zp_int tag ) const
{
    return m_tags[ (zp_size_t)tag ];
}
