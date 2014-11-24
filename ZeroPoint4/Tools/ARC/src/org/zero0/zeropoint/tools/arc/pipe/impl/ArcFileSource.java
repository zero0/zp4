package org.zero0.zeropoint.tools.arc.pipe.impl;

import java.io.File;
import java.util.Arrays;
import java.util.List;

import org.zero0.zeropoint.tools.arc.pipe.ArcResourceFactory;
import org.zero0.zeropoint.tools.arc.pipe.IArcResource;
import org.zero0.zeropoint.tools.arc.pipe.IArcSource;

public class ArcFileSource implements IArcSource
{
	private List< File > fileList;
	private int currentFile;
	
	@Override
	public void config( org.zero0.json.Value config )
	{
		currentFile = 0;
		
		String dir = config.get( "root" ).asString();
		
		File f = new File( dir );
		File[] files = f.listFiles();
		
		fileList = Arrays.asList( files );
	}
	
	@Override
	public IArcResource produceResouce()
	{
		IArcResource resource = null;
		
		if( currentFile < fileList.size() )
		{
			resource = ArcResourceFactory.createFile( fileList.get( currentFile ) );
			currentFile++;
		}
		
		return resource;
	}

}
