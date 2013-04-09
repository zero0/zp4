#include "zpDX11.h"
#include <D3D11.h>

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

zpShaderImpl::zpShaderImpl()
	: m_vertexShader( ZP_NULL )
	, m_pixelShader( ZP_NULL )
	, m_geometryShader( ZP_NULL )
	, m_computeShader( ZP_NULL )
	, m_vertexLayout( ZP_VERTEX_FORMAT_DESC_VERTEX_COLOR )
{}
zpShaderImpl::~zpShaderImpl()
{
	unload();
}

zp_bool zpShaderImpl::load( zpRenderingEngineImpl* engine )
{
	return engine->loadShader( this );
}
void zpShaderImpl::unload()
{
	ZP_SAFE_RELEASE( m_vertexShader );
	ZP_SAFE_RELEASE( m_pixelShader );
	ZP_SAFE_RELEASE( m_geometryShader );
	ZP_SAFE_RELEASE( m_computeShader );
}

#if 0
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
//if ZP_DEBUG
//	shaderFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_SKIP_OPTIMIZATION;
//endif

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
		t.Name = key.getChars();
		t.Definition = val.getChars();

		macros.pushBack( t );
	} );
	macros.pushBack( D3D10_SHADER_MACRO() );

	// if the pixel shader function is defined for this shader, compile and create it
	if( shaderProperties.hasProperty( "shader.ps" ) ) {
		zpString function = shaderProperties[ "shader.ps" ];
		
		hr = D3DX11CompileFromFile( shaderFile.getChars(), macros.begin(), ZP_NULL, function.getChars(), ps_version, shaderFlags, 0, ZP_NULL, &blob, &errors, ZP_NULL );
		if( errors ) {
			zpLog::error() << (const zp_char*)errors->GetBufferPointer() << zpLog::endl;
		}
		ZP_SAFE_RELEASE( errors );

		if( SUCCEEDED( hr ) ) {
			hr = engine->getDevice()->CreatePixelShader( blob->GetBufferPointer(), blob->GetBufferSize(), ZP_NULL, &m_pixelShader );
			if( FAILED( hr ) ) {
				zpLog::error() << "Failed to create Pixel Shader " << shaderFile << zpLog::endl;
			} else {
				zpLog::message() << "Pixel Shader created " << shaderFile << zpLog::endl;
			}
		} else {
			zpLog::error() << "Failed to compile Pixel Shader " << shaderFile << zpLog::endl;
		}
		ZP_SAFE_RELEASE( blob );

		m_type.mark( ZP_SHADER_TYPE_PIXEL );
	}

	// if the vertex shader function is defined for this shader, compile and create it
	if( shaderProperties.hasProperty( "shader.vs" ) ) {
		zpString function = shaderProperties[ "shader.vs" ];
		zpString layout = shaderProperties[ "shader.vs.layout" ];

		hr = D3DX11CompileFromFile( shaderFile.getChars(), macros.begin(), ZP_NULL, function.getChars(), vs_version, shaderFlags, 0, ZP_NULL, &blob, &errors, ZP_NULL );
		if( errors ) {
			zpLog::error() << (const zp_char*)errors->GetBufferPointer() << zpLog::endl;
		}
		ZP_SAFE_RELEASE( errors );

		if( SUCCEEDED( hr ) ) {
			hr = engine->getDevice()->CreateVertexShader( blob->GetBufferPointer(), blob->GetBufferSize(), ZP_NULL, &m_vertexShader );
			if( FAILED( hr ) ) {
				zpLog::error() << "Failed to create Vertex Shader " << shaderFile << zpLog::endl;
			} else {
				zpLog::message() << "Vertex Shader created " << shaderFile << zpLog::endl;
			}

			// get the vertex layout from the shader file
			m_vertexLayout = zpDX11VertexLayout::getLayoutFromDesc( layout, blob->GetBufferPointer(), blob->GetBufferSize() );
			if( m_vertexLayout == ZP_NULL ) {
				zpLog::error() << "Failed to get Vertex Layout " << layout << zpLog::endl;
			}
		} else {
			zpLog::error() << "Failed to compile Vertex Shader " << shaderFile << zpLog::endl;
		}
		ZP_SAFE_RELEASE( blob );

		m_type.mark( ZP_SHADER_TYPE_VERTEX );
	}

	// if the geometry shader function is defined for this shader, compile and create it
	if( shaderProperties.hasProperty( "shader.gs" ) ) {
		zpString function = shaderProperties[ "shader.gs" ];

		hr = D3DX11CompileFromFile( shaderFile.getChars(), macros.begin(), ZP_NULL, function.getChars(), gs_version, shaderFlags, 0, ZP_NULL, &blob, &errors, ZP_NULL );
		if( errors ) {
			zpLog::error() << (const zp_char*)errors->GetBufferPointer() << zpLog::endl;
		}
		ZP_SAFE_RELEASE( errors );

		if( SUCCEEDED( hr ) ) {
			zpProperties streamProperties = shaderProperties.getSubProperties( "shader.gsso.decl" );

			if( streamProperties.isEmpty() ) 
			// create non-stream out geometry shader
			{
				hr = engine->getDevice()->CreateGeometryShader( blob->GetBufferPointer(), blob->GetBufferSize(), ZP_NULL, &m_geometryShader );
				if( FAILED( hr ) ) {
					zpLog::error() << "Failed to create Geometry Shader " << shaderFile << zpLog::endl;
				} else {
					zpLog::message() << "Geometry Shader created " << shaderFile << zpLog::endl;
				}
			}
			else
			// create stream out geometry shader
			{
				zpArrayList<D3D11_SO_DECLARATION_ENTRY> entries;
				entries.ensureCapacity( streamProperties.size() );

				streamProperties.foreach( [ &entries ]( const zpString& key, const zpString& val ) {
					D3D11_SO_DECLARATION_ENTRY entry;
					zp_uint index = 0;

					// grab the values from the string using a scan
					//val.scan( "%d,%d,%d,%d,%d,%d", &index, &entry.Stream, &entry.SemanticIndex, &entry.StartComponent, &entry.ComponentCount, &entry.OutputSlot );
					entry.SemanticName = key.getChars();

					entries[ index ] = entry;
				} );

				zp_uint strides[1];
				strides[0] = shaderProperties.getInt( "shader.gsso.strides" );

				// compile the geometry shader with stream output
				hr = engine->getDevice()->CreateGeometryShaderWithStreamOutput( blob->GetBufferPointer(), blob->GetBufferSize(), entries.begin(), entries.size(), strides, 1, D3D11_SO_NO_RASTERIZED_STREAM, NULL, &m_geometryShader );
				if( FAILED( hr ) ) {
					zpLog::error() << "Failed to create Geometry Stream Out Shader " << shaderFile << zpLog::endl;
				} else {
					zpLog::message() << "Geometry Shader Stream Out created " << shaderFile << zpLog::endl;
				}
			}
		} else {
			zpLog::error() << "Failed to compile Geometry Shader " << shaderFile << zpLog::endl;
		}
		ZP_SAFE_RELEASE( blob );

		m_type.mark( ZP_SHADER_TYPE_GEOMETRY );
	}

	// if the compute shader function is defined for this shader, compile and create it
	if( shaderProperties.hasProperty( "shader.cs" ) ) {
		zpString function = shaderProperties[ "shader.cs" ];

		hr = D3DX11CompileFromFile( shaderFile.getChars(), macros.begin(), ZP_NULL, function.getChars(), cs_version, shaderFlags, 0, ZP_NULL, &blob, &errors, ZP_NULL );
		if( errors ) {
			zpLog::error() << (const zp_char*)errors->GetBufferPointer() << zpLog::endl;
		}
		ZP_SAFE_RELEASE( errors );

		if( SUCCEEDED( hr ) ) {
			hr = engine->getDevice()->CreateComputeShader( blob->GetBufferPointer(), blob->GetBufferSize(), ZP_NULL, &m_computeShader );
			if( FAILED( hr ) ) {
				zpLog::error() << "Failed to create Compute Shader " << shaderFile << zpLog::endl;
			} else {
				zpLog::message() << "Compute Shader created " << shaderFile << zpLog::endl;
			}
		} else {
			zpLog::error() << "Failed to compile Compute Shader " << shaderFile << zpLog::endl;
		}
		ZP_SAFE_RELEASE( blob );

		m_type.mark( ZP_SHADER_TYPE_COMPUTE );
	}

	return m_pixelShader || m_vertexShader || m_geometryShader || m_computeShader;
}
void zpDX11ShaderResource::unload() {
	ZP_SAFE_RELEASE( m_vertexShader );
	ZP_SAFE_RELEASE( m_pixelShader );
	ZP_SAFE_RELEASE( m_geometryShader );
	ZP_SAFE_RELEASE( m_computeShader );
	m_type.clear();
}

zpVertexLayout* zpDX11ShaderResource::getVertexLayout() {
	return m_vertexLayout;
}

ID3D11VertexShader* zpDX11ShaderResource::getVertexShader() {
	return m_vertexShader;
}
ID3D11PixelShader* zpDX11ShaderResource::getPixelShader() {
	return m_pixelShader;
}
ID3D11GeometryShader* zpDX11ShaderResource::getGeometryShader() {
	return m_geometryShader;
}
ID3D11ComputeShader* zpDX11ShaderResource::getComputeShader() {
	return m_computeShader;
}
#endif
