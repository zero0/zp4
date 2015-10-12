package org.zero0.singularity.task;

import java.io.File;

import org.zero0.json.Value;
import org.zero0.singularity.ISingularityTaskExecution;
import org.zero0.singularity.SingularityFileSet;
import org.zero0.singularity.SingularityTask;
import org.zero0.singularity.SingularityTaskExecutionInfo;
import org.zero0.singularity.SingularityTaskExecutionResult;

public class MakeDirTask extends SingularityTask
{
	private String dir;

	@Override
	protected void onSetup( Value config )
	{
		dir = config.get( "dir" ).asString(); 
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
				SingularityFileSet set = SingularityFileSet.Create( new File( dir ) );

				File f = new File( set.getFile( 0 ) );
				
				boolean ok = f.mkdir();
				return ok ? SingularityTaskExecutionResult.Success : SingularityTaskExecutionResult.Failure;
			}
		};
	}
}
