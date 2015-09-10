package org.zero0.json;

final class StringValue extends BaseValue
{
	private String value;
	
	public StringValue( String v )
	{
		value = v;
	}
	
	public ValueType getType() { return ValueType.String; }
	
	public boolean isString() { return true; }
	
	public boolean asBoolean() { return !value.isEmpty(); }
	public String asString() { return value; }
	
	public boolean isEmpty() { return value.isEmpty(); }
	public int getSize() { return value.length(); }
}
