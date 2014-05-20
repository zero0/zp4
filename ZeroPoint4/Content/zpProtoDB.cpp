#include "zpContent.h"

zpProtoDBHandle::zpProtoDBHandle()
	: m_category()
	, m_protoId()
	, m_protoDB( ZP_NULL )
	, m_index( ZP_PROTODB_INVALID_HANDLE )
	, m_data( ZP_NULL )
{}
zpProtoDBHandle::~zpProtoDBHandle()
{
	invalidate();
}

zp_bool zpProtoDBHandle::isValid() const
{
	return m_index != ZP_PROTODB_INVALID_HANDLE;
}

const void* zpProtoDBHandle::getData() const
{
	if( m_index == ZP_PROTODB_INVALID_HANDLE )
	{
		m_index = m_protoDB->m_entries.indexOf( m_protoId );
	}
	if( m_data == ZP_NULL )
	{
		m_data = (void*)( m_protoDB->m_database.getData() + ( m_index * m_protoDB->m_stride ) );
	}
	return m_data;
}

const zpString& zpProtoDBHandle::getProtoId() const
{
	return m_protoId;
}

void zpProtoDBHandle::invalidate() const
{
	m_index = ZP_PROTODB_INVALID_HANDLE;
	m_data = ZP_NULL;
}


zpProtoDB::zpProtoDB()
{}
zpProtoDB::~zpProtoDB()
{}

void zpProtoDB::initialize( zpProtoDBCreateFunc create, zp_uint stride )
{
	m_create = create;
	m_stride = stride;
}

void zpProtoDB::setup( const zpBison::Value& protoDb )
{
	// setup database
	zp_uint count = protoDb.size();
	m_database.reserve( m_stride * count );
	m_entries.reserve( count );

	zp_byte* data = (zp_byte*)m_database.getData();
	protoDb.foreachObject( [ this, &data ]( const zpBison::Value& k, const zpBison::Value& v )
	{
		m_entries.pushBackEmpty() = k.asCString();

		m_create( data, m_stride, v );

		data += m_stride;
	} );
}

void zpProtoDB::shutdown()
{
	m_entries.clear();
	m_database.clear();
}

void zpProtoDB::destroy()
{
}



zpProtoDBManager::zpProtoDBManager()
	: m_protoDbFile()
	, m_lastModifyTime( 0L )
{}
zpProtoDBManager::~zpProtoDBManager()
{}

void zpProtoDBManager::initialize( zp_uint numCatagories )
{
	m_catagories.reserve( numCatagories );
	m_protoDBs.reserve( numCatagories );

	m_handles.resize( 64 );
	zpProtoDBHandle* b = m_handles.begin();
	zpProtoDBHandle* e = m_handles.end();
	for( ; b != e; ++b )
	{
		m_freeHandles.pushBack( b );
	}
}
void zpProtoDBManager::initializeCategory( const zp_char* category, zpProtoDBCreateFunc create, zp_uint stride )
{
	zpString cat( category );
	if( m_catagories.indexOf( cat ) < 0 )
	{
		m_catagories.pushBack( cat );
		m_protoDBs.pushBackEmpty().initialize( create, stride );
	}
	else
	{
		ZP_ASSERT( false, "Category '%s' already exists", category );
	}
}
void zpProtoDBManager::setup()
{
	zp_bool ok;

	zpBison protoDB;
	ok = protoDB.readFromFile( m_protoDbFile );

	const zpBison::Value& root = protoDB.root();

	for( zp_uint i = 0, imax = m_catagories.size(); i < imax; ++i )
	{
		const zp_char* cat = m_catagories[ i ].str();
		m_protoDBs[ i ].setup( root[ cat ] );
	}
}
void zpProtoDBManager::shutdown()
{
	m_protoDBs.foreach( []( zpProtoDB& db )
	{
		db.shutdown();
	} );
}
void zpProtoDBManager::destroy()
{
	m_catagories.clear();

	m_protoDBs.foreach( []( zpProtoDB& db )
	{
		db.destroy();
	} );
	m_protoDBs.clear();
}

void zpProtoDBManager::reloadProtoDB()
{
	shutdown();
	setup();
}

#if ZP_USE_HOT_RELOAD
void zpProtoDBManager::reloadChangedProtoDB()
{
	zp_long mtime = zpFile::getFileModificationTime( m_protoDbFile.str() );
	if( mtime != m_lastModifyTime )
	{
		m_lastModifyTime = mtime;
		reloadProtoDB();
	}
}
#endif

void zpProtoDBManager::setProtoDBFile( const zp_char* protoDbFile )
{
	m_protoDbFile = protoDbFile;
}
const zpString& zpProtoDBManager::getProtoDBFile() const
{
	return m_protoDbFile;
}

const zpProtoDBHandle* zpProtoDBManager::getHandle( const zp_char* category, const zp_char* protoId )
{
	ZP_ASSERT( !m_freeHandles.isEmpty(), "Ran out of ProtoDB Handles" );

	zp_bool ok;
	zp_uint cat, proto;
	ok = m_catagories.findIndexIf( [ category ]( const zpString& c ) { return c == category; }, cat );
	if( !ok ) return ZP_NULL;

	zpProtoDB* db = &m_protoDBs[ cat ];
	ok = db->m_entries.findIndexIf( [ protoId ]( const zpString& p ) { return p == protoId; }, proto );
	if( !ok ) return ZP_NULL;


	zpProtoDBHandle* handle = m_freeHandles.back();
	m_freeHandles.popBack();
	m_usedHandles.pushBack( handle );

	handle->m_category = category;
	handle->m_protoId = protoId;
	handle->m_protoDB = db;
	handle->m_index = proto;
	handle->m_data = ZP_NULL;

	return handle;
}

void zpProtoDBManager::releaseHandle( zpProtoDBHandle*& handle )
{
	handle->invalidate();
	m_usedHandles.eraseAll( handle );
	m_freeHandles.pushBack( handle );

	handle = ZP_NULL;
}

void zpProtoDBManager::invalidateHandles()
{
	zpProtoDBHandle** b = m_usedHandles.begin();
	zpProtoDBHandle** e = m_usedHandles.begin();
	for( ; b != e; ++b )
	{
		(*b)->invalidate();
	}
}
