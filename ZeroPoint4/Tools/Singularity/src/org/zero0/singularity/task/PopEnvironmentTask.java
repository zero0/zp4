package org.zero0.singularity.task;

import org.zero0.json.Value;
import org.zero0.singularity.ISingularityTaskExecution;
import org.zero0.singularity.SingularityTask;
import org.zero0.singularity.SingularityTaskExecutionInfo;
import org.zero0.singularity.SingularityTaskExecutionResult;

public class PopEnvironmentTask extends SingularityTask
{
	@Override
	protected void onSetup( Value config )
	{
	}

	@Override
	protected void onTeardown()
	{
	}

	@Override
	protected ISingularityTaskExecution onExecute()
	{
		return new ISingularityTaskExecution()
		{
			@Override
			public SingularityTaskExecutionResult onRunStep( SingularityTaskExecutionInfo info )
			{
				info.getOut().println( "Popping Environment..." );
				
				getProject().getEnvironment().popEnvironment();
				return SingularityTaskExecutionResult.Success;
			}
		};
	}
}
