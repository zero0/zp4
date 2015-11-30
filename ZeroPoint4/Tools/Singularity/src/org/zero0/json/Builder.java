package org.zero0.json;

import java.util.List;
import java.util.Map;

public class Builder implements Value
{
	private Value data = Factory.Null;
	
	@Override
	public ValueType getType()
	{
		return data.getType();
	}

	@Override
	public boolean isNull() {
		// TODO Auto-generated method stub
		return data.isNull();
	}

	@Override
	public boolean isBoolean() {
		// TODO Auto-generated method stub
		return data.isBoolean();
	}

	@Override
	public boolean isInt() {
		// TODO Auto-generated method stub
		return data.isInt();
	}

	@Override
	public boolean isLong() {
		// TODO Auto-generated method stub
		return data.isLong();
	}

	@Override
	public boolean isFloat() {
		// TODO Auto-generated method stub
		return data.isFloat();
	}

	@Override
	public boolean isString() {
		// TODO Auto-generated method stub
		return data.isString();
	}

	@Override
	public boolean isArray() {
		// TODO Auto-generated method stub
		return data.isArray();
	}

	@Override
	public boolean isObject() {
		// TODO Auto-generated method stub
		return data.isObject();
	}

	@Override
	public boolean asBoolean() {
		// TODO Auto-generated method stub
		return data.asBoolean();
	}

	@Override
	public int asInt() {
		// TODO Auto-generated method stub
		return data.asInt();
	}

	@Override
	public long asLong() {
		// TODO Auto-generated method stub
		return data.asLong();
	}

	@Override
	public float asFloat() {
		// TODO Auto-generated method stub
		return data.asFloat();
	}

	@Override
	public String asString() {
		// TODO Auto-generated method stub
		return data.asString();
	}

	@Override
	public boolean isEmpty() {
		// TODO Auto-generated method stub
		return data.isEmpty();
	}

	@Override
	public int getSize() {
		// TODO Auto-generated method stub
		return data.getSize();
	}

	@Override
	public Value get( int index ) {
		// TODO Auto-generated method stub
		return data.get( index );
	}

	@Override
	public Value get( String key ) {
		// TODO Auto-generated method stub
		return data.get( key );
	}

	@Override
	public void set( int index, Value value ) {
		ensureArray();
		data.set( index, value );
	}

	public void set( int index, boolean value ) {
		ensureArray();
		data.set( index, Factory.createBoolean( value ) );
	}

	public void set( int index, int value ) {
		ensureArray();
		data.set( index, Factory.createInt( value ) );
	}
	
	public void set( int index, long value ) {
		ensureArray();
		data.set( index, Factory.createLong( value ) );
	}

	public void set( int index, float value ) {
		ensureArray();
		data.set( index, Factory.createFloat( value ) );
	}

	public void set( int index, String value ) {
		ensureArray();
		data.set( index, Factory.createString( value ) );
	}

	public void set( int index, List< Value > value ) {
		ensureArray();
		data.set( index, Factory.createArray( value ) );
	}
	
	public void set( int index, Map< String, Value > value ) {
		ensureArray();
		data.set( index, Factory.createObject( value ) );
	}

	@Override
	public void set( String key, Value value ) {
		ensureObject();
		data.set( key, value );
	}
	
	public void set( String key, boolean value ) {
		ensureObject();
		data.set( key, Factory.createBoolean( value ) );
	}

	public void set( String key, int value ) {
		ensureObject();
		data.set( key, Factory.createInt( value ) );
	}
	
	public void set( String key, long value ) {
		ensureObject();
		data.set( key, Factory.createLong( value ) );
	}

	public void set( String key, float value ) {
		ensureObject();
		data.set( key, Factory.createFloat( value ) );
	}

	public void set( String key, String value ) {
		ensureObject();
		data.set( key, Factory.createString( value ) );
	}

	public void set( String key, List< Value > value ) {
		ensureObject();
		data.set( key, Factory.createArray( value ) );
	}
	
	public void set( String key, Map< String, Value > value ) {
		ensureObject();
		data.set( key, Factory.createObject( value ) );
	}

	@Override
	public List<String> getMemberNames() {
		return data.getMemberNames();
	}

	public void set( Value value ) {
		data = value;
	}
	
	public void set( boolean value ) {
		data = Factory.createBoolean( value );
	}

	public void set( int value ) {
		data = Factory.createInt( value );
	}
	
	public void set( long value ) {
		data = Factory.createLong( value );
	}

	public void set( float value ) {
		data = Factory.createFloat( value );
	}

	public void set( String value ) {
		data = Factory.createString( value );
	}

	public void set( List< Value > value ) {
		data = Factory.createArray( value );
	}
	
	public void set( Map< String, Value > value ) {
		data = Factory.createObject( value );
	}
	
	private void ensureArray()
	{
		if( !data.isArray() )
		{
			data = Factory.createArray( null );
		}
	}
	
	private void ensureObject()
	{
		if( !data.isObject() )
		{
			data = Factory.createObject( null );
		}
	}
}
