package org.zero0.singularity;

import org.zero0.json.Value;

public abstract class SingularityTask
{
	private SingularityProject project;
	private String name;
	
	public String getName()
	{
		return name;
	}

	public ISingularityTaskExecution execute()
	{
		ISingularityTaskExecution exec = onExecute();
		return exec;
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
	
	public SingularityProject getProject()
	{
		return project;
	}
	
	protected abstract ISingularityTaskExecution onExecute();

	protected abstract void onSetup( Value config );
	
	protected abstract void onTeardown();
}
