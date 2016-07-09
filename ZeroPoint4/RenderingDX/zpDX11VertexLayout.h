#pragma once
#ifndef ZP_DX11_VERTEX_LAYOUT
#define ZP_DX11_VERTEX_LAYOUT

struct ID3D11InputLayout;

class zpDX11VertexLayout : public zpVertexLayout {
public:
    virtual ~zpDX11VertexLayout();

    zp_uint getStride() const;

private:
    zpDX11VertexLayout();
    
    static zpDX11VertexLayout* getLayoutFromDesc( const zpString& inputDesc, void* vertexShaderBlob, zp_uint vertexShaderBlobSize );

    ID3D11InputLayout* getInputLayout() const;

    ID3D11InputLayout* m_layout;
    zp_uint m_stride;

    //static zpHashMap<zpString, zpDX11VertexLayout*> s_layouts;

    friend class zpDX11RenderingContext;
    friend class zpDX11RenderingEngine;

    friend class zpDX11ShaderResource;
};
 
#endif