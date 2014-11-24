package org.zero0.zeropoint.tools.arc.pipe;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.Reader;

public final class ArcUtil
{
	private ArcUtil()
	{
	}

	public static String readFile( String file )
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
	
	private static < T extends IArcConfig > T createArcConfig( org.zero0.json.Value value )
	{
		T source = null;
		
		String sourceClass = value.get( "class" ).asString();
		
		try
		{
			Class<?> clazz = ClassLoader.getSystemClassLoader().loadClass( sourceClass );
			
			@SuppressWarnings( "unchecked" )
			Class< ? extends T > c = (Class< ? extends T >)clazz;
			
			source = c.newInstance();
			source.config( value.get( "params" ) );
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}
		
		return source;
	}
	
	public static IArcSource createArcSource( org.zero0.json.Value value )
	{
		IArcSource source = null;

		source = createArcConfig( value );
		
		return source;
	}
	
	public static IArcTarget createArcTarget( org.zero0.json.Value value )
	{
		IArcTarget target = null;
		
		target = createArcConfig( value );
		
		return target;
	}
	
	public static IArcPipe createArcPipe( org.zero0.json.Value value )
	{
		IArcPipe pipe = null;
		
		pipe = createArcConfig( value );
		
		return pipe;
	}
	
	public static IArcProcessor createArcProcessor( org.zero0.json.Value value )
	{
		IArcProcessor processor = null;
		
		processor = createArcConfig( value );
		
		return processor;
	}
}
