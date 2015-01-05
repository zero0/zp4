package org.zero0.zeropoint.tools.arc.pipe;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.zero0.zeropoint.tools.arc.util.FileUtil;

public class ArcConfig
{
	private Map< String, String > options;
	private org.zero0.json.Value config;
	
	public void load( String configFile )
	{
		String configData = FileUtil.readFileText( configFile );
		config = org.zero0.json.Parser.parse( configData );
		
		options = new HashMap< String, String >();
		readConfig( config.get( "options" ) );
	}
	
	private void readConfig( org.zero0.json.Value config )
	{
		List< String > names = config.getMemberNames();
		if( names != null )
		{
			for( String key : names )
			{
				String value = config.get( key ).asString();
				options.put( key, value );
			}
		}
	}
	
	public String getOption( String key )
	{
		String o = options.get( key );
		return o;
	}
	
	public void addOption( String key, String value )
	{
		options.put( key, value );
	}
	
	public String convertString( String str )
	{
		StringBuilder b = new StringBuilder();
		int s = -1, len = str.length();
		
		for( int i = 0; i < len; ++i )
		{
			char c = str.charAt( i );
			if( s < 0 && c == '{' )
			{
				s = i + 1;
			}
			else if( s >= 0 && c == '{' )
			{
				s = -1;
				b.append( c );
			}
			else if( s >= 0 && c == '}' )
			{
				String key = str.substring( s, i - s );
				s = -1;
				
				String value = options.get( key );
				if( value != null )
				{
					b.append( value );
				}
			}
			else if( s < 0 )
			{
				b.append( c );
			}
		}
		
		return b.toString();
	}
}	
