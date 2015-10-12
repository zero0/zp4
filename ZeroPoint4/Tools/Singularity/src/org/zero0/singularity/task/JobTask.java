package org.zero0.singularity.task;

import java.util.ArrayList;
import java.util.List;

import org.zero0.json.Value;
import org.zero0.singularity.ISingularityTaskExecution;
import org.zero0.singularity.SingularityTask;
import org.zero0.singularity.SingularityTaskExecutionInfo;
import org.zero0.singularity.SingularityTaskExecutionResult;

public class JobTask extends SingularityTask
{
	private List< SingularityTask > tasks = new ArrayList< SingularityTask >();
	
	@Override
	protected void onSetup( Value config )
	{
		Value jobs = config.get( "jobs" );
		
		int size = jobs.getSize();
		for( int i = 0; i < size; ++i )
		{
			Value taskConfig = jobs.get( i );
			SingularityTask task = getProject().createTask( getName() + " [" + i + "]", taskConfig );
			
			tasks.add( task );
		}
	}

	@Override
	protected void onTeardown()
	{
		for( int i = 0, imax = tasks.size(); i < imax; ++i )
		{
			SingularityTask task = tasks.get( i );
			task.teardown();
		}
		tasks.clear();
	}

	@Override
	protected ISingularityTaskExecution onExecute()
	{
		return new ISingularityTaskExecution()
		{
			private int currentTask = 0;
			private ISingularityTaskExecution currentExec = null;
			
			@Override
			public SingularityTaskExecutionResult onRunStep( SingularityTaskExecutionInfo info )
			{
				SingularityTaskExecutionResult result = SingularityTaskExecutionResult.Success;
				
				if( currentTask != tasks.size() )
				{
					if( currentExec == null )
					{
						SingularityTask task = tasks.get( currentTask );
						currentExec = task.execute();
					}
					
					result = currentExec.onRunStep( info );
					
					if( result == SingularityTaskExecutionResult.Success )
					{
						currentTask++;
						currentExec = null;
						result = SingularityTaskExecutionResult.InProgress;
					}
				}

				return result;
			}
		};
	}

}
