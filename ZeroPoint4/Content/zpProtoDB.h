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

	const zpString& getProtoId() const;
	const zpString& getCategory() const;

	void invalidate() const;

private:
	zpString m_category;
	zpString m_protoId;

	zpProtoDB* m_protoDB;
	mutable zp_int m_index;
	mutable const void* m_data;

	friend class zpProtoDB;
	friend class zpProtoDBManager;
};

typedef void (*zpProtoDBCreateFunc)( void* data, zp_uint stride, const zpBison::Value& def );

class zpProtoDB
{
public:
	zpProtoDB();
	~zpProtoDB();

	void initialize( zpProtoDBCreateFunc create, zp_uint stride );
	void setup( const zpBison::Value& protoDb );
	void shutdown();
	void destroy();


private:
	zp_uint m_stride;
	zpArrayList< zpString > m_entries;
	zpDataBuffer m_database;

	zpProtoDBCreateFunc m_create;

	friend class zpProtoDBHandle;
	friend class zpProtoDBManager;
};

#define ZP_PROTODB_CATEGORY( p, c, create )		(p)->initializeCategory( #c, create, sizeof( c ) )

class zpProtoDBManager
{
public:
	zpProtoDBManager();
	~zpProtoDBManager();

	void initialize( zp_uint numCatagories );
	void initializeCategory( const zp_char* category, zpProtoDBCreateFunc create, zp_uint stride );
	void setup();
	void shutdown();
	void destroy();

	void reloadProtoDB();

#if ZP_USE_HOT_RELOAD
	void reloadChangedProtoDB();
#endif

	void setProtoDBFile( const zp_char* protoDbFile );
	const zpString& getProtoDBFile() const;

	const zpProtoDBHandle* getHandle( const zp_char* category, const zp_char* protoId );
	void releaseHandle( zpProtoDBHandle*& handle );
	
	void invalidateHandles();

private:
	zpString m_protoDbFile;
	zp_long m_lastModifyTime;

	zpArrayList< zpString > m_catagories;
	zpArrayList< zpProtoDB > m_protoDBs;

	zpFixedArrayList< zpProtoDBHandle,  64 > m_handles;
	zpFixedArrayList< zpProtoDBHandle*, 64 > m_freeHandles;
	zpFixedArrayList< zpProtoDBHandle*, 64 > m_usedHandles;
};

#include "zpProtoDB.inl"

#endif
