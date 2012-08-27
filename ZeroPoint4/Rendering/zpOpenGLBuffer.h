#pragma once
#ifndef ZP_OPENGL_BUFFER_H
#define ZP_OPENGL_BUFFER_H

class zpOpenGLBuffer : public zpBuffer {
public:

	virtual ~zpOpenGLBuffer();

	void create( zpBufferType type, zpBufferBindType bind, zp_uint count, zp_uint stride, void* data = 0 );
	void destroy();

	void map( void** data, zpMapType mapType = ZP_MAP_TYPE_WRITE_DISCARD, zp_uint subResource = 0 );
	void unmap( zp_uint subResource = 0 );

	void update( zp_uint count, void* data );

	zp_uint getCount() const;
	zp_uint getStride() const;
	zpBufferType getBufferType() const;
	zpDisplayFormat getFormat() const;
	zpBufferBindType getBufferBindType() const;

private:
	zpOpenGLBuffer();

	zp_uint getBuffer() const;
	zp_uint getTarget() const;

	zp_uint m_count;
	zp_uint m_stride;
	zpBufferType m_type;
	zpDisplayFormat m_format;
	zpBufferBindType m_bind;

	zp_uint m_target;
	zp_uint m_buffer;

	friend class zpOpenGLRenderingEngine;
	friend class zpOpenGLRenderingContext;
};

#endif