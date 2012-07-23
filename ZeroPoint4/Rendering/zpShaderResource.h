#pragma once
#ifndef ZP_SHADER_RESOURCE_H
#define ZP_SHADER_RESOURCE_H

enum zpShaderType {
	ZP_SHADER_TYPE_VERTEX =		0x01,
	ZP_SHADER_TYPE_PIXEL =		0x02,
	ZP_SHADER_TYPE_GEOMETRY =	0x04,
	ZP_SHADER_TYPE_COMPUTE =	0x08,
};

ZP_PURE_INTERFACE zpShaderResource : public zpResource {

};

#endif
