package org.zero0.singularity;

public final class SingularityEnvironmentVariableFloat implements ISingularityEnvironmentVariable< Float >
{
	private float value;
	
	public SingularityEnvironmentVariableFloat( float value )
	{
		this.value = value;
	}
	
	@Override
	public Float getValue()
	{
		return value;
	}

	@Override
	public void setValue( Float value )
	{
		this.value = value;
	}

}
