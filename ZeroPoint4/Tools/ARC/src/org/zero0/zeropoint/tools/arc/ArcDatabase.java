package org.zero0.zeropoint.tools.arc;

import java.io.File;
import java.io.FileInputStream;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Properties;

import org.hsqldb.server.Server;

public class ArcDatabase
{
	private static final String dbSubProperties = "arc.db";
	private static final String dbName = "name";
	private static final String dbFile = "file";
	private static final String dbQueries = "queries";

	private static final String dbDropTables = "db.drop_tables";

	private static final String dbCreateTableFile = "db.create_table_file";
	private static final String dbCreateTableReference = "db.create_table_ref";
	private static final String dbClearTableFile = "db.clear_table_file";
	private static final String dbClearTableReference = "db.clear_table_ref";

	//private static final String dbAddFile = "db.add_file";
	private static final String dbMergeFile = "db.merge_file";

	private static final String dbLinkFiles = "db.link_files";

	private static final String dbListFiles = "db.list_files";

	private static final String dbGetFiles = "db.get_files";

	private static final String dbGetReferencedFrom = "db.get_referenced_from";

	//private static final String dbGetReferencedFromAll = "db.get_referenced_from_all";

	Properties properties;
	Properties queries;
	Connection connection;
	Server server;

	public ArcDatabase( Properties properties )
	{
		this.properties = Arc.getSubProperties( properties, dbSubProperties );
		this.queries = new Properties();
	}

	public void initialize()
	{
		try
		{
			server = new Server();
			server.setLogWriter( null );
			server.setSilent( true );
			server.setDatabaseName( 0, properties.getProperty( dbName ) );
			server.setDatabasePath( 0, "file:" + properties.getProperty( dbFile ) );
			server.start();

			Class.forName( "org.hsqldb.jdbcDriver" );
			connection = DriverManager.getConnection( "jdbc:hsqldb:hsql://localhost/" + properties.getProperty( dbName ), "SA", "" );
		}
		catch( Exception e )
		{
			Arc.getInstance().err( e.getMessage() );
		}

		try
		{
			queries.load( new FileInputStream( properties.getProperty( dbQueries ) ) );
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}
		
		createTableStructure();
	}

	void createTableStructure()
	{
		if( connection == null ) return;

		try
		{
			connection.prepareStatement( queries.getProperty( dbCreateTableFile ) ).execute();
			connection.prepareStatement( queries.getProperty( dbCreateTableReference ) ).execute();
		}
		catch( SQLException e )
		{
			Arc.getInstance().err( e.getMessage() );
		}
	}

	public void clean( boolean full )
	{
		try
		{
			if( full )
			{
				connection.prepareStatement( queries.getProperty( dbDropTables ) ).execute();
			}
			else
			{
				connection.prepareStatement( queries.getProperty( dbClearTableFile ) ).execute();
				connection.prepareStatement( queries.getProperty( dbClearTableReference ) ).execute();
			}
		}
		catch( SQLException e )
		{
			Arc.getInstance().err( e.getMessage() );
		}
	}

	public void shutdown()
	{
		try
		{
			connection.close();
			server.stop();
		}
		catch( SQLException e )
		{
			Arc.getInstance().err( e.getMessage() );
		}
	}
	
	public boolean filesAdded( String... files )
	{
		try
		{
			connection.setAutoCommit( false );
			PreparedStatement add = connection.prepareStatement( queries.getProperty( dbMergeFile ) );

			for( String file : files )
			{
				int hash = new File( file ).hashCode();
				add.setString( 1, file );
				add.setInt( 2, hash );
				add.setString( 3, file );
				add.setInt( 4, hash );
				add.executeUpdate();
			}

			connection.commit();
		}
		catch( SQLException e )
		{
			e.printStackTrace();
			return false;
		}
		finally
		{
			try
			{
				connection.setAutoCommit( true );
			}
			catch( SQLException e )
			{
				e.printStackTrace();
				return false;
			}
		}
		return true;
	}

	public boolean filesChanged( String... files )
	{
		try
		{
			connection.setAutoCommit( false );
			PreparedStatement add = connection.prepareStatement( queries.getProperty( dbMergeFile ) );

			for( String file : files )
			{
				int hash = new File( file ).hashCode();
				add.setString( 1, file );
				add.setInt( 2, hash );
				add.setString( 3, file );
				add.setInt( 4, hash );
				add.executeUpdate();
			}

			connection.commit();
		}
		catch( SQLException e )
		{
			e.printStackTrace();
			return false;
		}
		finally
		{
			try
			{
				connection.setAutoCommit( true );
			}
			catch( SQLException e )
			{
				e.printStackTrace();
				return false;
			}
		}
		return true;
	}

	public boolean removeFiles( String... file )
	{
		// connection.prepareStatement( "" );
		return true;
	}

	public void linkFiles( String srcFile, String refFile )
	{
		if( srcFile.equals( refFile ) ) return;

		try
		{
			PreparedStatement link = connection.prepareStatement( queries.getProperty( dbLinkFiles ) );
			link.setString( 1, srcFile );
			link.setString( 2, refFile );
			link.executeUpdate();
		}
		catch( SQLException e )
		{
			e.printStackTrace();
		}
	}

	public List<String> getAllFiles()
	{
		List<String> files = new ArrayList<String>();

		try
		{
			ResultSet result = connection.prepareStatement( queries.getProperty( dbGetFiles ) ).executeQuery();
			while( result.next() )
			{
				files.add( result.getString( 1 ) );
			}
		}
		catch( SQLException e )
		{
			e.printStackTrace();
		}

		return files;
	}

	public List<String> getFilesReferencedFrom( String srcFile, boolean directReference )
	{
		List<String> files = new ArrayList<String>();
		files.add( srcFile );

		try
		{
			PreparedStatement statement = connection.prepareStatement( queries.getProperty( dbGetReferencedFrom ) );
			statement.setString( 1, srcFile );
			ResultSet result = statement.executeQuery();
			while( result.next() )
			{
				files.add( result.getString( 1 ) );
			}
		}
		catch( SQLException e )
		{
			e.printStackTrace();
		}

		return files;
	}

	public void listFiles()
	{
		try
		{
			ResultSet result = connection.prepareStatement( queries.getProperty( dbListFiles ) ).executeQuery();
			StringBuilder sb = new StringBuilder();
			while( result.next() )
			{
				sb.append( result.getString( 2 ) );
				sb.append( '\n' );
			}
			Arc.getInstance().out( OutputLevel.Normal, sb.toString() );
		}
		catch( SQLException e )
		{
			e.printStackTrace();
		}
	}

	public List<String[]> execQuery( String query )
	{
		List<String[]> results = new LinkedList<String[]>();

		try
		{
			ResultSet result = connection.prepareStatement( query ).executeQuery();
			ResultSetMetaData meta = result.getMetaData();
			int count = meta.getColumnCount();

			if( count > 0 )
			{
				String[] columns = new String[ count ];
				for( int i = 0; i < count; ++i )
				{
					columns[i] = meta.getColumnLabel( i + 1 );
				}
				results.add( columns );
			}

			while( result.next() )
			{
				String[] row = new String[ count ];
				for( int i = 0; i < count; ++i )
				{
					row[i] = result.getString( i + 1 );
				}
				results.add( row );
			}
		}
		catch( SQLException e )
		{
			e.printStackTrace();
		}

		return results;
	}
}
