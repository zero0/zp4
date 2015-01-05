package org.zero0.zeropoint.tools.arc.util;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class FileWatcher implements Runnable, FilenameFilter
{
	private final Object lock = new Object();

	private Map< String, Long > filesToWatch;
	private List< FileWatcherListener > listeners;
	private List< String > includeExtensions;
	private List< String > filesChanged;
	private List< String > filesAdded;
	private List< String > filesRemoved;

	private File rootDir;
	private String rootPath;
	private Thread thread;

	private boolean isRunning;
	private boolean isEnabled;
	private long delay;

	public FileWatcher()
	{
		thread = null;
		filesToWatch = new HashMap< String, Long >();
		listeners = new ArrayList< FileWatcherListener >();
		includeExtensions = new ArrayList< String >();
		filesChanged = new ArrayList< String >();
		filesAdded = new ArrayList< String >();
		filesRemoved = new ArrayList< String >();

		isRunning = false;
		isEnabled = false;
		delay = 1000;
	}
	
	public void setDelay( long delay )
	{
		this.delay = delay;
		synchronized( lock )
		{
			lock.notify();
		}
	}
	public void setRootDir( String root )
	{
		rootDir = new File( root );
		rootPath = getStandardPathForFile( rootDir );
	}
	
	public String getRootDir()
	{
		return rootPath;
	}

	public void start()
	{
		isRunning = true;
		if( thread == null )
		{
			thread = new Thread( this );
			thread.start();
		}
	}
	public void clean( boolean full )
	{
		synchronized( filesToWatch )
		{
			filesToWatch.clear();
		}
	}
	
	public long getFileModificationTime( String filename )
	{
		Long modTime = filesToWatch.get( filename );
		return modTime == null ? -1 : modTime.longValue();
	}
	
	public List< String > getFiles()
	{
		List< String > allFiles = null;
		synchronized( filesToWatch )
		{
			allFiles = Collections.unmodifiableList( new ArrayList< String >( filesToWatch.keySet() ) );
		}

		return allFiles;
	}
	
	public int getNumFiles()
	{
		int size = 0;
		synchronized( filesToWatch )
		{
			size = filesToWatch.size();
		}
		
		return size;
	}

	public void addAcceptedFileExtension( String extension )
	{
		includeExtensions.add( "." + extension );
	}
	
	public void addAccaptedFileExtensions( String ... extensions )
	{
		for( String ext : extensions )
		{
			includeExtensions.add( "." + ext );
		}
	}

	public void addListener( FileWatcherListener listener )
	{
		listeners.add( listener );
	}

	public void removeListener( FileWatcherListener listener )
	{
		listeners.remove( listener );
	}

	public void removeAllListeners()
	{
		listeners.clear();
	}

	private void fireChangeListeners( String filePath )
	{
		for( FileWatcherListener l : listeners )
		{
			l.fileChanged( filePath );
		}
	}

	private void fireAddListeners( String filePath )
	{
		for( FileWatcherListener l : listeners )
		{
			l.fileAdded( filePath );
		}
	}
	
	private void fireRemovedListeners( String filePath )
	{
		for( FileWatcherListener l : listeners )
		{
			l.fileRemoved( filePath );
		}
	}
	
	private void fireFileListChanged()
	{
		for( FileWatcherListener l : listeners )
		{
			l.fileListChanged();
		}
	}
	
	public void setIsEnabled( boolean enabled )
	{
		isEnabled = enabled;
		synchronized( lock )
		{
			lock.notify();
		}
	}

	public boolean getIsEnabled()
	{
		return isEnabled;
	}

	public void shutdown()
	{
		isRunning = false;
		synchronized( lock )
		{
			lock.notify();
		}
	}

	private void checkUpdateFiles()
	{
		for( Map.Entry< String, Long > e : filesToWatch.entrySet() )
		{
			long lastModified = e.getValue();
			long currentModified = new File( e.getKey() ).lastModified();
			if( lastModified != currentModified )
			{
				e.setValue( currentModified );
				filesChanged.add( e.getKey() );
			}
		}
	}
	
	private void checkForAddedRemovedFiles( File file )
	{
		List< String > trackedFiles = new ArrayList< String >( filesToWatch.keySet() );
		List< String > allFilePaths = new ArrayList< String >();

		// get all the files in the directory
		getListOfAcceptableFiles( file, allFilePaths );
		
		// unknown files found, should add
		if( trackedFiles.size() < allFilePaths.size() )
		{
			allFilePaths.removeAll( trackedFiles );
			for( String path : allFilePaths )
			{
				filesToWatch.put( path, new File( path ).lastModified() );
				filesAdded.add( path );
			}
		}
		// files removed, should remove
		else if( trackedFiles.size() > allFilePaths.size() )
		{
			trackedFiles.removeAll( allFilePaths );
			for( String path : trackedFiles )
			{
				filesToWatch.remove( path );
				filesRemoved.add( path );
			}
		}
	}
	
	private void getListOfAcceptableFiles( File file, List< String > filePaths )
	{
		if( file.isDirectory() )
		{
			for( File f : file.listFiles( this ) )
			{
				getListOfAcceptableFiles( f, filePaths );
			}
		}
		else
		{
			filePaths.add( getStandardPathForFile( file ) );
		}
	}
	
	public String getStandardPathForFile( File file )
	{
		String path;
		try
		{
			path = file.getCanonicalPath();
		}
		catch( Exception e )
		{
			path = file.getAbsolutePath();
		}
		return path;
	}

	@Override
	public void run()
	{
		while( isRunning )
		{
			try
			{
				synchronized( lock )
				{
					lock.wait( delay );
				}
			}
			catch( InterruptedException e )
			{
			}
			if( !isRunning ) break;

			// lock around files to watch
			synchronized( filesToWatch )
			{
				// check for file updates
				checkUpdateFiles();
	
				// check for new or removed files
				checkForAddedRemovedFiles( rootDir );
			}
			
			// file changed
			if( !filesChanged.isEmpty() )
			{
				if( isEnabled )
				{
					for( String path : filesChanged )
					{
						fireChangeListeners( path );
					}
				}
				filesChanged.clear();
			}
			
			// file added
			if( !filesAdded.isEmpty() )
			{
				if( isEnabled )
				{
					for( String path : filesAdded )
					{
						fireAddListeners( path );
					}
				}
				filesAdded.clear();
				
				// file list changed
				fireFileListChanged();
			}
			
			// file removed
			if( !filesRemoved.isEmpty() )
			{
				if( isEnabled )
				{
					for( String path : filesRemoved )
					{
						fireRemovedListeners( path );
					}
				}
				filesRemoved.clear();

				// file list changed
				fireFileListChanged();
			}
		}
	}

	@Override
	public boolean accept( File dir, String name )
	{
		if( new File( dir, name ).isDirectory() )
		{
			return true;
		}
		else
		{
			for( String ext : includeExtensions )
			{
				if( name.endsWith( ext ) )
				{
					return true;
				}
			}
		}
		return false;
	}
}
