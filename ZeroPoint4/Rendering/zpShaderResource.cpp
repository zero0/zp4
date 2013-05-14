#include "zpRendering.h"

zpShader::zpShader( zpShaderImpl* impl )
	: m_shader( impl )
{}
zpShader::~zpShader()
{}

zpShaderImpl* zpShader::getShaderImpl() const
{
	return m_shader;
}

#if 0
zpShaderResource::zpShaderResource() {}
zpShaderResource::~zpShaderResource() {
	m_type.clear();
}

zp_bool zpShaderResource::isShaderType( zpShaderType type ) const {
	return m_type.isMarked( (zp_uint)type );
}
#endif
