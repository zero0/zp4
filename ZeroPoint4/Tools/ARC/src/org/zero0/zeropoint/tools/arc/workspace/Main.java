package org.zero0.zeropoint.tools.arc.workspace;

import java.util.HashMap;
import java.util.Map;

import net.winstone.Server;
import net.winstone.boot.BootStrap;

import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;
import org.zero0.zeropoint.tools.arc.Arc;
import org.zero0.zeropoint.tools.arc.pipe.IArcProcessor;
import org.zero0.zeropoint.tools.arc.pipe.impl.ArcSequentialProcessor;

public final class Main
{
	/**
	 * @param args
	 */
	public static void main( String[] args )
	{
		IArcProcessor p = new ArcSequentialProcessor();
		p.run();
		
		String endorsed = System.getProperty("java.endorsed.dirs");
		System.out.println( endorsed );
		
		System.setProperty( "java.endorsed.dirs", "" );
		
		Map< String, String > serverArgs = new HashMap< String, String >();
		//serverArgs.put( "webroot", "./www/servlets-examples" );
		serverArgs.put( "webroot", "./src/main/webapp/arc" );
		serverArgs.put( "useJasper", null );
		serverArgs.put( "javaHome", System.getenv( "JAVA_HOME" ).replace( " ", "%20" ) );
		
		BootStrap b = new BootStrap( serverArgs );
		Server s = b.boot();
		s.start();
		
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

		s.shutdown();

		System.setProperty( "java.endorsed.dirs", endorsed );
		
		display.dispose();

		Arc.getInstance().shutdown();
	}
}
