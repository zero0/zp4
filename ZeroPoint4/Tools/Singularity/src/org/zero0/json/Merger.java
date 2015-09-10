package org.zero0.json;

import java.util.List;


public final class Merger
{
	public static final Value merge( Value target, Value options )
	{
		Value[] ta = new Value[] { target };
		mergeInternal( ta, options );
		
		return ta[0];
	}
	
	private static final void mergeInternal( Value[] target, Value options )
	{
		switch( options.getType() )
		{
		case Null:
			break;
			
		case Array:
			Value a = target[0];
			int size = options.getSize();
			for( int i = 0; i < size; ++i )
			{
				Value[] ta = new Value[] { a.get( i ) };
				mergeInternal( ta, options.get( i ) );
				a.set( i, ta[0] );
			}
			break;
			
		case Object:
			Value o = target[0];
			
			List< String > names = options.getMemberNames();
			for( int i = 0, imax = names.size(); i < imax; ++i )
			{
				String name = names.get( i );
				
				Value[] to = new Value[] { o.get( name ) };
				mergeInternal( to, options.get( name ) );
				o.set( name, to[0] );
			}
			break;
			
		default:
			target[0] = options;
			break;
		}
	}
}
