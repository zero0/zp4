package org.zero0.singularity.task;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.zero0.json.Value;
import org.zero0.singularity.ISingularityTaskExecution;
import org.zero0.singularity.SingularityEnvironment;
import org.zero0.singularity.SingularityFileSet;
import org.zero0.singularity.SingularityTask;
import org.zero0.singularity.SingularityTaskExecutionInfo;
import org.zero0.singularity.SingularityTaskExecutionResult;

public class ProcTask extends SingularityTask
{
	private ProcessBuilder procBuilder;
	private boolean verbose;
	
	private enum ProcState
	{
		Init,
		Running,
		Complete,
		Finished,
	};
	
	@Override
	protected void onSetup( Value config )
	{
		verbose = config.get( "verbose" ).asBoolean();
		
		SingularityEnvironment env = getProject().getEnvironment();
		
		procBuilder = new ProcessBuilder();
		
		// setup exec and params
		String exe = config.get( "exe" ).asString();
		String exec = env.getVariable( exe );
		
		List< String > params = asParamList( config.get( "params" ), env );
		params.add( 0, exec );
		
		// set working dir
		String dir = config.get( "dir" ).asString();
		SingularityFileSet path = getProject().createFileSet( dir, false );
		File workingDir = path.getFile( 0 );
		
		procBuilder.directory( workingDir );
		procBuilder.command( params );
		
		// setup environment variables
		Map< String, String > procEnv = procBuilder.environment();
		Map< String, String > projEnv = env.getVariables();
		
		for( String key : projEnv.keySet() )
		{
			procEnv.put( key, projEnv.get( key ) );
		}
	}
	
	private List< String > asParamList( Value in, SingularityEnvironment env )
	{
		int size = in.getSize();
		List< String > out = new ArrayList< String >();
		
		for( int i = 0; i < size; ++i )
		{
			String s = in.get( i ).asString();
			out.add( env.getVariable( s ) );
		}
		
		return out;
	}

	@Override
	protected void onTeardown()
	{
		procBuilder = null;
	}

	@Override
	protected ISingularityTaskExecution onExecute()
	{
		class StreamGobbler extends Thread
		{
			private InputStream is;
			private PrintStream o;
			
			public StreamGobbler( InputStream in, PrintStream out )
			{
				is = in;
				o = out;
			}

			public void run()
			{
				try
				{
					BufferedReader br = new BufferedReader( new InputStreamReader( is ) );
					String line = null;
					while( ( line = br.readLine() ) != null )
					{
						o.println( line );
					}
				}
				catch( IOException ioe )
				{
					ioe.printStackTrace( o );
				}
			}
		}
		
		return new ISingularityTaskExecution()
		{
			private ProcState state = ProcState.Init;
			private SingularityTaskExecutionResult result;
			private StreamGobbler outGobbler;
			private StreamGobbler errGobbler;
			private Process proc;
			private int procCode;
			
			@Override
			public SingularityTaskExecutionResult onRunStep( SingularityTaskExecutionInfo info )
			{
				try
				{
					switch( state )
					{
					case Init:
						state = ProcState.Running;
						result = SingularityTaskExecutionResult.InProgress;
						procCode = Integer.MIN_VALUE;
						
						proc = procBuilder.start();
	
						outGobbler = new StreamGobbler( proc.getInputStream(), info.getOut() );
						errGobbler = new StreamGobbler( proc.getErrorStream(), info.getErr() );
						
						outGobbler.start();
						errGobbler.start();
						break;
						
					case Running:
						try
						{
							// throws an exception if the proc is not finished yet
							procCode = proc.exitValue();
							
							// proc has finished
							state = ProcState.Complete;
						}
						catch( Exception e )
						{
							state = ProcState.Running;
						}
						break;
						
					case Complete:
						result = procCode == 0 ? SingularityTaskExecutionResult.Success : SingularityTaskExecutionResult.Failure;
						proc.destroy();
						break;
					}
				}
				catch( Exception e )
				{
					result = SingularityTaskExecutionResult.Failure;
				}
				
				return result;
			}
		};
	}
}