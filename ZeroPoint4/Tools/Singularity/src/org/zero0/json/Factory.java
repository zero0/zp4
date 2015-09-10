package org.zero0.json;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public final class Factory
{
	public static final Value Null = new NullValue();
	
	public static final Value createBoolean( boolean value )
	{
		return new BooleanValue( value );
	}
	
	public static final Value createInt( int value )
	{
		return new IntValue( value );
	}
	
	public static final Value createLong( long value )
	{
		return new LongValue( value );
	}
	
	public static final Value createFloat( float value )
	{
		return new FloatValue( value );
	}
	
	public static final Value createString( String value )
	{
		return new StringValue( value );
	}
	
	public static final Value createArray( List< Value > value )
	{
		return new ArrayValue( value );
	}
	
	public static final Value createObject( Map< String, Value > value )
	{
		return new ObjectValue( value );
	}
	
	public static final Value clone( Value value )
	{
		switch( value.getType() )
		{
		case Boolean:
			return createBoolean( value.asBoolean() );
			
		case Int:
			return createInt( value.asInt() );
			
		case Long:
			return createLong( value.asLong() );
			
		case Float:
			return createFloat( value.asFloat() );
			
		case String:
			return createString( value.asString() );
			
		case Array:
			int size = value.getSize();
			List< Value > values = new ArrayList< Value >( size );
			for( int i = 0; i < size; ++i )
			{
				values.add( value.get( i ) );
			}
			return createArray( values );
			
		case Object:
			List< String > names = value.getMemberNames();
			Map< String, Value > map = new HashMap< String, Value >();
			for( int i = 0, imax = names.size(); i < imax; ++i )
			{
				String key = names.get( i );
				Value v = value.get( key );
				map.put( key, v );
			}
			return createObject( map );
		}
		
		return Null;
	}
}
