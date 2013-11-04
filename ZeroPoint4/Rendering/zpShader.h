#pragma once
#ifndef ZP_SHADER_H
#define ZP_SHADER_H

enum zpShaderType
{
	ZP_SHADER_TYPE_VERTEX =		0x01,
	ZP_SHADER_TYPE_PIXEL =		0x02,
	ZP_SHADER_TYPE_GEOMETRY =	0x04,
	ZP_SHADER_TYPE_COMPUTE =	0x08,
};

class zpShaderImpl;

class zpShader
{
public:
	zpShader();
	~zpShader();

	zpShaderImpl* getShaderImpl() const;

protected:
	zpShaderImpl* m_shader;

	friend class zpRenderingEngine;
};

class zpShaderResource : public zpResource< zpShader >
{
private:
	zp_bool load( const zp_char* filename, zpRenderingEngine* engine );
	void unload( zpRenderingEngine* engine );

	friend class zpShaderContentManager;

private:
};

class zpShaderResourceInstance : public zpResourceInstance< zpShaderResource >
{};

class zpShaderContentManager : public zpContentManager< zpShaderResource, zpShaderResourceInstance, zpShaderContentManager, 128 >
{
private:
	zp_bool createResource( zpShaderResource* res, const zp_char* filename );
	void destroyResource( zpShaderResource* res );
	void initializeInstance( zpShaderResourceInstance& instance ) {}

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
