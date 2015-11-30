package org.zero0.singularity;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

public final class SingularityFileSet
{
	private List< String > files = new ArrayList< String >();
	
	public SingularityFileSet()
	{
	}
	
	public int getNumFiles()
	{
		return files.size();
	}
	
	public String getPath( int index )
	{
		return files.get( index );
	}
	
	public File getFile( int index )
	{
		String file = files.get( index );
		return new File( file );
	}
	
	public void append( String file )
	{
		append( new File( file ) );
	}
	
	public void append( String file, boolean includeSubFiles )
	{
		append( new File( file ), includeSubFiles );
	}

	public void append( File file )
	{
		String fileToAdd = file.getAbsolutePath();
		
		if( !files.contains( fileToAdd ) )
		{
			files.add( fileToAdd );
		}
	}
	
	public void append( File file, boolean includeSubFiles )
	{
		if( file.isDirectory() && includeSubFiles )
		{
			File[] fileList = file.listFiles();
			for( File fileInList : fileList )
			{
				append( fileInList, includeSubFiles );
			}
		}
		else
		{
			append( file );
		}
	}
	
	public void filter( ISingularityFileSetFilter filter )
	{
		Iterator< String > iter = files.iterator();
		while( iter.hasNext() )
		{
			File f = new File( iter.next() );
			if( !filter.acceptFile( f ) )
			{
				iter.remove();
			}
		}
	}
	
	public String getCommonRootDirectory()
	{
		String root = "";
		
		if( files.size() > 0 )
		{
			List< String > minParts = new ArrayList< String >();
			for( int i = 0; i < files.size(); ++i )
			{
				String file = files.get( i );
				
				File f = new File( file );
				if( f.isFile() )
				{
					file = f.getParentFile().getAbsolutePath();
				}
				
				String[] parts = file.split( File.separator );
				List< String > listParts = Arrays.asList( parts );
				
				if( i == 0 )
				{
					minParts.addAll( listParts );
				}
				
				int s;
				
				for( s = 0; s < minParts.size() && s < listParts.size(); ++s )
				{
					if( !minParts.get( s ).equals( listParts.get( s ) ) )
					{
						break;
					}
				}
				
				for( ; s < minParts.size(); ++s )
				{
					minParts.remove( s );
				}
			}
			
			StringBuilder sb = new StringBuilder();
			for( int i = 0; i < minParts.size(); ++i )
			{
				sb.append( minParts.get( i ) ).append( File.pathSeparatorChar );
			}
			
			root = sb.toString();
		}
		
		return root;
	}
}
