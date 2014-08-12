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
			
			if( json != null && !json.isEmpty() )
			{
				json = jsonString;
				
				current = 0;
				end = json.length();
				
				nodes = new ArrayList< Value >();
				nodes.add( value );
				
				boolean success = parseValue();
				if( !success )
				{
					return Factory.Null;
				}
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
				break;
			case 't':
				token.type = ParserTokenType.ZP_JSON_TOKEN_TRUE;
				break;
			case 'f':
				token.type = ParserTokenType.ZP_JSON_TOKEN_FALSE;
				break;
			case 'n':
				token.type = ParserTokenType.ZP_JSON_TOKEN_NULL;
				break;
			case ':':
				token.type = ParserTokenType.ZP_JSON_TOKEN_MEMBER_SEP;
				break;
			case ',':
				token.type = ParserTokenType.ZP_JSON_TOKEN_ARRAY_SEP;
				break;
				
			default:
				break;
			}
			
			if( !ok )
			{
				token.type = ParserTokenType.ZP_JSON_TOKEN_ERROR;
			}
			token.end = current;
			
			return true;
		}
		
		private boolean parseValue()
		{
			return false;
		}
	}
	
	public static final Value parse( String json )
	{
		ParserImpl impl = new Parser.ParserImpl();

		Value value = impl.parse( json );
		return value;
	}
}
