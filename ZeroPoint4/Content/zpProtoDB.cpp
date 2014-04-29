#include "zpContent.h"

zpProtoDBManager::zpProtoDBManager()
	: m_protoDbFile()
	, m_lastModifyTime( 0L )
{}
zpProtoDBManager::~zpProtoDBManager()
{}

void zpProtoDBManager::initialize( const zp_char* protoDbFile )
{
	m_protoDbFile = protoDbFile;

#ifdef ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#define  ZP_PROTODB_CATEGORY( c )	m_protoDB##c##.initialize();
#include ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#endif
}
void zpProtoDBManager::setup()
{
	zp_bool ok;

	zpBison protoDB;
	ok = protoDB.readFromFile( m_protoDbFile );

	const zpBison::Value& root = protoDB.root();

#ifdef ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#define  ZP_PROTODB_CATEGORY( c )	m_protoDB##c##.setup( root[ #c# ] );
#include ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#endif
}
void zpProtoDBManager::shutdown()
{
#ifdef ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#define  ZP_PROTODB_CATEGORY( c )	m_protoDB##c##.shutdown();
#include ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#endif
}
void zpProtoDBManager::destroy()
{
#ifdef ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#define  ZP_PROTODB_CATEGORY( c )	m_protoDB##c##.destroy();
#include ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#endif
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

const zpString& zpProtoDBManager::getProtoDBFile() const
{
	return m_protoDbFile;
}

#ifdef ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#define  ZP_PROTODB_CATEGORY( c )	zpProtoDB< c >* zpProtoDBManager::get##c##ProtoDB() { return m_protoDB##c##; }
#include ZP_PROTODB_CATEGORY_FILE
#undef ZP_PROTODB_CATEGORY
#endif
