package org.zero0.zeropoint.tools.arc;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompilerListener;
import org.zero0.zeropoint.tools.arc.workspace.FileWatcher;
import org.zero0.zeropoint.tools.arc.workspace.FileWatcherListener;
import org.zero0.zeropoint.tools.arc.workspace.OutputAppender;
import org.zero0.zeropoint.tools.arc.workspace.OutputLevel;
import org.zero0.zeropoint.tools.arc.workspace.Platform;
import org.zero0.zeropoint.tools.arc.workspace.Rendering;

public final class Arc implements FileWatcherListener, ArcCompilerListener {

	private static String arcPropertiesFile = "arc.properties";

	private static String arcThreads = "arc.threads";
	private static String arcCompiler = "arc.compiler";
	private static String arcRoot = "arc.root";
	private static String arcPlatform = "arc.platform";
	private static String arcRendering = "arc.rendering";
	private static String arcOutputLevel = "arc.output-level";
	private static String arcAutoCompile = "arc.auto-compile";

	private final Map<Class<? extends ArcCompiler>, Properties> compilerProperties = new HashMap<Class<? extends ArcCompiler>, Properties>();

	private static Arc instance = null;

	public static Arc getInstance() {
		if( instance == null ) {
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

	String rootDir;

	Platform platform;
	Rendering rendering;
	OutputLevel outputLevel;

	OutputAppender outputAppender;

	volatile int numTasks;
	volatile int numSuccess;
	volatile int numErrored;

	private Arc() {
		properties = new Properties();
		compilers = new HashMap<String, Class<? extends ArcCompiler>>();
		extensionToCompiler = new HashMap<String, String>();
	}

	public final void initialize() {
		loadProperties();
		fileWatcher.addListener( this );
	}

	public final void loadProperties() {
		try {
			properties.load( new FileInputStream( new File( arcPropertiesFile ) ) );
		} catch( Exception e ) {
			err( e.getMessage() );
		}

		rendering = Rendering.valueOf( properties.getProperty( arcRendering, Rendering.DX11.name() ) );
		platform = Platform.valueOf( properties.getProperty( arcPlatform, Platform.Win32.name() ) );
		outputLevel = OutputLevel.valueOf( properties.getProperty( arcOutputLevel, OutputLevel.Normal.name() ) );

		out( OutputLevel.Verbos, "Loading properties..." );

		rootDir = properties.getProperty( arcRoot );
		fileWatcher = new FileWatcher( rootDir );

		autoCompile = Boolean.parseBoolean( properties.getProperty( arcAutoCompile, "false" ) );
		fileWatcher.setIsEnabled( autoCompile );

		int numThreads = Integer.parseInt( properties.getProperty( arcThreads, "10" ) );
		compilerExecutor = Executors.newFixedThreadPool( numThreads );

		String compilerName = "";
		Properties compiler = getSubProperties( properties, arcCompiler );
		for( Entry<Object, Object> e : compiler.entrySet() ) {
			String key = e.getKey().toString();
			String value = e.getValue().toString();

			// if there is no ".", it's the name and class of the compiler
			if( key.indexOf( '.' ) < 0 ) {
				try {
					Class<?> clazz = ClassLoader.getSystemClassLoader().loadClass( value );
					if( ArcCompiler.class.isAssignableFrom( clazz ) ) {
						@SuppressWarnings( "unchecked" )
						Class<? extends ArcCompiler> compilerClass = (Class<? extends ArcCompiler>) clazz;

						compilerProperties.put( compilerClass, new Properties() );
						compilers.put( key, compilerClass );
						compilerName = key;

						// get the sub properties for this compiler
						String exts = compiler.getProperty( compilerName + ".ext", "" ).toLowerCase();
						for( String ext : exts.split( "," ) ) {
							String extension = ext.trim();
							extensionToCompiler.put( extension, compilerName );
							fileWatcher.addAcceptedFileExtension( extension );
						}
					}
				} catch( ClassNotFoundException e1 ) {
					err( e1.getMessage() );
				}
			}
		}
	}

	private final void saveProperties() {
		OutputStream out = null;
		try {
			out = new BufferedOutputStream( new FileOutputStream( new File( arcPropertiesFile ) ) );
			properties.store( out, "" );
		} catch( Exception e ) {
			err( e.getMessage() );
		} finally {
			try {
				if( out != null )
					out.close();
			} catch( Exception e ) {
				err( e.getMessage() );
			}
		}
	}

	public final Collection<Class<? extends ArcCompiler>> getCompilers() {
		return Collections.unmodifiableCollection( compilers.values() );
	}

	public final Properties getCompilerProperties( Class<? extends ArcCompiler> clazz ) {
		return compilerProperties.get( clazz );
	}

	public final String getRootDirectory() {
		return rootDir;
	}

	public static Properties getSubProperties( Properties props, String subProperty ) {
		Properties sub = new Properties();

		for( Entry<Object, Object> e : props.entrySet() ) {
			String key = e.getKey().toString();
			int index = key.indexOf( subProperty );
			if( index != -1 ) {
				String subKey = key.substring( index + subProperty.length() + 1 );
				if( !subKey.isEmpty() ) {
					sub.put( subKey, e.getValue() );
				}
			}
		}

		return sub;
	}

	public final void reloadProperties() {
		properties.clear();
		loadProperties();
	}

	public final void addCompilerTask( String filePath ) {
		numTasks = 1;
		numErrored = 0;
		numSuccess = 0;

		String extension = filePath.substring( filePath.lastIndexOf( '.' ) + 1 ).toLowerCase();

		try {
			String compiler = extensionToCompiler.get( extension );
			Class<? extends ArcCompiler> clazz = compilers.get( compiler );
			if( clazz != null ) {
				ArcCompiler arcCompiler = clazz.newInstance();
				arcCompiler.setFileToCompile( filePath );
				arcCompiler.setPlatform( getPlatform() );
				arcCompiler.setRendering( getRendering() );
				arcCompiler.setListener( this );

				// arcCompiler.run();
				compilerExecutor.execute( arcCompiler );
			}
		} catch( Exception e ) {
			err( e.getMessage() );
		}
	}

	public final void addBulkCompilerTasks( String... filePaths ) {
		numTasks = filePaths.length;
		numErrored = 0;
		numSuccess = 0;

		for( String filePath : filePaths ) {
			String extension = filePath.substring( filePath.lastIndexOf( '.' ) + 1 ).toLowerCase();

			try {
				String compiler = extensionToCompiler.get( extension );
				Class<? extends ArcCompiler> clazz = compilers.get( compiler );
				if( clazz != null ) {
					ArcCompiler arcCompiler = clazz.newInstance();
					arcCompiler.setFileToCompile( filePath );
					arcCompiler.setPlatform( getPlatform() );
					arcCompiler.setRendering( getRendering() );
					arcCompiler.setListener( this );

					// arcCompiler.run();
					compilerExecutor.execute( arcCompiler );
				}
			} catch( Exception e ) {
				err( e.getMessage() );
			}
		}
	}

	@Override
	public final void fileChanged( String filePath ) {
		addCompilerTask( filePath );
	}

	public final Platform getPlatform() {
		return platform;
	}

	public final void setPlatform( Platform platform ) {
		this.platform = platform;
		saveProperties();
	}

	public final OutputLevel getOutputLevel() {
		return outputLevel;
	}

	public final void setOutputLevel( OutputLevel outputLevel ) {
		this.outputLevel = outputLevel;
		saveProperties();
	}

	public final Rendering getRendering() {
		return rendering;
	}

	public final void setRendering( Rendering rendering ) {
		this.rendering = rendering;
		saveProperties();
	}

	public final boolean getAutoCompile() {
		return autoCompile;
	}

	public final void setAutoCompile( boolean autoCompile ) {
		this.autoCompile = autoCompile;
		fileWatcher.setIsEnabled( autoCompile );
		saveProperties();
	}

	public final OutputAppender getOutputAppender() {
		return outputAppender;
	}

	public final void setOutputAppender( OutputAppender outputAppender ) {
		this.outputAppender = outputAppender;
	}

	public final void shutdown() {
		fileWatcher.removeAllListeners();
		fileWatcher.shutdown();
		compilerExecutor.shutdown();
	}

	public final void out( OutputLevel level, String text ) {
		// if( outputLevel.level >= level.level ) {
		// if( outputAppender != null ) {
		// outputAppender.printOut( text );
		// } else {
		// System.out.println( text );
		// }
		// }
		System.out.println( text );
	}

	public final void err( String text ) {
		// if( outputAppender != null ) {
		// outputAppender.printErr( text );
		// } else {
		// System.err.println( text );
		// }
		System.err.println( text );
	}

	public final void onCompileMessage( String filePath, String msg ) {
		out( OutputLevel.Normal, "Compile Message " + filePath );
	}

	public final void onCompileSuccess( String filePath, String msg ) {
		numTasks--;
		numSuccess++;
		out( OutputLevel.Compact, "Compile Success " + filePath );
	}

	public final void onCompileFailed( String filePath, String msg ) {
		numTasks--;
		numErrored++;
		err( "Compile Failed " + filePath );
	}
}
