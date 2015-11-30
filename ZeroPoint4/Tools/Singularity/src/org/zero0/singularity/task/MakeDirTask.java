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
	private SingularityFileSet dir;
	private int index;
	private boolean verbose;
	private boolean failOnError;

	@Override
	protected void onSetup( Value config )
	{
		Value dirConfig = config.get( "dir" );
		verbose = config.get( "verbose" ).asBoolean();
		failOnError = config.get( "failOnError" ).asBoolean();

		dir = getProject().createFileSetFromData( dirConfig );
		index = 0;
	}

	@Override
	protected void onTeardown()
	{
		index = 0;
	}

	@Override
	protected ISingularityTaskExecution onExecute()
	{
		return new ISingularityTaskExecution()
		{
			@Override
			public SingularityTaskExecutionResult onRunStep( SingularityTaskExecutionInfo info )
			{
				SingularityTaskExecutionResult r = SingularityTaskExecutionResult.InProgress;

				if( index < dir.getNumFiles() )
				{
					File f = dir.getFile( index++ );
					{
						boolean ok = f.mkdir();

						if( ok )
						{
							if( verbose ) {
								info.getOut().println( "Created Directory " + f.getName() );
							}
						}
						else
						{
							if( failOnError ) {
								r = SingularityTaskExecutionResult.Failure;
							}
							if( verbose ) {
								info.getErr().println( "Failed to create Directory " + f.getName() );
							}
						}
					}
				}
				else
				{
					r = SingularityTaskExecutionResult.Success;
					index = 0;
				}

				return r;
			}
		};
	}
}
