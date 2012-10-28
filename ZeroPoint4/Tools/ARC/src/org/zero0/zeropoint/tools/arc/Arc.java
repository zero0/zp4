package org.zero0.zeropoint.tools.arc;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompilerListener;
import org.zero0.zeropoint.tools.arc.compiler.ShaderCompiler;

public class Arc implements FileWatcherListener, ArcCompilerListener {

	private static String arcPropertiesFile = "arc.properties";
	
	private static String arcThreads = "arc.threads";
	private static String arcCompiler = "arc.compiler";
	private static String arcRoot = "arc.root";
	private static String arcIncludeExt = "arc.include.ext";
	
	private static Arc instance = null;
	
	public static Arc getInstance() {
		if( instance == null ) {
			instance = new Arc();
		}
		return instance;
	}
	
	FileWatcher fileWatcher;
	Executor compilerExecutor;
	Map<String, Class<? extends ArcCompiler>> compilers;
	Map<String, String> extensionToCompiler;
	Properties properties;
	List<String> includeExts;
	
	String rootDir;

	Platform platform;
	Rendering rendering;
	
	private Arc() {
		properties = new Properties();
		compilers = new HashMap<String, Class<? extends ArcCompiler>>();
		extensionToCompiler = new HashMap<String, String>();
		includeExts = new ArrayList<String>();
		
		loadProperties();
		
		fileWatcher.addListener( this );
	}
	
	@SuppressWarnings( "unchecked" )
	public void loadProperties() {
		try {
			properties.load( new FileInputStream( new File( arcPropertiesFile ) ) );
		} catch( FileNotFoundException e ) {
			e.printStackTrace();
		} catch( IOException e ) {
			e.printStackTrace();
		}
		
		rootDir = properties.getProperty( arcRoot );
		fileWatcher = new FileWatcher( rootDir );
		
		int numThreads = Integer.parseInt( properties.getProperty( arcThreads, "10" ) );
		compilerExecutor = Executors.newFixedThreadPool( numThreads );
		
		String includeExt = properties.getProperty( arcIncludeExt, "" ).toLowerCase();
		for( String ext : includeExt.split( "," ) ) {
			fileWatcher.addAcceptedFileExtension( ext.trim() );
		}
		
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
						compilers.put( key, (Class<? extends ArcCompiler>)clazz );
						compilerName = key;
						
						// get the sub properties for this compiler
						String exts = compiler.getProperty( compilerName + ".ext", "" ).toLowerCase();
						for( String ext : exts.split( "," ) ) {
							extensionToCompiler.put( ext.trim(), compilerName );
						}
					}
				} catch( ClassNotFoundException e1 ) {
					e1.printStackTrace();
				}
			}
		}
	}
	
	public String getRootDirectory() {
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
	
	public void reloadProperties() {
		properties.clear();
		loadProperties();
	}
	
	public void addCompilerTask( String filePath ) {
		String extension = filePath.substring( filePath.lastIndexOf( '.' ) + 1 ).toLowerCase();

		try {
			String compiler = extensionToCompiler.get( extension );
			Class<? extends ArcCompiler> clazz = compilers.get( compiler );
			if( clazz != null ){
				ArcCompiler arcCompilser = clazz.newInstance();
				arcCompilser.setFileToCompile( filePath );
				arcCompilser.setListener( this );
				
				compilerExecutor.execute( arcCompilser );
			}
		} catch( InstantiationException e ) {
			e.printStackTrace();
		} catch( IllegalAccessException e ) {
			e.printStackTrace();
		}
	}

	@Override
	public void fileChanged( String filePath ) {
		System.out.println( "File changed " + filePath );
		addCompilerTask( filePath );
	}
	
	public final Platform getPlatform() {
		return platform;
	}

	public final void setPlatform( Platform platform ) {
		this.platform = platform;
	}

	public final Rendering getRendering() {
		return rendering;
	}

	public final void setRendering( Rendering rendering ) {
		this.rendering = rendering;
	}

	public void shutdown() {
		fileWatcher.shutdown();
	}
	
	@Override
	public void onCompileMessage( String filePath, String msg ) {
		System.out.println( "Compile Message " + filePath );
	}

	@Override
	public void onCompileSuccess( String filePath, String msg ) {
		System.out.println( "Compile Success " + filePath );
	}

	@Override
	public void onCompileFailed( String filePath, String msg ) {
		System.out.println( "Compile Failed " + filePath );
	}
}
