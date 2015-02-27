#pragma once
#ifndef ZP_MEMORY_SYSTEM_H
#define ZP_MEMORY_SYSTEM_H

#define ZP_MEMORY_BLOCK_TABLE_SIZE	32
#define ZP_MEMORY_TRACK_POINTERS	1
#define ZP_MEMORY_SYSTEM_TRACKED_POINTERS	( 5 * 1024 )

struct zpMemoryBlock
{
	zp_uint size;
	zp_uint alignedSize;
	zpMemoryBlock* next;
	zpMemoryBlock* prev;
};

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

	void takeMemorySnapshot( zp_long currentTime, zp_uint strideInByts );

private:
	zpMemorySystem();

	void addBlock( zpMemoryBlock** table, zpMemoryBlock* block );
	void removeBlock( zpMemoryBlock** table, zpMemoryBlock* block );

	zp_uint m_totalMemory;
	zp_uint m_totalAlignedMemory;
	zp_uint m_allocatedMemorySize;

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
	zpMemoryBlock* m_freeTable[ ZP_MEMORY_BLOCK_TABLE_SIZE ];

	static zpMemorySystem s_memory;
};

#endif