package org.zero0.zeropoint.tools.cube;


public interface ICubeFileDatabase
{
	public void initialize();
	public void shutdown();
	
	public void setup();
	public void teardown();
	
	public void addFileListener( ICubeFileDatabaseListener listener );
	public void removeFileListener( ICubeFileDatabaseListener listener );
	public void clearFileListeners();
	
	public CubeFileInfo getFileData( String filename );
	public void updateFileData( String filename, CubeFileInfo data );
	public void moveFile( String fromFilename, String toFilename );
	public void deleteFile( String filename );
}
