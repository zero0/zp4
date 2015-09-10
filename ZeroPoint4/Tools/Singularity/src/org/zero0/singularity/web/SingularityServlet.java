package org.zero0.singularity.web;

import java.io.IOException;
import java.io.PrintWriter;

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
		PrintWriter out = resp.getWriter();
		out.write( "Hello servlet " + req.getPathInfo() );
	}
}
