package org.zero0.zeropoint.tools.arc.util;

public abstract class FileWatcherAdapter implements FileWatcherListener
{
	@Override
	public void fileChanged( String filePath ) {}

	@Override
	public void fileAdded( String filePath ) {}

	@Override
	public void fileRemoved( String filePath ) {}

	@Override
	public void fileListChanged() {}
}
