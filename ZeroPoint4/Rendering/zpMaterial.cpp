#include "zpRendering.h"
#include "Common/zpCommon.h"

#define ZP_MATERIAL_GLOBAL_CBUFFER    "$Globals"

zp_bool zpMaterialResource::load( const zp_char* filename, zpRenderingPipeline* pipeline, zp_uint materialId )
{
    zp_bool ok;
    m_filename = filename;

    zpBison material;
    ok = material.readFromFile( m_filename );

    if( ok )
    {
        //
        // Load material properties
        //

        zp_bool hasGlobalBuffer = false;

        // set material id
        m_resource.materialId = materialId;

        // get material root
        const zpBison::Value& materialRoot = material.root();

        // load sort bias
        zp_int sortBias = materialRoot[ "SortBias" ].asInt();
        m_resource.sortBias = sortBias;

        // load blend mode for the material
        const zpBison::Value& blendMode = materialRoot[ "BlendMode" ];
        if( !blendMode.isEmpty() )
        {
            zpBlendStateDesc blendDesc;
            pipeline->generateBlendStateDesc( blendMode, blendDesc );
            pipeline->getRenderingEngine()->createBlendState( m_resource.blend, blendDesc );
        }

        // load depth stencil for material
        const zpBison::Value& depth = materialRoot[ "Depth" ];
        if( !depth.isEmpty() )
        {
            zpDepthStencilStateDesc depthDesc;
            pipeline->generateDepthStencilStateDesc( depth, depthDesc );
            pipeline->getRenderingEngine()->createDepthStencilState( m_resource.depth, depthDesc );
        }

        // load raster state for material
        const zpBison::Value& raster = materialRoot[ "Raster" ];
        if( !raster.isEmpty() )
        {
            zpRasterStateDesc rasterDesc;
            pipeline->generateRasterStateDesc( raster, rasterDesc );
            pipeline->getRenderingEngine()->createRasterState( m_resource.raster, rasterDesc );
        }

        // load render queue
        m_resource.queue = ZP_RENDERING_QUEUE_OPAQUE;
        const zpBison::Value& renderQueue = materialRoot[ "Queue" ];
        if( !renderQueue.isEmpty() )
        {
            const zp_char* renderQueueStr = renderQueue.asCString();
            for( zp_int i = 0; i < zpRenderingQueue_Count; ++i )
            {
                if( zp_strcmp( renderQueueStr, g_renderingQueues[ i ] ) == 0 )
                {
                    m_resource.queue = (zpRenderingQueue)i;
                    break;
                }
            }
        }

        //
        // Load Shader slots
        //

        // load shader for the material
        const zp_char* shaderFile = materialRoot[ "Shader" ].asCString();
        ok = pipeline->getShaderContentManager()->getResource( shaderFile, m_resource.shader );
        ZP_ASSERT( ok, "Failed to load shader '%s' for material '%s'", shaderFile, getFilename().str() );

        // get shader data
        const zpBison::Value& shaderData = m_resource.shader.getResource()->getData()->getShaderData().root();

        // get globals for shader
        const zpBison::Value& shaderGlobals = shaderData[ "Globals" ];
        if( !shaderGlobals.isEmpty() )
        {
            hasGlobalBuffer = true;

            // global variables size
            zp_uint globalVariablesSize = shaderGlobals[ "Size" ].asInt();
            pipeline->getRenderingEngine()->createBuffer( m_resource.globalVariables, ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DYNAMIC, globalVariablesSize );

            // build constant buffer def list
            const zpBison::Value& globalVariables = shaderGlobals[ "Variables" ];
            m_resource.globalVariablesDef.reserve( globalVariables.size() );
            globalVariables.foreachArray( [ this ]( const zpBison::Value& glob ) {
                const zp_char* globalType = glob[ "Type" ].asCString();
                const zp_char* globalName = glob[ "Name" ].asCString();
                zp_uint globalSize =   (zp_uint)glob[ "Size" ].asInt();
                zp_uint globalOffset = (zp_uint)glob[ "Offset" ].asInt();

                zpMaterialGlobalVariable& def = m_resource.globalVariablesDef.pushBackEmpty();
                def.name = globalName;
                def.size = globalSize;
                def.offset = globalOffset;
            } );
        }

        zpFixedArrayList< zpString, 8 > textures;
        textures.resize( 8 );

        // pixel shader slots
        const zpBison::Value ps = shaderData[ "PS" ];
        if( !ps.isEmpty() )
        {
            const zpBison::Value& textureData = ps[ "Textures" ];
        
            textureData.foreachArray( [ &textures ]( const zpBison::Value& t ) {
                const zp_char* name = t[ "Name" ].asCString();
                zp_uint slot = t[ "Slot" ].asInt();

                textures[ slot ] = name;
            } );
        }

        //
        // Load material globals
        //

        // load textures for material
        const zpBison::Value& matTextures = materialRoot[ "Textures" ];
        if( !matTextures.isEmpty() )
        {
            m_resource.materialTextures.reserve( matTextures.size() );
            matTextures.foreachObject( [ this, pipeline, &textures ]( const zpBison::Value& key, const zpBison::Value& textureValue ) {
                zpString name( key.asCString() );
                zp_size_t slot = textures.indexOf( name );
                if( slot != zpArrayList< zpString >::npos )
                {
                    const zp_char* textureFile = textureValue[ "Texture" ].asCString();

                    const zpBison::Value& textureOffset = textureValue[ "Offset" ];
                    const zpBison::Value& textureScale = textureValue[ "Scale" ];
                    const zpBison::Value& textureSampler = textureValue[ "Sampler" ];

                    zpMaterialTexture& t = m_resource.materialTextures.pushBackEmpty();
                    t.name = name;
                    t.scaleOffset = zpMath::Vector4( textureScale[ 0 ].asFloat(), textureScale[ 1 ].asFloat(), textureOffset[ 0 ].asFloat(), textureOffset[ 1 ].asFloat() );

                    zp_bool ok = pipeline->getTextureContentManager()->getResource( textureFile, t.texture );
                    ZP_ASSERT( ok, "Failed to get texture %s", textureFile );

                    zpSamplerStateDesc desc;
                    pipeline->generateSamplerStateDesc( textureSampler, desc );
                    pipeline->getRenderingEngine()->createSamplerState( t.sampler, desc );

                    t.slot = slot;
                }
            } );
        }

        // load globals for material
        const zpBison::Value& matGlobals = materialRoot[ "Globals" ];
        if( hasGlobalBuffer && !matGlobals.isEmpty() )
        {
            zpRenderingContext* cxt = pipeline->getRenderingEngine()->getImmediateRenderingContext();

            zp_byte* buffer;
            cxt->map( &m_resource.globalVariables, (void**)&buffer );

            if( !m_resource.materialTextures.isEmpty() )
            {
                zpFixedStringBuffer< 64 > textureST;
                for( zp_size_t i = 0; i < m_resource.materialTextures.size(); ++i )
                {
                    const zpMaterialTexture& matTexture = m_resource.materialTextures[ i ];
                    
                    textureST.clear();
                    textureST << matTexture.name << "_ST";

                    zpMaterialGlobalVariable* var;
                    zp_bool found = m_resource.globalVariablesDef.findIf( [ &textureST ]( const zpMaterialGlobalVariable& def ){
                        return def.name == textureST.str();
                    }, &var );
                    ZP_ASSERT_WARN( found, "Shader did not define global %s", textureST.str() );

                    if( found )
                    {
                        zp_float* ptr = (zp_float*)( buffer + var->offset );
                        zpMath::Vector4Store4( matTexture.scaleOffset, ptr );
                    }
                }
            }

            matGlobals.foreachObject( [ this, buffer ]( const zpBison::Value& globalKey, const zpBison::Value& globalValue ) {
                const zp_char* globalName = globalKey.asCString();

                const zp_char* globalType = globalValue[ "Type" ].asCString();
                const zpBison::Value& value = globalValue[ "Value" ];

                zpMaterialGlobalVariable* var;
                zp_bool found = m_resource.globalVariablesDef.findIf( [ globalName ]( const zpMaterialGlobalVariable& def ){
                    return def.name == globalName;
                }, &var );

                if( found )
                {
                    zp_float* ptr = (zp_float*)( buffer + var->offset );
                    if( zp_strcmp( globalType, "Float" ) == 0 )
                    {
                        zp_float val = value.asFloat();
                        *ptr = val;
                    }
                    else if( zp_strcmp( globalType, "Float2" ) == 0 )
                    {
                        zpVector2f val( value[ 0 ].asFloat(), value[ 1 ].asFloat() );
                        ptr[ 0 ] = val.getX();
                        ptr[ 1 ] = val.getY();
                    }
                    else if( zp_strcmp( globalType, "Float4" ) == 0 )
                    {
                        zpVector4f val = zpMath::Vector4( value[ 0 ].asFloat(), value[ 1 ].asFloat(), value[ 2 ].asFloat(), value[ 3 ].asFloat() );
                        zpMath::Vector4Store4( val, ptr );
                    }
                    else if( zp_strcmp( globalType, "Color" ) == 0 )
                    {
                        zpColor4f val( value[ 0 ].asFloat(), value[ 1 ].asFloat(), value[ 2 ].asFloat(), value[ 3 ].asFloat() );
                        ( *(zpColor4f*)( buffer + var->offset ) ) = val;
                        val.store4( ptr );
                    }
                    //else if( zp_strcmp( globalType, "float4x4" ) == 0 )
                    //{
                    //    zpVector4f val( value[ 0 ].asFloat(), value[ 1 ].asFloat(), value[ 2 ].asFloat(), value[ 3 ].asFloat() );
                    //    ( *(zpVector4f*)( buffer + var->offset ) ) = val;
                    //}
                }
                else
                {
                    zp_printfln( "Shader did not define global %s", globalName );
                }
            } );

            cxt->unmap( &m_resource.globalVariables );
        }


#if 0

        // load textures for the material
        const zpBison::Value& textures = materialRoot[ "Textures" ];
        m_resource.textures.reserve( textures.size() );
        textures.foreachArray( [ this, pipeline, &ok ]( const zpBison::Value& v )
        {
            const zp_char* tex = v.asCString();

            zpMaterial::zpMaterialTextureSampler& t = m_resource.textures.pushBackEmpty();
            t.bindSlots = ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER;

            ok = pipeline->getTextureContentManager()->getResource( tex, t.texture );
            ZP_ASSERT( ok, "Failed to load texture '%s' for material '%s'", tex, getFilename().str() );

            zpSamplerStateDesc sampler;
            pipeline->getRenderingEngine()->createSamplerState( t.sampler, sampler );
        } );

        // load constants from the material
        const zpBison::Value& constants = materialRoot[ "Constants" ];


        // load texture samplers for the material
        const zpBison::Value& samples = materialRoot[ "Samplers" ];
        samples.foreachArrayIndexed( [ this, pipeline ]( zp_uint i, const zpBison::Value& v )
        {
            zpMaterial::zpMaterialTextureSampler& t = m_resource.textures[ i ];

            zpSamplerStateDesc sampler;
            pipeline->generateSamplerStateDesc( v, sampler );
            pipeline->getRenderingEngine()->createSamplerState( t.sampler, sampler );
        } );

        
#endif

    }

    return ok;
}
void zpMaterialResource::unload( zpRenderingPipeline* pipeline )
{
    m_resource.shader.release();

    pipeline->getRenderingEngine()->destroyBuffer( m_resource.globalVariables );
    m_resource.globalVariablesDef.clear();

    m_resource.materialTextures.foreach( []( zpMaterialTexture& t ) { t.texture.release(); } );
    m_resource.materialTextures.clear();

    m_resource.materialId = 0;
    m_resource.sortBias = 0;
}

zpMaterialContentManager::zpMaterialContentManager()
    : m_currentMaterialId( 0 )
{}
zp_bool zpMaterialContentManager::createResource( zpMaterialResource* res, const zp_char* filename )
{
    return res->load( filename, getApplication()->getRenderPipeline(), ++m_currentMaterialId );
}
void zpMaterialContentManager::destroyResource( zpMaterialResource* res )
{
    res->unload( getApplication()->getRenderPipeline() );
}
