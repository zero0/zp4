package org.zero0.singularity.cmd;

import org.zero0.json.Value;
import org.zero0.singularity.SingularityTask;
import org.zero0.singularity.SingularityTaskExecutionResult;

import sun.security.action.GetPropertyAction;

public class PopEnvironmentCommand extends SingularityTask
{

	@Override
	protected SingularityTaskExecutionResult onRun()
	{
		getProject().getEnvironment().popEnvironment();
		return SingularityTaskExecutionResult.Success;
	}

	@Override
	protected void onSetup( Value config )
	{
	}

	@Override
	protected void onTeardown()
	{
	}
}
