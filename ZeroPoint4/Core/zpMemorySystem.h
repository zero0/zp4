#pragma once
#ifndef ZP_MEMORY_SYSTEM_H
#define ZP_MEMORY_SYSTEM_H

#define ZP_MEMORY_BLOCK_TABLE_SIZE	32
#define ZP_MEMORY_TRACK_POINTERS	1
#define ZP_MEMORY_SYSTEM_TRACKED_POINTERS	( 5 * 1024 )

struct zpMemoryBlock
{
	zp_size_t size;
	zp_size_t alignedSize;
	zpMemoryBlock* next;
	zpMemoryBlock* prev;
};

class zpMemorySystem
{
public:
	~zpMemorySystem();

	static zpMemorySystem* getInstance();
	void initialize( zp_size_t size );
	void shutdown();

	void* allocate( zp_size_t size );
	void deallocate( void* ptr );

	void printAllAllocatedMemoryStackTrace();
	void printAllocatedMemoryStackTrack( zp_size_t index );

	void takeMemorySnapshot( zp_long currentTime, zp_size_t strideInByts );

private:
	zpMemorySystem();

	void addBlock( zpMemoryBlock** table, zpMemoryBlock* block );
	void removeBlock( zpMemoryBlock** table, zpMemoryBlock* block );

	zp_size_t m_totalMemory;
	zp_size_t m_totalAlignedMemory;
	zp_size_t m_allocatedMemorySize;

	zp_size_t m_numAllocs;
	zp_size_t m_numDeallocs;
	zp_size_t m_memAllocated;
	zp_size_t m_memDeallocated;
	zp_size_t m_memUsed;
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