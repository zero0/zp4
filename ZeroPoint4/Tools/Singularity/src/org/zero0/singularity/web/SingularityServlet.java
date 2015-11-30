package org.zero0.singularity.web;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.zero0.json.Factory;
import org.zero0.json.Value;
import org.zero0.json.Writer;
import org.zero0.singularity.SingularityEngine;
import org.zero0.singularity.SingularityProject;
import org.zero0.singularity.web.ISingularityServletHandler.Response;

public class SingularityServlet extends HttpServlet
{
	private static final long serialVersionUID = -2038537219691492021L;
	
	private Map< String, ISingularityServletHandler > handlers;
	
	@Override
	public void init() throws ServletException
	{
		handlers = new HashMap< String, ISingularityServletHandler >();
		
		handlers.put( "execute", new ExecuteTaskServletHandler() );
		handlers.put( "status", new TaskStatusServletHandler() );
	}
	
	@Override
	public void destroy()
	{
		handlers.clear();
	}
	
	@Override
	protected void doGet( HttpServletRequest req, HttpServletResponse resp ) throws ServletException, IOException
	{
		processPath( req, resp );
	}
	
	@Override
	protected void doPost( HttpServletRequest req, HttpServletResponse resp ) throws ServletException, IOException
	{
		processPath( req, resp );
	}
	
	private void processPath( HttpServletRequest req, HttpServletResponse resp ) throws ServletException, IOException
	{
		String path = req.getPathInfo();
		if( path == null )
		{
			path = "";
		}
		else if( path.startsWith( "/" ) )
		{
			path = path.substring( path.indexOf( "/" ) + 1 );
		}
		
		String action = "";
		
		List< String> args = new ArrayList< String >();
		args.addAll( Arrays.asList( path.split( "/" ) ) );
		
		if( args.size() > 0 )
		{
			action = args.get( 0 );
			args.remove( 0 );
		}
		
		Map< String, String > params = new HashMap< String, String >();
		
		Enumeration< ? > e = req.getParameterNames();
		while( e.hasMoreElements() )
		{
			String name = e.nextElement().toString();
			params.put( name, req.getParameter( name ) );
		}
		
		ISingularityServletHandler handler = handlers.get( action );
		if( handler != null )
		{
			Value data = Factory.createObject( null );
			Response r = handler.onHandleRequest( args, params, data );
			data.set( "code", Factory.createInt( r.getCode() ) );
			
			String respData = Writer.write( data );
			resp.getWriter().write( respData );
			resp.flushBuffer();
		}
	}
}
