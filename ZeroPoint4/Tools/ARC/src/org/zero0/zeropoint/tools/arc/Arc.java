package org.zero0.zeropoint.tools.arc;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.apache.tools.ant.Project;
import org.apache.tools.ant.ProjectHelper;
import org.apache.tools.ant.helper.ProjectHelper2;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompilerListener;
import org.zero0.zeropoint.tools.arc.util.FileWatcher;
import org.zero0.zeropoint.tools.arc.util.FileWatcherListener;
import org.zero0.zeropoint.tools.arc.util.OutputAppender;

public final class Arc implements FileWatcherListener, ArcCompilerListener
{
	private static String arcPropertiesFile = "arc.properties";

	private static String arcThreads = "arc.threads";
	private static String arcCompiler = "arc.compiler";
	private static String arcInRoot = "arc.in-root";
	private static String arcOutRoot = "arc.out-root";
	private static String arcPlatform = "arc.platform";
	private static String arcRendering = "arc.rendering";
	private static String arcOutputLevel = "arc.output-level";
	private static String arcAutoCompile = "arc.auto-compile";
	private static String arcExeLevel = "arc.exe-mode";
	private static String arcTempRoot = "arc.temp-root";

	private final Map<Class<? extends ArcCompiler>, Properties> compilerProperties = new HashMap<Class<? extends ArcCompiler>, Properties>();

	private static Arc instance = null;

	public static Arc getInstance()
	{
		if( instance == null )
		{
			instance = new Arc();
		}
		return instance;
	}

	FileWatcher fileWatcher;
	ExecutorService compilerExecutor;
	Map<String, Class<? extends ArcCompiler>> compilers;
	Map<String, String> extensionToCompiler;
	Properties properties;
	boolean autoCompile;

	String inRootDir;
	String outRootDir;
	String tmpRootDir;
	
	Platform platform;
	Rendering rendering;
	OutputLevel outputLevel;
	ExecutableMode executableMode;

	OutputAppender outputAppender;

	volatile int numTasks;
	volatile int numSuccess;
	volatile int numErrored;

	ArcListener listener;
	ArcDatabase database;

	private Arc()
	{
		properties = new Properties();
		compilers = new HashMap<String, Class<? extends ArcCompiler>>();
		extensionToCompiler = new HashMap<String, String>();
	}

	public final void initialize()
	{
		loadProperties();

		database = new ArcDatabase( properties );
		database.initialize();

		fileWatcher.addListener( this );
		fileWatcher.start();
	}

	public final void loadProperties()
	{
		try
		{
			properties.load( new FileInputStream( new File( arcPropertiesFile ) ) );
		}
		catch( Exception e )
		{
			err( e.getMessage() );
		}

		rendering = Rendering.valueOf( properties.getProperty( arcRendering, Rendering.DX11.name() ) );
		platform = Platform.valueOf( properties.getProperty( arcPlatform, Platform.Win32.name() ) );
		outputLevel = OutputLevel.valueOf( properties.getProperty( arcOutputLevel, OutputLevel.Normal.name() ) );
		executableMode = ExecutableMode.valueOf( properties.getProperty( arcExeLevel, ExecutableMode.Debug.name() ) );
		
		out( OutputLevel.Verbos, "Loading properties..." );

		inRootDir = properties.getProperty( arcInRoot );
		inRootDir = getFullDirectoryPath( inRootDir );
		
		outRootDir = properties.getProperty( arcOutRoot );		
		outRootDir = getFullDirectoryPath( outRootDir );
		
		tmpRootDir = properties.getProperty( arcTempRoot );
		tmpRootDir = getFullDirectoryPath( tmpRootDir );
		
		autoCompile = Boolean.parseBoolean( properties.getProperty( arcAutoCompile, "false" ) );
		
		fileWatcher = new FileWatcher( inRootDir );
		fileWatcher.setIsEnabled( autoCompile );

		int numThreads = Integer.parseInt( properties.getProperty( arcThreads, "10" ) );
		compilerExecutor = Executors.newFixedThreadPool( numThreads );

		String compilerName = "";
		Properties compiler = getSubProperties( properties, arcCompiler );
		for( Entry<Object, Object> e : compiler.entrySet() )
		{
			String key = e.getKey().toString();
			String value = e.getValue().toString();

			// if there is no ".", it's the name and class of the compiler
			if( key.indexOf( '.' ) < 0 )
			{
				try
				{
					Class<?> clazz = ClassLoader.getSystemClassLoader().loadClass( value );
					if( ArcCompiler.class.isAssignableFrom( clazz ) )
					{
						@SuppressWarnings( "unchecked" )
						Class<? extends ArcCompiler> compilerClass = (Class<? extends ArcCompiler>) clazz;

						compilerProperties.put( compilerClass, new Properties() );
						compilers.put( key, compilerClass );
						compilerName = key;

						// get the sub properties for this compiler
						String exts = compiler.getProperty( compilerName + ".ext", "" ).toLowerCase();
						for( String ext : exts.split( "," ) )
						{
							String extension = ext.trim();
							extensionToCompiler.put( extension, compilerName );
							fileWatcher.addAcceptedFileExtension( extension );
						}
					}
					else
					{
						throw new ClassNotFoundException( "Failed to load compiler " + value );
					}
				}
				catch( ClassNotFoundException e1 )
				{
					err( e1.getMessage() );
				}
			}
		}
	}

