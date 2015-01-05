#include "Main.h"
#include <D3DX11.h>
#include <D3Dcompiler.h>

#define DX_VERTEX_SHADER	"vs"
#define DX_PIXEL_SHADER		"ps"

#define SHADER_MODEL_3_0	"3_0"
#define SHADER_MODEL_4_0	"4_0"
#define SHADER_MODEL_5_0	"5_0"

#define DX9_VERTEX_SHADER	DX_VERTEX_SHADER "_" SHADER_MODEL_3_0
#define DX10_VERTEX_SHADER	DX_VERTEX_SHADER "_" SHADER_MODEL_4_0
#define DX11_VERTEX_SHADER	DX_VERTEX_SHADER "_" SHADER_MODEL_5_0

#define DX9_PIXEL_SHADER	DX_PIXEL_SHADER "_" SHADER_MODEL_3_0
#define DX10_PIXEL_SHADER	DX_PIXEL_SHADER "_" SHADER_MODEL_4_0
#define DX11_PIXEL_SHADER	DX_PIXEL_SHADER "_" SHADER_MODEL_5_0

#pragma comment( lib, "d3d11" )
#pragma comment( lib, "dxgi" )
#pragma comment( lib, "dxguid" )

#if ZP_DEBUG
#pragma comment( lib, "d3dx11d" )
#else
#pragma comment( lib, "d3dx11" )
#endif

class DXShaderInclude : public ID3D10Include
{
public:
	DXShaderInclude( const zp_char* systemDir, const zp_char* localDir )
		: m_systemDir( systemDir )
		, m_localDir( localDir )
	{}
	~DXShaderInclude()
	{}

	HRESULT __stdcall Open( D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes )
	{
		zpStringBuffer includeFilePath;
		switch( IncludeType )
		{
		case D3D_INCLUDE_LOCAL:
			includeFilePath.append( m_localDir );
			break;
		case D3D_INCLUDE_SYSTEM:
			includeFilePath.append( m_systemDir );
			break;
		default:
			return E_FAIL;
		}

		includeFilePath.append( zpFile::sep );
		includeFilePath.append( pFileName );

		zpFile includeFile( includeFilePath.str() );
		if( includeFile.open( ZP_FILE_MODE_BINARY_READ ) )
		{
			includeFile.readFileBinary( m_data );
			includeFile.close();

			*ppData = m_data.getData();
			*pBytes = m_data.size();
		}
		else
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT __stdcall Close( LPCVOID pData )
	{
		m_data.destroy();
		return S_OK;
	}

private:
	zpString m_systemDir;
	zpString m_localDir;

	zpDataBuffer m_data;
};

DXShaderCompiler::DXShaderCompiler( DXShaderCompilerPlatform platform )
	: m_platform( platform )
{}
DXShaderCompiler::~DXShaderCompiler()
{}

void DXShaderCompiler::initializePlatform()
{
	zpString one( "1" );

	// add the HLSL api macro
	m_macros.put( zpString( MACRO_SHADER_API_HLSL ), one );

	// add the platform specific macro
	switch( m_platform )
	{
	case DX_SHADER_COMPILER_PLATFORM_DX9:
		m_macros.put( zpString( MACRO_SHADER_DX9 ), one );
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX10:
		m_macros.put( zpString( MACRO_SHADER_DX10 ), one );
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX11:
		m_macros.put( zpString( MACRO_SHADER_DX11 ), one );
		break;
	}
}

zp_bool DXShaderCompiler::compileShaderVSPlatform( zpDataBuffer& data )
{
	const zp_char* profile;
	switch( m_platform )
	{
	case DX_SHADER_COMPILER_PLATFORM_DX9:
		profile = DX9_VERTEX_SHADER;
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX10:
		profile = DX10_VERTEX_SHADER;
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX11:
		profile = DX11_VERTEX_SHADER;
		break;
	}

	const zp_char* mainFunc = DEFAULT_VS_MAIN;
	const zpString* f;
	if( m_pragmas.find( zpString( PRAGMA_VERTEX ), &f ) )
	{
		mainFunc = f->str();
	}

	zp_bool ok = compileShaderPlatform( mainFunc, profile, data );
	return ok;
}
zp_bool DXShaderCompiler::compileShaderPSPlatform( zpDataBuffer& data )
{
	const zp_char* profile;
	switch( m_platform )
	{
	case DX_SHADER_COMPILER_PLATFORM_DX9:
		profile = DX9_PIXEL_SHADER;
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX10:
		profile = DX10_PIXEL_SHADER;
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX11:
		profile = DX11_PIXEL_SHADER;
		break;
	}

	const zp_char* mainFunc = DEFAULT_PS_MAIN;
	const zpString* f;
	if( m_pragmas.find( zpString( PRAGMA_FRAGMENT ), &f ) )
	{
		mainFunc = f->str();
	}

	zp_bool ok = compileShaderPlatform( mainFunc, profile, data );
	return ok;
}

zp_bool DXShaderCompiler::compileShaderGSPlatform( zpDataBuffer& data )
{
	return false;
}
zp_bool DXShaderCompiler::compileShaderCSPlatform( zpDataBuffer& data )
{
	return false;
}

zp_bool DXShaderCompiler::compileShaderPlatform( const zp_char* mainFunc, const zp_char* profile, zpDataBuffer& data ) const
{
	// get the shader text and size
	const zp_char* shaderText = m_shaderText.str();
	zp_uint shaderTextSize = m_shaderText.length();

	// convert macros to DX macros, adding an empty one to for the end
	zpArrayList< D3D10_SHADER_MACRO > macros;
	if( !m_macros.isEmpty() )
	{
		macros.reserve( macros.size() + 1 );
		m_macros.foreach( [ &macros ]( const zpString& k, const zpString& v )
		{
			D3D10_SHADER_MACRO& shaderMacro = macros.pushBackEmpty();
			shaderMacro.Name = k.str();
			shaderMacro.Definition = v.str();
		} );
		macros.pushBackEmpty();
	}

	// process flags
	zp_uint flags = 0;
	switch( m_optimizationLevel )
	{
	case -1: flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION; break;
	case 0:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL0; break;
	case 1:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL1; break;
	case 2:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL2; break;
	case 3:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3; break;
	case 4:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_WARNINGS_ARE_ERRORS; break;
	case 5:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS; break;
	default: flags |= D3DCOMPILE_OPTIMIZATION_LEVEL0; break;
	}

	// create include finder
	DXShaderInclude include( m_localIncludeDir.str(), m_systemIncludeDir.str() );

	// compile shader
	HRESULT hr;
	ID3D10Blob* shaderBlob = ZP_NULL;
	ID3D10Blob* errors = ZP_NULL;
	hr = D3DX11CompileFromMemory( shaderText, shaderTextSize, m_inputFile.str(), macros.begin(), &include, mainFunc, profile, flags, 0, ZP_NULL, &shaderBlob, &errors, ZP_NULL );

	// print any errors
	if( errors )
	{
		const zp_char* errorText = (const zp_char*)errors->GetBufferPointer();
		zpLog::warning() << errorText << zpLog::endl;
	}

	// write the shader data into the out buffer
	if( shaderBlob )
	{
		data.reset();
		data.writeBulk( (const zp_byte*)shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize() );
	}

	// release blobs
	ZP_SAFE_RELEASE( shaderBlob );
	ZP_SAFE_RELEASE( errors );

	// return if compiled successfully
	return SUCCEEDED( hr );
}
