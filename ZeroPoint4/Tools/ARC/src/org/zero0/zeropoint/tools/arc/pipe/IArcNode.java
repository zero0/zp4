package org.zero0.zeropoint.tools.arc.pipe;

public interface IArcNode
{
	public IArcResource produceResouce();
	public void consumeResource( IArcResource resource );
}
