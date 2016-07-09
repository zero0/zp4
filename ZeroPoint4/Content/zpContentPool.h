#pragma once
#ifndef ZP_CONTENT_POOL_H
#define ZP_CONTENT_POOL_H

template<typename T, zp_uint Count>
ZP_ABSTRACT_CLASS zpContentPool
{
public:
    zpContentPool();
    ~zpContentPool();

    void setup( zpMemorySystem* memory );
    void teardown();

    template<typename R>
    T* create( const R& param );

    template<typename R0, typename R1>
    T* create( const R0& param0, const R1& param1 );

    void destroy( T* obj );

    zp_uint size() const;

    T* getUsed( zp_uint index );
    zp_uint getUsedCount() const;

protected:
    virtual void onCreate( T* o ) {}
    virtual void onDestroy( T* o ) {}

    zpFixedArrayList< T*, Count > m_used;
    zpFixedArrayList< T*, Count > m_free;

    zpMemorySystem* m_memory;
};

#include "zpContentPool.inl"

#endif
