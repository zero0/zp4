#pragma once
#ifndef ZP_MEMORY_SYSTEM_H
#define ZP_MEMORY_SYSTEM_H

void* operator new( zp_uint size );
void* operator new[]( zp_uint size );

void operator delete( void* ptr );
void operator delete[]( void* ptr );

class zpMemorySystem {
public:
	~zpMemorySystem();

	static zpMemorySystem* getInstance();

	void* allocate( zp_uint size );
	void* allocateArray( zp_uint size );

	void deallocate( void* ptr );
	void deallocateArray( void* ptr );

private:
	zpMemorySystem();

	zp_uint m_numAllocs;
	zp_uint m_numArrayAllocs;
	zp_uint m_numDeallocs;
	zp_uint m_numArrayDeallocs;

	zp_uint m_memAllocated;
	zp_uint m_memDeallocated;

	static zpMemorySystem s_instance;
};

#endif