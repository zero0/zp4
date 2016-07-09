#include "zpCore.h"
/*
zpReferencedObject::zpReferencedObject() : m_referenceCount( 1 ), m_isMarked( true ) {}
zpReferencedObject::~zpReferencedObject() {}

void zpReferencedObject::addReference() const {
    ++m_referenceCount;
}
zp_bool zpReferencedObject::removeReference() const {
    if( m_referenceCount == 0 ) return true;

    --m_referenceCount;
    if( m_referenceCount == 0 ) {
        if( m_isMarked ) delete (zpReferencedObject*)this;
        return true;
    }
    return false;
}

zp_uint zpReferencedObject::getReferenceCount() const {
    return m_referenceCount;
}

void zpReferencedObject::markForAutoDelete( zp_bool marked ) const {
    m_isMarked = marked;
}
zp_bool zpReferencedObject::isMarkedForAutoDelete() const {
    return m_isMarked;
}
*/