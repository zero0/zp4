#pragma once
#ifndef ZP_SHADER_H
#define ZP_SHADER_H

class zpShaderImpl;

class zpShader
{
public:
    zpShader();
    ~zpShader();

    zpShaderImpl* getShaderImpl() const;
    const zpBison& getShaderData() const;

protected:
    zpShaderImpl* m_shader;
    zpBison m_shaderData;

    friend class zpRenderingEngine;
    friend class zpShaderResource;
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
