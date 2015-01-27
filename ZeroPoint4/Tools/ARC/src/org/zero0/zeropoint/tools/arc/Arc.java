package org.zero0.zeropoint.tools.arc;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompilerListener;
import org.zero0.zeropoint.tools.arc.util.FileWatcher;
import org.zero0.zeropoint.tools.arc.util.FileWatcherListener;
import org.zero0.zeropoint.tools.arc.util.OutputAppender;

public final class Arc implements FileWatcherListener, ArcCompilerListener
{
	private static final String arcPropertiesFile = "arc.properties";
	private static final String filesPropertiesFile = "files.properties";
	
	private static final String arcDirBase = "arc.dir.base";
	private static final String arcDirAssets = "arc.dir.assets";
	private static final String arcDirTemp = "arc.dir.temp";
	private static final String arcDirOut = "arc.dir.out";
	private static final String arcDirExe = "arc.dir.exe";
	private static final String arcDirToolsExe = "arc.dir.exe-tools";

	private static final String arcCompiler = "arc.compiler";

	private static final String arcProject = "arc.project";
	private static final String arcThreads = "arc.threads";
	private static final String arcWatcherDelay = "arc.watcher-delay";
	private static final String arcPlatform = "arc.platform";
	private static final String arcRendering = "arc.rendering";
	private static final String arcOutputLevel = "arc.output-level";
	private static final String arcAutoCompile = "arc.auto-compile";
	private static final String arcExeLevel = "arc.exe-mode";

	//private final Map<Class<? extends ArcCompiler>, Properties> compilerProperties = new HashMap<Class<? extends ArcCompiler>, Properties>();

	private static final Arc instance = new Arc();

	public static Arc getInstance()
	{
		return instance;
	}

