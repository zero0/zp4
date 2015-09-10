package org.zero0.singularity;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public final class SingularityFileSet
{
	private List< String > files = new ArrayList< String >();
	
	private SingularityFileSet()
	{
	}
	
	public int getNumFiles()
	{
		return files.size();
	}
	
	public String getFile( int index )
	{
		String file = files.get( index );
		return file;
	}
	
	public static SingularityFileSet Create( File file )
	{
		SingularityFileSet set = new SingularityFileSet();
		
		Append( set, file );

		return set;
	}
	
	public static SingularityFileSet Create( File file, boolean includeFiles )
	{
		SingularityFileSet set = new SingularityFileSet();
		
		Append( set, file, includeFiles );

		return set;
	}
	
	public static SingularityFileSet Append( SingularityFileSet set, File file )
	{
		set.files.add( file.getAbsolutePath() );

		return set;
	}
	
	public static SingularityFileSet Append( SingularityFileSet set, File file, boolean includeFiles )
	{
		if( file.isDirectory() && includeFiles )
		{
			 File[] fileList = file.listFiles();
			 for( File fileInList : fileList )
			 {
				 set.files.add( fileInList.getAbsolutePath() );
			 }
		}
		else
		{
			set.files.add( file.getAbsolutePath() );
		}

		return set;
	}
}
