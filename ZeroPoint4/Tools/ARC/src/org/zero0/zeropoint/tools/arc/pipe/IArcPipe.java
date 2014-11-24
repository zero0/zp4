package org.zero0.zeropoint.tools.arc.pipe;

public interface IArcPipe extends IArcSource, IArcTarget
{
	void connect( IArcSource source, IArcTarget target );
}
