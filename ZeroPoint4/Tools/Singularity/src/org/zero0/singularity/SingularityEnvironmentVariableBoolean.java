package org.zero0.singularity;

public final class SingularityEnvironmentVariableBoolean implements ISingularityEnvironmentVariable< Boolean >
{
	private boolean value;
	
	public SingularityEnvironmentVariableBoolean( boolean value )
	{
		this.value = value;
	}

	@Override
	public Boolean getValue()
	{
		return value;
	}

	@Override
	public void setValue( Boolean value )
	{
		this.value = value;
	}
}
