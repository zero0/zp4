package org.zero0.singularity;

public final class SingularityEnvironmentVariableInteger implements ISingularityEnvironmentVariable< Integer >
{
	private Integer value;
	
	public SingularityEnvironmentVariableInteger( int value )
	{
		this.value = value;
	}
	
	@Override
	public Integer getValue()
	{
		return value;
	}

	@Override
	public void setValue( Integer value )
	{
		this.value = value;
	}

}
