#include "zpCore.h"

zpMemorySystem::zpMemorySystem() : m_allocator( ZP_NULL ) {}
zpMemorySystem::~zpMemorySystem() {}

zpMemorySystem zpMemorySystem::s_instance;
zpMemorySystem* zpMemorySystem::getInstance() {
	return &s_instance;
}

void zpMemorySystem::initialize( zpMemoryAllocator* allocator ) {
	m_allocator = allocator;
}
void zpMemorySystem::initializeDefault() {
	struct zpDefaultMemoryAllocator : zpMemoryAllocator {
		void* allocate( zp_uint size ) {
			return malloc( size );
		}
		void* allocateArray( zp_uint size, zp_uint count ) {
			return malloc( size );
		}
		void* allocateAligned( zp_uint size, zp_uint align ) {
			return _aligned_malloc( size, align );
		}
		void* allocatePlacement( zp_uint size, void* ptr ) {
			return ptr;
		}

		void deallocate( void* ptr ) {
			free( ptr );
		}
		void deallocateArray( void* ptr ) {
			free( ptr) ;
		}
		void deallocateAligned( void* ptr ) {
			_aligned_free( ptr );
		}
		void deallocatePlacement( void* ptr ) {}
	};

	initialize( new zpDefaultMemoryAllocator );
}
void zpMemorySystem::initializeDebug() {
#if ZP_DEBUG
	struct zpDebugMemoryAllocator : zpMemoryAllocator {
		void* allocate( zp_uint size ) {
			void* mem = malloc( size );
			if( !mem ) {
				ZP_ON_DEBUG_MSG( "Error allocating memory: %u bytes", size );
			}
			return mem;
		}
		void* allocateArray( zp_uint size, zp_uint count ) {
			void* mem = malloc( size );
			if( !mem ) {
				ZP_ON_DEBUG_MSG( "Error allocating array memory: %u bytes, %u count", size, count );
			}
			return mem;
		}
		void* allocateAligned( zp_uint size, zp_uint align ) {
			void* mem = _aligned_malloc( size, align );
			if( !mem ) {
				ZP_ON_DEBUG_MSG( "Error allocating aligned memory: %u bytes, %u alignment", size, align );
			}
			return mem;
		}
		void* allocatePlacement( zp_uint size, void* ptr ) {
			if( !ptr ) {
				ZP_ON_DEBUG_MSG( "Error allocating placement memory: %u bytes", size );
			}
			return ptr;
		}

		void deallocate( void* ptr ) {
			free( ptr );
		}
		void deallocateArray( void* ptr ) {
			free( ptr) ;
		}
		void deallocateAligned( void* ptr ) {
			_aligned_free( ptr );
		}
		void deallocatePlacement( void* ptr ) {}
	};

	initialize( new zpDebugMemoryAllocator );
#else
	initializeDefault();
#endif
}

void zpMemorySystem::setMemoryAllocator( zpMemoryAllocator* allocator ) {
	m_allocator = allocator;
}
zpMemoryAllocator* zpMemorySystem::getMemoryAllocator() const {
	return m_allocator;
}

zp_bool zpMemorySystem::isInitialized() const {
	return m_allocator != ZP_NULL;
}