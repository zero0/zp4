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

	zp_bool load( zpRenderingEngineImpl* engine, const zpBison& shaderFile );
	void unload();

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11GeometryShader* m_geometryShader;
	ID3D11ComputeShader* m_computeShader;

	zpVertexFormatDesc m_vertexLayout;

	friend class zpRenderingEngineImpl;
	friend class zpRenderingContextImpl;
};

#if 0
class zpDX11ShaderResource : public zpShaderResource {
public:
	virtual ~zpDX11ShaderResource();

	zp_bool load();
	void unload();

	zpVertexLayout* getVertexLayout();

private:
	zpDX11ShaderResource();
	
	ID3D11VertexShader* getVertexShader();
	ID3D11PixelShader* getPixelShader();
	ID3D11GeometryShader* getGeometryShader();
	ID3D11ComputeShader* getComputeShader();

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11GeometryShader* m_geometryShader;
	ID3D11ComputeShader* m_computeShader;

	zpVertexLayout* m_vertexLayout;

	friend class zpDX11RenderingEngine;
	friend class zpDX11RenderingContext;
};
#endif

#endif