package org.zero0.zeropoint.tools.arc.pipe.impl;

import org.zero0.json.Value;
import org.zero0.zeropoint.tools.arc.pipe.IArcResource;
import org.zero0.zeropoint.tools.arc.pipe.IArcSource;
import org.zero0.zeropoint.tools.arc.pipe.IArcTarget;

public class DirectArcPipe extends AbstractArcPipe
{
	@Override
	public boolean process()
	{
		IArcResource resource = produceResouce();
		if( resource != null )
		{
			consumeResource( resource );
		}
		return resource == null;
	}

	@Override
	public void config( Value config )
	{
	}

	@Override
	public boolean visit( IArcSource source )
	{
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean visit( IArcTarget target )
	{
		// TODO Auto-generated method stub
		return false;
	}

}
