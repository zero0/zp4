#include "zpRendering.h"

zpShaderResource::zpShaderResource() {}
zpShaderResource::~zpShaderResource() {
	m_type.clear();
}

zp_bool zpShaderResource::isShaderType( zpShaderType type ) const {
	return m_type.isMarked( (zp_uint)type );
}
