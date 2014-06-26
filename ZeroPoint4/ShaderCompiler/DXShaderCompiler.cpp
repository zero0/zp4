#include "Main.h"
#include <D3DX11.h>
#include <D3Dcompiler.h>

DXShaderCompiler::DXShaderCompiler( DXShaderCompilerPlatform platform )
	: m_platform( platform )
{}
DXShaderCompiler::~DXShaderCompiler()
{}

zp_bool DXShaderCompiler::compileShaderVSPlatform( zpDataBuffer& data ) const
{
	const zp_char* profile;
	switch( m_platform )
	{
	case DX_SHADER_COMPILER_PLATFORM_DX9:
		profile = "vs_3_0";
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX10:
		profile = "vs_4_0";
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX11:
		profile = "vs_5_0";
		break;
	}

	zpString mainFunc( DEFAULT_VS_MAIN );
	const zpString* f;
	if( m_pragmas.find( zpString( PRAGMA_VERTEX ), &f ) )
	{
		mainFunc = *f;
	}

	return compileShaderPlatform( mainFunc.str(), profile, data );
}
zp_bool DXShaderCompiler::compileShaderPSPlatform( zpDataBuffer& data ) const
{
	const zp_char* profile;
	switch( m_platform )
	{
	case DX_SHADER_COMPILER_PLATFORM_DX9:
		profile = "ps_3_0";
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX10:
		profile = "ps_4_0";
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX11:
		profile = "ps_5_0";
		break;
	}

	zpString mainFunc( DEFAULT_PS_MAIN );
	const zpString* f;
	if( m_pragmas.find( zpString( PRAGMA_FRAG ), &f ) )
	{
		mainFunc = *f;
	}

	return compileShaderPlatform( mainFunc.str(), profile, data );
}

zp_bool DXShaderCompiler::compileShaderGSPlatform( zpDataBuffer& data ) const
{
	return false;
}
zp_bool DXShaderCompiler::compileShaderCSPlatform( zpDataBuffer& data ) const
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

	// compile shader
	HRESULT hr;
	ID3D10Blob* shaderBlob = ZP_NULL;
	ID3D10Blob* errors = ZP_NULL;
	hr = D3DX11CompileFromMemory( shaderText, shaderTextSize, m_inputFile.str(), macros.begin(), ZP_NULL, mainFunc, profile, flags, 0, ZP_NULL, &shaderBlob, &errors, ZP_NULL );

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
