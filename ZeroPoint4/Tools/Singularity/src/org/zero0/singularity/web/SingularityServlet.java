package org.zero0.singularity.web;

import java.io.IOException;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class SingularityServlet extends HttpServlet
{
	private static final long serialVersionUID = -2038537219691492021L;
	
	@Override
	public void init() throws ServletException
	{
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
			path = "/";
		}
		
		String[] args = path.split( "/" );
		
		if( args.length == 0 )
		{
			RequestDispatcher disp = req.getRequestDispatcher( "../project.jsp" );
			disp.include( req, resp );
		}
		else
		{
			
		}
	}
}
