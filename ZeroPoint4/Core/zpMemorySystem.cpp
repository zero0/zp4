#include "zpCore.h"
#include <malloc.h>
//#include <new>

#if ZP_USE_MEMORY_SYSTEM
//void* operator new( zp_uint size ) {
//	return zpMemorySystem::getInstance()->allocate( size );
//}
//void* operator new[]( zp_uint size ) {
//	return zpMemorySystem::getInstance()->allocate( size );
//}
//
//void operator delete( void* ptr ) {
//	zpMemorySystem::getInstance()->deallocate( ptr );
//}
//void operator delete[]( void* ptr ) {
//	zpMemorySystem::getInstance()->deallocate( ptr );
//}
#else

#if ZP_USE_ALIGNMENT
void* operator new( zp_uint size ) {
	return zp_aligned_malloc( size, ZP_MALLOC_ALIGNMENT );
}

void operator delete( void* ptr ) {
	zp_aligned_free( ptr );
}
#else
void* operator new( zp_size size ) {
	return zp_malloc( size );
}

void operator delete( void* ptr ) {
	zp_free( ptr );
}
#endif

#endif

zpMemorySystem::zpMemorySystem()
	: m_totalMemory( 0 )
	, m_numAllocs( 0 )
	, m_numDeallocs( 0 )
	, m_memAllocated( 0 )
	, m_memDeallocated( 0 )
	, m_memUsed( 0 )
	, m_allMemory( ZP_NULL )
	, m_alignedMemory( ZP_NULL )
{
}
zpMemorySystem::~zpMemorySystem()
{
}
zpMemorySystem zpMemorySystem::s_memory;

zpMemorySystem* zpMemorySystem::getInstance()
{
	return &s_memory;
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
	if( m_totalMemory == 0 ) return zp_malloc( size );

	++m_numAllocs;
	m_memAllocated += size;
	m_memUsed += size;
	//void* ptr = zp_malloc( size + sizeof( zp_uint ) );
	//zp_uint* i = (zp_uint*)ptr;
	//*i = size;
#if ZP_MEMORY_TRACK_POINTERS
	//m_allocedPointers.pushBack( ptr );
	//m_stackTraces.pushBackEmpty();
#endif
	//return (void*)( i + 1 );

	zp_uint alignedSize = ZP_MEMORY_ALIGN_SIZE( size + sizeof( zpMemoryBlock ) );

	zp_uint index = ZP_MEMORY_TABLE_INDEX( alignedSize );
	zpMemoryBlock* freeBlock = m_freeTable[ index ];

	// if a free block was not found, find the next largest block to allocate from
	if( freeBlock == ZP_NULL )
	{
		for( zp_uint i = ZP_MEMORY_BLOCK_TABLE_SIZE - 1; i != 0 ; --i )
		{
			freeBlock = m_freeTable[ i ];
			if( freeBlock != ZP_NULL )
			{
				break;
			}
		}
	}

	zpMemoryBlock* block = ZP_NULL;
	// if the free block size can fit the aligned size, use the block
	if( freeBlock->alignedSize <= alignedSize )
	{
		// remove the block from the free list since it will be used
		removeBlock( m_freeTable, freeBlock );

		// store block and resize
		block = freeBlock;
		block->size = size;
		block->alignedSize = alignedSize;
	}
	else
	{
		// remove the block from the free list
		removeBlock( m_freeTable, freeBlock );

		// decrease block size
		freeBlock->alignedSize -= alignedSize;
		freeBlock->size -= size;

		// re-add block to free table (could go into different bucket)
		addBlock( m_freeTable, freeBlock );

		// find new location for memory
		zp_byte* memory = (zp_byte*)freeBlock;
		memory += freeBlock->alignedSize;

		// cast memory as block
		block = reinterpret_cast<zpMemoryBlock*>( memory );
		block->size = size;
		block->alignedSize = alignedSize;
	}

	// move free table to used table
	addBlock( m_blockTable, block );

	return (void*)( block + 1 );
}

