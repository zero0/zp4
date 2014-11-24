package org.zero0.zeropoint.tools.arc.pipe.impl;

import org.zero0.json.Value;
import org.zero0.zeropoint.tools.arc.pipe.IArcResource;
import org.zero0.zeropoint.tools.arc.pipe.IArcTarget;

public class ArcPrintTarget implements IArcTarget
{

	@Override
	public void consumeResource( IArcResource resource )
	{
		System.out.println( resource.toString() );
	}

	@Override
	public void config( Value config )
	{
	}

}
