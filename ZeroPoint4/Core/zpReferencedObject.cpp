#include "zpCore.h"

zpReferencedObject::zpReferencedObject() : m_referenceCount( 1 ) {}
zpReferencedObject::~zpReferencedObject() {}

void zpReferencedObject::addReference() const {
	++m_referenceCount;
}
void zpReferencedObject::removeReference() const {
	--m_referenceCount;
	if( m_referenceCount == 0 ) {
		delete (zpReferencedObject*)this;
	}
}

zp_uint zpReferencedObject::getReferenceCount() const {
	return m_referenceCount;
}
