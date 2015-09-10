package org.zero0.json;

final class IntValue extends BaseValue
{
	private int value;
	
	public IntValue( int v )
	{
		value = v;
	}
	
	public ValueType getType() { return ValueType.Int; }
	
	public boolean isInt() { return true; }
	
	public boolean asBoolean() { return value != 0; }
	public int asInt() { return value; }
	public long asLong() { return (long)value; }
	public float asFloat() { return (float)value; }
	public String asString() { return Integer.toString( value ); }
}
