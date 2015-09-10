package org.zero0.singularity.cmd;

import org.zero0.json.Value;
import org.zero0.singularity.*;

public class EchoCommand extends SingularityTask
{
	private String text = null;
	
	@Override
	protected SingularityTaskExecutionResult onRun()
	{
		System.out.println( text );
		
		return SingularityTaskExecutionResult.Success;
	}

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

}
