package org.zero0.singularity.task;

import org.zero0.json.Value;
import org.zero0.singularity.ISingularityTaskExecution;
import org.zero0.singularity.SingularityFileSet;
import org.zero0.singularity.SingularityProject;
import org.zero0.singularity.SingularityTask;
import org.zero0.singularity.SingularityTaskExecutionInfo;
import org.zero0.singularity.SingularityTaskExecutionResult;

public class CopyTask extends SingularityTask
{
	enum CopyTaskType
	{
		None,
		
		FileToFile,
		DirectoryToDirectory,
	};
	
	private SingularityFileSet input;
	private SingularityFileSet output;
	private boolean forceOverride;
	private boolean verbose;
	private CopyTaskType copyType;
	
	@Override
	protected void onSetup( Value config )
	{
		Value in = config.get( "input" );
		Value out = config.get( "output" );
		Value force = config.get( "force" );
		Value verb = config.get( "verbose" );
		
		copyType = CopyTaskType.None;
		
		input = new SingularityFileSet();
		input = new SingularityFileSet();
		
		AsStringOrArray( getProject(), in, input );
		AsStringOrArray( getProject(), out, output );
		
		forceOverride = force.isBoolean() ? force.asBoolean() : false;
		verbose = verb.isBoolean() ? verb.asBoolean() : false;
	}

	@Override
	protected void onTeardown()
	{
	}
	
	private static void AsStringOrArray( SingularityProject proj, Value in, SingularityFileSet input )
	{
		if( in.isString() )
		{
			String s = in.asString();
			
			proj.appendToFileSet( input, s, true );
		}
		else if( in.isArray() )
		{
			int size = in.getSize();
			for( int i = 0; i < size; ++i )
			{
				String s = in.get( i ).asString();
				
				proj.appendToFileSet( input, s, true );
			}
		}
	}

	@Override
	protected ISingularityTaskExecution onExecute()
	{
		return new ISingularityTaskExecution()
		{
			private int index = 0;
			
			@Override
			public SingularityTaskExecutionResult onRunStep( SingularityTaskExecutionInfo info )
			{
				if( copyType == CopyTaskType.None )
				{
					return SingularityTaskExecutionResult.Failure;
				}
				
				SingularityTaskExecutionResult r;
				
				if( index < input.getNumFiles() )
				{
					r = SingularityTaskExecutionResult.InProgress;
				}
				else
				{
					r = SingularityTaskExecutionResult.Success;
				}
				
				return r;
			}
		};
	}
}
