package org.zero0.zeropoint.tools.arc.workspace;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

public class PrintOutWrapper extends PrintStream {
    private PrintOutAppender appender;

    public PrintOutWrapper( PrintOutAppender appender ) {
	super( new ByteArrayOutputStream() );
	this.appender = appender;
    }

    @Override
    public void write( byte[] buf, int off, int len ) {
	super.write( buf, off, len );
	appender.appendFromPrintOut( new String( buf, off, len ) );
    }

    public void clear() {
	this.out = new ByteArrayOutputStream();
    }
}
