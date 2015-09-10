package org.zero0.json;

import java.util.List;

public final class Writer
{
	public static final String write( Value value )
	{
		return write( value, false, "" );
	}
	
	public static final String write( Value value, boolean prettyPrint, String tab )
	{
		StringBuilder sb = new StringBuilder();
		
		writeJson( sb, value, prettyPrint ? 0 : -1, tab );
		
		return sb.toString();
	}
	
	private static final void writeJson( StringBuilder sb, Value value, int indent, String tab )
	{
		ValueType type = value.getType();
		switch( type )
		{
		case Null:
			sb.append( "null" );
			break;
		case Boolean:
		case Int:
		case Long:
		case Float:
			sb.append( value.asString() );
			break;
		case String:
			sb.append( '"' ).append( value.asString().replace( "\\", "\\\\" ) ).append( '"' );
			break;
		case Array:
			sb.append( '[' );
			if( !value.isEmpty() )
			{
				writeNewLine( sb, indent );
				int ind = indent > -1 ? indent + 1 : indent;
				writeIndent( sb, ind, tab );
				writeJson( sb, value.get( 0 ), ind, tab );
				
				for( int i = 1; i < value.getSize(); ++i )
				{
					sb.append( ',' );
					writeNewLine( sb, ind );
					writeIndent( sb, ind, tab );
					writeJson( sb, value.get( i ), ind, tab );
				}
				
				writeNewLine( sb, indent );
				writeIndent( sb, indent, tab );
			}
			sb.append( ']' );
			break;
		case Object:
			sb.append( '{' );
			if( !value.isEmpty() )
			{
				writeNewLine( sb, indent );
				int ind = indent > -1 ? indent + 1 : indent;
				writeIndent( sb, ind, tab );
				
				List< String > memberNames = value.getMemberNames();
				
				sb.append( '"' ).append( memberNames.get( 0 ) ).append( '"' );
				
				writeObjectSeperator( sb, ind );
				
				writeJson( sb, value.get( memberNames.get( 0 ) ), ind, tab );
				
				for( int i = 1; i < memberNames.size(); ++i )
				{
					sb.append( ',' );
					writeNewLine( sb, ind );
					writeIndent( sb, ind, tab );
					
					sb.append( '"' ).append( memberNames.get( i ) ).append( '"' );
					
					writeObjectSeperator( sb, ind );
					
					writeJson( sb, value.get( memberNames.get( i ) ), ind, tab );
				}
				
				writeNewLine( sb, indent );
				writeIndent( sb, indent, tab );
			}
			sb.append( '}' );
			break;
		}
	}
	
	private static final void writeIndent( StringBuilder sb, int indent, String tab )
	{
		for( int i = 0; i < indent; ++i )
		{
			sb.append( tab );
		}
	}
	
	private static final void writeNewLine( StringBuilder sb, int indent )
	{
		if( indent > -1 )
		{
			sb.append( '\n' );
		}
	}
	
	private static final void writeObjectSeperator( StringBuilder sb, int indent )
	{
		if( indent < 0 )
		{
			sb.append( ':' );
		}
		else
		{
			sb.append( " : " );
		}
	}
}
