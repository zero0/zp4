package org.zero0.json;

final class FloatValue extends BaseValue
{
	private float value;
	
	public FloatValue( float v )
	{
		value = v;
	}
	
	public ValueType getType() { return ValueType.Float; }
	
	public boolean isFloat() { return true; }
	
	public boolean asBoolean() { return value != 0f; }
	public int asInt() { return (int)value; }
	public long asLong() { return (long)value; }
	public float asFloat() { return value; }
	public String asString() { return Float.toString( value ); }
}
