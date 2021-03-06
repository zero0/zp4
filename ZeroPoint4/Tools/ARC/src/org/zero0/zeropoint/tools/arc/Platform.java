package org.zero0.zeropoint.tools.arc;

public enum Platform
{
	Win32( "x86" ),
	Win64( "x64" ),
	Mac( "Mac" ),
	Android( "Android" ),
	iOS( "iOS" ),
	;
	
	private final String arch;
	
	private Platform( String a )
	{
		arch = a;
	}
	
	public final String getArch()
	{
		return arch;
	}
}
