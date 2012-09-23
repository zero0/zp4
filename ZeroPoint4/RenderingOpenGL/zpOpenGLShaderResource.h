#pragma once
#ifndef ZP_OPENGL_SHADER_RESOURCE_H
#define ZP_OPENGL_SHADER_RESOURCE_H

class zpOpenGLShaderResource : public zpShaderResource {
public:
	virtual ~zpOpenGLShaderResource();

	zp_bool load();
	void unload();

	zpVertexLayout* getVertexLayout();

private:
	zpOpenGLShaderResource();

	zp_uint getShaderProgram();
	zp_uint getVertexShader();
	zp_uint getFragmentShader();
	zp_uint getGeometryShader();

	zp_uint m_shaderProgram;
	zp_uint m_vertexShader;
	zp_uint m_fragmentShader;
	zp_uint m_geometryShader;

	zpVertexLayout* m_vertexLayout;

	friend class zpOpenGLRenderingEngine;
	friend class zpOpenGLRenderingContext;
};

#endif