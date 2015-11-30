package org.zero0.singularity.web;

import java.util.List;
import java.util.Map;

import org.zero0.json.Builder;
import org.zero0.json.Factory;
import org.zero0.json.Value;
import org.zero0.singularity.SingularityEngine;
import org.zero0.singularity.SingularityProject;
import org.zero0.singularity.SingularityTaskExecutionInfo;

public class TaskStatusServletHandler implements ISingularityServletHandler
{
	@Override
	public Response onHandleRequest( List<String> arguments, Map<String, String> parameters, Value data )
	{
		if( arguments.size() < 2 )
		{
			data.set( "error", Factory.createString( "Invalid argument count " + arguments.size() ) );
			return Response.Failed;
		}
		
		String project = arguments.get( 0 );
		String taskId = arguments.get( 1 );
		int id = -1;
		
		try
		{
			id = Integer.parseInt( taskId );
		}
		catch( Exception e )
		{
			data.set( "error", Factory.createString( "Invalid task " + taskId ) );
			return Response.Failed;
		}
		
		SingularityProject proj = SingularityEngine.getInstance().getProject( project );
		if( proj == null )
		{
			data.set( "error", Factory.createString( "Unknown project " + project ) );
			return Response.Failed;
		}
		
		SingularityTaskExecutionInfo taskInfo = proj.getTaskInfo( id );
		if( taskInfo == null )
		{
			data.set( "error", Factory.createString( "Unable to find task " + taskId ) );
			return Response.Failed;
		}
		
		data.set( "taskInfo", taskInfoToJson( taskInfo ) );
		
		return Response.Success;
	}
	
	private Value taskInfoToJson( SingularityTaskExecutionInfo taskInfo )
	{
		Builder info = new Builder();
		info.set( "id", taskInfo.getId() );
		info.set( "name", taskInfo.getName() );
		info.set( "startTime", taskInfo.getStartTime() );
		info.set( "endTime", taskInfo.getEndTime() );
		info.set( "progress", taskInfo.getProgress() );
		info.set( "running", taskInfo.isRunning() );
		info.set( "result", taskInfo.getResult().toString() );
		
		return info;
	}
}
