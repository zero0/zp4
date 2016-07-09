#pragma once
#ifndef ZP_REFERENCED_OBJECT_H
#define ZP_REFERENCED_OBJECT_H

ZP_PURE_INTERFACE zpReferencedObject {
public:
    virtual void addReference() const = 0;
    virtual zp_bool removeReference() const = 0;

    virtual zp_uint getReferenceCount() const = 0;

    virtual void markForAutoDelete( zp_bool marked ) const = 0;
    virtual zp_bool isMarkedForAutoDelete() const = 0;
};

#endif