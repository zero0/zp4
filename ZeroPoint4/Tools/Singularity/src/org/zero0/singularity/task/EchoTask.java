package org.zero0.singularity.task;

import org.zero0.json.Value;
import org.zero0.singularity.ISingularityTaskExecution;
import org.zero0.singularity.SingularityEnvironment;
import org.zero0.singularity.SingularityTask;
import org.zero0.singularity.SingularityTaskExecutionInfo;
import org.zero0.singularity.SingularityTaskExecutionResult;

public class EchoTask extends SingularityTask
{
	private String msg;
	private String err;

	@Override
	protected void onSetup( Value config )
	{
		Value configMsg = config.get( "msg" );
		Value configErr = config.get( "err" );

		SingularityEnvironment env = getProject().getEnvironment();
		msg = configMsg.isString() ? env.getVariable( configMsg.asString() ) : null;
		err = configErr.isString() ? env.getVariable( configErr.asString() ) : null;
	}

	@Override
	protected void onTeardown()
	{
		msg = null;
		err = null;
	}

	@Override
	protected ISingularityTaskExecution onExecute()
	{
		return new ISingularityTaskExecution()
		{
			@Override
			public SingularityTaskExecutionResult onRunStep( SingularityTaskExecutionInfo info )
			{
				if( msg != null )
				{
					info.getOut().println( msg );
				}

				if( err != null )
				{
					info.getErr().println( err );
				}

				return SingularityTaskExecutionResult.Success;
			}
		};
	}
}
