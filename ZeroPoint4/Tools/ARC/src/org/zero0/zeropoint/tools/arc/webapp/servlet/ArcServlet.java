package org.zero0.zeropoint.tools.arc.webapp.servlet;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.zero0.zeropoint.tools.arc.Arc;

public class ArcServlet extends HttpServlet
{
	private static final long serialVersionUID = 6174372443391838947L;

	@Override
	protected void doGet( HttpServletRequest req, HttpServletResponse resp ) throws ServletException, IOException
	{
		PrintWriter out = resp.getWriter();
		out.println( "hello, world" );
		out.println( Arc.getInstance().getAssetsDirectory() );
		out.close();
	}
}
