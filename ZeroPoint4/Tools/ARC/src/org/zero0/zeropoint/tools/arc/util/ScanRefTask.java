package org.zero0.zeropoint.tools.arc.util;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.Vector;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.filters.util.ChainReaderHelper;
import org.apache.tools.ant.types.FilterChain;
import org.apache.tools.ant.types.Path;
import org.apache.tools.ant.types.Resource;

public class ScanRefTask extends Task
{
	private List<Path> paths = new ArrayList<Path>();
	private List<FilterChain> filterChains = new ArrayList<FilterChain>();
	private String reference = null;

	public void addPath( Path path )
	{
		paths.add( path );
	}

	public void addFilterChain( FilterChain chain )
	{
		filterChains.add( chain );
	}

	public void setReference( String reference )
	{
		this.reference = reference;
	}

	protected void validate()
	{
		if( reference == null || reference == "" ) throw new BuildException( "Reference not specified" );
	}

	public void execute()
	{
		validate();

		Set<String> refs = new HashSet<String>();
		Vector<FilterChain> chains = new Vector<FilterChain>( filterChains );
		for( Path p : paths )
		{
			for( Resource s : p )
			{
				log( s.getName() );

				BufferedReader br = null;
				try
				{
					ChainReaderHelper crh = new ChainReaderHelper();
					crh.setProject( getProject() );
					crh.setFilterChains( chains );
					crh.setBufferSize( s.size() );
					
					br = new BufferedReader( new InputStreamReader( s.getInputStream() ) );//new FileInputStream( s ) ) );
					
					crh.setPrimaryReader( br );
					String line = crh.readFully( crh.getAssembledReader() );
					
					String[] lines = line.split( "\r?\n\r?" );
					refs.addAll( Arrays.asList( lines ) );
				}
				catch( FileNotFoundException e )
				{
				}
				catch( IOException e )
				{
				}
				finally
				{
					if( br != null )
					{
						try
						{
							br.close();
						}
						catch( IOException e )
						{
						}
					}
				}
			}
		}
		
		//getProject().addReference( referenceName, value )
	}
}
