package org.zero0.zeropoint.tools.arc;

import java.io.File;

import org.apache.tools.ant.BuildEvent;
import org.apache.tools.ant.BuildListener;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.ProjectHelper;
import org.apache.tools.ant.helper.ProjectHelper2;

public class ArcAnt implements BuildListener
{
	private static ArcAnt instance = new ArcAnt();
	
	private String buildFileName = "build.xml";
	private ProjectHelper projectHelper;
	private Project project;
	
	public static ArcAnt instance()
	{
		return instance;
	}
	
	private ArcAnt()
	{
	}
	
	public void init()
	{
		File buildFile = new File( buildFileName );
		project = new Project();
		project.setUserProperty( "ant.file", buildFile.getAbsolutePath() );
		project.init();

		projectHelper = ProjectHelper.getProjectHelper();
		project.addReference( "ant.projectHelper", projectHelper );
		project.addBuildListener( this );

		projectHelper.parse( project, buildFile );
	}
	
	public void execute( String targetName )
	{
		project.executeTarget( targetName );
	}

	@Override
	public void buildFinished( BuildEvent be )
	{
		System.out.println( "build finished: " + be.getMessage() );
	}

	@Override
	public void buildStarted( BuildEvent be )
	{
		System.out.println( "build started: " + be.getMessage() );
	}

	@Override
	public void messageLogged( BuildEvent be )
	{
		System.out.println( "be: " + be.getMessage() );
	}

	@Override
	public void targetFinished( BuildEvent be )
	{
		System.out.println( "finished: " + be.getMessage() );
	}

	@Override
	public void targetStarted( BuildEvent be )
	{
		System.out.println( "started: " + be.getMessage() );
	}

	@Override
	public void taskFinished( BuildEvent be )
	{
		System.out.println( "task finished: " + be.getMessage() );
	}

	@Override
	public void taskStarted( BuildEvent be )
	{
		System.out.println( "task started: " + be.getMessage() );
	}
}
