#include "zpRendering.h"
#include "zpDX11.h"
#include <D3D11.h>
#include "zpDX11Util.h"

zpDX11VertexLayout::zpDX11VertexLayout() :
	m_layout( ZP_NULL )
{}
zpDX11VertexLayout::~zpDX11VertexLayout() {
	ZP_SAFE_RELEASE( m_layout );
}

zp_uint zpDX11VertexLayout::getStride() const {
	return m_stride;
}

zpHashMap<zpString, zpDX11VertexLayout*> zpDX11VertexLayout::s_layouts;
zpDX11VertexLayout* zpDX11VertexLayout::getLayoutFromDesc( const zpString& inputDesc, void* vertexShaderBlob, zp_uint vertexShaderBlobSize ) {
	zpDX11VertexLayout* vertexLayout = ZP_NULL;

	if( !s_layouts.find( inputDesc, &vertexLayout ) ) {
		// @TODO: for now, just do a simple look up for the defined vertex types
		if( inputDesc == "simple" ) {
			D3D11_INPUT_ELEMENT_DESC desc[] = {
				{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			ID3D11InputLayout* layout = ZP_NULL;
			zp_uint length = ZP_ARRAY_LENGTH( desc );
			HRESULT hr;
			
			hr = ( (zpDX11RenderingEngine*)zpRenderingFactory::getRenderingEngine() )->getDevice()->CreateInputLayout( desc, length, vertexShaderBlob, vertexShaderBlobSize, &layout );
			
			vertexLayout = new zpDX11VertexLayout;
			vertexLayout->m_layout = layout;
			vertexLayout->m_stride = sizeof( zpVertexPositionColor );

			s_layouts[ inputDesc ] = vertexLayout;
		} else if( inputDesc == "textured_normal" ) {
			D3D11_INPUT_ELEMENT_DESC desc[] = {
				{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 16,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 32,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			ID3D11InputLayout* layout = ZP_NULL;
			zp_uint length = ZP_ARRAY_LENGTH( desc );
			HRESULT hr;

			hr = ( (zpDX11RenderingEngine*)zpRenderingFactory::getRenderingEngine() )->getDevice()->CreateInputLayout( desc, length, vertexShaderBlob, vertexShaderBlobSize, &layout );

			vertexLayout = new zpDX11VertexLayout;
			vertexLayout->m_layout = layout;
			vertexLayout->m_stride = sizeof( zpVertexPositionNormalTexture );

			s_layouts[ inputDesc ] = vertexLayout;
		}
	}

	return vertexLayout;
}

ID3D11InputLayout* zpDX11VertexLayout::getInputLayout() const {
	return m_layout;
}
