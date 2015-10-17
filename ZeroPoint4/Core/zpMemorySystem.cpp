#include "zpCore.h"
#include <malloc.h>

#define ZP_MEMORY_SYSTEM_START_CANARY	0xCAFEBABE
#define ZP_MEMORY_SYSTEM_END_CANARY		0xDEADBEEF

#define ZP_MEMORY_INCREMENT_SHIFT	4
#define ZP_MEMORY_INCREMENT_SIZE	( 1 << ZP_MEMORY_INCREMENT_SHIFT )	// 64bit alignment
#define ZP_MEMORY_INCREMENT_MASK	( ZP_MEMORY_INCREMENT_SIZE - 1 )
#define ZP_MEMORY_ALIGN_SIZE( s )	( ( (s) + ZP_MEMORY_INCREMENT_SIZE ) & ( ~ZP_MEMORY_INCREMENT_MASK ) )
#define ZP_MEMORY_TABLE_INDEX( s )	ZP_MIN( ( (s) >> ZP_MEMORY_INCREMENT_SHIFT ), ( ZP_MEMORY_BLOCK_TABLE_SIZE - 1 ) )
//#define ZP_MEMORY_TABLE_INDEX( s )	( (s) % ZP_MEMORY_BLOCK_TABLE_SIZE )

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
void* zpMemorySystem::allocate( zp_size_t size ) {
	//m_memAllocated += size;
	//++m_numAllocs;
	return zp_aligned_malloc( size, ZP_MALLOC_ALIGNMENT );
}

void zpMemorySystem::deallocate( void* ptr ) {
	//++m_numDeallocs;
	zp_aligned_free( ptr );
}
#else
void* zpMemorySystem::allocate( zp_size_t size )
{
	if( m_totalMemory == 0 ) return zp_malloc( size );

	++m_numAllocs;
	m_memAllocated += size;
	m_memUsed += size;

	zp_size_t alignedSize = ZP_MEMORY_ALIGN_SIZE( size + sizeof( zpMemoryBlock ) );

	zp_size_t index = ZP_MEMORY_TABLE_INDEX( alignedSize );
	zpMemoryBlock* freeBlock = m_freeTable[ index ];

	// if a free block was not found, find the next largest block to allocate from
	if( freeBlock == ZP_NULL )
	{
		for( zp_size_t i = ZP_MEMORY_BLOCK_TABLE_SIZE - 1; i != 0 ; --i )
		{
			freeBlock = m_freeTable[ i ];
			if( freeBlock != ZP_NULL )
			{
				break;
			}
		}
	}
	
	ZP_ASSERT( freeBlock != ZP_NULL, "Unable to find free block of memory to use" );
	//ZP_ASSERT( freeBlock->alignedSize > alignedSize, "Free block too small to use. Free block is %d, %d needed", freeBlock->alignedSize, alignedSize );

	// remove the block from the free list since it will be used
	removeBlock( m_freeTable, freeBlock );
	
	zpMemoryBlock* block = ZP_NULL;
	// if the free block size can fit the aligned size, use the block
	if( freeBlock->alignedSize <= alignedSize )
	{
		// store block and resize
		block = freeBlock;
		block->size = size;
		block->alignedSize = alignedSize;
	}
	// otherwise, decrease the size of the block and add it back to the free list for later use
	else
	{
		// decrease block size
		freeBlock->alignedSize -= alignedSize;
		freeBlock->size -= size;

		// re-add block to free table (could go into different bucket)
		addBlock( m_freeTable, freeBlock );

		// find new location for memory
		zp_byte* memory = (zp_byte*)freeBlock;
		memory += freeBlock->alignedSize;

		// cast memory as block
		block = reinterpret_cast< zpMemoryBlock* >( memory );
		block->size = size;
		block->alignedSize = alignedSize;
	}

	// move free table to used table
	addBlock( m_blockTable, block );

	void* ptr = (void*)( block + 1 );

#if ZP_MEMORY_TRACK_POINTERS
	m_allocedPointers.pushBack( ptr );
	m_stackTraces.pushBackEmpty();
#endif

	return ptr;
}

