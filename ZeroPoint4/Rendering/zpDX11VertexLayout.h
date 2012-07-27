#pragma once
#ifndef ZP_DX11_VERTEX_LAYOUT
#define ZP_DX11_VERTEX_LAYOUT

struct ID3D11InputLayout;

class zpDX11VertexLayout : public zpVertexLayout {
public:
	virtual ~zpDX11VertexLayout();

	static zpDX11VertexLayout* getLayoutFromDesc( const zpString& desc );

private:
	zpDX11VertexLayout();
	
	ID3D11InputLayout* getInputLayout() const;

	ID3D11InputLayout* m_layout;

	static zpHashMap<zpString, zpDX11VertexLayout*> s_layouts;

	friend class zpDX11RenderingContext;
	friend class zpDX11RenderingEngine;
};
 
#endif