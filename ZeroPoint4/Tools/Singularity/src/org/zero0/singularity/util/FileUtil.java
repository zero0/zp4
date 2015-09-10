package org.zero0.singularity.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import org.zero0.json.Factory;
import org.zero0.json.Parser;
import org.zero0.json.Value;

public final class FileUtil
{
	public static final Value parseFile( File file )
	{
		Value config = Factory.Null;
		
		try
		{
			int len = -1;
			char[] buff = new char[ 2048 ];
			StringBuilder sb = new StringBuilder();
			
			BufferedReader r = new BufferedReader( new FileReader( file ) );
			while( ( len = r.read( buff, 0, buff.length ) ) != -1 )
			{
				sb.append( buff, 0, len );
			}
			
			String json = sb.toString();
			
			config = Parser.parse( json );
		}
		catch( FileNotFoundException e )
		{
			e.printStackTrace();
		}
		catch( IOException e )
		{
			e.printStackTrace();
		}
		
		return config;
	}
}
