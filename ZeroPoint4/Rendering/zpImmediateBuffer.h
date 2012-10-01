#pragma once
#ifndef ZP_IMMEDIATE_BUFFER_H
#define ZP_IMMEDIATE_BUFFER_H

template<typename V, typename I>
class zpImmediateBuffer {
public:
	zpImmediateBuffer();
	~zpImmediateBuffer();

	void create( zp_uint vertexBufferCount, zp_uint indexBufferCount );
	void destroy();

	void addPoint( const V& v0 );
	void addLine( const V& v0, const V& v1 );
	void addTriangle( const V& v0, const V& v1, const V& v2 );
	void addQuad( const V& v0, const V& v1, const V& v2, const V& v3 );

	void clear();
	void draw();

private:
	zp_uint m_vertexSize;
	zp_uint m_indexSize;

	zpTopology m_topology;

	zpBuffer* m_vertexBuffer;
	zpBuffer* m_indexBuffer;
};

#include "zpImmediateBuffer.inl"

#endif