package org.zero0.zeropoint.tools.cube.util;

public interface FileWatcherListener
{
	void fileChanged( String filePath );
	
	void fileAdded( String filePath );
	
	void fileRemoved( String filePath );
	
	void fileListChanged();
}
