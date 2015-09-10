package org.zero0.json;

final class LongValue extends BaseValue
{
	private long value;
	
	public LongValue( long v )
	{
		value = v;
	}
	
	public ValueType getType() { return ValueType.Long; }
	
	public boolean isLong() { return true; }
	
	public boolean asBoolean() { return value != 0L; }
	public int asInt() { return (int)value; }
	public long asLong() { return value; }
	public float asFloat() { return (float)value; }
	public String asString() { return Long.toString( value ); }
}
