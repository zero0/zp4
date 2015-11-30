package org.zero0.singularity.web;

import java.util.List;
import java.util.Map;

import org.zero0.json.Value;

public interface ISingularityServletHandler
{
	public enum Response
	{
		Success( 200 ),
		Failed( 500 ),
		;
		
		private final int code;
		
		private Response( int c )
		{
			code = c;
		}
		
		public final int getCode()
		{
			return code;
		}
	};

	Response onHandleRequest( List< String > arguments, Map< String, String > parameters, Value data );
}