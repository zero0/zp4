package org.zero0.zeropoint.tools.arc.pipe;

import java.io.File;
import java.io.FileFilter;

class ArcFileResource implements IArcResource
{
	private File[] files;
	
	public ArcFileResource( File file )
	{
		this.files = new File[]{ file };
	}
	
	@Override
	public String toString()
	{
		return files[0].getName();
	}

	@Override
	public File[] getFiles()
	{
		return files;
	}
};

class ArcFileListResource implements IArcResource, FileFilter
{
	private File[] files;
	private String pattern;
	
	public ArcFileListResource( File rootDir, String pattern )
	{
		this.pattern = pattern;
		this.files = rootDir.listFiles( this );
	}
	
	@Override
	public String toString()
	{
		return files.length > 0 ? files[0].getName() : super.toString();
	}

	@Override
	public File[] getFiles()
	{
		return files;
	}

	@Override
	public boolean accept( File pathname )
	{
		if( pattern == null || pattern.isEmpty() )
		{
			return true;
		}
		
		String name = pathname.getAbsolutePath();
		boolean matches = name.matches( pattern );
		return matches;
	}
};

public final class ArcResourceFactory
{
	public static IArcResource createFile( File file )
	{
		return new ArcFileResource( file );
	}
	
	public static IArcResource createFileList( File rootDir )
	{
		return new ArcFileListResource( rootDir, null );
	}
	
	public static IArcResource createFileList( File rootDir, String pattern )
	{
		return new ArcFileListResource( rootDir, pattern );
	}
}
