package org.zero0.zeropoint.tools.arc.compiler;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.lang.ProcessBuilder.Redirect;
import java.util.ArrayList;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.List;
import java.util.Map.Entry;
import java.util.Properties;

import org.zero0.zeropoint.tools.arc.Arc;
import org.zero0.zeropoint.tools.arc.Platform;
import org.zero0.zeropoint.tools.arc.Rendering;

public class ShaderCompiler extends ArcCompiler {

    enum ShaderType {
	VERTEX( "vs" ), PIXEL( "ps" ), GEOMETRY( "gs" );

	private String type;

	private ShaderType( String type ) {
	    this.type = type;
	}

	final public String getType() {
	    return type;
	}
    }

    static final List<ShaderType> types = new ArrayList<ShaderType>();

    static final String DX_X86_EXEC = "\"%DXSDK_DIR%Utilities\\bin\\x86\\fxc.exe\"";
    static final String DX_X64_EXEC = "\"%DXSDK_DIR%Utilities\\bin\\x64\\fxc.exe\"";

    static final String GL_X86_EXEC = "";
    static final String GL_X64_EXEC = "";

    static final String OPTION_OPTIMIZATION = "optimize";
    static final String OPTION_OPTIMIZATION_DEBUG = "debug";
    static final String OPTION_OPTIMIZATION_LOW = "low";
    static final String OPTION_OPTIMIZATION_HIGH = "high";

    static final String DX_OPTION_OPTIMIZATION_DEBUG = " /Od /Zi";
    static final String DX_OPTION_OPTIMIZATION_LOW = " /O1";
    static final String DX_OPTION_OPTIMIZATION_HIGH = " /O3";

    static final String DX10_TYPE = "4_0";
    static final String DX11_TYPE = "5_0";

    static EnumMap<Rendering, String> dxProfiles = new EnumMap<Rendering, String>( Rendering.class );

    static EnumMap<Rendering, HashMap<String, String>> renderingOptions = new EnumMap<Rendering, HashMap<String, String>>( Rendering.class );

    static EnumMap<Platform, EnumMap<Rendering, String>> shaders = new EnumMap<Platform, EnumMap<Rendering, String>>( Platform.class );
    static {
	shaders.put( Platform.WIN32, new EnumMap<Rendering, String>( Rendering.class ) );
	shaders.put( Platform.WIN64, new EnumMap<Rendering, String>( Rendering.class ) );

	shaders.get( Platform.WIN32 ).put( Rendering.DIRECTX10, DX_X86_EXEC );
	shaders.get( Platform.WIN32 ).put( Rendering.DIRECTX11, DX_X86_EXEC );
	shaders.get( Platform.WIN32 ).put( Rendering.OPENGL, GL_X86_EXEC );

	shaders.get( Platform.WIN64 ).put( Rendering.DIRECTX10, DX_X64_EXEC );
	shaders.get( Platform.WIN64 ).put( Rendering.DIRECTX11, DX_X64_EXEC );
	shaders.get( Platform.WIN64 ).put( Rendering.OPENGL, GL_X64_EXEC );

	renderingOptions.put( Rendering.DIRECTX10, new HashMap<String, String>() );
	renderingOptions.get( Rendering.DIRECTX10 ).put( OPTION_OPTIMIZATION_DEBUG, DX_OPTION_OPTIMIZATION_DEBUG );
	renderingOptions.get( Rendering.DIRECTX10 ).put( OPTION_OPTIMIZATION_LOW, DX_OPTION_OPTIMIZATION_LOW );
	renderingOptions.get( Rendering.DIRECTX10 ).put( OPTION_OPTIMIZATION_HIGH, DX_OPTION_OPTIMIZATION_HIGH );

	renderingOptions.put( Rendering.DIRECTX11, new HashMap<String, String>() );
	renderingOptions.get( Rendering.DIRECTX11 ).put( OPTION_OPTIMIZATION_DEBUG, DX_OPTION_OPTIMIZATION_DEBUG );
	renderingOptions.get( Rendering.DIRECTX11 ).put( OPTION_OPTIMIZATION_LOW, DX_OPTION_OPTIMIZATION_LOW );
	renderingOptions.get( Rendering.DIRECTX11 ).put( OPTION_OPTIMIZATION_HIGH, DX_OPTION_OPTIMIZATION_HIGH );

	types.add( ShaderType.VERTEX );
	types.add( ShaderType.GEOMETRY );
	types.add( ShaderType.PIXEL );

	dxProfiles.put( Rendering.DIRECTX10, DX10_TYPE );
	dxProfiles.put( Rendering.DIRECTX11, DX11_TYPE );
    }

