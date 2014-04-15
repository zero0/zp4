package org.zero0.json;

final class BooleanValue extends BaseValue
{
	private final boolean value;
	
	public BooleanValue( boolean v )
	{
		value = v;
	}
	
	public ValueType getType() { return ValueType.Boolean; }
	
	public boolean isBoolean() { return true; }
	
	public boolean asBoolean() { return value; }
	public int asInt() { return value ? 1 : 0; }
	public long asLong() { return value ? 1L : 0L; }
	public float asFloat() { return value ? 1f : 0f; }
	public String asString() { return Boolean.toString( value ); }
}
