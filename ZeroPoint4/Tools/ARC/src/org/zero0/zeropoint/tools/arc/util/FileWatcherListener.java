package org.zero0.zeropoint.tools.arc.util;

public interface FileWatcherListener
{
	void fileChanged( String filePath );
	
	void fileAdded( String filePath );
}
