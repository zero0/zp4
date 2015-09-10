package org.zero0.singularity;

public interface ISingularityEnvironmentVariable< T >
{
	T getValue();
	void setValue( T value );
}
