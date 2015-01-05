package org.zero0.zeropoint.tools.arc.pipe;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

import org.zero0.zeropoint.tools.arc.util.FileUtil;

public class ArcApplication
{
	private ArcConfig config;
	
	private Map< String, IArcProcessor > processors;
	
	private Properties properties;
	
	public ArcApplication()
	{
		config = new ArcConfig();
		processors = new HashMap< String, IArcProcessor >();
		properties = new Properties();
	}
	
	public void load()
	{
		try
		{
			properties.clear();
			properties.load( new BufferedInputStream( new FileInputStream( "arc.properties" ) ) );
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}

		processors.clear();
		
		config.load( properties.getProperty( "arc.config" ) );
	}
	
	public void unload()
	{
		
	}
	
	public void reload()
	{
		unload();
		load();
	}
	
	private void readProcessors()
	{
		String processorsDir = properties.getProperty( "arc.processors" );
		File procDir = new File( processorsDir );
		
		File[] procFiles = procDir.listFiles( new FilenameFilter()
		{
			@Override
			public boolean accept( File dir, String name )
			{
				return name.endsWith( ".json" );
			}
		} );
		
		for( File proc : procFiles )
		{
			String filename = proc.getName();
			
			String txt = FileUtil.readFileText( proc.getAbsolutePath() );
			org.zero0.json.Value json = org.zero0.json.Parser.parse( txt );
			
			IArcProcessor arcProc = createArcProcessor( json );
			
			processors.put( filename, arcProc );
		}
	}
	

	private static < T extends IArcConfig > T createArcConfig( org.zero0.json.Value value )
	{
		T source = null;
		
		String sourceClass = value.get( "class" ).asString();
		
		try
		{
			Class<?> clazz = ClassLoader.getSystemClassLoader().loadClass( sourceClass );
			
			@SuppressWarnings( "unchecked" )
			Class< ? extends T > c = (Class< ? extends T >)clazz;
			
			source = c.newInstance();
			source.config( value.get( "params" ) );
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}
		
		return source;
	}
	
	public static IArcSource createArcSource( org.zero0.json.Value value )
	{
		IArcSource source = null;

		source = createArcConfig( value );
		
		return source;
	}
	
	public static IArcTarget createArcTarget( org.zero0.json.Value value )
	{
		IArcTarget target = null;
		
		target = createArcConfig( value );
		
		return target;
	}
	
	public static IArcPipe createArcPipe( org.zero0.json.Value value )
	{
		IArcPipe pipe = null;
		
		pipe = createArcConfig( value );
		
		return pipe;
	}
	
	public static IArcProcessor createArcProcessor( org.zero0.json.Value value )
	{
		IArcProcessor processor = null;
		
		processor = createArcConfig( value );
		
		return processor;
	}
}
