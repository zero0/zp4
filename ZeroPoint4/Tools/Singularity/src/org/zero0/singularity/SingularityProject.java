package org.zero0.singularity;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.zero0.json.Value;
import org.zero0.json.Merger;
import org.zero0.singularity.util.FileUtil;

public class SingularityProject
{
	private String id;
	private String name;
	private Value config;
	
	private SingularityEnvironment environment = new SingularityEnvironment();
	private Map< String, Class< ? extends SingularityTask > > commands = new HashMap< String, Class< ? extends SingularityTask > >();
	
	private List< SingularityTask > tasks = new ArrayList< SingularityTask >();
	
	public String getId()
	{
		return id;
	}
	
	public String getName()
	{
		return name;
	}
	
	public String getDescription()
	{
		return config.get( "description" ).asString();
	}
	
	public void setup( File projectRoot, Value engineConfig )
	{
		id = projectRoot.getName();
		
		File configFile = new File( projectRoot, id + ".config" );
		config = FileUtil.parseFile( configFile );
		config = Merger.merge( config, engineConfig );
		
		name = config.get( "name" ).asString();
		
		createCommands();
		
		createEnvironment();
		
		createTasks( projectRoot );
	}
	
	public void teardown()
	{
		commands.clear();
		
		tasks.clear();
	}
	
	private void createEnvironment()
	{

	}
	
	private void createCommands()
	{
		ClassLoader cl = ClassLoader.getSystemClassLoader();
		
		Value cmds = config.get( "commands" );
		if( !cmds.isEmpty() )
		{
			List< String > names = cmds.getMemberNames();
			for( String cmdName : names )
			{
				String cmd = cmds.get( cmdName ).asString();
				try
				{
					Class<?> clazz = cl.loadClass( cmd );
					if( SingularityTask.class.isAssignableFrom( clazz ) )
					{
						@SuppressWarnings( "unchecked" )
						Class< ? extends SingularityTask > stClazz = (Class< ? extends SingularityTask >)clazz;
						
						commands.put( cmdName, stClazz );
					}
				}
				catch( ClassNotFoundException e )
				{
					e.printStackTrace();
				}
			}
		}
	}
	
	private void createTasks( File projectRoot )
	{
		File[] taskFiles = projectRoot.listFiles( new FilenameFilter()
		{
			@Override
			public boolean accept( File dir, String name )
			{
				return name.endsWith( ".task" );
			}
		} );
		
		for( File taskFile : taskFiles )
		{
			Value taskConfig = FileUtil.parseFile( taskFile );
			String name = taskFile.getName();
			
			SingularityTask task = createTask( name.substring( 0, name.lastIndexOf( '.' ) ), taskConfig );
			if( task != null )
			{
				tasks.add( task );
			}
		}
	}
	
	public SingularityTask createTask( String name, Value config )
	{
		SingularityTask task = null;
		
		try
		{
			String taskName = config.get( "task" ).asString();
			Class< ? extends SingularityTask > clazz = commands.get( taskName );
			
			task = clazz.newInstance();
			task.setup( this, name, config.get( "args" ) );
		}
		catch( Exception e )
		{
			
		}
		
		return task;
	}

	public SingularityEnvironment getEnvironment()
	{
		return environment;
	}
}
