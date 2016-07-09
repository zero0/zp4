#include "zpContent.h"

zpProtoDBHandle::zpProtoDBHandle()
    : m_category( ZP_NULL )
    , m_protoId( ZP_NULL )
    , m_protoDB( ZP_NULL )
    , m_mod( 0 )
    , m_index( ZP_PROTODB_INVALID_HANDLE )
    , m_data( ZP_NULL )
{}
zpProtoDBHandle::~zpProtoDBHandle()
{
    invalidate();

    m_category = ZP_NULL;
    m_protoId = ZP_NULL;
    m_protoDB = ZP_NULL;
}

zp_bool zpProtoDBHandle::isValid() const
{
    return m_index != ZP_PROTODB_INVALID_HANDLE;
}

const void* zpProtoDBHandle::getData() const
{
    if( m_mod != m_protoDB->getModVersion() )
    {
        m_index = ZP_PROTODB_INVALID_HANDLE;
        m_data = ZP_NULL;
    }

    if( m_index == ZP_PROTODB_INVALID_HANDLE )
    {
        m_index = m_protoDB->findProtoEntry( m_protoId );
        m_data = ZP_NULL;
    }
    
    if( m_data == ZP_NULL )
    {
        m_data = m_protoDB->getProtoEntry( m_index );
    }

    return m_data;
}

const zp_char* zpProtoDBHandle::getProtoId() const
{
    return m_protoId;
}
const zp_char* zpProtoDBHandle::getCategory() const
{
    return m_category;
}

void zpProtoDBHandle::invalidate()
{
    m_index = ZP_PROTODB_INVALID_HANDLE;
    m_data = ZP_NULL;
}
void zpProtoDBHandle::setup( const zp_char* category, const zp_char* protoId, const zpProtoDB* protoDB, zp_int index )
{
    m_category = category;
    m_protoId = protoId;
    m_protoDB = protoDB;
    m_mod = protoDB->getModVersion();
    m_index = index;
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
    m_mod = 0;
}

void zpProtoDB::setup( const zp_char* category, const zpBison::Value& protoDb )
{
    m_category = category;

    // setup database
    zp_uint count = protoDb.size();
    m_database.reserve( m_stride * count );
    m_entries.reserve( count );

    // increment mod count
    ++m_mod;

    zp_byte* data = (zp_byte*)m_database.getData();
    protoDb.foreachObject( [ this, &data ]( const zpBison::Value& k, const zpBison::Value& v )
    {
        m_entries.pushBackEmpty() = k.asCString();

        m_create( data, m_stride, v );

        data += m_stride;
    } );
}

void zpProtoDB::teardown()
{
    m_entries.clear();
    m_database.clear();
}

void zpProtoDB::destroy()
{
    m_entries.destroy();
    m_database.destroy();
}

zp_int zpProtoDB::findProtoEntry( const zp_char* protoId ) const
{
    zp_size_t index;
    zp_bool ok = m_entries.findIndexIf( [ protoId ]( const zpString& s ) { return s == protoId; }, index );
    return ok ? (zp_int)index : ZP_PROTODB_INVALID_HANDLE;
}

const void* zpProtoDB::getProtoEntry( zp_int index ) const
{
    const void* d = (const void*)( m_database.getData() + ( index * m_stride ) );
    return d;
}

const zpArrayList< zpString > zpProtoDB::getAllProtoIDs() const
{
    return m_entries;
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
}
void zpProtoDBManager::initializeCategory( const zp_char* category, zpProtoDBCreateFunc create, zp_uint stride )
{
    zpString cat( category );
    if( m_catagories.indexOf( cat ) == zpArrayList< zpString >::npos )
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
        m_protoDBs[ i ].setup( cat, root[ cat ] );
    }
}
void zpProtoDBManager::teardown()
{
    m_protoDBs.foreach( []( zpProtoDB& db )
    {
        db.teardown();
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
    teardown();
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

zp_bool zpProtoDBManager::getPrototype( const zp_char* category, const zp_char* protoId, zpProtoDBHandle& handle )
{
    zp_bool ok;
    zp_uint index;
    const zpProtoDB* db;

    ok = m_protoDBs.findIf( [ category ]( const zpProtoDB& protoDB ) { return protoDB.getCategory() == category; }, &db );
    if( !ok ) return false;

    index = db->findProtoEntry( protoId );
    if( index == ZP_PROTODB_INVALID_HANDLE ) return false;

    handle.setup( category, protoId, db, index );

    return true;
}
