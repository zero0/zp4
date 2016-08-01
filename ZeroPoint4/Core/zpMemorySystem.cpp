#include "zpCore.h"
#include <malloc.h>

#define ZP_MEMORY_SYSTEM_START_CANARY    0xCAFEBABE
#define ZP_MEMORY_SYSTEM_END_CANARY        0xDEADBEEF

#define ZP_MEMORY_INCREMENT_SHIFT    6
#define ZP_MEMORY_INCREMENT_SIZE    ( 1 << ZP_MEMORY_INCREMENT_SHIFT )    // 64bit alignment
#define ZP_MEMORY_INCREMENT_MASK    ( ZP_MEMORY_INCREMENT_SIZE - 1 )
#define ZP_MEMORY_ALIGN_SIZE( s )    ( ( (s) + ZP_MEMORY_INCREMENT_SIZE ) & ( ~ZP_MEMORY_INCREMENT_MASK ) )
#define ZP_MEMORY_TABLE_INDEX( s )      _MemoryTableIndex( s ) //( ( (s) >> ZP_MEMORY_INCREMENT_SHIFT ) % ZP_MEMORY_BLOCK_TABLE_SIZE )
//#define ZP_MEMORY_TABLE_INDEX( s )    ZP_MIN( ( (s) >> ZP_MEMORY_INCREMENT_SHIFT ), ( ZP_MEMORY_BLOCK_TABLE_SIZE - 1 ) )
//#define ZP_MEMORY_TABLE_INDEX( s )    ( (s) % ZP_MEMORY_BLOCK_TABLE_SIZE )

zp_size_t _MemoryTableIndex( zp_size_t size )
{
    zp_size_t index = 0;

    while( size > ZP_MEMORY_INCREMENT_SIZE )
    {
        size = size >> 1;
        ++index;
    }

    return index;
}


void _insertAfter( zpMemoryBlock* block, zpMemoryBlock* newBlock )
{
    newBlock->next = block->next;
    newBlock->prev = block;

    block->next->prev = newBlock;
    block->next = newBlock;
}

void _insertBefore( zpMemoryBlock* node, zpMemoryBlock* newBlock )
{
    _insertAfter( node->prev, newBlock );
}

void _removeBlock( zpMemoryBlock* node )
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
}

