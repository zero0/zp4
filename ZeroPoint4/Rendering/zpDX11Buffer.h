#pragma once
#ifndef ZP_DX11_BUFFER_H
#define ZP_DX11_BUFFER_H

class zpDX11RenderingEngine;
struct ID3D11Buffer;

class zpDX11Buffer : public zpBuffer {
public:
	zpDX11Buffer();
	zpDX11Buffer( zpDX11RenderingEngine* engine );
	virtual ~zpDX11Buffer();

	void create( zpBufferType type, zpBufferBind bind, zp_uint count, zp_uint stride, void* data = 0 );
	void release();

	void map( void** data, zpRenderingMapType mapType = ZP_RENDERING_MAP_TYPE_WRITE_DISCARD, zp_uint subResource = 0 );
	void unmap( zp_uint subResource = 0 );

	void update( zp_uint count, void* data );

	void bind();
	void unbind();

	zp_uint getCount() const;
	zp_uint getStride() const;

private:
	ID3D11Buffer* getBuffer() const;

	zp_uint m_count;
	zp_uint m_stride;
	zpBufferType m_type;
	zpBufferBind m_bind;

	ID3D11Buffer* m_buffer;
	zpDX11RenderingEngine* m_engine;

	friend class zpDX11RenderingEngine;
	friend class zpDX11RenderingContext;
};

#endif