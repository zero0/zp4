package org.zero0.zeropoint.tools.arc.pipe.impl;

import java.util.List;

import org.zero0.json.Value;
import org.zero0.zeropoint.tools.arc.pipe.IArcPipe;
import org.zero0.zeropoint.tools.arc.pipe.IArcProcessor;
import org.zero0.zeropoint.tools.arc.pipe.IArcSource;
import org.zero0.zeropoint.tools.arc.pipe.IArcTarget;

public class ArcSequentialProcessor implements IArcProcessor
{
	private List< IArcSource > sources;
	private List< IArcTarget > targets;
	private List< IArcPipe > pipes;

	@Override
	public void config( Value config )
	{
		
	}

}
