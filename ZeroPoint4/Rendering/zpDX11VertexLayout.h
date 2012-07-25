#pragma once
#ifndef ZP_DX11_VERTEX_LAYOUT
#define ZP_DX11_VERTEX_LAYOUT

struct ID3D11InputLayout;

class zpDX11VertexLayout : public zpVertexLayout {
public:
	virtual ~zpDX11VertexLayout();

private:
	zpDX11VertexLayout();
	
	ID3D11InputLayout* getInputLayout() const;

	ID3D11InputLayout* m_layout;

	friend class zpDX11RenderingContext;
	friend class zpDX11RenderingEngine;
};

#endif