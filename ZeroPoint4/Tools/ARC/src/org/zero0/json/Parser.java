package org.zero0.json;

import java.util.ArrayList;
import java.util.List;

public final class Parser
{
	private enum ParserTokenType
	{
		ZP_JSON_TOKEN_EOF,
		ZP_JSON_TOKEN_OBJECT_OPEN,
		ZP_JSON_TOKEN_OBJECT_CLOSE,
		ZP_JSON_TOKEN_ARRAY_OPEN,
		ZP_JSON_TOKEN_ARRAY_CLOSE,
		ZP_JSON_TOKEN_DATA_OPEN,
		ZP_JSON_TOKEN_DATA_CLOSE,
		ZP_JSON_TOKEN_STRING,
		ZP_JSON_TOKEN_NUMBER,
		ZP_JSON_TOKEN_TRUE,
		ZP_JSON_TOKEN_FALSE,
		ZP_JSON_TOKEN_NULL,
		ZP_JSON_TOKEN_MEMBER_SEP,
		ZP_JSON_TOKEN_ARRAY_SEP,
		ZP_JSON_TOKEN_COMMENT,
		ZP_JSON_TOKEN_ERROR,
	};
	
	private static final class ParserToken
	{
		public int start;
		public int end;
		public ParserTokenType type;
	}
	
	private static final class ParserImpl
	{
		private String json;
		
		private int current;
		private int end;
		
		private List< Value > nodes;
		
		public Value parse( String jsonString )
		{
			Value value = Factory.Null;
			
			if( jsonString != null && !jsonString.isEmpty() )
			{
				json = jsonString;
				
				current = 0;
				end = json.length();
				
				nodes = new ArrayList< Value >();

				pushNewValue( Factory.Null );
				
				boolean success = readValue();
				if( !success )
				{
					return Factory.Null;
				}
				
				value = popValue();
			}
			
			return value;
		}
		
		private void skipWhitespace()
		{
			for( ; current != end && Character.isWhitespace( json.charAt( current ) ); ++current );
		}
		
		private char nextChar()
		{
			return json.charAt( current++ );
		}
		
		private boolean matches( String pattern )
		{
			int length = pattern.length();
			if( ( end - current ) < length )
			{
				return false;
			}
			
			int index = length;
			while( index-- != 0 )
			{
				if( json.charAt( current + index ) != pattern.charAt( index ) )
				{
					return false;
				}
			}
			
			current += length;
			return true;
		}
		
		private boolean readToken( ParserToken token )
		{
			boolean ok = true;
			skipWhitespace();
			
			token.start = current;
			char c = nextChar();
			
			switch( c ) {
			case '{':
				token.type = ParserTokenType.ZP_JSON_TOKEN_OBJECT_OPEN;
				break;
			case '}':
				token.type = ParserTokenType.ZP_JSON_TOKEN_OBJECT_CLOSE;
				break;
			case '[':
				token.type = ParserTokenType.ZP_JSON_TOKEN_ARRAY_OPEN;
				break;
			case ']':
				token.type = ParserTokenType.ZP_JSON_TOKEN_ARRAY_CLOSE;
				break;
			case '"':
				token.type = ParserTokenType.ZP_JSON_TOKEN_STRING;
				readString();
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
				token.type = ParserTokenType.ZP_JSON_TOKEN_NUMBER;
				readNumber();
				break;
			case 't':
				token.type = ParserTokenType.ZP_JSON_TOKEN_TRUE;
				ok = matches( "rue" );
				break;
			case 'f':
				token.type = ParserTokenType.ZP_JSON_TOKEN_FALSE;
				ok = matches( "alse" );
				break;
			case 'n':
				token.type = ParserTokenType.ZP_JSON_TOKEN_NULL;
				ok = matches( "ull" );
				break;
			case ':':
				token.type = ParserTokenType.ZP_JSON_TOKEN_MEMBER_SEP;
				break;
			case ',':
				token.type = ParserTokenType.ZP_JSON_TOKEN_ARRAY_SEP;
				break;
			case '\0':
				token.type = ParserTokenType.ZP_JSON_TOKEN_EOF;
				break;
			default:
				ok = false;
				break;
			}
			
			if( !ok )
			{
				token.type = ParserTokenType.ZP_JSON_TOKEN_ERROR;
			}
			token.end = current;
			
			return true;
		}
		
		private boolean readValue()
		{
			ParserToken token = new ParserToken();
			readToken( token );
			
			boolean ok = true;
			switch( token.type )
			{
			case ZP_JSON_TOKEN_OBJECT_OPEN:
				ok = readObject();
				break;
			case ZP_JSON_TOKEN_ARRAY_OPEN:
				ok = readArray();
				break;
			case ZP_JSON_TOKEN_STRING:
				StringBuilder str = new StringBuilder();
				tokenToString( token, str );
				setCurrentValue( Factory.createString( str.toString() ) );
				break;
			case ZP_JSON_TOKEN_NUMBER:
				tokenToNumber( token );
				break;
			case ZP_JSON_TOKEN_TRUE:
				setCurrentValue( Factory.createBoolean( true ) );
				break;
			case ZP_JSON_TOKEN_FALSE:
				setCurrentValue( Factory.createBoolean( false ) );
				break;
			case ZP_JSON_TOKEN_NULL:
				setCurrentValue( Factory.Null );
				break;
			}
			
			return ok;
		}
		
