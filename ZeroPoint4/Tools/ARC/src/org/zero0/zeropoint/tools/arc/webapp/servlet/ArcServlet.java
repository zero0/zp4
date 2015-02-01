package org.zero0.zeropoint.tools.arc.webapp.servlet;

import java.io.FileReader;
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

		String readFile = req.getParameter( "read" );
		String file = req.getParameter( "file" );
		String fullFile = req.getParameter( "fullfile" );

		String compile = req.getParameter( "compile" );


		if( readFile != null && !readFile.isEmpty() )
		{
			String fullFilePath = ( fullFile == null || fullFile.isEmpty() ) ? Arc.getInstance().getAssetsDirectory() + file : fullFile;
			try
			{
				FileReader fr = new FileReader( fullFilePath );

				char[] buff = new char[ 256 ];
				int length;
				while( ( length = fr.read( buff ) ) != -1 )
				{
					out.write( buff, 0, length );
				}
			}
			catch( Exception e )
			{
			}
		}
		else if( compile != null && !compile.isEmpty() && compile != "null" )
		{
			Arc.getInstance().addCompilerTask( Arc.getInstance().getAssetsDirectory() + compile );
		}

		out.close();
	}
}
