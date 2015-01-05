package org.zero0.zeropoint.tools.arc.util;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.Reader;

public final class FileUtil
{
	private FileUtil()
	{
	}

	public static String readFileText( String file )
	{
		StringBuilder b = new StringBuilder();
		
		try
		{
			Reader r = new BufferedReader( new FileReader( file ) );
	
			char[] buff = new char[ 256 ];
			int len = -1;
			while( ( len = r.read( buff ) ) != -1 )
			{
				b.append( buff, 0, len );
			}
		}
		catch( Exception e )
		{	
		}
		
		String data = b.toString();
		return data;
	}
	
}
