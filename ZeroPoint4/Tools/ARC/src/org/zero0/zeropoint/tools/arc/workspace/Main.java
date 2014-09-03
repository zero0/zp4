package org.zero0.zeropoint.tools.arc.workspace;

import java.io.IOException;

import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;
import org.zero0.zeropoint.tools.arc.Arc;
import org.zero0.zeropoint.tools.arc.ArcAnt;

public final class Main
{
	public static void antmain( String[] args )
	{
		ArcAnt.instance().execute( "clean-tools" );
		
		try {
			System.in.read();
		} catch( IOException e ) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * @param args
	 */
	public static void main( String[] args )
	{
		org.zero0.json.Value v = org.zero0.json.Parser.parse( "[ 10, 123.455, false, null, \"hello world\", { \"test\" : -234 } ]" );
		System.out.println( org.zero0.json.Writer.write( v, true, "    " ) );
		
		Arc.getInstance().initialize();

		final Display display = new Display();
		Display.setAppName( "ARC" );

		Shell shell = new Shell( display );
		shell.setLayout( new FillLayout() );
		shell.setText( "ARC" );
		shell.setSize( 800, 600 );
		shell.setMinimumSize( 800, 600 );

		new Workspace( shell );

		shell.open();
		while( !shell.isDisposed() )
		{
			if( !display.readAndDispatch() )
			{
				display.sleep();
			}
		}

		display.dispose();

		Arc.getInstance().shutdown();
	}
}
