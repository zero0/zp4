package org.zero0.singularity;

import java.io.PrintStream;

public final class SingularityTaskExecutionInfo
{
	private final int id;
	private final String name;
	
	private boolean isRunning;
	private float progress;
	private long startTime;
	private long endTime;
	private SingularityTaskExecutionResult result;
	
	private PrintStream out;
	private PrintStream err;
	
	public SingularityTaskExecutionInfo( int taskId, String taskName, PrintStream stdOut, PrintStream stdErr )
	{
		id = taskId;
		name = taskName;
		
		progress = 0f;
		startTime = 0L;
		endTime = 0L;
		result = SingularityTaskExecutionResult.None;
		
		out = stdOut;
		err = stdErr;
		
		isRunning = false;
	}
	
	public void start()
	{
		isRunning = true;
		progress = 0f;
		startTime = System.currentTimeMillis();
	}
	
	public void end( SingularityTaskExecutionResult r )
	{
		endTime = System.currentTimeMillis();
		progress = 1f;
		result = r;
		isRunning = false;
	}
	
	public int getId()
	{
		return id;
	}
	
	public String getName()
	{
		return name;
	}
	
	public boolean isRunning()
	{
		return isRunning;
	}
	
	public float getProgress()
	{
		return progress;
	}
	
	public void setProgress( float p )
	{
		progress = Math.min( Math.max( p, 0f ), 1f );
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
	
	public PrintStream getOut()
	{
		return out;
	}
	
	public PrintStream getErr()
	{
		return err;
	}
}