	public final String getFullDirectoryPath( String path )
	{
		String fullPath = null;
		try
		{
			File outDir = new File( path );
			outDir.mkdirs();
			
			fullPath = outDir.getCanonicalPath()
			.replace( "{Platform}", platform.name() )      
			.replace( "{ExeMode}", executableMode.name() )
			.replace( "{Rendering}", rendering.name() )
			;		
		}
		catch( IOException e )
		{
			err( "Unable to create directory structure: " + path );
		}
		
		return fullPath;
	}
	
	private final void saveProperties()
	{
		OutputStream out = null;
		try
		{
			out = new BufferedOutputStream( new FileOutputStream( new File( arcPropertiesFile ) ) );
			properties.store( out, "" );
		}
		catch( Exception e )
		{
			err( e.getMessage() );
		}
		finally
		{
			try
			{
				if( out != null )
				{
					out.close();
				}
			}
			catch( Exception e )
			{
				err( e.getMessage() );
			}
		}
	}
	
	public void clean( boolean full )
	{
		database.clean( full );
		fileWatcher.clean( full );
	}

	public final Collection<Class<? extends ArcCompiler>> getCompilers()
	{
		return Collections.unmodifiableCollection( compilers.values() );
	}

	public final Properties getCompilerProperties( Class<? extends ArcCompiler> clazz )
	{
		return compilerProperties.get( clazz );
	}

	public final String getRootDirectory()
	{
		return inRootDir;
	}
	
	public final String getOutputRootDirectory()
	{
		return outRootDir;
	}

	public final String getTempDirectory()
	{
		return tmpRootDir;
	}
	
	public final ArcDatabase getArcDatabase()
	{
		return database;
	}

	public static Properties getSubProperties( Properties props, String subProperty )
	{
		Properties sub = new Properties();

		for( Entry<Object, Object> e : props.entrySet() )
		{
			String key = e.getKey().toString();
			int index = key.indexOf( subProperty );
			if( index != -1 )
			{
				String subKey = key.substring( index + subProperty.length() + 1 );
				if( !subKey.isEmpty() )
				{
					sub.put( subKey, e.getValue() );
				}
			}
		}

		return sub;
	}

	public final void reloadProperties()
	{
		properties.clear();
		loadProperties();
	}

	public final void addCompilerTask( File file )
	{
		addCompilerTask( fileWatcher.getStandardPathForFile( file ) );
	}
	
	public final void addCompilerTask( String filePath )
	{
		addCompilerTasks( filePath );
	}

