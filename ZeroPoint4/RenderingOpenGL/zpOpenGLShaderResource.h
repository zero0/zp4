#pragma once
#ifndef ZP_OPENGL_SHADER_RESOURCE_H
#define ZP_OPENGL_SHADER_RESOURCE_H

class zpShaderImpl
{
public:
	zpShaderImpl();
	~zpShaderImpl();

	zp_bool load( zpRenderingEngineImpl* engine, const zpBison::Value& shaderFile );
	void unload();

private:
	zp_uint m_shaderProgram;
	zp_uint m_vertexShader;
	zp_uint m_fragmentShader;
	zp_uint m_geometryShader;
	zp_uint m_computeShader;

	zpVertexFormatDesc m_vertexLayout;

	friend class zpRenderingEngineImpl;
	friend class zpRenderingContextImpl;
};

#endif
