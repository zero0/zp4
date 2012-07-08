#pragma once
#ifndef ZP_DX11_SHADER_RESOURCE_H
#define ZP_DX11_SHADER_RESOURCE_H

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11ComputeShader;

class zpDX11ShaderResource : public zpShaderResource {
public:
	zpDX11ShaderResource();
	virtual ~zpDX11ShaderResource();

	zp_bool load();
	void unload();

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11GeometryShader* m_geometryShader;
	ID3D11ComputeShader* m_computeShader;
};

#endif