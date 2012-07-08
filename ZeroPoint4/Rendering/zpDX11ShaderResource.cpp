#include "zpRendering.h"
#include "zpDX11.h"
#include <D3DX11.h>
#include <D3Dcompiler.h>

#define VERSION_2_0	"2_0"
#define VERSION_2_1	"2_1"
#define VERSION_3_0	"3_0"
#define VERSION_3_1	"3_1"
#define VERSION_4_0	"4_0"
#define VERSION_4_1	"4_1"
#define VERSION_5_0	"5_0"

#define PS_SHADER	"ps_"
#define VS_SHADER	"vs_"
#define GS_SHADER	"gs_"
#define CS_SHADER	"cs_"

zpDX11ShaderResource::zpDX11ShaderResource() :
	m_vertexShader( ZP_NULL ),
	m_pixelShader( ZP_NULL ),
	m_geometryShader( ZP_NULL ),
	m_computeShader( ZP_NULL )
{}
zpDX11ShaderResource::~zpDX11ShaderResource() {
	unload();
}

zp_bool zpDX11ShaderResource::load() {
	zpProperties shaderProperties( getFilename() );
	if( shaderProperties.isEmpty() ) return false;

	zpDX11RenderingEngine* engine = (zpDX11RenderingEngine*)zpRenderingFactory::getRenderingEngine();
	ID3DBlob* blob = ZP_NULL;
	ID3DBlob* errors = ZP_NULL;
	HRESULT hr;

	zp_dword shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#if ZP_DEBUG
	shaderFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	const zp_char* ps_version;
	const zp_char* gs_version;
	const zp_char* vs_version;
	const zp_char* cs_version;

	// default shader versions to proper levels
	switch( engine->getEngineType() ) {
	default:
	case ZP_RENDERING_ENGINE_DX10:
		ps_version = PS_SHADER VERSION_4_0;
		vs_version = VS_SHADER VERSION_4_0;
		gs_version = GS_SHADER VERSION_4_0;
		cs_version = CS_SHADER VERSION_4_0;
		break;
	case ZP_RENDERING_ENGINE_DX10_1:
		ps_version = PS_SHADER VERSION_4_1;
		vs_version = VS_SHADER VERSION_4_1;
		gs_version = GS_SHADER VERSION_4_1;
		cs_version = CS_SHADER VERSION_4_1;
		break;
	case ZP_RENDERING_ENGINE_DX11:
		ps_version = PS_SHADER VERSION_5_0;
		vs_version = VS_SHADER VERSION_5_0;
		gs_version = GS_SHADER VERSION_5_0;
		cs_version = CS_SHADER VERSION_5_0;
		break;
	}

	// get the actual shader file that will be compiled
	zpString shaderFile = shaderProperties[ "shader.file" ];
	zpString root = getFilename().substring( 0, getFilename().lastIndexOf( zpFile::sep ) + 1 );
	zpStringBuffer buff( root );
	buff << shaderFile;
	shaderFile = buff.toString();

	// get any macros that were defined
	zpProperties macroProperties = shaderProperties.getSubProperties( "shader.macro" );
	zpArrayList<D3D10_SHADER_MACRO> macros;
	macroProperties.foreach( [ &macros ]( const zpString& key, const zpString& val ) {
		D3D10_SHADER_MACRO t;
		t.Name = key.c_str();
		t.Definition = val.c_str();

		macros.pushBack( t );
	} );
	macros.pushBack( D3D10_SHADER_MACRO() );

	// if the pixel shader function is defined for this shader, compile and create it
	if( shaderProperties.hasProperty( "shader.ps" ) ) {
		zpString function = shaderProperties[ "shader.ps" ];
		
		hr = D3DX11CompileFromFile( shaderFile.c_str(), macros.begin(), ZP_NULL, function.c_str(), ps_version, shaderFlags, 0, ZP_NULL, &blob, &errors, ZP_NULL );
		if( errors ) {
			zp_printfcln( ZP_CC( ZP_CC_BLACK, ZP_CC_LIGHT_YELLOW ), "Error compiling shader [%s]: %s", shaderFile.c_str(), (zp_char*)errors->GetBufferPointer() );
		}
		ZP_SAFE_RELEASE( errors );

		if( SUCCEEDED( hr ) ) {
			hr = engine->getDevice()->CreatePixelShader( blob->GetBufferPointer(), blob->GetBufferSize(), ZP_NULL, &m_pixelShader );
			if( FAILED( hr ) ) {
				zp_printfln( "Failed to create Pixel Shader %s", shaderFile.c_str() );
			}
		} else {
			zp_printfln( "Failed to compile Pixel Shader %s", shaderFile.c_str() );
		}
		ZP_SAFE_RELEASE( blob );
	}

	// if the vertex shader function is defined for this shader, compile and create it
	if( shaderProperties.hasProperty( "shader.vs" ) ) {
		zpString function = shaderProperties[ "shader.vs" ];
		zpString layout = shaderProperties[ "shader.vs.layout" ];
	}

	// if the geometry shader function is defined for this shader, compile and create it
	if( shaderProperties.hasProperty( "shader.gs" ) ) {
		zpString function = shaderProperties[ "shader.gs" ];

		hr = D3DX11CompileFromFile( shaderFile.c_str(), macros.begin(), ZP_NULL, function.c_str(), gs_version, shaderFlags, 0, ZP_NULL, &blob, &errors, ZP_NULL );
		if( errors ) {
			zp_printfcln( ZP_CC( ZP_CC_BLACK, ZP_CC_LIGHT_YELLOW ), "Error compiling shader [%s]: %s", shaderFile.c_str(), (zp_char*)errors->GetBufferPointer() );
		}
		ZP_SAFE_RELEASE( errors );

		if( SUCCEEDED( hr ) ) {
			zpProperties streamProperties = shaderProperties.getSubProperties( "shader.gsso.decl" );

			if( streamProperties.isEmpty() ) {
				hr = engine->getDevice()->CreateGeometryShader( blob->GetBufferPointer(), blob->GetBufferSize(), ZP_NULL, &m_geometryShader );
				if( FAILED( hr ) ) {
					zp_printfln( "Failed to create Geometry Shader %s", shaderFile.c_str() );
				}
			} else {

			}
		}
	}

	// if the compute shader function is defined for this shader, compile and create it
	if( shaderProperties.hasProperty( "shader.cs" ) ) {
		zpString function = shaderProperties[ "shader.cs" ];

		hr = D3DX11CompileFromFile( shaderFile.c_str(), macros.begin(), ZP_NULL, function.c_str(), cs_version, shaderFlags, 0, ZP_NULL, &blob, &errors, ZP_NULL );
		if( errors ) {
			zp_printfcln( ZP_CC( ZP_CC_BLACK, ZP_CC_LIGHT_YELLOW ), "Error compiling shader [%s]: %s", shaderFile.c_str(), (zp_char*)errors->GetBufferPointer() );
		}
		ZP_SAFE_RELEASE( errors );

		if( SUCCEEDED( hr ) ) {
			hr = engine->getDevice()->CreateComputeShader( blob->GetBufferPointer(), blob->GetBufferSize(), ZP_NULL, &m_computeShader );
			if( FAILED( hr ) ) {
				zp_printfln( "Failed to create Compute Shader %s", shaderFile.c_str() );
			}
		} else {
			zp_printfln( "Failed to compile Compute Shader %s", shaderFile.c_str() );
		}
		ZP_SAFE_RELEASE( blob );
	}

	return m_pixelShader || m_vertexShader || m_geometryShader || m_computeShader;
}
void zpDX11ShaderResource::unload() {
	ZP_SAFE_RELEASE( m_vertexShader );
	ZP_SAFE_RELEASE( m_pixelShader );
	ZP_SAFE_RELEASE( m_geometryShader );
	ZP_SAFE_RELEASE( m_computeShader );
}