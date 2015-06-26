#pragma once
#ifndef ZP_DX11_SHADER_RESOURCE_H
#define ZP_DX11_SHADER_RESOURCE_H

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11ComputeShader;

class zpShaderImpl
{
public:
	zpShaderImpl();
	~zpShaderImpl();

	zp_bool load( zpRenderingEngineImpl* engine, const zpBison::Value& shaderFile );
	void unload();

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11GeometryShader* m_geometryShader;
	ID3D11ComputeShader* m_computeShader;

	zp_hash m_vertexLayout;

	friend class zpRenderingEngineImpl;
	friend class zpRenderingContextImpl;
};

#endif