void zpMemorySystem::deallocate( void* ptr )
{
	if( m_totalMemory == 0 )
	{
		zp_free( ptr );
		return;
	}

	++m_numDeallocs;
	//zp_uint* i = (zp_uint*)ptr;
	//--i;

#if ZP_MEMORY_TRACK_POINTERS
	//zp_int p = m_allocedPointers.indexOf( i );
	//ZP_ASSERT_WARN( p != -1, "Unknown allocation being deallocated" );
	//if( p < 0 )
	//{
	//	zp_printfln( "Unknown allocation size %d", *i );
	//
	//	--i;
	//	p = m_allocedPointers.indexOf( i );
	//	ZP_ASSERT_WARN( p != -1, "Another unknown allocation being deallocated" );
	//
	//	zp_printfln( "Another unknown allocation size %d", *i );
	//
	//	printAllocatedMemoryStackTrack( p );
	//}
	//m_allocedPointers.erase( p );
	//m_stackTraces.erase( p );
#endif

	//m_memDeallocated += *i;
	//m_memUsed -= *i;

	//zp_free( i );
	//return;

	zpMemoryBlock* block = ( reinterpret_cast<zpMemoryBlock*>( ptr ) - 1 );

	m_memDeallocated += block->size;
	m_memUsed -= block->size;

	// remove block from used table
	removeBlock( m_blockTable, block );

	// add block to free table
	addBlock( m_freeTable, block );

	return;

	zpMemoryBlock* next = block->next;
	if( next > block )
	{
		removeBlock( m_freeTable, next );

		block->size += next->size;
		block->alignedSize += next->alignedSize;

		removeBlock( m_freeTable, block );

		addBlock( m_freeTable, block );
	}

	zpMemoryBlock* prev = block->prev;
	if( prev < block )
	{
		removeBlock( m_freeTable, prev );

		prev->size += block->size;
		prev->alignedSize += block->alignedSize;

		removeBlock( m_freeTable, block );

		addBlock( m_freeTable, prev );
	}
}
#endif


void zpMemorySystem::printAllAllocatedMemoryStackTrace()
{
#if ZP_MEMORY_TRACK_POINTERS
	if( !m_stackTraces.isEmpty() )
	{
		m_stackTraces.foreach( []( const zpStackTrace& t ) { t.print(); zp_printfln( "" ); } );
	}
#endif
}
void zpMemorySystem::printAllocatedMemoryStackTrack( zp_int index )
{
#if ZP_MEMORY_TRACK_POINTERS
	m_stackTraces[ index ].print();
	zp_printfln( "" );
#endif
}


void zpMemorySystem::initialize( zp_uint size ) 
{
	m_totalMemory = size;

	zpStackTrace::Initialize();

	zp_zero_memory_array( m_blockTable );
	zp_zero_memory_array( m_freeTable );

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
	block->size = m_totalMemory;
	block->alignedSize = paddedSize;

	addBlock( m_freeTable, block );
}
void zpMemorySystem::shutdown()
{

#if ZP_MEMORY_TRACK_POINTERS
	if( !m_stackTraces.isEmpty() )
	{
		m_stackTraces.foreach( []( const zpStackTrace& t ) { t.print(); zp_printfln( "" ); } );
	}
	ZP_ASSERT_WARN( m_allocedPointers.isEmpty(), "Not all allocated memory freed" );
#endif

	ZP_ASSERT_WARN( m_memUsed == 0, "Possible memory leak of %d", m_memUsed );
	ZP_ASSERT_WARN( m_numAllocs - m_numDeallocs == 0, "Missing %d allocs->frees", m_numAllocs - m_numDeallocs );	
	ZP_ASSERT_WARN( m_memAllocated - m_memDeallocated == 0, "Memory leak of %d", m_memAllocated - m_memDeallocated );

	ZP_SAFE_FREE( m_allMemory );

	zpStackTrace::Shutdown();

	m_totalMemory = 0;
}

void zpMemorySystem::addBlock( zpMemoryBlock** table, zpMemoryBlock* block )
{
	zp_uint index = ZP_MEMORY_TABLE_INDEX( block->size );
	zpMemoryBlock* mem = table[ index ];

	if( mem )
	{
		// if block is less than the head size, push back
		if( block->size < mem->size )
		{
			block->next = mem;
			block->prev = mem->prev;
		}
		// otherwise, push front and assign head since it's now the larges block
		else
		{
			block->next = mem->next;
			block->prev = mem;

			table[ index ] = block;
		}
		
		block->next->prev = block;
		block->prev->next = block;
	} 
	else
	{
		block->next = block;
		block->prev = block;

		table[ index ] = block;
	}
}
void zpMemorySystem::removeBlock( zpMemoryBlock** table, zpMemoryBlock* block )
{
	zp_uint index = ZP_MEMORY_TABLE_INDEX( block->size );

	block->prev->next = block->next;
	block->next->prev = block->prev;

	if( block == table[ index ] )
	{
		if( block != block->next )
		{
			table[ index ] = block->next;
		}
		else
		{
			table[ index ] = ZP_NULL;
		}
	}
}