void addBlock( zpMemoryBlock** table, zpMemoryBlock* block )
{
    zp_size_t index = ZP_MEMORY_TABLE_INDEX( block->size );
    zpMemoryBlock* tableHead = table[ index ];

    if( tableHead )
    {
        // if the block is larger than the top, insert front
        if( block->alignedSize > tableHead->alignedSize )
        {
            if( tableHead->prev )
            {
                _insertBefore( tableHead, block );
            }

            table[ index ] = block;
        }
        // otherwise, while there is a next point
        else
        {
            zpMemoryBlock* h = tableHead;
            zpMemoryBlock* n = tableHead->next;
            while( n && n != tableHead )
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
void removeBlock( zpMemoryBlock** table, zpMemoryBlock* block )
{
    zp_size_t index = ZP_MEMORY_TABLE_INDEX( block->alignedSize );

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

zpMemorySystem::zpMemorySystem()
    : m_totalMemory( 0 )
    , m_numAllocs( 0 )
    , m_memUsed( 0 )
    , m_allMemory( ZP_NULL )
    , m_alignedMemory( ZP_NULL )
    , m_memoryBlockHead( ZP_NULL )
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


void zpMemorySystem::initialize( zp_size_t size ) 
{
    m_totalMemory = size;

    zpStackTrace::Initialize();

    //zp_zero_memory_array( m_blockTable );
    //zp_zero_memory_array( m_freeTable );

    m_totalAlignedMemory = ZP_MEMORY_ALIGN_SIZE( size );
    m_allocatedMemorySize = m_totalAlignedMemory + ZP_MEMORY_INCREMENT_SIZE + sizeof( zpMemoryBlock );

    m_allMemory = (zp_byte*)zp_malloc( m_allocatedMemorySize + sizeof( zp_size_t ) );
    *( (zp_size_t*)( m_allMemory + m_allocatedMemorySize ) ) = ZP_MEMORY_SYSTEM_END_CANARY;

#if ZP_DEBUG
    zp_memset( m_allMemory, 0, m_allocatedMemorySize );
#endif

    zp_size_t headerAlignmentDistance = ZP_MEMORY_INCREMENT_SIZE - ( (zp_size_t)m_allMemory & ZP_MEMORY_INCREMENT_MASK );
    m_alignedMemory = m_allMemory + headerAlignmentDistance;

    zpMemoryBlock* block = reinterpret_cast< zpMemoryBlock* >( m_alignedMemory );
    block->size = 0;
    block->alignedSize = m_allocatedMemorySize - headerAlignmentDistance;
    block->next = block;
    block->prev = block;

    m_memoryBlockHead = block;
}
void zpMemorySystem::shutdown()
{
    ZP_ASSERT( *( (zp_size_t*)( m_allMemory + m_allocatedMemorySize ) ) == ZP_MEMORY_SYSTEM_END_CANARY, "Buffer overrun" );

#if ZP_MEMORY_TRACK_POINTERS
    if( !m_stackTraces.isEmpty() )
    {
        m_stackTraces.foreach( []( const zpStackTrace& t ) { t.print(); zp_printfln( "" ); } );
    }
    ZP_ASSERT_WARN( m_allocedPointers.isEmpty(), "Not all allocated memory freed" );
#endif

    ZP_ASSERT_WARN( m_memUsed == 0, "Possible memory leak of %d", m_memUsed );
    ZP_ASSERT_WARN( m_numAllocs == 0, "Missing %d allocs->frees", m_numAllocs );

    ZP_SAFE_FREE( m_allMemory );

    zpStackTrace::Shutdown();

    m_totalMemory = 0;
    m_totalAlignedMemory = 0;
    m_allocatedMemorySize = 0;
}

void* zpMemorySystem::allocate( zp_size_t size )
{
    if( m_totalMemory == 0 ) return zp_malloc( size );

    ++m_numAllocs;
    m_memUsed += size;

    zp_size_t alignedSize = ZP_MEMORY_ALIGN_SIZE( size + sizeof( zpMemoryBlock ) );

    void* ptr = ZP_NULL;

    zpMemoryBlock* block = m_memoryBlockHead->prev;
    for( ; ; )
    {
        // if the block is not used
        if( block->size == 0 )
        {
            // if alignment size is the same, use block directly
            if( block->alignedSize == alignedSize )
            {
                block->size = size;

                ptr = (void*)( block + 1 );
                break;
            }
            // otherwise, split block if size is larger
            else if( block->alignedSize > alignedSize )
            {
                // remove aligned size from block
                block->alignedSize -= alignedSize;

                // split
                zp_byte* b = ( (zp_byte*)block ) + block->alignedSize;
                zpMemoryBlock* newBlock = (zpMemoryBlock*)b;
                newBlock->size = size;
                newBlock->alignedSize = alignedSize;

                _insertAfter( block, newBlock );

                ptr = (void*)( newBlock + 1 );
                break;
            }
        }
        // if we've reached the head, break
        else if( block == m_memoryBlockHead )
        {
            break;
        }

        block = block->prev;
    }

#if ZP_MEMORY_TRACK_POINTERS
    m_allocedPointers.pushBack( ptr );
    m_stackTraces.pushBackEmpty();
#endif

    ZP_ASSERT( ptr, "Failed to allocate memory!" );
    return ptr;
}

void zpMemorySystem::deallocate( void* ptr )
{
    if( m_totalMemory == 0 )
    {
        zp_free( ptr );
        return;
    }

    --m_numAllocs;

    zpMemoryBlock* block = ( reinterpret_cast< zpMemoryBlock* >( ptr ) - 1 );

#if ZP_MEMORY_TRACK_POINTERS
    zp_size_t p = m_allocedPointers.indexOf( ptr );
    ZP_ASSERT_WARN( p != zpArrayList< void* >::npos, "Unknown allocation being deallocated" );
    m_allocedPointers.erase( p );
    m_stackTraces.erase( p );
#endif

    m_memUsed -= block->size;

    // clear block size
    block->size = 0;

    // attempt merge next if block and next are contiguous
    zpMemoryBlock* nextBlock = (zpMemoryBlock*)( (zp_byte*)block + block->alignedSize );
    if( block->next->size == 0 && nextBlock == block->next )
    {
        block->alignedSize += block->next->alignedSize;
        _removeBlock( block->next );
    }

    // attempt merge prev if prev and block are contiguous
    zpMemoryBlock* prevBlock = (zpMemoryBlock*)( (zp_byte*)block->prev + block->prev->alignedSize );
    if( block->prev->size == 0 && prevBlock == block )
    {
        block->prev->alignedSize += block->alignedSize;
        _removeBlock( block );
    }
}

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
#if 1
    const zpPackedColor colorEmpty( 10, 10, 10 );
    const zpPackedColor colorEmptyPadding( 64, 64, 64 );
    const zpPackedColor colorHeader( 128, 128, 0 );
    const zpPackedColor colorUsed( 0, 128, 0 );
    const zpPackedColor colorUsedWaisted( 0, 0, 128 );
    const zpPackedColor colorUnused( 128, 0, 0 );
    const zpPackedColor colorUnusedWaisted( 64, 0, 0 );

#define CONCAT( a, b )    a##b
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
    
    *d++ = (zp_byte)(width & 0xFF);
    *d++ = (zp_byte)((width >> 8) & 0xFF);

    *d++ = (zp_byte)(height & 0xFF);
    *d++ = (zp_byte)((height >> 8) & 0xFF);

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
    //for( zp_size_t i = 0; i < ZP_MEMORY_BLOCK_TABLE_SIZE; ++i )
    {
        zpMemoryBlock* block = m_memoryBlockHead;

        do
        {
            zp_size_t offset = (zp_byte*)block - m_alignedMemory;

            writeColor( d + ( offset ) * depth, block->alignedSize, colorUnused );
            writeColor( d + ( offset ) * depth, sizeof( zpMemoryBlock ), colorHeader );
            writeColor( d + ( offset + sizeof( zpMemoryBlock ) ) * depth, block->size, colorUsed );
            writeColor( d + ( offset + sizeof( zpMemoryBlock ) + block->size ) * depth, block->alignedSize - block->size, colorUsedWaisted );

            block = block->next;
        }
        while( block != m_memoryBlockHead );
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
#endif
}

zp_size_t zpMemorySystem::getMemoryUsed() const
{
    return m_memUsed;
}
