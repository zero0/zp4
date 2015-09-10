package org.zero0.singularity;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;

import com.sun.xml.internal.ws.developer.UsesJAXBContext;

public class SingularityEnvironment
{
	private ArrayList< HashMap<String,ISingularityEnvironmentVariable<?>>> variables = new ArrayList< HashMap< String, ISingularityEnvironmentVariable< ? > > >();
	
	public SingularityEnvironment()
	{
		variables.add( new HashMap<String, ISingularityEnvironmentVariable<?>>() );
	}
	
	public void pushEnvironment()
	{
		HashMap<String,ISingularityEnvironmentVariable<?>> p = new HashMap<String, ISingularityEnvironmentVariable<?>>( getCurrentVariables() );
		variables.add( p );
	}
	
	public void popEnvironment()
	{
		int size = variables.size();
		if( size > 1 )
		{
			variables.remove( size - 1 );
		}
	}
	
	private Map<String,ISingularityEnvironmentVariable<?>> getCurrentVariables()
	{
		int size = variables.size();
		Map<String,ISingularityEnvironmentVariable<?>> p = variables.get( size - 1 );
		return p;
	}
	
	public String getVariable( String key )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();
		
		ISingularityEnvironmentVariable< ? > r = p.get( key );
		
		String t = r.toString();
		return t;
	}
	
	public void setVariable( String key, String value )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();
		p.put( key, new SingularityEnvironmentVariableString( value ) );
	}
	
	public static String getVar( SingularityEnvironment env, String key )
	{
		StringBuilder sb = new StringBuilder();
		boolean inBracket = false;
		for( int i = 0, s = 0, e = 0, imax = key.length(); i < imax; ++i )
		{
			char c = key.charAt( s );
			if( inBracket )
			{
				if( c == '}' )
				{
					
				}
			}
			else
			{
				if( c == '$' )
				{
					++s;
					c = key.charAt( s );
					if( c == '$' )
					{
						sb.append( '$' );
						continue;
					}
					if( c == '{' )
					{
						inBracket = true;
					}
				}
				else
				{
					sb.append( c );
				}
			}
		}
		
		return sb.toString();
	}
}
