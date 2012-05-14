#pragma once
#ifndef ZP_MEMORY_SYSTEM_H
#define ZP_MEMORY_SYSTEM_H

struct ZP_NO_VTABLE zpMemoryAllocator {
	virtual void* allocate( zp_uint size ) = 0;
	virtual void* allocateArray( zp_uint size, zp_uint count ) = 0;
	virtual void* allocateAligned( zp_uint size, zp_uint align ) = 0;
	virtual void* allocatePlacement( zp_uint size, void* ptr ) = 0;

	virtual void deallocate( void* ptr ) = 0;
	virtual void deallocateArray( void* ptr ) = 0;
	virtual void deallocateAligned( void* ptr ) = 0;
	virtual void deallocatePlacement( void* ptr ) = 0;
};

class zpMemorySystem {
public:
	~zpMemorySystem();

	static zpMemorySystem* getInstance();

	void initialize( zpMemoryAllocator* allocator );
	void initializeDefault();
	void initializeDebug();

	void setMemoryAllocator( zpMemoryAllocator* allocator );
	zpMemoryAllocator* getMemoryAllocator() const;

	zp_bool isInitialized() const;

private:
	zpMemorySystem();

	zpMemoryAllocator* m_allocator;

	static zpMemorySystem s_instance;
};

#endif