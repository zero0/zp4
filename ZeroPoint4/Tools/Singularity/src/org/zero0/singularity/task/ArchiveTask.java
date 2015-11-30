package org.zero0.singularity.task;

import org.zero0.json.Value;
import org.zero0.singularity.ISingularityTaskExecution;
import org.zero0.singularity.SingularityFileSet;
import org.zero0.singularity.SingularityTask;
import org.zero0.singularity.SingularityTaskExecutionInfo;
import org.zero0.singularity.SingularityTaskExecutionResult;

public class ArchiveTask extends SingularityTask
{
	enum ArchiveMethod
	{
		Zip,
		GZip,
		Tar,
	};

	private SingularityFileSet dir;
	private SingularityFileSet archive;
	private ArchiveMethod method;
	
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
				return SingularityTaskExecutionResult.Success;
			}
		};
	}
}
