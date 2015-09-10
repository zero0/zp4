package org.zero0.singularity;

public final class SingularityEnvironmentVariableString implements ISingularityEnvironmentVariable< String >
{
	private String value;
	
	public SingularityEnvironmentVariableString( String value )
	{
		this.value = value;
	}
	
	@Override
	public String getValue()
	{
		// TODO Auto-generated method stub
		return value;
	}

	@Override
	public void setValue( String value )
	{
		this.value = value;
	}
}
