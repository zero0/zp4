package org.zero0.zeropoint.tools.arc.compiler;

import java.io.File;
import java.util.Properties;

import org.zero0.zeropoint.tools.arc.ExecutableMode;
import org.zero0.zeropoint.tools.arc.Platform;
import org.zero0.zeropoint.tools.arc.Rendering;

public abstract class ArcCompiler implements Runnable
{

	public enum ArcCompilerPropertyType
	{
		Boolean, Int, Float, String
	};

	private Properties properties = new Properties();
	private String fileToCompile = "";
	private String inputDir = "";
	private String outputDir = "";
	private String tempDir = "";
	private ArcCompilerListener listener = null;
	private Platform platform = Platform.Win32;
	private Rendering rendering = Rendering.DX11;
	private ExecutableMode executableMode = ExecutableMode.Debug;
	
	protected int compileTime = 0;

	public final void setProperties( Properties properties )
	{
		this.properties.clear();
		this.properties.putAll( properties );
	}
	
	public final Properties getProperties()
	{
		return this.properties;
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
	
	public final void run()
	{
		String fullOutputDir = getFileToCompile();
		fullOutputDir = fullOutputDir.substring( 0, fullOutputDir.lastIndexOf( File.separatorChar ) );
		fullOutputDir = getOutputDirectory() + File.separator + fullOutputDir;
		new File( fullOutputDir ).mkdirs();
		
		long startTime = System.currentTimeMillis();
		runCompiler();
		long endTime = System.currentTimeMillis();
		compileTime = (int) ( endTime - startTime );
	}

	public final int getCompileTime()
	{
		return compileTime;
	}
	
	public final String getFullFilePath()
	{
		return inputDir + File.separator + fileToCompile;
	}

	public abstract void runCompiler();
}
