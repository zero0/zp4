package org.zero0.zeropoint.tools.cube;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import org.zero0.json.Parser;
import org.zero0.json.Value;
import org.zero0.zeropoint.tools.cube.impl.JdbmFileDatabase;
import org.zero0.zeropoint.tools.cube.util.FileWatcher;
import org.zero0.zeropoint.tools.cube.util.JobManager;

public class Cube
{
	private static final String ConfigFile = "cube.config";
	
	private FileWatcher fileWatcher;
	private ICubeFileDatabase fileDatabase;
	private JobManager jobManager;

	public void initialize()
	{
		fileWatcher = new FileWatcher();
		fileDatabase = new JdbmFileDatabase();
		jobManager = new JobManager();
		
		fileDatabase.initialize();
	}
	
	public void setup()
	{
		fileDatabase.setup();

		fileWatcher.start();
		jobManager.start();
		
		config();
	}
	
	public void teardown()
	{
		fileDatabase.teardown();

		fileWatcher.stop();
		jobManager.stop();
	}
	
	public void shutdown()
	{
		fileDatabase.shutdown();
	}
	
	private void config()
	{
		StringBuilder sb = new StringBuilder();
		
		try
		{
			int len = 0;
			char[] buff = new char[ 512 ];
			BufferedReader br = new BufferedReader( new FileReader( ConfigFile ) );
			while( ( len = br.read( buff ) ) >= 0 )
			{
				sb.append( buff, 0, len );
			}
		}
		catch( IOException e )
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		String json = sb.toString();
		Value config = Parser.parse( json );
		
		String rootDir = config.get( "root" ).asString();
		if( !rootDir.equals( fileWatcher.getRootDir() ) )
		{
			fileWatcher.setRootDir( rootDir );
			fileWatcher.clean( true );
		}
		
		long fileWatchterDelay = config.get( "delay" ).asLong();
		fileWatcher.setDelay( fileWatchterDelay );
		
		Value acceptedExtensions = config.get( "extensions" );
		for( int i = 0, imax = acceptedExtensions.getSize(); i < imax; ++i )
		{
			String ext = acceptedExtensions.get( i ).asString();
			fileWatcher.addAcceptedFileExtension( ext );
		}
	}
}