    Properties shaderFileDesc = new Properties();

    String getOptions( ShaderType type ) {
	if( !shaderFileDesc.containsKey( "shader." + type.getType() ) ) {
	    return null;
	}
	String options = "";

	// profile type
	options += " /T " + type.getType() + "_" + dxProfiles.get( getRendering() );

	// optimization level (defaults to low)
	options += shaderFileDesc.containsKey( OPTION_OPTIMIZATION ) ? renderingOptions.get( getRendering() ).get( shaderFileDesc.get( OPTION_OPTIMIZATION ) ) : renderingOptions.get( getRendering() ).get( OPTION_OPTIMIZATION_LOW );

	// entry point
	options += " /E " + shaderFileDesc.getProperty( "shader." + type.getType() );

	// macros
	Properties macros = Arc.getSubProperties( shaderFileDesc, "shader.macro" );
	for( Entry<Object, Object> macro : macros.entrySet() ) {
	    options += " /D " + macro.getKey().toString() + "=" + macro.getValue().toString();
	}

	// file input
	options += " \"" + fileToCompile.substring( 0, fileToCompile.lastIndexOf( File.separatorChar ) ) + File.separatorChar + shaderFileDesc.getProperty( "shader.file" ) + "\"";
	
	return options;
    }

    String getCompilerExecString( ShaderType type ) {
	String compilerExec = "\"" + shaders.get( getPlatform() ).get( getRendering() );
	
	String options = getOptions( type );
	if( options == null ) {
	    return null;
	}
	
	compilerExec += options + "\"";

	return compilerExec;
    }

    void compileDX() {
	Runtime r = Runtime.getRuntime();
	Process p = null;
	for( ShaderType type : types ) {
	    try {
		String exec = getCompilerExecString( type );
		if( exec == null ) continue;
		
		ProcessBuilder pb = new ProcessBuilder();
		pb.redirectInput( Redirect.INHERIT );
		pb.redirectError( Redirect.INHERIT );
		pb.redirectOutput( Redirect.INHERIT );
		pb.command( "cmd.exe", "/C", exec );
		
		System.out.println( "Executing: " + exec );
		p = pb.start();
		
		BufferedReader br = new BufferedReader( new InputStreamReader( p.getInputStream() ) );
		BufferedWriter bw = new BufferedWriter( new OutputStreamWriter( p.getOutputStream() ) );
		BufferedReader be = new BufferedReader( new InputStreamReader( p.getErrorStream() ) );
		
		p.waitFor();
		
		System.out.println( br.readLine() );
		System.err.println( br.readLine() );
	    } catch( InterruptedException e ) {
		System.err.println();
	    } catch( IOException e ) {
		e.printStackTrace();
	    } finally {
		if( p != null ) {
		    p.destroy();
		}
	    }
	}
    }

    void compileGL() {

    }

    void compile() {
	switch( getRendering() ) {
	case DIRECTX10:
	case DIRECTX11:
	    compileDX();
	    break;

	case OPENGL:
	    compileGL();
	    break;
	}
    }

    @Override
    public void run() {
	try {
	    shaderFileDesc.load( new FileInputStream( new File( fileToCompile ) ) );
	} catch( FileNotFoundException e ) {
	    e.printStackTrace();
	} catch( IOException e ) {
	    e.printStackTrace();
	}

	shaderFileDesc.putAll( properties );

	compile();
    }
}
