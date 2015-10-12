package org.zero0.singularity;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.winstone.Server;
import net.winstone.boot.BootStrap;

import org.zero0.json.Factory;
import org.zero0.json.Value;
import org.zero0.singularity.util.FileUtil;

public class SingularityEngine
{
	private static SingularityEngine instance;
	
	public static SingularityEngine getInstance()
	{
		return instance;
	}
	
	private List< SingularityProject > projects = new ArrayList< SingularityProject >();
	
	private Value config = Factory.Null;
	
	private Server server;
	
	public SingularityProject getProject( int index )
	{
		return projects.get( index );
	}
	
	public SingularityProject getProject( String id )
	{
		SingularityProject proj = null;
		for( int i = 0, imax = projects.size(); i < imax; ++i )
		{
			SingularityProject p = projects.get( i );
			if( p.getId().equals( id ) )
			{
				proj = p;
				break;
			}
		}
		return proj;
	}
	
	public List< String > getProjectIds()
	{
		List< String > ids = new ArrayList< String >( projects.size() );
		for( int i = 0, imax = projects.size(); i < imax; ++i )
		{
			SingularityProject proj = projects.get( i );
			ids.add( proj.getId() );
		}
		return ids;
	}
	
	public String getProjectId( int index )
	{
		return projects.get( index ).getId();
	}
	
	public int getNumProjects()
	{
		return projects.size();
	}
	
	public void setup()
	{
		instance = this;
		
		setupConfig();
		
		createProjects();
		
		setupServer();
	}
	
	public void teardown()
	{
		teardownServer();
		
		projects.clear();
		
		config = Factory.Null;
		
		instance = null;
	}
	
	private void setupConfig()
	{
		config = FileUtil.parseFile( new File( "./singularity.config" ) );
	}

	private void createProjects()
	{
		File projectDir = new File( "./projects" );
		
		if( projectDir.isDirectory() )
		{
			for( String proj : projectDir.list() )
			{
				File projRoot = new File( projectDir, proj );
				if( projRoot.isDirectory() )
				{
					SingularityProject p = createProject( projRoot );
					projects.add( p );
				}
			}
		}
	}
	
	private SingularityProject createProject( File projectRoot )
	{
		SingularityProject proj = null;
		
		proj = new SingularityProject();
		proj.setup( projectRoot, config );
		
		return proj;
	}
	
	private void setupServer()
	{
		System.setProperty( "java.endorsed.dirs", "" );
		
		Map< String, String > arguments = new HashMap< String, String >();
		arguments.put( "webroot", "./www" );
		arguments.put( "useJasper", "true" );
		arguments.put( "fork", "true" );
		arguments.put( "directoryListings", "false" );
		arguments.put( "fork", "true" );
		
		BootStrap b = new BootStrap( arguments );
		server = b.boot();

		server.start();
	}
	
	private void teardownServer()
	{
		server.shutdown();
		server.destroy();
		server = null;
	}
}
