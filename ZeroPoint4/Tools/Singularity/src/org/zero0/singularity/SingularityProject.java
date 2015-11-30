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
	private int taskIds;

	private final SingularityEnvironment environment = new SingularityEnvironment();
	private final Map< String, Class< ? extends SingularityTask > > commands = new HashMap< String, Class< ? extends SingularityTask > >();

	private final List< SingularityTask > tasks = new ArrayList< SingularityTask >();
	private final List< SingularityTaskExecutionInfo > infos = new ArrayList< SingularityTaskExecutionInfo >();

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
		taskIds = 0;

		File configFile = new File( projectRoot, id + ".config" );
		config = FileUtil.parseFile( configFile );
		config = Merger.merge( config, engineConfig );

		name = config.get( "name" ).asString();
		description = config.get( "description" ).asString();

		Value env = config.get( "environment" );
		createEnvironment( env );

		Value cmds = config.get( "commands" );
		createCommands( cmds );

		createTasks( projectRoot );
	}

	public void teardown()
	{
		infos.clear();

		commands.clear();

		tasks.clear();
	}

	private void createEnvironment( Value env )
	{
		if( !env.isEmpty() )
		{
			List< String > names = env.getMemberNames();
			for( String envName : names )
			{
				Value envVar = env.get( envName );

				if( envVar.isInt() || envVar.isLong() )
				{
					environment.setVariable( envName, envVar.asInt() );
				}
				else if( envVar.isFloat() )
				{
					environment.setVariable( envName, envVar.asFloat() );
				}
				else if( envVar.isBoolean() )
				{
					environment.setVariable( envName, envVar.asBoolean() );
				}
				else
				{
					environment.setVariable( envName, envVar.asString() );
				}
			}
		}
	}

	private void createCommands( Value cmds )
	{
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
			task.setup( this, name, config );
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}

		return task;
	}

	public SingularityEnvironment getEnvironment()
	{
		return environment;
	}

	public void appendToFileSet( SingularityFileSet set, String path, boolean includeSubFiles )
	{
		String envPath = environment.getVariable( path );

		set.append( envPath, includeSubFiles );
	}

	public SingularityFileSet createFileSet( String path, boolean includeSubFiles )
	{
		SingularityFileSet set = new SingularityFileSet();

		String envPath = environment.getVariable( path );

		set.append( envPath, includeSubFiles );

		return set;
	}

	public SingularityFileSet createFileSetFromData( Value data )
	{
		SingularityFileSet set = new SingularityFileSet();

		if( data.isObject() )
		{
			Value file = data.get( "file" );
			if( !file.isEmpty() && file.isString() )
			{
				set.append( environment.getVariable( file.asString() ), false );
			}

			Value files = data.get( "files" );
			if( !files.isEmpty() && files.isArray() )
			{
				for( int i = 0, imax = files.getSize(); i < imax; ++i )
				{
					set.append( environment.getVariable( files.get( i ).asString() ), false );
				}
			}

			Value dir = data.get( "dir" );
			if( !dir.isEmpty() && dir.isString() )
			{
				set.append( environment.getVariable( dir.asString() ), true );
			}

			Value dirs = data.get( "dirs" );
			if( !dirs.isEmpty() && dirs.isArray() )
			{
				for( int i = 0, imax = dirs.getSize(); i < imax; ++i )
				{
					set.append( environment.getVariable( dirs.get( i ).asString() ), true );
				}
			}

			Value path = data.get( "path" );
			if( !path.isEmpty() && path.isString() )
			{
				set.append( environment.getVariable( path.asString() ), false );
			}

			Value paths = data.get( "paths" );
			if( !paths.isEmpty() && paths.isArray() )
			{
				for( int i = 0, imax = paths.getSize(); i < imax; ++i )
				{
					set.append( environment.getVariable( paths.get( i ).asString() ), false );
				}
			}

			Value filters = data.get( "filters" );
			if( !filters.isEmpty() && filters.isArray() )
			{
				for( int i = 0, imax = filters.getSize(); i < imax; ++i )
				{
					Value filter = filters.get( i );

				}
			}
		}

		return set;
	}

	public List< SingularityTaskExecutionInfo > getTaskInfos( String taskName )
	{
		List< SingularityTaskExecutionInfo > taskInfos = new ArrayList< SingularityTaskExecutionInfo >();

		synchronized( lock )
		{
			for( int i = 0, imax = infos.size(); i < imax; ++i )
			{
				SingularityTaskExecutionInfo t = infos.get( i );
				if( t.getName().equals( taskName ) )
				{
					taskInfos.add( t );
				}
			}
		}

		return taskInfos;
	}

	public SingularityTaskExecutionInfo getTaskInfo( int id )
	{
		SingularityTaskExecutionInfo info = null;

		synchronized( lock )
		{
			for( int i = 0, imax = infos.size(); i < imax; ++i )
			{
				SingularityTaskExecutionInfo t = infos.get( i );
				if( t.getId() == id )
				{
					info = t;
					break;
				}
			}
		}

		return info;
	}

	public List< SingularityTaskExecutionInfo > getRunningTaskInfos()
	{
		List< SingularityTaskExecutionInfo > taskInfos = new ArrayList< SingularityTaskExecutionInfo >();

		synchronized( lock )
		{
			for( int i = 0, imax = infos.size(); i < imax; ++i )
			{
				SingularityTaskExecutionInfo t = infos.get( i );
				if( t.isRunning() )
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
			if( t.getName().equals( taskName ) )
			{
				task = t;
				break;
			}
		}

		return task;
	}

	public int executeTask( final String taskName )
	{
		SingularityTask task = getTask( taskName );
		int r = -1;
		if( task != null )
		{
			final int taskId = taskIds++;
			r = taskId;

			final ISingularityTaskExecution e = task.execute();

			exec.execute( new Runnable()
			{
				@Override
				public void run()
				{
					SingularityTaskExecutionInfo info = new SingularityTaskExecutionInfo( taskId, taskName, System.out, System.err );
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

		return r;
	}
}
