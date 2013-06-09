package org.zero0.zeropoint.tools.arc.zp;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.List;
import java.util.Map.Entry;
import java.util.Properties;

import org.zero0.zeropoint.tools.arc.Arc;
import org.zero0.zeropoint.tools.arc.OutputLevel;
import org.zero0.zeropoint.tools.arc.Platform;
import org.zero0.zeropoint.tools.arc.Rendering;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;

import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonGenerator;

public class ShaderCompiler extends ArcCompiler
{
	enum ShaderType
	{
		VERTEX( "vs" ), PIXEL( "ps" ), GEOMETRY( "gs" ), GEOMETRY_STREAMOUT( "gsso" ), COMPUTE( "cs" );

		private final String type;

		private ShaderType( String type )
		{
			this.type = type;
		}

		public final String getType()
		{
			return type;
		}
	}

	static final String ZPSD_HEADER = "ZPSD";
	static final String DX10_HEADER = "DX10";
	static final String DX11_HEADER = "DX11";
	static final String GL20_HEADER = "GL20";

	static final List<ShaderType> shaderTypes = new ArrayList<ShaderType>();

	static final String DX_X86_EXEC = "\"%DXSDK_DIR%Utilities\\bin\\x86\\fxc.exe\"";
	static final String DX_X64_EXEC = "\"%DXSDK_DIR%Utilities\\bin\\x64\\fxc.exe\"";

	static final String GL_X86_EXEC = "";
	static final String GL_X64_EXEC = "";

	static final String OPTION_OPTIMIZATION = "optimize";
	static final String OPTION_OPTIMIZATION_DEBUG = "debug";
	static final String OPTION_OPTIMIZATION_LOW = "low";
	static final String OPTION_OPTIMIZATION_HIGH = "high";
	static final String OPTION_HEADER = "header";

	static final String DX_OPTION_OPTIMIZATION_DEBUG = " /Od /Zi";
	static final String DX_OPTION_OPTIMIZATION_LOW = " /O1";
	static final String DX_OPTION_OPTIMIZATION_HIGH = " /O3";

	static final String DX10_TYPE = "4_0";
	static final String DX11_TYPE = "5_0";

	static EnumMap<Rendering, String> dxProfiles = new EnumMap<Rendering, String>( Rendering.class );

	static EnumMap<Rendering, HashMap<String, String>> renderingOptions = new EnumMap<Rendering, HashMap<String, String>>( Rendering.class );

	static EnumMap<Platform, EnumMap<Rendering, String>> shaders = new EnumMap<Platform, EnumMap<Rendering, String>>( Platform.class );
	static
	{
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
		renderingOptions.get( Rendering.DX10 ).put( OPTION_HEADER, DX10_HEADER );

		renderingOptions.put( Rendering.DX11, new HashMap<String, String>() );
		renderingOptions.get( Rendering.DX11 ).put( OPTION_OPTIMIZATION_DEBUG, DX_OPTION_OPTIMIZATION_DEBUG );
		renderingOptions.get( Rendering.DX11 ).put( OPTION_OPTIMIZATION_LOW, DX_OPTION_OPTIMIZATION_LOW );
		renderingOptions.get( Rendering.DX11 ).put( OPTION_OPTIMIZATION_HIGH, DX_OPTION_OPTIMIZATION_HIGH );
		renderingOptions.get( Rendering.DX11 ).put( OPTION_HEADER, DX11_HEADER );

		shaderTypes.addAll( Arrays.asList( ShaderType.values() ) );

		dxProfiles.put( Rendering.DX10, DX10_TYPE );
		dxProfiles.put( Rendering.DX11, DX11_TYPE );
	}

	Properties shaderFileDesc = new Properties();
	EnumMap<ShaderType, String> outCompiledFiles = new EnumMap<ShaderType, String>( ShaderType.class );

	String getOptions( ShaderType type )
	{
		if( !shaderFileDesc.containsKey( "shader." + type.getType() ) )
		{
			return null;
		}
		StringBuilder options = new StringBuilder();

		options.append( " /nologo /Op" );

		// profile type
		options.append( " /T " ).append( type.getType() ).append( '_' ).append( dxProfiles.get( getRendering() ) );

		// optimization level (defaults to low)
		String opt = null;
		switch( getExecutableMode() )
		{
		case Debug:
			opt = renderingOptions.get( getRendering() ).get( OPTION_OPTIMIZATION_HIGH );
			break;
		case Release:
			opt = renderingOptions.get( getRendering() ).get( OPTION_OPTIMIZATION_HIGH );
			break;
		}
		options.append( opt );

		// entry point
		options.append( " /E " ).append( shaderFileDesc.getProperty( "shader." + type.getType() ) );

		// macros
		Properties macros = Arc.getSubProperties( shaderFileDesc, "shader.macro" );
		for( Entry<Object, Object> macro : macros.entrySet() )
		{
			options.append( " /D " ).append( macro.getKey().toString() ).append( '=' ).append( macro.getValue().toString() );
		}

		// output file
		String outDir = getTempDirectory() + File.separatorChar + getFileToCompile().substring( 0, getFileToCompile().lastIndexOf( File.separatorChar ) );
		Arc.getInstance().getFullDirectoryPath( outDir );

		String outFile = getTempDirectory() + File.separatorChar + getFileToCompile().replace( ".shader", "." + type.getType() + "_shader" );
		options.append( " /Fo " ).append( '"' ).append( outFile ).append( '"' );
		outCompiledFiles.put( type, outFile );

		String filepath = getFullFilePath();
		filepath = filepath.substring( 0, filepath.lastIndexOf( File.separatorChar ) + 1 );

		// file input
		options.append( ' ' ).append( '"' ).append( filepath ).append( shaderFileDesc.getProperty( "shader.file" ) ).append( '"' );

		return options.toString();
	}

