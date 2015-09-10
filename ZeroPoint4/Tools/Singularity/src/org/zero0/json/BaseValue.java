package org.zero0.json;

import java.util.List;

class BaseValue implements Value
{
	@Override
	public ValueType getType()
	{
		return ValueType.Null;
	}
	
	@Override
	public boolean isNull()
	{
		return false;
	}

	@Override
	public boolean isBoolean()
	{
		return false;
	}

	@Override
	public boolean isInt()
	{
		return false;
	}

	@Override
	public boolean isLong()
	{
		return false;
	}

	@Override
	public boolean isFloat()
	{
		return false;
	}

	@Override
	public boolean isString()
	{
		return false;
	}

	@Override
	public boolean isArray()
	{
		return false;
	}

	@Override
	public boolean isObject()
	{
		return false;
	}

	@Override
	public boolean asBoolean()
	{
		return false;
	}

	@Override
	public int asInt()
	{
		return 0;
	}

	@Override
	public long asLong()
	{
		return 0L;
	}

	@Override
	public float asFloat()
	{
		return 0f;
	}

	@Override
	public String asString()
	{
		return "";
	}

	@Override
	public boolean isEmpty()
	{
		return false;
	}

	@Override
	public int getSize()
	{
		return 0;
	}

	@Override
	public Value get( int index )
	{
		return Factory.Null;
	}

	@Override
	public Value get( String key )
	{
		return Factory.Null;
	}

	@Override
	public void set( int index, Value value )
	{
	}

	@Override
	public void set( String key, Value value )
	{
	}

	@Override
	public List< String > getMemberNames()
	{
		return null;
	}
}
