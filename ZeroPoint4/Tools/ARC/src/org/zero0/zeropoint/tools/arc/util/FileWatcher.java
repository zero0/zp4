package org.zero0.zeropoint.tools.arc.util;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;


public class FileWatcher implements Runnable, FilenameFilter
{
	final Object lock = new Object();

	Map<String, Long> filesToWatch;
	List<FileWatcherListener> listeners;
	List<String> includeExtensions;
	List<String> filesChanged;
	List<String> filesAdded;

	File rootDir;
	String rootPath;
	Thread thread;

	boolean isRunning;
	boolean isEnabled;
	long delay;

	public FileWatcher()
	{
		thread = null;
		filesToWatch = new HashMap<String, Long>();
		listeners = new ArrayList<FileWatcherListener>();
		includeExtensions = new ArrayList<String>();
		filesChanged = new ArrayList<String>();
		filesAdded = new ArrayList<String>();

		isRunning = false;
		isEnabled = false;
		delay = 1000;
	}
	
	public void setRootDir( String root )
	{
		rootDir = new File( root );
		rootPath = root;
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
		filesToWatch.clear();
	}
	
	public long getFileModificationTime( String filename )
	{
		Long modTime = filesToWatch.get( filename );
		return modTime == null ? -1 : modTime.longValue();
	}
	
	public List<String> getFiles()
	{
		return Collections.unmodifiableList( new ArrayList<String>( filesToWatch.keySet() ) );
	}
	
	public int getNumFiles()
	{
		return filesToWatch.size();
	}

	public void addFile( File file )
	{
		//synchronized( lock )
		{
			if( file.isDirectory() )
			{
				for( File f : file.listFiles( this ) )
				{
					addFile( f );
				}
			}
			else
			{
				//filesToWatch.put( getStandardPathForFile( file ), file.lastModified() );
				filesAdded.add( getStandardPathForFile( file ) );
			}
		}
	}

	public void addAcceptedFileExtension( String extension )
	{
		includeExtensions.add( "." + extension );
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

	void fireChangeListeners( String filePath )
	{
		for( FileWatcherListener l : listeners )
		{
			l.fileChanged( filePath );
		}
	}

	void fireAddListeners( String filePath )
	{
		for( FileWatcherListener l : listeners )
		{
			l.fileAdded( filePath );
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

	void checkUpdateFiles()
	{
		for( Map.Entry<String, Long> e : filesToWatch.entrySet() )
		{
			long lastModified = e.getValue();
			long currentModified = new File( e.getKey() ).lastModified();
			if( lastModified != currentModified )
			{
				e.setValue( currentModified );
				//fireChangeListeners( e.getKey() );
				filesChanged.add( e.getKey() );
			}
		}
	}

	void checkNewFiles( File file )
	{
		if( file.isDirectory() )
		{
			for( File f : file.listFiles( this ) )
			{
				checkNewFiles( f );
			}
		}
		else
		{
			String path = getStandardPathForFile( file );
			if( !filesToWatch.containsKey( path ) )
			{
				filesToWatch.put( path, file.lastModified() );
				//fireAddListeners( path );
				filesAdded.add( path );
			}
		}
	}
	
	void checkRemovedFiles( File file )
	{
		// get all the files in the directory
		List<String> filePaths = new ArrayList<String>();
		getListOfAcceptableFiles( file, filePaths );
		
		// remove all the tracked files from the ones in the path
		Set<String> trackedFiles = filesToWatch.keySet();
		if( trackedFiles.removeAll( filePaths ) )
		{
			// if there are any paths left, remove them from the tracked files
			for( String s : filePaths )
			{
				filesToWatch.remove( s );
			}
		}
	}
	
	private void getListOfAcceptableFiles( File file, List<String> filePaths )
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
		String path = "";
		try
		{
			path = file.getCanonicalPath();
			
			//path = path.substring( rootPath.length() );
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

			if( isEnabled )
			{
				synchronized( lock )
				{
					// check for file updates
					checkUpdateFiles();

					// check for new files added
					checkNewFiles( rootDir );
					
					// check for removed files
					//checkRemovedFiles( rootDir );
				}
				
				if( !filesChanged.isEmpty() )
				{
					for( String path : filesChanged )
					{
						fireChangeListeners( path );
					}
					filesChanged.clear();
				}
				
				if( !filesAdded.isEmpty() )
				{
					for( String path : filesAdded )
					{
						fireAddListeners( path );
					}
					filesAdded.clear();
				}
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