	FileWatcher fileWatcher;
	ExecutorService compilerExecutor;
	Map<String, Class<? extends ArcCompiler>> compilers;
	Map<String, String> extensionToCompiler;
	Map<String, String> compilerOutExtension;
	Map<String, String[]> compilerInExtension;
	Properties properties;
	Properties filesProperties;
	boolean autoCompile;

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
		filesProperties = new Properties();
		compilers = new HashMap<String, Class<? extends ArcCompiler>>();
		extensionToCompiler = new HashMap<String, String>();
		compilerOutExtension = new HashMap<String, String>();
		compilerInExtension = new HashMap<String, String[]>();
	}

	public final void initialize()
	{
		loadProperties();

		database = new ArcDatabase( properties );
		database.initialize();

		fileWatcher.addListener( this );
		fileWatcher.start();
	}

	@SuppressWarnings( "unchecked" )
	public <T extends Enum<T>> T getEnumProperty( Properties props, String prop, T defaultEnum )
	{
		return (T)T.valueOf( defaultEnum.getClass(), props.getProperty( prop, defaultEnum.name() ) );
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
		
		try
		{
			filesProperties.load( new FileInputStream( new File( filesPropertiesFile ) ) );
		}
		catch( Exception e )
		{
			err( e.getMessage() );
		}
		
		rendering = getEnumProperty( properties, arcRendering, Rendering.DX11 );
		platform = getEnumProperty( properties, arcPlatform, Platform.Win32 );
		outputLevel = getEnumProperty( properties, arcOutputLevel, OutputLevel.Normal );
		executableMode = getEnumProperty( properties, arcExeLevel, ExecutableMode.Debug );

		out( OutputLevel.Verbos, "Loading properties..." );
		
		autoCompile = Boolean.parseBoolean( properties.getProperty( arcAutoCompile, "false" ) );
		
		fileWatcher = new FileWatcher();
		fileWatcher.setRootDir( getAssetsDirectory() );
		
		long watcherDelay = Long.parseLong( properties.getProperty( arcWatcherDelay, "1000" ) );
		fileWatcher.setDelay( watcherDelay );

		int numThreads = Integer.parseInt( properties.getProperty( arcThreads, "10" ) );
		compilerExecutor = Executors.newFixedThreadPool( numThreads );

		Properties compilersProp = getSubProperties( properties, arcCompiler );
		for( Entry<Object, Object> e : compilersProp.entrySet() )
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
						Class<? extends ArcCompiler> compilerClass = (Class<? extends ArcCompiler>)clazz;

						//compilerProperties.put( compilerClass, new Properties() );
						compilers.put( key, compilerClass );

						// get the extensions for this compiler
						String exts = compilersProp.getProperty( key + ".ext", "" ).toLowerCase();
						String[] extsArr = exts.split( "," );
						for( int i = 0; i < extsArr.length; ++i )
						{
							extsArr[i] = extsArr[i].trim();
						}
						compilerInExtension.put( key, extsArr );
						
						// add extensions to file watcher
						for( String extension : extsArr )
						{
							extensionToCompiler.put( extension, key );
							fileWatcher.addAcceptedFileExtension( extension );
						}
						
						// get the output extension for this compiler
						String outExt = compilersProp.getProperty( key + ".out", "" ).toLowerCase();
						if( !outExt.isEmpty() )
						{
							compilerOutExtension.put( key, outExt );
						}
					}
					else
					{
						throw new ClassCastException( "Failed to load compiler " + value );
					}
				}
				catch( Exception e1 )
				{
					err( e1.getMessage() );
				}
			}
		}

		fileWatcher.setIsEnabled( autoCompile );
	}

	public final String getFullDirectoryPath( String path )
	{
		String fullPath = null;
		{
			path = path
					.replace( "{Platform}", platform.name() )
					.replace( "{Arch}", platform.getArch() )
					.replace( "{ExeMode}", executableMode.name() )
					.replace( "{Rendering}", rendering.name() )
					.replace( "{Project}", getProject() )
					;

			fullPath = fileWatcher.getStandardPathForFile( new File( path ) );
		}
		
		return fullPath;
	}
	
	public final void saveProperties()
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
	
	public final String[] getCompilerExtentions( String compiler )
	{
		return compilerInExtension.get( compiler );
	}

	public final Properties getCompilerProperties( Class<? extends ArcCompiler> clazz )
	{
		return null; //new Properties(); //compilerProperties.get( clazz );
	}

	public final String getProject()
	{
		return properties.getProperty( arcProject );
	}
	
	public final String getBaseDirectory()
	{
		return getFullDirectoryPath( properties.getProperty( arcDirBase ) );
	}
	
	public final String getAssetsDirectory()
	{
		return getFullDirectoryPath( properties.getProperty( arcDirBase ) + properties.getProperty( arcDirAssets ) );
	}
	
	public final String getOutputRootDirectory()
	{
		return getFullDirectoryPath( properties.getProperty( arcDirBase ) + properties.getProperty( arcDirOut ) );
	}

	public final String getTempDirectory()
	{
		return getFullDirectoryPath( properties.getProperty( arcDirBase ) + properties.getProperty( arcDirTemp ) );
	}
	
	public final String getToolsExeDirectory()
	{
		return getFullDirectoryPath( properties.getProperty( arcDirBase ) + properties.getProperty( arcDirToolsExe ) );
	}

	public final String getExeDirectory()
	{
		return getFullDirectoryPath( properties.getProperty( arcDirBase ) + properties.getProperty( arcDirExe ) );
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
		filesProperties.clear();
		loadProperties();
	}
	
	public long getFileModificationTime( String filename )
	{
		return fileWatcher.getFileModificationTime( filename );
	}
	
	public final List<String> getFiles()
	{
		return fileWatcher.getFiles();
	}
	
	public final List<String> getLocalFiles()
	{
		List<String> files = new ArrayList<String>( getFiles() );

		for( int i = 0, imax = files.size(); i < imax; ++i )
		{
			files.set( i, files.get( i ).replace( getAssetsDirectory(), "" ) );
		}
		
		return files;
	}
	
	public final int getNumFiles()
	{
		return fileWatcher.getNumFiles();
	}
	
	public final List<String> getFilesByExtension( String extension )
	{
		List<String> allFiles = getFiles();
		List<String> files = new ArrayList<String>();
		
		for( String file : allFiles )
		{
			if( file.endsWith( "." + extension ) )
			{
				files.add( file );
			}
		}
		
		return files;
	}
	
	public final List<String> getFilesByExtensions( String[] extensions )
	{
		List<String> allFiles = getFiles();
		List<String> files = new ArrayList<String>();
		
		for( String file : allFiles )
		{
			for( String ext : extensions )
			{
				if( file.endsWith( "." + ext ) )
				{
					files.add( file );
					break;
				}
			}
		}
		
		return files;
	}

	public final void executeCompiler( String compiler )
	{
		Class<? extends ArcCompiler> clazz = compilers.get( compiler );
		if( clazz != null )
		{
			try
			{
				ArcCompiler arcCompiler = clazz.newInstance();
				arcCompiler.addProperties( properties );
				arcCompiler.setFileToCompile( "" );
				arcCompiler.setProject( getProject() );
				arcCompiler.setPlatform( getPlatform() );
				arcCompiler.setRendering( getRendering() );
				arcCompiler.setExectuableMode( getExecutableMode() );
				arcCompiler.setInputDirectory( getAssetsDirectory() );
				arcCompiler.setOutputDirectory( getOutputRootDirectory() );
				arcCompiler.setTempDirectory( getTempDirectory() );
				arcCompiler.setListener( this );
				if( compilerOutExtension.containsKey( compiler ) )
				{
					arcCompiler.setOutputExtension( compilerOutExtension.get( compiler ) );
				}

				compilerExecutor.execute( arcCompiler );
			}
			catch( Exception e )
			{
				err( e.getMessage() );
			}
		}
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
		addCompilerTasks( Arrays.asList( filePaths ) );
	}
	
	public final void addCompilerTasks( List<String> filePaths )
	{
		numTasks = filePaths.size();
		numErrored = 0;
		numSuccess = 0;

		fireArcProcessStart();
		for( String filePath : filePaths )
		{
			int extensionIndex = filePath.lastIndexOf( '.' );
			String extension = filePath.substring( extensionIndex + 1 ).toLowerCase();

			String smallFileName = filePath.replace( getAssetsDirectory(), "" );
			smallFileName = smallFileName.replace( '\\', '/' );

			int smallIndex = smallFileName.lastIndexOf( '.' );
			char c = smallFileName.charAt( 0 );
			int startIndex = c == '/' ? 1 : 0;
			smallFileName = smallFileName.substring( startIndex, smallIndex );
			
			Properties fileProperty = getSubProperties( filesProperties, smallFileName );

			try
			{
				String compiler = extensionToCompiler.get( extension );
				Class<? extends ArcCompiler> clazz = compilers.get( compiler );
				if( clazz != null )
				{
					ArcCompiler arcCompiler = clazz.newInstance();
					arcCompiler.addProperties( properties );
					arcCompiler.addProperties( fileProperty );
					arcCompiler.setFileToCompile( filePath );
					arcCompiler.setProject( getProject() );
					arcCompiler.setPlatform( getPlatform() );
					arcCompiler.setRendering( getRendering() );
					arcCompiler.setExectuableMode( getExecutableMode() );
					arcCompiler.setInputDirectory( getAssetsDirectory() );
					arcCompiler.setOutputDirectory( getOutputRootDirectory() );
					arcCompiler.setTempDirectory( getTempDirectory() );
					arcCompiler.setListener( this );
					if( compilerOutExtension.containsKey( compiler ) )
					{
						arcCompiler.setOutputExtension( compilerOutExtension.get( compiler ) );
					}

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
	
	@Override
	public void fileRemoved( String filePath )
	{
		database.removeFiles( filePath );
	}

	@Override
	public void fileListChanged()
	{
	}

	public final Platform getPlatform()
	{
		return platform;
	}

	public final void setPlatform( Platform platform )
	{
		this.platform = platform;
		//saveProperties();
	}

	public final OutputLevel getOutputLevel()
	{
		return outputLevel;
	}

	public final void setOutputLevel( OutputLevel outputLevel )
	{
		this.outputLevel = outputLevel;
		//saveProperties();
	}

	public final Rendering getRendering()
	{
		return rendering;
	}

	public final void setRendering( Rendering rendering )
	{
		this.rendering = rendering;
		//saveProperties();
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
		//saveProperties();
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
		if( outputLevel.ordinal() >= level.ordinal() )
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

	public final void addFileWatcherListener( FileWatcherListener listener )
	{
		fileWatcher.addListener( listener );
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
		
		if( numTasks == 0 )
		{
			fireArcProcessEnd();
		}
	}

	public final void onCompileFailed( String filePath, String msg, ArcCompiler compiler )
	{
		numTasks--;
		numErrored++;
		err( "Compile Failed " + filePath );
		fireArcProcessUpdate( false, compiler.getCompileTime() );
		
		if( numTasks == 0 )
		{
			fireArcProcessEnd();
		}
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
