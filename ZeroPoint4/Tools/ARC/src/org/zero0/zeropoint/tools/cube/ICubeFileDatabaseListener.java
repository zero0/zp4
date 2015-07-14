package org.zero0.zeropoint.tools.cube;

public interface ICubeFileDatabaseListener
{
	public void onFileAdded( String filename );
	
	public void onFileRemoved( String filename );
	
	public void onFileChanged( String filename );
	
	public void onFileMoved( String fromFilename, String toFilename );
}
