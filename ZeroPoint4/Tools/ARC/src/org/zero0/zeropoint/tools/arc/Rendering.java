package org.zero0.zeropoint.tools.arc;

public enum Rendering {
	DIRECTX11( "DX11" ),
	OPENGL( "GL" )
	;
	
	private String name;
	
	private Rendering( String name ) {
		this.name = name;
	}
	
	final String getName() {
		return name;
	}
}
