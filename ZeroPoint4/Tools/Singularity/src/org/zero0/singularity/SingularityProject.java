package org.zero0.singularity;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.zero0.json.Merger;
import org.zero0.json.Value;
import org.zero0.singularity.util.FileUtil;

public class SingularityProject
{
	private final Object lock = new Object();
	
	private String id;
	private String name;
	private String description;
	private Value config;
	
	private SingularityEnvironment environment = new SingularityEnvironment();
	private Map< String, Class< ? extends SingularityTask > > commands = new HashMap< String, Class< ? extends SingularityTask > >();
	
	private List< SingularityTask > tasks = new ArrayList< SingularityTask >();
	private List< SingularityTaskExecutionInfo > infos = new ArrayList< SingularityTaskExecutionInfo >();
	
	private ExecutorService exec;
	
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
		return description;
	}
	
	public void setup( File projectRoot, Value engineConfig )
	{
		exec = Executors.newCachedThreadPool();
		
		id = projectRoot.getName();
		
		File configFile = new File( projectRoot, id + ".config" );
		config = FileUtil.parseFile( configFile );
		config = Merger.merge( config, engineConfig );
		
		name = config.get( "name" ).asString();
		description = config.get( "description" ).asString();
		
		createCommands();
		
		createEnvironment();
		
		createTasks( projectRoot );
	}
	
	public void teardown()
	{
		infos.clear();
		
		commands.clear();
		
		tasks.clear();
	}
	
	private void createEnvironment()
	{

	}
	
	private void createCommands()
	{
		Value cmds = config.get( "commands" );
		if( !cmds.isEmpty() )
		{
			ClassLoader cl = ClassLoader.getSystemClassLoader();
			
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
	
	public List< SingularityTaskExecutionInfo > getTaskInfo( String taskName )
	{
		List< SingularityTaskExecutionInfo > taskInfos = new ArrayList< SingularityTaskExecutionInfo >();
		
		synchronized( lock )
		{
			for( int i = 0, imax = infos.size(); i < imax; ++i )
			{
				SingularityTaskExecutionInfo t = infos.get( i );
				if( t.getTaskName().equals( taskName ) )
				{
					taskInfos.add( t );
				}
			}
		}
		
		return taskInfos;
	}
	
	public List< String > getTaskNames()
	{
		List< String > taskNames = new ArrayList< String >( tasks.size() );
		for( int i = 0, imax = tasks.size(); i < imax; ++i )
		{
			SingularityTask t = tasks.get( i );
			taskNames.add( t.getName() );
		}
		
		return taskNames;
	}
	
	public SingularityTask getTask( String taskName )
	{
		SingularityTask task = null;
		for( int i = 0, imax = tasks.size(); i < imax; ++i )
		{
			SingularityTask t = tasks.get( i );
			if( t.getName() == taskName )
			{
				task = t;
				break;
			}
		}
		
		return task;
	}
	
	public void executeTask( final String taskName )
	{
		SingularityTask task = getTask( taskName );
		
		if( task != null )
		{
			final ISingularityTaskExecution e = task.execute();
			
			exec.execute( new Runnable()
			{
				@Override
				public void run()
				{
					SingularityTaskExecutionInfo info = new SingularityTaskExecutionInfo( infos.size(), taskName );
					synchronized( lock )
					{
						infos.add( info );
					}
					
					info.start();
					
					SingularityTaskExecutionResult result;

					do
					{
						result = e.onRunStep( info );
						Thread.yield();
					}
					while( result == SingularityTaskExecutionResult.InProgress );
					
					info.end( result );
				}
			} );
		}
	}
}
