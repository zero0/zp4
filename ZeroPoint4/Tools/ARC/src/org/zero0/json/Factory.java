package org.zero0.json;

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
}
