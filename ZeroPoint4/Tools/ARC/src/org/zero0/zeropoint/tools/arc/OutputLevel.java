package org.zero0.zeropoint.tools.arc;

public enum OutputLevel {
    Compact(1), Normal(2), Verbos(3);
    
    public final int level;
    
    private OutputLevel( int level ) {
	this.level = level;
    }
}
