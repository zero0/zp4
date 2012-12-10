#include "zpCore.h"
#include <malloc.h>

#if ZP_USE_MEMORY_SYSTEM
//void* operator new( zp_uint size ) {
//	return zpMemorySystem::getInstance()->allocate( size );
//}
//void* operator new[]( zp_uint size ) {
//	return zpMemorySystem::getInstance()->allocateArray( size );
//}
//
//void operator delete( void* ptr ) {
//	zpMemorySystem::getInstance()->deallocate( ptr );
//}
//void operator delete[]( void* ptr ) {
//	zpMemorySystem::getInstance()->deallocateArray( ptr );
//}
#else

#if ZP_USE_ALIGNMENT
void* operator new( zp_uint size ) {
	return zp_aligned_malloc( size, ZP_MALLOC_ALIGNMENT );
}
void* operator new[]( zp_uint size ) {
	return zp_aligned_calloc( size, 1, ZP_MALLOC_ALIGNMENT );
}

void operator delete( void* ptr ) {
	zp_aligned_free( ptr );
}
void operator delete[]( void* ptr ) {
	zp_aligned_free( ptr );
}
#else
void* operator new( zp_uint size ) {
	return zp_malloc( size );
}
void* operator new[]( zp_uint size ) {
	return zp_calloc( 1, size );
}

void operator delete( void* ptr ) {
	zp_free( ptr );
}
void operator delete[]( void* ptr ) {
	zp_free( ptr );
}
#endif

#endif

zpMemorySystem::zpMemorySystem()
	: m_numAllocs( 0 )
	, m_numArrayAllocs( 0 )
	, m_numDeallocs( 0 )
	, m_numArrayDeallocs( 0 )
	, m_memAllocated( 0 )
	, m_memDeallocated( 0 )
{}
zpMemorySystem::~zpMemorySystem() {}

zpMemorySystem zpMemorySystem::s_instance;
zpMemorySystem* zpMemorySystem::getInstance() {
	return &s_instance;
}

#if ZP_USE_ALIGNMENT
void* zpMemorySystem::allocate( zp_uint size ) {
	m_memAllocated += size;
	++m_numAllocs;
	return zp_aligned_malloc( size, ZP_MALLOC_ALIGNMENT );
}
void* zpMemorySystem::allocateArray( zp_uint size ) {
	m_memAllocated += size;
	++m_numArrayAllocs;
	return zp_aligned_calloc( size, 1, ZP_MALLOC_ALIGNMENT );
}

void zpMemorySystem::deallocate( void* ptr ) {
	++m_numDeallocs;
	zp_aligned_free( ptr );
}
void zpMemorySystem::deallocateArray( void* ptr ) {
	++m_numArrayDeallocs;
	zp_aligned_free( ptr );
}
#else
void* zpMemorySystem::allocate( zp_uint size ) {
	return zp_malloc( size );
}
void* zpMemorySystem::allocateArray( zp_uint size ) {
	return zp_calloc( 1, size );
}

void zpMemorySystem::deallocate( void* ptr ) {
	zp_free( ptr );
}
void zpMemorySystem::deallocateArray( void* ptr ) {
	zp_free( ptr );
}
#endif