package org.zero0.singularity.cmd;

import java.io.File;

import org.zero0.json.Value;
import org.zero0.singularity.*;
import org.zero0.singularity.SingularityTask;

public class MakeDirCommand extends SingularityTask
{
	private String dir;
	
	@Override
	protected SingularityTaskExecutionResult onRun()
	{
		SingularityFileSet set = SingularityFileSet.Create( new File( dir ) );

		File f = new File( set.getFile( 0 ) );
		
		boolean ok = f.mkdir();
		return ok ? SingularityTaskExecutionResult.Success : SingularityTaskExecutionResult.Failure;
	}

	@Override
	protected void onSetup( Value config )
	{
		dir = config.get( "dir" ).asString(); 
	}

	@Override
	protected void onTeardown()
	{

	}
}
