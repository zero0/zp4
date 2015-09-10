package org.zero0.singularity.cmd;

import java.util.ArrayList;
import java.util.List;

import org.zero0.json.Value;
import org.zero0.singularity.SingularityTask;
import org.zero0.singularity.SingularityTaskExecutionResult;

public class JobCommand extends SingularityTask
{
	private List< SingularityTask > tasks = new ArrayList< SingularityTask >();
	
	@Override
	protected SingularityTaskExecutionResult onRun()
	{
		for( int i = 0, imax = tasks.size(); i < imax; ++i )
		{
			SingularityTask task = tasks.get( i );
			task.run();
		}
		
		return SingularityTaskExecutionResult.Success;
	}

	@Override
	protected void onSetup( Value config )
	{
		int size = config.getSize();
		for( int i = 0; i < size; ++i )
		{
			Value taskConfig = config.get( i );
			SingularityTask task = getProject().createTask( getName() + "[" + i + "]", taskConfig );
			
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

}