void zpMemorySystem::deallocate( void* ptr )
{
	if( m_totalMemory == 0 )
	{
		zp_free( ptr );
		return;
	}

	++m_numDeallocs;

	zpMemoryBlock* block = ( reinterpret_cast< zpMemoryBlock* >( ptr ) - 1 );

#if ZP_MEMORY_TRACK_POINTERS
	zp_size_t p = m_allocedPointers.indexOf( ptr );
	ZP_ASSERT_WARN( p != zpArrayList< void* >::npos, "Unknown allocation being deallocated" );
	m_allocedPointers.erase( p );
	m_stackTraces.erase( p );
#endif

	m_memDeallocated += block->size;
	m_memUsed -= block->size;

	// remove block from used table
	removeBlock( m_blockTable, block );

	// add block to free table
	addBlock( m_freeTable, block );

	return;

	zpMemoryBlock* next = block->next;
	if( next && next > block )
	{
		removeBlock( m_freeTable, next );

		block->size += next->size;
		block->alignedSize += next->alignedSize;

		removeBlock( m_freeTable, block );

		addBlock( m_freeTable, block );
	}

	//zpMemoryBlock* prev = block->prev;
	//if( prev < block )
	//{
	//	removeBlock( m_freeTable, prev );
	//
	//	prev->size += block->size;
	//	prev->alignedSize += block->alignedSize;
	//
	//	removeBlock( m_freeTable, block );
	//
	//	addBlock( m_freeTable, prev );
	//}
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
void zpMemorySystem::printAllocatedMemoryStackTrack( zp_size_t index )
{
#if ZP_MEMORY_TRACK_POINTERS
	m_stackTraces[ index ].print();
	zp_printfln( "" );
#endif
}

inline void _writeColor3( zp_byte* data, zp_size_t count, const zpPackedColor& color )
{
	for( zp_size_t i = 0; i < count; ++i )
	{
		*data++ = color.getBlue();
		*data++ = color.getGreen();
		*data++ = color.getRed();
	}
}

inline void _writeColor4( zp_byte* data, zp_size_t count, const zpPackedColor& color )
{
	for( zp_size_t i = 0; i < count; ++i )
	{
		*data++ = color.getBlue();
		*data++ = color.getGreen();
		*data++ = color.getRed();
		*data++ = color.getAlpha();
	}
}

void zpMemorySystem::takeMemorySnapshot( zp_long currentTime, zp_size_t strideInBytes )
{
	const zpPackedColor colorEmpty( 10, 10, 10 );
	const zpPackedColor colorEmptyPadding( 64, 64, 64 );
	const zpPackedColor colorHeader( 128, 128, 0 );
	const zpPackedColor colorUsed( 0, 128, 0 );
	const zpPackedColor colorUsedWaisted( 0, 0, 128 );
	const zpPackedColor colorUnused( 128, 0, 0 );
	const zpPackedColor colorUnusedWaisted( 64, 0, 0 );

#define CONCAT( a, b )	a##b
#define depth 3
#define writeColor CONCAT( _writeColor, 3 )

	zp_size_t width = strideInBytes;
	zp_size_t height = m_totalAlignedMemory / strideInBytes;
	height += ( m_totalAlignedMemory % strideInBytes > 0 ? 1 : 0 );
	zp_size_t imageSize = width * height * depth;
	zp_size_t headerSize = 18;
	zp_byte* data = (zp_byte*)zp_malloc( imageSize + headerSize );
	zp_byte* d = data; //.getData() + index;

	*d++ = 0; // 0 ID length = no id
	*d++ = 0; // 1 color map type = no color map
	*d++ = 2; // 2 image type = uncompressed true color
	*d++ = 0; *d++ = 0; *d++ = 0; *d++ = 0; *d++ = 0; // color map spec = empty
	*d++ = 0; *d++ = 0; // x origin of image
	*d++ = 0; *d++ = 0; // y origin of image
	
	*d++ = (zp_byte)(width & 0x00FF);
	*d++ = (zp_byte)(width & 0xFF00) / 256;

	*d++ = (zp_byte)(height & 0x00FF);
	*d++ = (zp_byte)(height & 0xFF00) / 256;

	*d++ = depth * 8;
	*d++ = 0x24;

	// clear entire image
	//writeColor( d, imageSize, colorEmpty );
	zp_memset( d, 0, imageSize );

	// write padding colors
	writeColor( d, m_alignedMemory - m_allMemory, colorEmptyPadding );

	// increment d to so offset is zero based
	d += m_alignedMemory - m_allMemory;

	// write free table
	for( zp_size_t i = 0; i < ZP_MEMORY_BLOCK_TABLE_SIZE; ++i )
	{
		zpMemoryBlock* block = m_freeTable[ i ];
		if( block == ZP_NULL ) continue;

		do
		{
			zp_size_t offset = (zp_byte*)block - m_alignedMemory;

			//_writeColor( d + offset * 4, block->alignedSize, colorUsed );
			writeColor( d + ( offset ) * depth, sizeof( zpMemoryBlock ), colorHeader );
			writeColor( d + ( offset + sizeof( zpMemoryBlock ) ) * depth, block->size, colorUnused );
			
			if( block->alignedSize >= block->size )
			{
				writeColor( d + ( offset + sizeof( zpMemoryBlock ) + block->size ) * depth, block->alignedSize - block->size, colorUnusedWaisted );
			}
			else
			{
				writeColor( d + ( offset + sizeof( zpMemoryBlock ) + block->size ) * depth, block->size - block->alignedSize, colorUnusedWaisted );
			}

			block = block->next;
		}
		while( block != m_freeTable[ i ] );
	}

	// write block table
	for( zp_size_t i = 0; i < ZP_MEMORY_BLOCK_TABLE_SIZE; ++i )
	{
		zpMemoryBlock* block = m_blockTable[ i ];
		if( block == ZP_NULL ) continue;

		do
		{
			zp_size_t offset = (zp_byte*)block - m_alignedMemory;

			//_writeColor( d + offset * 4, block->alignedSize, colorUsed );
			writeColor( d + ( offset ) * depth, sizeof( zpMemoryBlock ), colorHeader );
			writeColor( d + ( offset + sizeof( zpMemoryBlock ) ) * depth, block->size, colorUsed );
			writeColor( d + ( offset + sizeof( zpMemoryBlock ) + block->size ) * depth, block->alignedSize - block->size, colorUsedWaisted );

			block = block->next;
		}
		while( block != m_blockTable[ i ] );
	}

	zpFixedStringBuffer< 256 > filename;
	filename << "MemorySnapshot_" << currentTime << ".tga";

	zpFile snapshotFile( filename.str() );
	if( snapshotFile.open( ZP_FILE_MODE_BINARY_TRUNCATE_WRITE ) )
	{
		snapshotFile.writeBuffer( data, imageSize + headerSize );
		snapshotFile.close();
	}

	zp_free( data );

#undef CONCAT
#undef depth
#undef writeColor
}

void zpMemorySystem::initialize( zp_size_t size ) 
{
	m_totalMemory = size;

	zpStackTrace::Initialize();

	zp_zero_memory_array( m_blockTable );
	zp_zero_memory_array( m_freeTable );

	m_totalAlignedMemory = ZP_MEMORY_ALIGN_SIZE( size );
	m_allocatedMemorySize = m_totalAlignedMemory + ZP_MEMORY_INCREMENT_SIZE + sizeof( zpMemoryBlock );

	m_allMemory = (zp_byte*)zp_malloc( m_allocatedMemorySize + sizeof( zp_uint ) );
#if ZP_DEBUG
	zp_memset( m_allMemory, 0, m_allocatedMemorySize );
	*( (zp_uint*)( m_allMemory + m_allocatedMemorySize ) ) = ZP_MEMORY_SYSTEM_END_CANARY;
#endif

	zp_int distance = ZP_MEMORY_INCREMENT_SIZE - ( (zp_int)m_allMemory & ZP_MEMORY_INCREMENT_MASK );
	m_alignedMemory = m_allMemory + distance;

	zpMemoryBlock* block = reinterpret_cast< zpMemoryBlock* >( m_alignedMemory );
	block->size = m_totalMemory;
	block->alignedSize = m_totalAlignedMemory;

	addBlock( m_freeTable, block );
}
void zpMemorySystem::shutdown()
{
	ZP_ASSERT( *( (zp_uint*)( m_allMemory + m_allocatedMemorySize ) ) == ZP_MEMORY_SYSTEM_END_CANARY, "Buffer overrun" );

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

void _insertAfter( zpMemoryBlock* block, zpMemoryBlock* newBlock )
{
	newBlock->next = block->next;
	newBlock->prev = block;

	block->next->prev = newBlock;
	block->prev->next = newBlock;
}

void _insertBefore( zpMemoryBlock* node, zpMemoryBlock* newBlock )
{
	_insertAfter( node->prev, newBlock );
}

void zpMemorySystem::addBlock( zpMemoryBlock** table, zpMemoryBlock* block )
{
	zp_size_t index = ZP_MEMORY_TABLE_INDEX( block->size );
	zpMemoryBlock* tableHead = table[ index ];

	if( tableHead )
	{
		// if the block is larger than the top, insert front
		if( block->alignedSize > tableHead->alignedSize )
		{
			_insertBefore( tableHead, block );

			table[ index ] = block;
		}
		// otherwise, while there is a next point
		else
		{
			zpMemoryBlock* h = tableHead;
			zpMemoryBlock* n = tableHead->next;
			while( n != tableHead )
			{
				if( block->alignedSize > n->alignedSize )
				{
					break;
				}
				h = n;
				n = n->next;
			}
			_insertAfter( h, block );
		}

		/*
		// if block is less than the head size, push back
		if( block->alignedSize <= mem->alignedSize )
		{
			zpMemoryBlock* m = mem;
			while( block->alignedSize < )
			block->next = mem;
			//block->prev = mem->prev;
		}
		// otherwise, push front and assign head since it's now the larges block
		else
		{
			block->next = mem->next;
			//block->prev = mem;

			table[ index ] = block;
		}
		
		//block->next->prev = block;
		//block->prev->next = block;
		*/
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
	zp_size_t index = ZP_MEMORY_TABLE_INDEX( block->size );

	//block->prev->next = block->next;
	//block->next->prev = block->prev;

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