		private void pushNewValue( Value jsonValue )
		{
			nodes.add( jsonValue );
		}
		
		private Value popValue()
		{
			return nodes.remove( nodes.size() - 1 );
		}
		
		private void setCurrentValue( Value jsonValue )
		{
			nodes.set( nodes.size() - 1, jsonValue );
		}
		
		private Value getCurrentValue()
		{
			return nodes.get( nodes.size() - 1 );
		}
		
		private boolean readObject()
		{
			setCurrentValue( Factory.createObject( null ) );
			
			ParserToken token = new ParserToken();
			ParserToken colon = new ParserToken();
			ParserToken comma = new ParserToken();
			StringBuilder name = new StringBuilder();
			
			while( readToken( token ) )
			{
				if( token.type == ParserTokenType.ZP_JSON_TOKEN_ARRAY_CLOSE && name.length() == 0 )
				{
					return true;
				}
				
				if( token.type != ParserTokenType.ZP_JSON_TOKEN_STRING )
				{
					break;
				}
				
				name.replace( 0, name.length(), "" );
				tokenToString( token, name );
				
				if( !readToken( colon ) || colon.type != ParserTokenType.ZP_JSON_TOKEN_MEMBER_SEP )
				{
					return false;
				}
				
				String n = name.toString();
				Value v = getCurrentValue().get( n );
				
				pushNewValue( v );
				boolean ok = readValue();
				
				v = popValue();
				getCurrentValue().set( n, v );
				
				if( !ok )
				{
					return false;
				}
				
				if( !readToken( comma ) || ( comma.type != ParserTokenType.ZP_JSON_TOKEN_OBJECT_CLOSE && comma.type != ParserTokenType.ZP_JSON_TOKEN_ARRAY_SEP ) )
				{
					return false;
				}
				
				if( comma.type == ParserTokenType.ZP_JSON_TOKEN_OBJECT_CLOSE )
				{
					return true;
				}
			}
			
			return false;
		}
		
		private boolean readArray()
		{
			setCurrentValue( Factory.createArray( null ) );
			skipWhitespace();
			
			if( json.charAt( current ) == ']' )
			{
				readToken( new ParserToken() );
				return true;
			}
			
			ParserToken token = new ParserToken();
			int index = 0;
			boolean isDone = false;
			while( !isDone )
			{
				Value v = getCurrentValue().get( index );
				pushNewValue( v );
				boolean ok = readValue();
				v = popValue();
				getCurrentValue().set( index, v );
				
				++index;
				
				if( !ok )
				{
					return false;
				}
				
				ok = readToken( token );
				if( !ok || !( token.type == ParserTokenType.ZP_JSON_TOKEN_ARRAY_SEP || token.type == ParserTokenType.ZP_JSON_TOKEN_ARRAY_CLOSE ) )
				{
					return false;
				}
				
				isDone = token.type == ParserTokenType.ZP_JSON_TOKEN_ARRAY_CLOSE;
			}
			
			return true;
		}
		
		private void tokenToString( ParserToken token, StringBuilder str )
		{
			int s = token.start + 1;
			int e = token.end - 1;
			
			while( s != e )
			{
				char c = json.charAt( s++ );
				if( c == '"' )
				{
					break;
				}
				else if( c == '\\' )
				{
					if( s == e )
					{
						break;
					}
					else
					{
						char esc = json.charAt( s++ );
						switch( esc )
						{
						case '"': str.append( '"' ); break;
						case '/': str.append( '/' ); break;
						case '\\': str.append( '\\' ); break;
						case 'b': str.append( '\b' ); break;
						case 'f': str.append( '\f' ); break;
						case 'n': str.append( '\n' ); break;
						case 'r': str.append( '\r' ); break;
						case 't': str.append( '\t' ); break;
						case 'u':
							break;
						default:
							break;
						}
					}
				}
				else
				{
					str.append( c );
				}
			}
		}
		
		private void tokenToNumber( ParserToken token )
		{
			int s = token.start;
			if( json.charAt( s ) == '-' )
			{
				++s;
			}
			
			boolean isDouble = false;
			for( int i = s; i != token.end && !isDouble; ++i )
			{
				isDouble = isDouble || __jsonIn( json.charAt( i ), '.', 'e', 'E', '+', '-' );
			}
			
			if( isDouble )
			{
				float v = Float.parseFloat( json.substring( token.start, token.end ) );
				setCurrentValue( Factory.createFloat( v ) );
			}
			else
			{
				int v = Integer.parseInt( json.substring( token.start, token.end ) );
				setCurrentValue( Factory.createInt( v ) );
			}
		}
		
		private boolean readString()
		{
			char c = '\0';
			while( current != end )
			{
				c = nextChar();
				if( c == '\\' )
				{
					nextChar();
				}
				else if( c == '"' )
				{
					break;
				}
			}

			return c == '"';
		}
		
		private boolean readNumber()
		{
			for( ; current != end && ( Character.isDigit( json.charAt( current ) ) || __jsonIn( json.charAt( current ), '.', 'e', 'E', '+', '-' ) ); ++current );
			return true;
		}
		
		boolean __jsonIn( char value, char a, char b, char c, char d, char e )
		{
			return value == a || value == b || value == c || value == d || value == e;
		}
	}
	
	public static final Value parse( String json )
	{
		ParserImpl impl = new Parser.ParserImpl();

		Value value = impl.parse( json );
		return value;
	}
}
