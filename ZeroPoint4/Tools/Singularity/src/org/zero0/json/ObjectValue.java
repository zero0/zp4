package org.zero0.json;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

final class ObjectValue extends BaseValue
{
	private final Map< String, Value > value = new HashMap< String, Value >();
	
	public ObjectValue( Map< String, Value > v )
	{
		if( v != null )
		{
			value.putAll( v );
		}
	}
	
	public ValueType getType() { return ValueType.Object; }
	
	public boolean isObject() { return true; }
	
	public boolean asBoolean() { return !value.isEmpty(); }
	
	public boolean isEmpty() { return value.isEmpty(); }
	public int getSize() { return value.size(); }
	
	public Value get( String key )
	{
		Value ret;
		return ( ret = value.get( key ) ) == null ? Factory.Null : ret;
	}
	
	public void set( String key, Value v )
	{
		value.put( key, v );
	}
	
	public List< String > getMemberNames()
	{
		return Collections.unmodifiableList( new ArrayList< String >( value.keySet() ) );
	}
}
