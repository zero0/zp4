package org.zero0.singularity.task;

import java.util.List;

import org.zero0.json.Factory;
import org.zero0.json.Value;
import org.zero0.singularity.ISingularityTaskExecution;
import org.zero0.singularity.SingularityEnvironment;
import org.zero0.singularity.SingularityTask;
import org.zero0.singularity.SingularityTaskExecutionInfo;
import org.zero0.singularity.SingularityTaskExecutionResult;

public class PushEnvironmentTask extends SingularityTask
{
	private Value environment = Factory.Null;
	
	@Override
	protected void onSetup( Value config )
	{
		environment = Factory.clone( config );
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
				info.getOut().println( "Pushing Environment..." );
				
				SingularityEnvironment env = getProject().getEnvironment();
				env.pushEnvironment();
				
				if( !environment.isEmpty() )
				{
					List< String > names = environment.getMemberNames();
					for( int i = 0, imax = names.size(); i < imax; ++i )
					{
						String name = names.get( i );
						Value value = environment.get( name );
						
						env.setVariable( name, value.asString() );
					}
				}
				
				return SingularityTaskExecutionResult.Success;
			}
		};
	}

}
