package org.zero0.singularity.task;

import org.zero0.json.Value;
import org.zero0.singularity.*;

public class EchoTask extends SingularityTask
{
	private String text = null;
	
	@Override
	protected void onSetup( Value config )
	{
		text = config.get( "text" ).asString();
	}

	@Override
	protected void onTeardown()
	{
		text = null;
	}

	@Override
	protected ISingularityTaskExecution onExecute()
	{
		return new ISingularityTaskExecution()
		{
			@Override
			public SingularityTaskExecutionResult onRunStep( SingularityTaskExecutionInfo info )
			{
				info.getOut().println( text );
			
				return SingularityTaskExecutionResult.Success;
			}
		};
	}

}
