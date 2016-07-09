#pragma once
#ifndef ZP_PROTODB_H
#define ZP_PROTODB_H

enum
{
    ZP_PROTODB_INVALID_HANDLE = -1,
};

class zpProtoDB;
class zpProtoDBManager;

class zpProtoDBHandle
{
public:
    zpProtoDBHandle();
    ~zpProtoDBHandle();

    zp_bool isValid() const;

    template< typename T >
    const T* getData() const { return (const T*)getData(); }

    const void* getData() const;

    const zp_char* getProtoId() const;
    const zp_char* getCategory() const;

    void invalidate();
    void setup( const zp_char* category, const zp_char* protoId, const zpProtoDB* protoDB, zp_int index );

private:
    const zp_char* m_category;
    const zp_char* m_protoId;

    const zpProtoDB* m_protoDB;

    mutable zp_uint m_mod;
    mutable zp_int m_index;
    mutable const void* m_data;
};

typedef void (*zpProtoDBCreateFunc)( void* data, zp_uint stride, const zpBison::Value& def );

class zpProtoDB
{
public:
    zpProtoDB();
    ~zpProtoDB();

    void initialize( zpProtoDBCreateFunc create, zp_uint stride );
    void setup( const zp_char* category, const zpBison::Value& protoDb );
    void teardown();
    void destroy();

    const zpString& getCategory() const { return m_category; }
    zp_uint getModVersion() const { return m_mod; }

    zp_int findProtoEntry( const zp_char* protoId ) const;
    const void* getProtoEntry( zp_int index ) const;

    const zpArrayList< zpString > getAllProtoIDs() const;

private:
    zp_uint m_stride;
    zp_uint m_mod;

    zpString m_category;
    zpArrayList< zpString > m_entries;
    zpDataBuffer m_database;

    zpProtoDBCreateFunc m_create;
};

#define ZP_PROTODB_CATEGORY( p, c )        (p)->initializeCategory( #c, zpProtoDBCreate##c, sizeof( c ) )

class zpProtoDBManager
{
public:
    zpProtoDBManager();
    ~zpProtoDBManager();

    void initialize( zp_uint numCatagories );
    void initializeCategory( const zp_char* category, zpProtoDBCreateFunc create, zp_uint stride );
    void setup();
    void teardown();
    void destroy();

    void reloadProtoDB();

#if ZP_USE_HOT_RELOAD
    void reloadChangedProtoDB();
#endif

    zp_bool isProtoDBLoaded() const;

    void setProtoDBFile( const zp_char* protoDbFile );
    const zpString& getProtoDBFile() const;

    zp_bool getPrototype( const zp_char* category, const zp_char* protoId, zpProtoDBHandle& handle );

private:
    zp_bool m_isLoaded;

    zpString m_protoDbFile;
    zp_long m_lastModifyTime;

    zpArrayList< zpString > m_catagories;
    zpArrayList< zpProtoDB > m_protoDBs;
};

#include "zpProtoDB.inl"

#endif
