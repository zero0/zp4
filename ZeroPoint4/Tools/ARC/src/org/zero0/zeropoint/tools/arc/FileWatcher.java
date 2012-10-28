package org.zero0.zeropoint.tools.arc;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.swing.filechooser.FileNameExtensionFilter;

public class FileWatcher implements Runnable, FilenameFilter {

	Object lock = new Object();
	
	Map<File, Long> filesToWatch;
	List<FileWatcherListener> listeners;
	List<String> includeExtensions;

	File rootDir;
	Thread thread;

	boolean isRunning;
	boolean isEnabled;
	long delay;

	public FileWatcher( String rootPath ) {
		filesToWatch = new HashMap<File, Long>();
		listeners = new ArrayList<FileWatcherListener>();
		includeExtensions = new ArrayList<String>();

		isRunning = true;
		isEnabled = true;
		delay = 1000;

		rootDir = new File( rootPath );

		thread = new Thread( this );
		thread.start();
	}

	public Set<File> getFiles() {
		return Collections.unmodifiableSet( filesToWatch.keySet() );
	}

	public synchronized void addFile( File file ) {
		if( file.isDirectory() ) {
			for( File f : file.listFiles( this ) ) {
				addFile( f );
			}
		} else {
			filesToWatch.put( file, file.lastModified() );
		}
	}

	public void addAcceptedFileExtension( String extension ) {
		includeExtensions.add( extension );
	}
	
	public void addListener( FileWatcherListener listener ) {
		listeners.add( listener );
	}

	void fireListeners( String filePath ) {
		for( FileWatcherListener l : listeners ) {
			l.fileChanged( filePath );
		}
	}

	public void setIsEnabled( boolean enabled ) {
		isEnabled = enabled;
	}

	public boolean getIsEnabled() {
		return isEnabled;
	}

	public void shutdown() {
		isRunning = false;
		synchronized( lock ) {
			lock.notify();
		}
	}

	void checkUpdateFiles() {
		for( Map.Entry<File, Long> e : filesToWatch.entrySet() ) {
			long lastModified = e.getValue();
			long currentModified = e.getKey().lastModified();
			if( lastModified != currentModified ) {
				e.setValue( currentModified );
				fireListeners( e.getKey().getName() );
			}
		}
	}

	void checkNewFiles( File file ) {
		if( file.isDirectory() ) {
			for( File f : file.listFiles( this ) ) {
				checkNewFiles( f );
			}
		} else {
			if( !filesToWatch.containsKey( file ) ) {
				filesToWatch.put( file, file.lastModified() );
				fireListeners( file.getName() );
			}
		}
	}


	@Override
	public void run() {
		while( isRunning ) {
			try {
				synchronized( lock ) {
					lock.wait( delay );
				}
			} catch( InterruptedException e ) {
			}
			if( !isRunning )
				break;

			if( isEnabled ) {
				synchronized( this ) {
					checkUpdateFiles();

					checkNewFiles( rootDir );
				}
			}
		}
	}

	@Override
	public boolean accept( File dir, String name ) {
		if( new File( dir, name ).isDirectory() ) {
			return true;
		} else {
			for( String ext : includeExtensions ) {
				if( name.endsWith( ext ) ) {
					return true;
				}
			}
		}
		return false;
	}
}
