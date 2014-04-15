package org.zero0.json;

import java.util.ArrayList;
import java.util.List;

final class ArrayValue extends BaseValue
{
	private final List< Value > value = new ArrayList< Value >();
	
	public ArrayValue( List< Value > v )
	{
		if( v != null )
		{
			value.addAll( v );
		}
	}
	
	public ValueType getType() { return ValueType.Array; }
	
	public boolean isArray() { return true; }

	public boolean asBoolean() { return value.isEmpty(); }
	
	public boolean isEmpty() { return value.isEmpty(); }
	public int getSize() { return value.size(); }
	
	public Value get( int index )
	{
		int max = index + 1;
		while( value.size() < max )
		{
			value.add( Factory.Null );
		}
		return value.get( index );
	}
	public void set( int index, Value v )
	{
		int max = index + 1;
		while( value.size() < max )
		{
			value.add( Factory.Null );
		}
		value.set( index, v );
	}
}
