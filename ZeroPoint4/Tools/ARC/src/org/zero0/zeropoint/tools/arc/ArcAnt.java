package org.zero0.zeropoint.tools.arc;

import java.io.File;

import org.apache.tools.ant.BuildEvent;
import org.apache.tools.ant.BuildListener;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.ProjectHelper;
import org.apache.tools.ant.helper.ProjectHelper2;
import org.apache.tools.ant.types.FileSet;

public class ArcAnt implements BuildListener
{
	private static ArcAnt instance = null;
	
	private String buildFileName = "build.xml";
	private ProjectHelper projectHelper;
	private Project project;
	
	public static ArcAnt instance()
	{
		if( instance == null )
		{
			instance = new ArcAnt();
		}
		return instance;
	}
	
	private ArcAnt()
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
		System.out.println( "\\\\\\ build finished: " + be.getMessage() );
	}

	@Override
	public void buildStarted( BuildEvent be )
	{
		System.out.println( "/// build started: " + be.getMessage() );
	}

	@Override
	public void messageLogged( BuildEvent be )
	{
		if( be.getPriority() <= Project.MSG_WARN )
		System.out.println( "+++ " + be.getPriority() + " -> " + be.getMessage() );
	}

	@Override
	public void targetFinished( BuildEvent be )
	{
		System.out.println( "<<< target finished: " + be.getMessage() );
	}

	@Override
	public void targetStarted( BuildEvent be )
	{
		System.out.println( ">>> target started: " + be.getMessage() );
	}

	@Override
	public void taskFinished( BuildEvent be )
	{
		System.out.println( "((( task finished: " + be.getMessage() );
	}

	@Override
	public void taskStarted( BuildEvent be )
	{
		System.out.println( "))) task started: " + be.getMessage() );
	}
}
