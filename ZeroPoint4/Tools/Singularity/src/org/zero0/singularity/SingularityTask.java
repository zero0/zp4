package org.zero0.singularity;

import org.zero0.json.Value;

public abstract class SingularityTask implements Runnable
{
	public static final int Success = 1;
	public static final int Failure = 0;
	
	protected SingularityFileSet inputFileSet;
	protected SingularityFileSet outputFileSet;
	
	private SingularityProject project;
	private String name;
	
	public String getName()
	{
		return name;
	}
	
	public void run()
	{
		onRun();
	}
	
	public void setup( SingularityProject project, String name, Value config )
	{
		this.project = project;
		this.name = name;
		
		onSetup( config );
	}
	
	public void teardown()
	{
		onTeardown();
		
		this.project = null;
	}

	public SingularityFileSet getInputFileSet()
	{
		return inputFileSet;
	}
	
	public SingularityFileSet getOutputFileSet()
	{
		return outputFileSet;
	}
	
	public SingularityProject getProject()
	{
		return project;
	}
	
	protected abstract SingularityTaskExecutionResult onRun();
	
	protected abstract void onSetup( Value config );
	
	protected abstract void onTeardown();
}
