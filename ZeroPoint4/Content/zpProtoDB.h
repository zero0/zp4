#pragma once
#ifndef ZP_PROTODB_H
#define ZP_PROTODB_H

enum
{
	ZP_PROTODB_INVALID_HANDLE = -1,
};

template< typename T >
class zpProtoDB;

template< typename T >
class zpProtoDBHandle
{
public:
	~zpProtoDBHandle();

	zp_bool isValid() const;
	const T* getData() const;

	const zpString& getProtoId() const;

	void invalidate() const;

private:
	zpProtoDBHandle();

	zpString m_protoId;
	zpProtoDB< T >* m_protoDB;

	mutable zp_int m_index;
	mutable T* m_data;

	friend class zpProtoDB< T >;
};

template< typename T >
class zpProtoDB
{
public:
	zpProtoDB();
	~zpProtoDB();

	void initialize();
	void setup( const zpBison::Value& protoDb );
	void shutdown();
	void destroy();

	const zpProtoDBHandle< T >* getHandle( const zp_char* protoId );
	void releaseHandle( const zpProtoDBHandle< T >*& handle );

	void invalidateHandles();

private:
	zpArrayList< zpString > m_entries;
	zpArrayList< T > m_database;

	zpFixedArrayList< zpProtoDBHandle< T >,  32 > m_handles;
	zpFixedArrayList< zpProtoDBHandle< T >*, 32 > m_freeHandles;
	zpFixedArrayList< zpProtoDBHandle< T >*, 32 > m_usedHandles;

	friend class zpProtoDBHandle< T >;
};

#include "zpProtoDB.inl"

class zpProtoDBManager
{
public:
	zpProtoDBManager();
	~zpProtoDBManager();

	void initialize( const zp_char* protoDbFile );
	void setup();
	void shutdown();
	void destroy();

	void reloadProtoDB();

#if ZP_USE_HOT_RELOAD
	void reloadChangedProtoDB();
#endif

	const zpString& getProtoDBFile() const;

#ifdef ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#define  ZP_PROTODB_CATEGORY( c )	zpProtoDB< c >* get##c##ProtoDB();
#include ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#endif

private:
	zpString m_protoDbFile;
	zp_long m_lastModifyTime;

#ifdef ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#define  ZP_PROTODB_CATEGORY( c )	zpProtoDB< c > m_protoDB##c##;
#include ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#endif
};

#endif
