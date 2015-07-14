package org.zero0.zeropoint.tools.cube.impl;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import jdbm.RecordManager;
import jdbm.RecordManagerFactory;
import jdbm.helper.Serializer;

import org.zero0.zeropoint.tools.cube.CubeFileInfo;
import org.zero0.zeropoint.tools.cube.ICubeFileDatabase;
import org.zero0.zeropoint.tools.cube.ICubeFileDatabaseListener;

public class JdbmFileDatabase implements ICubeFileDatabase, Serializer
{
	private static final long serialVersionUID = -5227595887498956509L;
	
	private final static String DatabaseDir = "db/";
	private final static String FileDatabaseName = DatabaseDir + "files";
	
	private final static String FileNamespace = "file:";
	
	private RecordManager fileDatabase = null;
	
	private List< ICubeFileDatabaseListener > listeners;

	@Override
	public void initialize()
	{
		try
		{
			fileDatabase = RecordManagerFactory.createRecordManager( FileDatabaseName );
		}
		catch( IOException e )
		{
			e.printStackTrace();
		}
	}

	@Override
	public void shutdown()
	{
		try
		{
			fileDatabase.close();
			fileDatabase = null;
		}
		catch( IOException e )
		{
			e.printStackTrace();
		}
	}

	@Override
	public void setup()
	{
		listeners = new ArrayList< ICubeFileDatabaseListener >();
	}

	@Override
	public void teardown()
	{
		clearFileListeners();
	}

	@Override
	public void addFileListener( ICubeFileDatabaseListener listener )
	{
		listeners.add( listener );
	}

	@Override
	public void removeFileListener( ICubeFileDatabaseListener listener )
	{
		listeners.remove( listener );
	}

	@Override
	public void clearFileListeners()
	{
		listeners.clear();
	}

	@Override
	public CubeFileInfo getFileData( String filename )
	{
		CubeFileInfo info = null;
		
		try
		{
			long id = fileDatabase.getNamedObject( FileNamespace + filename );
			if( id > 0L )
			{
				info = (CubeFileInfo)fileDatabase.fetch( id, this );
			}
		}
		catch( IOException e )
		{
			e.printStackTrace();
		}
		
		return info;
	}

	@Override
	public void updateFileData( String filename, CubeFileInfo data )
	{
		try
		{
			long id = fileDatabase.getNamedObject( FileNamespace + filename );
			if( id == 0L )
			{
				id = fileDatabase.insert( data, this );
				fileDatabase.setNamedObject( FileNamespace + filename, id );
				
				fireFileAdded( filename );
			}
			else
			{
				fileDatabase.update( id, data, this );
				
				fireFileChanged( filename );
			}
		}
		catch( IOException e )
		{
			e.printStackTrace();
		}
	}

	@Override
	public void moveFile( String fromFilename, String toFilename )
	{
	}
	
	@Override
	public void deleteFile( String filename )
	{
		try
		{
			long id = fileDatabase.getNamedObject( FileNamespace + filename );
			if( id > 0L )
			{
				fileDatabase.delete( id );
				
				fireFileRemoved( filename );
			}
		}
		catch( IOException e )
		{
			e.printStackTrace();
		}
	}

	@Override
	public Object deserialize( byte[] buffer ) throws IOException
	{
		CubeFileInfo info = new CubeFileInfo( buffer );
		return info;
	}

	@Override
	public byte[] serialize( Object fileinfo ) throws IOException
	{
		CubeFileInfo info = (CubeFileInfo)fileinfo;
		return info.serialize();
	}
	
	private void fireFileAdded( String filename )
	{
		for( ICubeFileDatabaseListener l : listeners )
		{
			l.onFileAdded( filename );
		}
	}
	
	private void fireFileChanged( String filename )
	{
		for( ICubeFileDatabaseListener l : listeners )
		{
			l.onFileChanged( filename );
		}
	}
	
	private void fireFileMoved( String fromFilename, String toFilename )
	{
		for( ICubeFileDatabaseListener l : listeners )
		{
			l.onFileMoved( fromFilename, toFilename );
		}
	}
	
	private void fireFileRemoved( String filename )
	{
		for( ICubeFileDatabaseListener l : listeners )
		{
			l.onFileRemoved( filename );
		}
	}
}
