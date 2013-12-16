#pragma once
#ifndef ZP_MEMORY_SYSTEM_H
#define ZP_MEMORY_SYSTEM_H

#if ZP_USE_MEMORY_SYSTEM
void* operator new( zp_uint size );

void operator delete( void* ptr );
#endif

#define ZP_MEMORY_BLOCK_TABLE_SIZE	32
#define ZP_MEMORY_INCREMENT_SHIFT	6
#define ZP_MEMORY_INCREMENT_SIZE	( 1 << ZP_MEMORY_INCREMENT_SHIFT )	// 64bit alignment
#define ZP_MEMORY_INCREMENT_MASK	( ZP_MEMORY_INCREMENT_SIZE - 1 )
#define ZP_MEMORY_ALIGN_SIZE( s )	( ( (s) + ZP_MEMORY_INCREMENT_SIZE ) & ( ~ZP_MEMORY_INCREMENT_MASK ) )
//#define ZP_MEMORY_TABLE_INDEX( s )	ZP_MIN( ( (s) >> ZP_MEMORY_INCREMENT_SHIFT ), ( ZP_MEMORY_BLOCK_TABLE_SIZE - 1 ) )
#define ZP_MEMORY_TABLE_INDEX( s )	( (s) % ZP_MEMORY_BLOCK_TABLE_SIZE )
#define ZP_MEMORY_TRACK_POINTERS	0

class zpMemorySystem {
public:
	~zpMemorySystem();

	static zpMemorySystem* getInstance();
	void initialize( zp_uint size );
	void shutdown();

	void* allocate( zp_uint size );

	void deallocate( void* ptr );

private:
	zpMemorySystem();

	struct zpMemoryBlock
	{
		zp_uint size;
		zpMemoryBlock* next;
		zpMemoryBlock* prev;
	};

	void addBlock( zpMemoryBlock* block );
	void removeBlock( zpMemoryBlock* block );

	zp_uint m_numAllocs;
	zp_uint m_numDeallocs;
	zp_uint m_memAllocated;
	zp_uint m_memDeallocated;
	zp_uint m_memUsed;
#if ZP_MEMORY_TRACK_POINTERS
	zpFixedArrayList< void*, 2 * 1024 > m_allocedPointers;
#endif
	zp_byte* m_allMemory;
	zp_byte* m_alignedMemory;

	zpMemoryBlock* m_blockTable[ ZP_MEMORY_BLOCK_TABLE_SIZE ];
};

#endif