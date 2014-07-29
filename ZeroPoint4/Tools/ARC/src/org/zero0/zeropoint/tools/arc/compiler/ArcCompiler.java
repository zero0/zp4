package org.zero0.zeropoint.tools.arc.compiler;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import org.zero0.zeropoint.tools.arc.Arc;
import org.zero0.zeropoint.tools.arc.ExecutableMode;
import org.zero0.zeropoint.tools.arc.OutputLevel;
import org.zero0.zeropoint.tools.arc.Platform;
import org.zero0.zeropoint.tools.arc.Rendering;

public abstract class ArcCompiler implements Runnable
{
	private Properties properties = new Properties();
	private String project = "";
	private String fileToCompile = "";
	private String inputDir = "";
	private String outputDir = "";
	private String tempDir = "";
	private String outputFile = "";
	private String outputExtension = null;
	private int compileTime = 0;
	private ArcCompilerListener listener = null;
	private Platform platform = Platform.Win32;
	private Rendering rendering = Rendering.DX11;
	private ExecutableMode executableMode = ExecutableMode.Debug;
	
	public final void setProject( String project )
	{
		this.project = project;
	}
	
	public final String getProject()
	{
		return project;
	}

	public final void addProperties( Properties properties )
	{
		this.properties.putAll( properties );
	}
	
	public final Properties getProperties()
	{
		return this.properties;
	}
	
	public final void setOutputExtension( String extension )
	{
		outputExtension = extension;
	}
	
	public final void setFileToCompile( String filePath )
	{
		fileToCompile = filePath;
	}
	
	public final String getFileToCompile()
	{
		return this.fileToCompile;
	}
	
	public final void setOutputDirectory( String outputDir )
	{
		this.outputDir = outputDir;
	}
	
	public final String getOutputDirectory()
	{
		return this.outputDir;
	}

	public final void setInputDirectory( String inputDir )
	{
		this.inputDir = inputDir;
	}
	
	public final String getInputDirectory()
	{
		return this.inputDir;
	}
	
	public final void setTempDirectory( String tempDir )
	{
		this.tempDir = tempDir;
	}
	
	public final String getTempDirectory()
	{
		return this.tempDir;
	}

	public final void setListener( ArcCompilerListener listener )
	{
		this.listener = listener;
	}

	public final ArcCompilerListener getListener()
	{
		return this.listener;	
	}
	
	public final Platform getPlatform()
	{
		return platform;
	}

	public final void setPlatform( Platform platform )
	{
		this.platform = platform;
	}

	public final Rendering getRendering()
	{
		return rendering;
	}

	public final void setRendering( Rendering rendering )
	{
		this.rendering = rendering;
	}

	public final void setExectuableMode( ExecutableMode executableMode )
	{
		this.executableMode = executableMode;
	}
	
	public final ExecutableMode getExecutableMode()
	{
		return this.executableMode;
	}
	
	public final String getOutputFile()
	{
		return outputFile;
	}
	
	public final void run()
	{
		String directory = getFileToCompile();
		if( !directory.isEmpty() )
		{
			directory = directory.substring( getInputDirectory().length() );
			outputFile = getOutputDirectory() + directory + "b";
			if( outputExtension != null )
			{
				outputFile = outputFile.substring( 0, outputFile.lastIndexOf( '.' ) + 1 ) + outputExtension;
			}
			directory = outputFile.substring( 0, outputFile.lastIndexOf( File.separatorChar ) + 1 );
			
			Arc.getInstance().out( OutputLevel.Verbos, "Creating Directory: " + directory );
			new File( directory ).mkdirs();
		}
		
		long startTime = System.currentTimeMillis();
		internalRunCompiler();
		long endTime = System.currentTimeMillis();
		compileTime = (int) ( endTime - startTime );
	}

	public final int getCompileTime()
	{
		return compileTime;
	}

	public String getWorkingDirectory()
	{
		return inputDir;
	}
	
	private void internalRunCompiler()
	{
		String className = getClass().getSimpleName();
		String compilerName;
		
		if( getClass().isAnnotationPresent( ArcCompilerName.class ) )
		{
			compilerName = getClass().getAnnotation( ArcCompilerName.class ).value();
		}
		else
		{
			compilerName = className;
		}
		
		Arc.getInstance().out( OutputLevel.Verbos, className + ": " + getFileToCompile() );
		
		String exe = "";
		if( !compilerName.isEmpty() )
		{
			exe = Arc.getInstance().getToolsExeDirectory() + File.separator + compilerName + ".exe";
		}
		
		List<String> params = new ArrayList<String>();
		if( !exe.isEmpty() ) params.add( exe );
		params.addAll( getCompilerParams() );
		
		StringBuilder sb = new StringBuilder( "Executing: " );
		for( String p : params )
		{
			sb.append( p ).append( ' ' );
		}
		Arc.getInstance().out( OutputLevel.Verbos, sb.toString() );
		
		ProcessBuilder pb = new ProcessBuilder();
		pb.command( params );
		pb.directory( new File( getWorkingDirectory() ) );

		Process p = null;
		try
		{
			p = pb.start();

			class StreamGobbler extends Thread
			{
				private InputStream is;
				
				public StreamGobbler( InputStream in )
				{
					is = in;
				}

				public void run()
				{
					try
					{
						BufferedReader br = new BufferedReader( new InputStreamReader( is ) );
						String line = null;
						while( ( line = br.readLine() ) != null )
						{
							Arc.getInstance().out( OutputLevel.Normal, line );
						}
					}
					catch( IOException ioe )
					{
						ioe.printStackTrace();
					}
				}
			}
			
			StreamGobbler bis = new StreamGobbler( p.getInputStream() );
			StreamGobbler bes = new StreamGobbler( p.getErrorStream() );

			bis.start();
			bes.start();
			
			p.waitFor();
						
			if( listener != null ) listener.onCompileSuccess( this.getFileToCompile(), "Success", this );
		}
		catch( Exception e )
		{
			e.printStackTrace();
			
			if( listener != null ) listener.onCompileFailed( this.getFileToCompile(), "Failed", this );
		}
	}
	
	protected abstract List<String> getCompilerParams();
}
