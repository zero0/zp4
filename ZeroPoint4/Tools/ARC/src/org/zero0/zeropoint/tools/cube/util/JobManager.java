package org.zero0.zeropoint.tools.cube.util;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class JobManager implements Runnable
{
	private ExecutorService exec;
	
	public JobManager()
	{
		exec = null;
	}
	
	public void start()
	{
		if( exec == null )
		{
			exec = Executors.newSingleThreadExecutor();
		}
	}
	
	public void stop()
	{
		exec.shutdown();
	}
	
	public void shutdown()
	{
		exec.shutdown();
		exec = null;
	}
	
	public void queueJob( Runnable job )
	{
		exec.execute( job );
	}

	@Override
	public void run()
	{
	}
}
