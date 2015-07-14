package org.zero0.zeropoint.tools.cube;

import java.io.File;
import java.io.IOException;

import org.zero0.json.Factory;
import org.zero0.json.Parser;
import org.zero0.json.Value;
import org.zero0.json.Writer;

public class CubeFileInfo implements Comparable< CubeFileInfo >
{
	private String filePath;
	private long fileSize;
	private long fileHash;
	private long lastModifiedDate;
	private Value meta;
	
	public CubeFileInfo()
	{
		filePath = "";
		fileSize = 0L;
		fileHash = 0L;
		lastModifiedDate = 0L;
		meta = Factory.Null;
	}
	
	public CubeFileInfo( String rootDir, File file )
	{
		this( rootDir, file, Factory.Null );
	}
	
	public CubeFileInfo( String rootDir, File file, Value metaData )
	{
		String path = "";
		try
		{
			path = file.getCanonicalPath();
		}
		catch( IOException e )
		{
			path = file.getAbsolutePath();
		}
		
		filePath = path.replace( rootDir, "" );
		
		fileSize = file.length();
		fileHash = file.hashCode();
		lastModifiedDate = file.lastModified();
		meta = metaData;
	}
	
	public CubeFileInfo( CubeFileInfo o )
	{
		filePath = o.filePath;
		fileSize = o.fileSize;
		fileHash = o.fileHash;
		lastModifiedDate = o.lastModifiedDate;
		meta = o.meta;
	}
	
	public CubeFileInfo( byte[] data )
	{
		String json = new String( data );
		Value v = Parser.parse( json );
		
		filePath = v.get( "filePath" ).asString();
		fileSize = v.get( "fileSize" ).asLong();
		fileHash = v.get( "fileHash" ).asLong();
		lastModifiedDate = v.get( "lastModifiedDate" ).asLong();
		meta = v.get( "meta" );
	}
	
	public String getFilePath()
	{
		return filePath;
	}
	
	public String getFilename()
	{
		return filePath.substring( filePath.lastIndexOf( File.separatorChar ) + 1 );
	}
	
	public String getFileExtension()
	{
		String filename = getFilename();
		return filename.substring( filename.lastIndexOf( '.' ) + 1 );
	}
	
	public long getFileSize()
	{
		return fileSize;
	}
	
	public long getFileHash()
	{
		return fileHash;
	}
	
	public long getFileLastModifiedDate()
	{
		return lastModifiedDate;
	}
	
	public void setFileLastModifiedDate( long lastModified )
	{
		lastModifiedDate = lastModified;
	} 
	
	public Value getMetaData()
	{
		return meta;
	}
	
	public void setMetaData( Value metaData )
	{
		meta = metaData;
	}
	
	public byte[] serialize()
	{
		Value info = Factory.createObject( null );
		info.set( "filePath", Factory.createString( filePath ) );
		info.set( "fileSize", Factory.createLong( fileSize ) );
		info.set( "fileHash", Factory.createLong( fileHash ) );
		info.set( "lastModifiedDate", Factory.createLong( lastModifiedDate ) );
		info.set( "meta", meta );
		
		String json = Writer.write( info );
		return json.getBytes();
	}

	@Override
	public int compareTo( CubeFileInfo o )
	{
		int cmp = filePath.compareTo( o.filePath );
		if( cmp == 0 )
		{
			cmp = (int)( fileHash - o.fileHash );
			if( cmp == 0 )
			{
				cmp = (int)( lastModifiedDate - o.lastModifiedDate );
			}
		}
		
		return cmp;
	}
}