	String getCompilerExecString( ShaderType type )
	{
		String options = getOptions( type );
		if( options == null )
		{
			return null;
		}

		StringBuilder exec = new StringBuilder();
		exec.append( '"' ).append( shaders.get( getPlatform() ).get( getRendering() ) ).append( options ).append( '"' );
		return exec.toString();
	}

	void compileDX()
	{
		// compile each file
		Process p = null;
		for( ShaderType type : shaderTypes )
		{
			try
			{
				String exec = getCompilerExecString( type );
				if( exec == null ) continue;
				Arc.getInstance().out( OutputLevel.Verbos, "ShaderCompiler Executing: " + exec );

				ProcessBuilder pb = new ProcessBuilder();
				pb.redirectErrorStream( true );
				// pb.redirectInput( Redirect.PIPE );
				// pb.redirectError( Redirect.PIPE );
				// pb.redirectOutput( Redirect.PIPE );
				pb.command( "cmd.exe", "/C", exec );

				p = pb.start();

				BufferedReader br = new BufferedReader( new InputStreamReader( p.getInputStream() ) );
				// BufferedWriter bw = new BufferedWriter( new
				// OutputStreamWriter( p.getOutputStream() ) );
				BufferedReader be = new BufferedReader( new InputStreamReader( p.getErrorStream() ) );

				p.waitFor();

				StringBuilder sb = new StringBuilder();
				char[] buff = new char[ 128 ];
				int numRead = -1;
				while( ( numRead = br.read( buff ) ) != -1 )
				{
					sb.append( buff, 0, numRead );
				}
				Arc.getInstance().out( OutputLevel.Normal, sb.toString() );

				sb.delete( 0, sb.length() );
				while( ( numRead = be.read( buff ) ) != -1 )
				{
					sb.append( buff, 0, numRead );
				}
				if( sb.length() > 0 )
				{
					Arc.getInstance().err( sb.toString() );
				}
			}
			catch( Exception e )
			{
				Arc.getInstance().err( e.getMessage() );
			}
			finally
			{
				if( p != null )
				{
					p.destroy();
				}
			}
		}

		// compile to json
		try
		{
			String shaderFile = getOutputDirectory() + File.separatorChar + getFileToCompile().replace( ".shader", ".json" );

			JsonFactory j = new JsonFactory();
			JsonGenerator g = j.createGenerator( new FileOutputStream( shaderFile ) );

			g.writeStartObject();
			{
				// write each shader type's compiled content
				byte[] buff = new byte[ 128 ];
				int numRead = -1;
				for( ShaderType shaderType : outCompiledFiles.keySet() )
				{
					g.writeObjectFieldStart( shaderType.getType().toUpperCase() );

					File inFile = new File( outCompiledFiles.get( shaderType ) );

					ByteArrayOutputStream data = new ByteArrayOutputStream( 4096 );
					DataOutputStream dos = new DataOutputStream( data );
					BufferedInputStream bis = new BufferedInputStream( new FileInputStream( inFile ) );
					while( ( numRead = bis.read( buff ) ) != -1 )
					{
						dos.write( buff, 0, numRead );
					}
					bis.close();

					inFile.delete();

					g.writeBinaryField( "Data", data.toByteArray() );

					switch( shaderType )
					{
					case VERTEX:
						g.writeStringField( "Format", shaderFileDesc.getProperty( "shader.vs.layout" ) );
						break;
					case GEOMETRY_STREAMOUT:
						{							
							String stridesProp = shaderFileDesc.getProperty( "shader.gsso.strides" );
							String[] strides = stridesProp.split( "," );
							g.writeArrayFieldStart( "Strides" );
							{
								for( String s : strides )
								{
									g.writeNumber( Integer.parseInt( s.trim() ) );
								}
							}
							g.writeEndArray();
	
							Properties decl = Arc.getSubProperties( shaderFileDesc, "shader.gsso.decl" );
							g.writeArrayFieldStart( "Entries" );
							{
								for( Object k : decl.keySet() )
								{
									String entity = decl.get( k ).toString();
									String[] entityParts = entity.split( "," );
									
									g.writeStartObject();
									{
										g.writeObjectField( "Stream", 			Integer.parseInt( entityParts[ 0 ].trim() ) );
										g.writeObjectField( "SemanticIndex", 	Integer.parseInt( entityParts[ 1 ].trim() ) );
										g.writeObjectField( "StartComponent", 	Integer.parseInt( entityParts[ 2 ].trim() ) );
										g.writeObjectField( "ComponentCount", 	Integer.parseInt( entityParts[ 3 ].trim() ) );
										g.writeObjectField( "OutputSlot", 		Integer.parseInt( entityParts[ 4 ].trim() ) );
										g.writeObjectField( "SemanticName", 	k.toString() );
									}
									g.writeEndObject();
								}
							}
							g.writeEndArray();
						}
						break;
					}

					g.writeEndObject();
				}
			}
			g.writeEndObject();

			g.close();
		}
		catch( IOException e )
		{
			e.printStackTrace();
		}

	}

	void compileGL()
	{

	}

	@Override
	public void runCompiler()
	{
		try
		{
			shaderFileDesc.load( new FileInputStream( getFullFilePath() ) );
		}
		catch( Exception e )
		{
			Arc.getInstance().err( e.getMessage() );
		}

		shaderFileDesc.putAll( getProperties() );

		switch( getRendering() )
		{
		case DX10:
		case DX11:
			compileDX();
			break;

		case GL:
			compileGL();
			break;
		}
	}
}
