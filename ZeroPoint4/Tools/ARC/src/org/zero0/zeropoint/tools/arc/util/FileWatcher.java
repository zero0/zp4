package org.zero0.zeropoint.tools.arc.util;

import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
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

	File rootDir;
	String rootPath;
	Thread thread;

	boolean isRunning;
	boolean isEnabled;
	long delay;

	public FileWatcher( String root )
	{
		thread = null;
		filesToWatch = new HashMap<String, Long>();
		listeners = new ArrayList<FileWatcherListener>();
		includeExtensions = new ArrayList<String>();

		isRunning = true;
		isEnabled = false;
		delay = 1000;

		rootDir = new File( root );
		try
		{
			rootPath = rootDir.getCanonicalPath();
		}
		catch( IOException e )
		{
			rootPath = rootDir.getAbsolutePath();
		}
	}

	public void start()
	{
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
	
	public Set<String> getFiles()
	{
		return Collections.unmodifiableSet( filesToWatch.keySet() );
	}

	public void addFile( File file )
	{
		synchronized( lock )
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
				filesToWatch.put( getStandardPathForFile( file ), file.lastModified() );
			}
		}
	}

	public void addAcceptedFileExtension( String extension )
	{
		includeExtensions.add( extension );
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
				fireChangeListeners( e.getKey() );
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
				fireAddListeners( path );
			}
		}
	}
	
	public String getStandardPathForFile( File file )
	{
		String path = "";
		try
		{
			path = file.getCanonicalPath();
			
			path = path.substring( rootPath.length() + 1 );
		}
		catch( Exception e )
		{
			e.printStackTrace();
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
					checkUpdateFiles();

					checkNewFiles( rootDir );
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
