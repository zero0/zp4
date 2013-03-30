#pragma once
#ifndef ZP_BUFFER_H
#define ZP_BUFFER_H

class zpBufferImpl;

class zpBuffer
{
	ZP_NON_COPYABLE( zpBuffer );
public:
	~zpBuffer();

	zp_uint getSize() const;
	zp_uint getStride() const;
	zpBufferType getBufferType() const;
	zpDisplayFormat getFormat() const;
	zpBufferBindType getBufferBindType() const;

private:
	zpBuffer( zpBufferImpl* buffer );

	zpBufferImpl* m_buffer;

	friend class zpRenderingEngine;
	friend class zpRenderingContext;
};

#if 0
ZP_PURE_INTERFACE zpBuffer {
public:

	template<typename S>
	void create( zpBufferType type, zpBufferBindType bind ) {
		create( type, bind, 0, sizeof( S ), ZP_NULL );
	}

	template<typename S, zp_uint I>
	void create( zpBufferType type, zpBufferBindType bind, const S (&data)[I] ) {
		create( type, bind, I, sizeof( S ), (void*)data );
	}

	template<typename S>
	void create( zpBufferType type, zpBufferBindType bind, const S& data ) {
		create( type, bind, 1, sizeof( S ), (void*)data );
	}

	template<typename S, zp_uint I>
	void update( const S (&data)[I] ) {
		update( I, (void*)data );
	}

	template<typename S>
	void update( const S& data ) {
		update( 1, (void*)&data );
	}

	template<typename S>
	void map( S** data ) {
		map( (void**)data );
	}	

	virtual void create( zpBufferType type, zpBufferBindType bind, zp_uint count, zp_uint stride, void* data = 0 );
	virtual void destroy() = 0;

	virtual void map( void** data, zpMapType mapType = ZP_MAP_TYPE_WRITE_DISCARD, zp_uint subResource = 0 ) = 0;
	virtual void unmap( zp_uint subResource = 0 ) = 0;

	virtual void update( zp_uint count, void* data = 0 ) = 0;

	virtual zp_uint getCount() const = 0;
	virtual zp_uint getStride() const = 0;
	virtual zpBufferType getBufferType() const = 0;
	virtual zpDisplayFormat getFormat() const = 0;
	virtual zpBufferBindType getBufferBindType() const = 0;
};
#endif

#endif