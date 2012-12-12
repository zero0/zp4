package org.zero0.zeropoint.tools.arc;

import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;

public final class Main {

    /**
     * @param args
     */
    public static void main( String[] args ) {
	final Display display = new Display();
	Display.setAppName( "ARC" );

	Shell shell = new Shell( display );
	shell.setLayout( new FillLayout() );
	shell.setText( "ARC - Automated Resource Compiler" );
	shell.setSize( 800, 600 );
	shell.setMinimumSize( 800, 600 );

	new Workspace( shell );

	shell.open();
	while( !shell.isDisposed() ) {
	    if( !display.readAndDispatch() ) {
		display.sleep();
	    }
	}

	display.dispose();

	Arc.getInstance().shutdown();
    }
}
