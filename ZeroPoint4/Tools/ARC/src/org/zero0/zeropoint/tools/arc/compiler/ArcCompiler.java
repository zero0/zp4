package org.zero0.zeropoint.tools.arc.compiler;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.IOException;
import java.lang.ProcessBuilder.Redirect;
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

	public final void setProperties( Properties properties )
	{
		this.properties.clear();
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
		
		ProcessBuilder pb = new ProcessBuilder();
		pb.command( params );
		pb.directory( new File( getWorkingDirectory() ) );
		pb.redirectOutput( Redirect.PIPE );
		pb.redirectError( Redirect.PIPE );
		
		Process p = null;
		try
		{
			p = pb.start();
			
			p.waitFor();
			
			BufferedInputStream bis = new BufferedInputStream( p.getInputStream() );
			BufferedInputStream bes = new BufferedInputStream( p.getErrorStream() );

			byte[] buff = new byte[64];
			StringBuilder sb = new StringBuilder();
			int len = -1;
			while( ( len = bis.read( buff ) ) != -1 )
			{
				sb.append( new String( buff, 0, len ) );
			}
			
			if( sb.length() > 0 ) Arc.getInstance().out( OutputLevel.Normal, sb.toString() );
			
			sb = new StringBuilder();
			len = -1;
			while( ( len = bes.read( buff ) ) != -1 )
			{
				sb.append( new String( buff, 0, len ) );
			}
			
			if( sb.length() > 0 ) Arc.getInstance().err( sb.toString() );
		}
		catch( IOException e )
		{
			e.printStackTrace();
		}
		catch( InterruptedException e )
		{
			e.printStackTrace();
		}
	}
	
	protected abstract List<String> getCompilerParams();
}
