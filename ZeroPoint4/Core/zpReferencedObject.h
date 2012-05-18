#pragma once
#ifndef ZP_REFERENCED_OBJECT_H
#define ZP_REFERENCED_OBJECT_H

class ZP_NO_VTABLE zpReferencedObject {
public:
	zpReferencedObject();
	virtual ~zpReferencedObject();

	void addReference() const;
	void removeReference() const;

	zp_uint getReferenceCount() const;

private:
	mutable zp_uint m_referenceCount;
};

#endif