package org.zero0.zeropoint.tools.arc;

public enum Platform {
    WIN32( "Win32" ), WIN64( "Win64" );

    private String name;

    private Platform( String name ) {
	this.name = name;
    }

    final String getName() {
	return name;
    }
}
