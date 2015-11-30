package org.zero0.singularity;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class SingularityEnvironment
{
	private final List< HashMap<String,ISingularityEnvironmentVariable<?>>> variables = new ArrayList< HashMap< String, ISingularityEnvironmentVariable< ? > > >();

	public SingularityEnvironment()
	{
		HashMap<String,ISingularityEnvironmentVariable<?>> p = new HashMap<String, ISingularityEnvironmentVariable<?>>();
		variables.add( p );
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

	private Map< String, ISingularityEnvironmentVariable<?> > getCurrentVariables()
	{
		int size = variables.size();
		Map<String,ISingularityEnvironmentVariable<?>> p = variables.get( size - 1 );
		return p;
	}

	public Map< String, String > getVariables()
	{
		Map< String, ISingularityEnvironmentVariable<?> > p = getCurrentVariables();

		Map< String, String > env = new HashMap< String, String >();

		for( String key : p.keySet() )
		{
			ISingularityEnvironmentVariable<?> value = p.get( key );
			env.put( key, value.getValue().toString() );
		}

		return env;
	}

	public List< String > getVariableKeys()
	{
		Map< String, ISingularityEnvironmentVariable<?> > p = getCurrentVariables();

		List< String > keys = new ArrayList< String >( p.keySet() );
		return keys;
	}

	public String getVariableRaw( String key )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();

		ISingularityEnvironmentVariable< ? > r = p.get( key );

		return r.getValue().toString();
	}

	public String getVariable( String key )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();

		ISingularityEnvironmentVariable< ? > r = p.get( key );
		String s = r == null ? key : r.getValue().toString();

		StringBuilder sb = new StringBuilder();
		boolean isComplete = true;

		do
		{
			isComplete = true;
			boolean inBracket = false;
			for( int i = 0, e = 0, imax = s.length(); i < imax; ++i )
			{
				char c = s.charAt( i );
				if( inBracket )
				{
					if( c == '}' )
					{
						String subKey = s.substring( e, i );
						String subValue = getVariableRaw( subKey );

						sb.append( subValue );
						inBracket = false;
					}
				}
				else
				{
					if( c == '$' )
					{
						++i;
						c = s.charAt( i );
						if( c == '$' )
						{
							sb.append( '$' );
							continue;
						}

						if( c == '{' )
						{
							inBracket = true;
							e = i + 1;
						}
					}
					else
					{
						sb.append( c );
					}
				}
			}

			if( sb.indexOf( "${" ) >= 0 )
			{
				isComplete = false;
				s = sb.toString();
				sb.replace( 0, sb.length(), "" );
			}
		} while( !isComplete );

		return sb.toString();
	}

	public String getVariableString( String key )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();

		ISingularityEnvironmentVariable< ? > r = p.get( key );
		SingularityEnvironmentVariableString s = (SingularityEnvironmentVariableString) r;

		return s.getValue();
	}

	public int getVariableInteger( String key )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();

		ISingularityEnvironmentVariable< ? > r = p.get( key );
		SingularityEnvironmentVariableInteger s = (SingularityEnvironmentVariableInteger) r;

		return s.getValue();
	}

	public boolean getVariableBoolean( String key )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();

		ISingularityEnvironmentVariable< ? > r = p.get( key );
		SingularityEnvironmentVariableBoolean s = (SingularityEnvironmentVariableBoolean) r;

		return s.getValue();
	}

	public float getVariableFloat( String key )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();

		ISingularityEnvironmentVariable< ? > r = p.get( key );
		SingularityEnvironmentVariableFloat s = (SingularityEnvironmentVariableFloat) r;

		return s.getValue();
	}

	public void setVariable( String key, String value )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();
		p.put( key, new SingularityEnvironmentVariableString( value ) );
	}

	public void setVariable( String key, int value )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();
		p.put( key, new SingularityEnvironmentVariableInteger( value ) );
	}

	public void setVariable( String key, boolean value )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();
		p.put( key, new SingularityEnvironmentVariableBoolean( value ) );
	}

	public void setVariable( String key, float value )
	{
		Map<String,ISingularityEnvironmentVariable<?>> p = getCurrentVariables();
		p.put( key, new SingularityEnvironmentVariableFloat( value ) );
	}
}