	public final void addCompilerTasks( String... filePaths )
	{
		numTasks = filePaths.length;
		numErrored = 0;
		numSuccess = 0;

		fireArcProcessStart();
		for( String filePath : filePaths )
		{
			String extension = filePath.substring( filePath.lastIndexOf( '.' ) + 1 ).toLowerCase();

			try
			{
				String compiler = extensionToCompiler.get( extension );
				Class<? extends ArcCompiler> clazz = compilers.get( compiler );
				if( clazz != null )
				{
					ArcCompiler arcCompiler = clazz.newInstance();
					arcCompiler.setFileToCompile( filePath );
					arcCompiler.setPlatform( getPlatform() );
					arcCompiler.setRendering( getRendering() );
					arcCompiler.setExectuableMode( getExecutableMode() );
					arcCompiler.setInputDirectory( getRootDirectory() );
					arcCompiler.setOutputDirectory( getOutputRootDirectory() );
					arcCompiler.setTempDirectory( getTempDirectory() );
					arcCompiler.setListener( this );

					// arcCompiler.run();
					compilerExecutor.execute( arcCompiler );
				}
			}
			catch( Exception e )
			{
				err( e.getMessage() );
			}
		}
	}

	@Override
	public final void fileChanged( String filePath )
	{
		database.filesChanged( filePath );

		List<String> files = database.getFilesReferencedFrom( filePath, true );
		addCompilerTasks( files.toArray( new String[ files.size() ] ) );
	}

	@Override
	public final void fileAdded( String filePath )
	{
		database.filesAdded( filePath );
		addCompilerTasks( filePath );
	}

	public final Platform getPlatform()
	{
		return platform;
	}

	public final void setPlatform( Platform platform )
	{
		this.platform = platform;
		saveProperties();
	}

	public final OutputLevel getOutputLevel()
	{
		return outputLevel;
	}

	public final void setOutputLevel( OutputLevel outputLevel )
	{
		this.outputLevel = outputLevel;
		saveProperties();
	}

	public final Rendering getRendering()
	{
		return rendering;
	}

	public final void setRendering( Rendering rendering )
	{
		this.rendering = rendering;
		saveProperties();
	}

	public final void setExectuableMode( ExecutableMode executableMode )
	{
		this.executableMode = executableMode;
	}
	
	public final ExecutableMode getExecutableMode()
	{
		return this.executableMode;
	}
	
	public final boolean getAutoCompile()
	{
		return autoCompile;
	}

	public final void setAutoCompile( boolean autoCompile )
	{
		this.autoCompile = autoCompile;
		fileWatcher.setIsEnabled( autoCompile );
		saveProperties();
	}

	public final OutputAppender getOutputAppender()
	{
		return outputAppender;
	}

	public final void setOutputAppender( OutputAppender outputAppender )
	{
		this.outputAppender = outputAppender;
	}

	public final void shutdown()
	{
		fileWatcher.removeAllListeners();
		fileWatcher.shutdown();

		database.shutdown();

		compilerExecutor.shutdown();
	}

	public final void out( OutputLevel level, String text )
	{
		if( outputLevel.level >= level.level )
		{
			if( outputAppender != null )
			{
				outputAppender.printOut( text );
			}
			else
			{
				System.out.println( text );
			}
		}
	}

	public final void err( String text )
	{
		if( outputAppender != null )
		{
			outputAppender.printErr( text );
		}
		else
		{
			System.err.println( text );
		}
	}

	public final void setListener( ArcListener listener )
	{
		this.listener = listener;
	}

	public final void onCompileMessage( String filePath, String msg, ArcCompiler compiler )
	{
		out( OutputLevel.Normal, "Compile Message " + filePath );
	}

	public final void onCompileSuccess( String filePath, String msg, ArcCompiler compiler )
	{
		numTasks--;
		numSuccess++;
		out( OutputLevel.Compact, "Compile Success " + filePath );
		fireArcProcessUpdate( true, compiler.getCompileTime() );
	}

	public final void onCompileFailed( String filePath, String msg, ArcCompiler compiler )
	{
		numTasks--;
		numErrored++;
		err( "Compile Failed " + filePath );
		fireArcProcessUpdate( false, compiler.getCompileTime() );
	}

	void fireArcProcessStart()
	{
		if( listener != null ) listener.onArcProcessStart( numTasks );
	}

	void fireArcProcessUpdate( boolean success, int completeTime )
	{
		if( listener != null ) listener.onArcProcessUpdate( success, completeTime, numTasks, numSuccess, numErrored );
	}

	void fireArcProcessEnd()
	{
		if( listener != null ) listener.onArcProcessEnd();
	}
}
