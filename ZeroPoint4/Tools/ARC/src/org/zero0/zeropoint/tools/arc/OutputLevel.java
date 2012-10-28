package org.zero0.zeropoint.tools.arc;

public enum OutputLevel {
	NORMAL( "Normal" ),
	VERBOSE( "Verbose" ),
	;
	
	private String name;
	
	private OutputLevel( String name ) {
		this.name = name;
	}
	
	final String getName() {
		return name;
	}
}
