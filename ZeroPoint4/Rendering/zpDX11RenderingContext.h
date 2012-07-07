#pragma once
#ifndef ZP_DX11_RENDERING_CONTEXT_H
#define ZP_DX11_RENDERING_CONTEXT_H

struct ID3D11DeviceContext;

class zpDX11RenderingContext : public zpRenderingContext {
public:
	zpDX11RenderingContext();
	zpDX11RenderingContext( ID3D11DeviceContext* context );
	virtual ~zpDX11RenderingContext();

	void map( zpBuffer* buffer, zpRenderingMapType mapType, zp_uint subResource, void** data );
	void unmap( zpBuffer* buffer, zp_uint subResource );

private:
	ID3D11DeviceContext* m_context;
};

#endif