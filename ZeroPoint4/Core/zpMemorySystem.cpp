#include "zpCore.h"
#include <malloc.h>

#if ZP_USE_MEMORY_SYSTEM
void* operator new( zp_uint size ) {
	return zpMemorySystem::getInstance()->allocate( size );
}

void operator delete( void* ptr ) {
	zpMemorySystem::getInstance()->deallocate( ptr );
}
#else

#if ZP_USE_ALIGNMENT
void* operator new( zp_uint size ) {
	return zp_aligned_malloc( size, ZP_MALLOC_ALIGNMENT );
}

void operator delete( void* ptr ) {
	zp_aligned_free( ptr );
}
#else
void* operator new( zp_uint size ) {
	return zp_malloc( size );
}

void operator delete( void* ptr ) {
	zp_free( ptr );
}
#endif

#endif

zpMemorySystem::zpMemorySystem()
	: m_numAllocs( 0 )
	, m_numDeallocs( 0 )
	, m_memAllocated( 0 )
	, m_memDeallocated( 0 )
	, m_totalMemorySize( 0 )
	, m_allMemory( ZP_NULL )
	, m_alignedMemory( ZP_NULL )
{
	initialize( ZP_MEMORY_MB( 1 ) );
}
zpMemorySystem::~zpMemorySystem() {
	ZP_ASSERT_WARN( m_memAllocated - m_memDeallocated == 0, "Memory leak of %d", m_memAllocated - m_memDeallocated );

	ZP_SAFE_FREE( m_allMemory );
}

zpMemorySystem* zpMemorySystem::getInstance() {
	static zpMemorySystem memory;
	return &memory;
}

#if ZP_USE_ALIGNMENT
void* zpMemorySystem::allocate( zp_uint size ) {
	//m_memAllocated += size;
	//++m_numAllocs;
	return zp_aligned_malloc( size, ZP_MALLOC_ALIGNMENT );
}

void zpMemorySystem::deallocate( void* ptr ) {
	//++m_numDeallocs;
	zp_aligned_free( ptr );
}
#else
void* zpMemorySystem::allocate( zp_uint size )
{
	zp_uint alignedSize = ZP_MEMORY_ALIGN_SIZE( size + sizeof( zpMemoryBlock ) );

	zpMemoryBlock* freeBlock = m_blockTable[ ZP_MEMORY_TABLE_INDEX( alignedSize ) ];
	if( freeBlock == ZP_NULL )
	{
		for( zp_uint i = ZP_MEMORY_BLOCK_TABLE_SIZE - 1; i != 0 ; --i )
		{
			freeBlock = m_blockTable[ i ];
			if( freeBlock != ZP_NULL )
			{
				break;
			}
		}
	}

	zpMemoryBlock* block = ZP_NULL;
	if( freeBlock->size < alignedSize )
	{
		block = freeBlock;
	}
	else
	{
		freeBlock->size -= alignedSize;

		zp_byte* memory = (zp_byte*)freeBlock;
		memory += freeBlock->size;

		block = reinterpret_cast<zpMemoryBlock*>( memory );
		block->size = alignedSize;
	
		addBlock( block );
	}
	
	m_memAllocated += alignedSize;

	return (void*)( block + 1 );
}

void zpMemorySystem::deallocate( void* ptr )
{
	zpMemoryBlock* block = ( reinterpret_cast<zpMemoryBlock*>( ptr ) - 1 );

	m_memDeallocated += block->size;

	zpMemoryBlock* next = block->next;
	if( next > block )
	{
		removeBlock( next );

		block->size += next->size;
		block->next = next->next;
		block->next->prev = block;
	}

	zpMemoryBlock* prev = block->prev;
	if( prev )
	{
		if( prev < block )
		{
			removeBlock( prev );

			prev->size += block->size;
			prev->next = block->next;
			prev->next->prev = prev;

			block = prev;
		}

		addBlock( prev );
	}
}
#endif

void zpMemorySystem::initialize( zp_uint size ) 
{
	zp_zero_memory_array( m_blockTable );

	zp_uint paddedSize = ZP_MEMORY_ALIGN_SIZE( size );
	paddedSize += ZP_MEMORY_INCREMENT_SIZE + sizeof( zpMemoryBlock );

	m_allMemory = (zp_byte*)zp_malloc( paddedSize );
#if ZP_DEBUG
	zp_memset( m_allMemory, 0, paddedSize );
#endif
	zp_byte* memOffset = m_allMemory + sizeof( zpMemoryBlock );
	zp_int distance = ZP_MEMORY_INCREMENT_SIZE - ( (zp_int)memOffset & ZP_MEMORY_INCREMENT_MASK );
	m_alignedMemory = m_allMemory + distance;
	
	zpMemoryBlock* block = reinterpret_cast<zpMemoryBlock*>( m_alignedMemory );
	block->size = paddedSize;
	block->next = ZP_NULL;
	block->prev = ZP_NULL;

	addBlock( block );
}
void zpMemorySystem::addBlock( zpMemoryBlock* block )
{
	zp_uint index = ZP_MEMORY_TABLE_INDEX( block->size );
	zpMemoryBlock* mem = m_blockTable[ index ];

	if( mem != ZP_NULL )
	{
		block->next = mem;
		block->prev = mem->prev;
		if( block->prev )
		{
			block->prev->next = block;
		}
		mem->prev = block;

		if( mem->size < block->size )
		{
			m_blockTable[ index ] = block;
		}
	} 
	else
	{
		block->next = ZP_NULL;
		block->prev = ZP_NULL;

		m_blockTable[ index ] = block;
	}
}
void zpMemorySystem::removeBlock( zpMemoryBlock* block )
{
	zp_uint index = ZP_MEMORY_TABLE_INDEX( block->size );

	block->prev->next = block->next;
	block->next->prev = block->prev;

	if( block == m_blockTable[ index ] )
	{
		if( block != block->next )
		{
			m_blockTable[ index ] = block->next;
		}
		else
		{
			m_blockTable[ index ] = ZP_NULL;
		}
	}
}