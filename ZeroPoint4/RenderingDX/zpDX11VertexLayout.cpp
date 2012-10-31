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
	zpDX11VertexLayout** found = ZP_NULL;

	if( s_layouts.find( inputDesc, &found ) ) {
		vertexLayout = *found;
	} else {
		ID3D11InputLayout* layout = ZP_NULL;
		zp_uint stride = 0;
		HRESULT hr;

		// @TODO: for now, just do a simple look up for the defined vertex types
		if( inputDesc == "position_color" ) {
			D3D11_INPUT_ELEMENT_DESC desc[] = {
				{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			
			hr = ( (zpDX11RenderingEngine*)zpRenderingFactory::getRenderingEngine() )->getDevice()->CreateInputLayout( desc, ZP_ARRAY_LENGTH( desc ), vertexShaderBlob, vertexShaderBlobSize, &layout );
			
			stride = sizeof( zpVertexPositionColor );
		} else if( inputDesc == "position_texture" ) {
			D3D11_INPUT_ELEMENT_DESC desc[] = {
				{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 16,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			hr = ( (zpDX11RenderingEngine*)zpRenderingFactory::getRenderingEngine() )->getDevice()->CreateInputLayout( desc, ZP_ARRAY_LENGTH( desc ), vertexShaderBlob, vertexShaderBlobSize, &layout );

			stride = sizeof( zpVertexPositionTexture );
		} else if( inputDesc == "position_texture_normal" ) {
			D3D11_INPUT_ELEMENT_DESC desc[] = {
				{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 16,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 32,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			hr = ( (zpDX11RenderingEngine*)zpRenderingFactory::getRenderingEngine() )->getDevice()->CreateInputLayout( desc, ZP_ARRAY_LENGTH( desc ), vertexShaderBlob, vertexShaderBlobSize, &layout );

			stride = sizeof( zpVertexPositionNormalTexture );
		}

		vertexLayout = new zpDX11VertexLayout;
		vertexLayout->m_layout = layout;
		vertexLayout->m_stride = stride;

		s_layouts[ inputDesc ] = vertexLayout;
	}

	return vertexLayout;
}

ID3D11InputLayout* zpDX11VertexLayout::getInputLayout() const {
	return m_layout;
}
