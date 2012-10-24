#pragma once
#ifndef ZP_SHADER_RESOURCE_H
#define ZP_SHADER_RESOURCE_H

enum zpShaderType {
	ZP_SHADER_TYPE_VERTEX =		0x01,
	ZP_SHADER_TYPE_PIXEL =		0x02,
	ZP_SHADER_TYPE_GEOMETRY =	0x04,
	ZP_SHADER_TYPE_COMPUTE =	0x08,
};

ZP_ABSTRACT_CLASS zpShaderResource : public zpResource {
public:
	virtual ~zpShaderResource();

	zp_bool isShaderType( zpShaderType type ) const;

protected:
	zpShaderResource();

	zpFlag8 m_type;
};

ZP_RESOURCE_INSTANCE_TEMPLATE_START( zpShaderResource )
ZP_RESOURCE_INSTANCE_TEMPLATE_END

#endif
