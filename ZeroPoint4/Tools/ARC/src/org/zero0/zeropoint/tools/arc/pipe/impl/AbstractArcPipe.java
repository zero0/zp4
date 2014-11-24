package org.zero0.zeropoint.tools.arc.pipe.impl;

import org.zero0.zeropoint.tools.arc.pipe.IArcPipe;
import org.zero0.zeropoint.tools.arc.pipe.IArcResource;
import org.zero0.zeropoint.tools.arc.pipe.IArcSource;
import org.zero0.zeropoint.tools.arc.pipe.IArcTarget;

public abstract class AbstractArcPipe implements IArcPipe
{
	protected IArcSource source;
	protected IArcTarget target;
	
	@Override
	public IArcResource produceResouce()
	{
		IArcResource resource = null;
		
		if( source != null )
		{
			resource = source.produceResouce();
		}
		
		return resource;
	}

	@Override
	public void consumeResource( IArcResource resource )
	{
		if( target != null )
		{
			target.consumeResource( resource );
		}
	}
	
	@Override
	public void connect( IArcSource source, IArcTarget target )
	{
		this.source = source;
		this.target = target;
	}

}
