package org.zero0.singularity.cmd;

import java.util.ArrayList;
import java.util.List;

import org.zero0.json.Value;
import org.zero0.singularity.SingularityTask;
import org.zero0.singularity.SingularityTaskExecutionResult;

public class CopyCommand extends SingularityTask
{
	List< String > input = new ArrayList< String >();
	List< String > output = new ArrayList< String >();
	
	@Override
	protected SingularityTaskExecutionResult onRun()
	{
		return SingularityTaskExecutionResult.Success;
	}

	@Override
	protected void onSetup( Value config )
	{
		Value in = config.get( "input" );
		Value out = config.get( "output" );
	
		AsStringOrArray( in, input );
		AsStringOrArray( out, output );
	}

	@Override
	protected void onTeardown()
	{
		input.clear();
		output.clear();
	}
	
	private static void AsStringOrArray( Value in, List< String > input )
	{
		if( in.isString() )
		{
			input.add( in.asString() );
		}
		else if( in.isArray() )
		{
			int size = in.getSize();
			for( int i = 0; i < size; ++i )
			{
				input.add( in.get( i ).asString() );
			}
		}
	}
}
