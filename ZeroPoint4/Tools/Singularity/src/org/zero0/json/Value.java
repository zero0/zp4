package org.zero0.json;

import java.util.List;

public interface Value
{
	public ValueType getType();
	
	public boolean isNull();
	public boolean isBoolean();
	public boolean isInt();
	public boolean isLong();
	public boolean isFloat();
	public boolean isString();
	public boolean isArray();
	public boolean isObject();
	
	public boolean asBoolean();
	public int asInt();
	public long asLong();
	public float asFloat();
	public String asString();
	
	public boolean isEmpty();
	public int getSize();
	
	public Value get( int index );
	public Value get( String key );
	
	public void set( int index, Value value );
	public void set( String key, Value value );
	
	public List< String > getMemberNames();
}
