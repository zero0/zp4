package org.zero0.zeropoint.tools.arc.compiler;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.List;
import java.util.Map.Entry;
import java.util.Properties;

import org.zero0.zeropoint.tools.arc.Arc;
import org.zero0.zeropoint.tools.arc.workspace.OutputLevel;
import org.zero0.zeropoint.tools.arc.workspace.Platform;
import org.zero0.zeropoint.tools.arc.workspace.Rendering;

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
		shaders.put( Platform.Win32, new EnumMap<Rendering, String>( Rendering.class ) );
		shaders.put( Platform.Win64, new EnumMap<Rendering, String>( Rendering.class ) );

		shaders.get( Platform.Win32 ).put( Rendering.DX10, DX_X86_EXEC );
		shaders.get( Platform.Win32 ).put( Rendering.DX11, DX_X86_EXEC );
		shaders.get( Platform.Win32 ).put( Rendering.GL, GL_X86_EXEC );

		shaders.get( Platform.Win64 ).put( Rendering.DX10, DX_X64_EXEC );
		shaders.get( Platform.Win64 ).put( Rendering.DX11, DX_X64_EXEC );
		shaders.get( Platform.Win64 ).put( Rendering.GL, GL_X64_EXEC );

		renderingOptions.put( Rendering.DX10, new HashMap<String, String>() );
		renderingOptions.get( Rendering.DX10 ).put( OPTION_OPTIMIZATION_DEBUG, DX_OPTION_OPTIMIZATION_DEBUG );
		renderingOptions.get( Rendering.DX10 ).put( OPTION_OPTIMIZATION_LOW, DX_OPTION_OPTIMIZATION_LOW );
		renderingOptions.get( Rendering.DX10 ).put( OPTION_OPTIMIZATION_HIGH, DX_OPTION_OPTIMIZATION_HIGH );

		renderingOptions.put( Rendering.DX11, new HashMap<String, String>() );
		renderingOptions.get( Rendering.DX11 ).put( OPTION_OPTIMIZATION_DEBUG, DX_OPTION_OPTIMIZATION_DEBUG );
		renderingOptions.get( Rendering.DX11 ).put( OPTION_OPTIMIZATION_LOW, DX_OPTION_OPTIMIZATION_LOW );
		renderingOptions.get( Rendering.DX11 ).put( OPTION_OPTIMIZATION_HIGH, DX_OPTION_OPTIMIZATION_HIGH );

		types.add( ShaderType.VERTEX );
		types.add( ShaderType.GEOMETRY );
		types.add( ShaderType.PIXEL );

		dxProfiles.put( Rendering.DX10, DX10_TYPE );
		dxProfiles.put( Rendering.DX11, DX11_TYPE );
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
		String options = getOptions( type );
		if( options == null ) {
			return null;
		}

		return "\"" + shaders.get( getPlatform() ).get( getRendering() ) + options + "\"";
	}

	void compileDX() {
		Process p = null;
		for( ShaderType type : types ) {
			try {
				String exec = getCompilerExecString( type );
				if( exec == null )
					continue;

				ProcessBuilder pb = new ProcessBuilder();
				// pb.redirectInput( Redirect.PIPE );
				// pb.redirectError( Redirect.PIPE );
				// pb.redirectOutput( Redirect.PIPE );
				pb.command( "cmd.exe", "/C", exec );

				Arc.getInstance().out( OutputLevel.Verbos, "Executing: " + exec );
				p = pb.start();

				BufferedReader br = new BufferedReader( new InputStreamReader( p.getInputStream() ) );
				// BufferedWriter bw = new BufferedWriter( new
				// OutputStreamWriter( p.getOutputStream() ) );
				BufferedReader be = new BufferedReader( new InputStreamReader( p.getErrorStream() ) );

				p.waitFor();

				StringBuilder sb = new StringBuilder();
				String s;
				while( ( s = br.readLine() ) != null ) {
					sb.append( s );
				}
				Arc.getInstance().out( OutputLevel.Normal, sb.toString() );

				sb.delete( 0, sb.length() );
				while( ( s = be.readLine() ) != null ) {
					sb.append( s );
				}
				if( sb.length() > 0 ) {
					Arc.getInstance().err( sb.toString() );
				}

			} catch( Exception e ) {
				Arc.getInstance().err( e.getMessage() );
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
		case DX10:
		case DX11:
			compileDX();
			break;

		case GL:
			compileGL();
			break;
		}
	}

	@Override
	public void run() {
		try {
			shaderFileDesc.load( new FileInputStream( new File( fileToCompile ) ) );
		} catch( Exception e ) {
			Arc.getInstance().err( e.getMessage() );
		}

		shaderFileDesc.putAll( properties );

		compile();
	}
}
