#pragma once
#ifndef ZP_MEMORY_SYSTEM_H
#define ZP_MEMORY_SYSTEM_H

#define ZP_MEMORY_BLOCK_TABLE_SIZE	32
#define ZP_MEMORY_INCREMENT_SHIFT	6
#define ZP_MEMORY_INCREMENT_SIZE	( 1 << ZP_MEMORY_INCREMENT_SHIFT )	// 64bit alignment
#define ZP_MEMORY_INCREMENT_MASK	( ZP_MEMORY_INCREMENT_SIZE - 1 )
#define ZP_MEMORY_ALIGN_SIZE( s )	( ( (s) + ZP_MEMORY_INCREMENT_SIZE ) & ( ~ZP_MEMORY_INCREMENT_MASK ) )
//#define ZP_MEMORY_TABLE_INDEX( s )	ZP_MIN( ( (s) >> ZP_MEMORY_INCREMENT_SHIFT ), ( ZP_MEMORY_BLOCK_TABLE_SIZE - 1 ) )
#define ZP_MEMORY_TABLE_INDEX( s )	( (s) % ZP_MEMORY_BLOCK_TABLE_SIZE )
#define ZP_MEMORY_TRACK_POINTERS	1
#define ZP_MEMORY_SYSTEM_TRACKED_POINTERS	( 5 * 1024 )

class zpMemorySystem
{
public:
	~zpMemorySystem();

	static zpMemorySystem* getInstance();
	void initialize( zp_uint size );
	void shutdown();

	void* allocate( zp_uint size );
	void deallocate( void* ptr );

	void printAllAllocatedMemoryStackTrace();
	void printAllocatedMemoryStackTrack( zp_int index );

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

	zp_uint m_totalMemory;

	zp_uint m_numAllocs;
	zp_uint m_numDeallocs;
	zp_uint m_memAllocated;
	zp_uint m_memDeallocated;
	zp_uint m_memUsed;
#if ZP_MEMORY_TRACK_POINTERS
	zpFixedArrayList< void*,        ZP_MEMORY_SYSTEM_TRACKED_POINTERS > m_allocedPointers;
	zpFixedArrayList< zpStackTrace, ZP_MEMORY_SYSTEM_TRACKED_POINTERS > m_stackTraces;
#endif
	zp_byte* m_allMemory;
	zp_byte* m_alignedMemory;

	zpMemoryBlock* m_blockTable[ ZP_MEMORY_BLOCK_TABLE_SIZE ];

	static zpMemorySystem s_memory;
};

#endif