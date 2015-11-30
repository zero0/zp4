package org.zero0.singularity.web;

import java.util.List;
import java.util.Map;

import org.zero0.json.Factory;
import org.zero0.json.Value;
import org.zero0.singularity.SingularityEngine;
import org.zero0.singularity.SingularityProject;
import org.zero0.singularity.web.ISingularityServletHandler.Response;

public class ExecuteTaskServletHandler implements ISingularityServletHandler
{
	@Override
	public Response onHandleRequest( List< String > arguments, Map< String, String > parameters, Value data )
	{
		if( arguments.size() < 2 )
		{
			data.set( "error", Factory.createString( "Invalid argument count " + arguments.size() ) );
			return Response.Failed;
		}
		
		String project = arguments.get( 0 );
		String taskName = arguments.get( 1 );
		
		SingularityProject proj = SingularityEngine.getInstance().getProject( project );
		if( proj == null )
		{
			data.set( "error", Factory.createString( "Unknown project " + project ) );
			return Response.Failed;
		}
		
		int taskId = proj.executeTask( taskName );
		if( taskId < 0 )
		{
			data.set( "error", Factory.createString( "Unable to start task " + taskName ) );
			return Response.Failed;
		}
		
		data.set( "taskId", Factory.createInt( taskId ) );
		
		return Response.Success;
	}
}
