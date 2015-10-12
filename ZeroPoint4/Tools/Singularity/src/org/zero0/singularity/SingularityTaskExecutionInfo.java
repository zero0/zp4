package org.zero0.singularity;

import java.io.PrintWriter;
import java.io.StringWriter;

public final class SingularityTaskExecutionInfo
{
	private final int id;
	private final String taskName;
	
	private long startTime;
	private long endTime;
	private SingularityTaskExecutionResult result;
	
	private StringWriter outString;
	private StringWriter errString;
	
	private PrintWriter out;
	private PrintWriter err;
	
	public SingularityTaskExecutionInfo( int id, String taskName )
	{
		this.id = id;
		this.taskName = taskName;
		
		startTime = 0L;
		endTime = 0L;
		result = null;
		
		outString = new StringWriter();
		errString = new StringWriter();
		
		out = new PrintWriter( outString );
		err = new PrintWriter( errString );
	}
	
	public void start()
	{
		this.startTime = System.currentTimeMillis();
	}
	
	public void end( SingularityTaskExecutionResult result )
	{
		this.endTime = System.currentTimeMillis();
		this.result = result;
	}
	
	public int getId()
	{
		return id;
	}
	
	public String getTaskName()
	{
		return taskName;
	}
	
	public long getStartTime()
	{
		return startTime;
	}
	
	public long getEndTime()
	{
		return endTime;
	}
	
	public SingularityTaskExecutionResult getResult()
	{
		return result;
	}
	
	public PrintWriter getOut()
	{
		return out;
	}
	
	public PrintWriter getErr()
	{
		return err;
	}
}
