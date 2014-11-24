package org.zero0.zeropoint.tools.arc.pipe;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

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
			
			String txt = ArcUtil.readFile( proc.getAbsolutePath() );
			org.zero0.json.Value json = org.zero0.json.Parser.parse( txt );
			
			IArcProcessor arcProc = ArcUtil.createArcProcessor( json );
			
			processors.put( filename, arcProc );
		}
	}
}
