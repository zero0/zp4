#pragma once
#ifndef ZP_SHADER_RESOURCE_H
#define ZP_SHADER_RESOURCE_H

enum zpShaderType {
	ZP_SHADER_TYPE_VERTEX =		0x01,
	ZP_SHADER_TYPE_PIXEL =		0x02,
	ZP_SHADER_TYPE_GEOMETRY =	0x04,
	ZP_SHADER_TYPE_COMPUTE =	0x08,
};

#define ZP_SHADER_FILE_HEADER	ZP_MAKE_UINT( 'Z', 'P', 'S', 'B' )

struct zpShaderFileHeader
{
	enum zpShaderFileHeaderTypes : zp_uint
	{
		VS,
		PS,
		GS,
		CS,

		zpShaderHeaderTypes_Count,
		zpShaderHeaderTypes_Force32 = ZP_FORECE_32BIT,
	};

	zp_uint fileType;
	zp_uint shaderType;
	zp_uint shaderVersion;
	zp_uint vertexLayout;

	zp_uint shaderLengths[ zpShaderHeaderTypes_Count ];
};

class zpShaderImpl;

class zpShader
{
public:
	~zpShader();

	zpShaderImpl* getShaderImpl() const;

protected:
	zpShader( zpShaderImpl* impl );

	zpShaderImpl* m_shader;

	friend class zpRenderingEngine;
};

class zpShaderResource : public zpResource
{

};

ZP_RESOURCE_INSTANCE_TEMPLATE_START( zpShaderResource )
ZP_RESOURCE_INSTANCE_TEMPLATE_END

#endif
