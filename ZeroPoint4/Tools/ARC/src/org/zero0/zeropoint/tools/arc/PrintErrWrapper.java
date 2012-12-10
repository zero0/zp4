package org.zero0.zeropoint.tools.arc;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

public class PrintErrWrapper extends PrintStream {
    private PrintErrAppender appender;

    public PrintErrWrapper( PrintErrAppender appender ) {
	super( new ByteArrayOutputStream() );
	this.appender = appender;
    }

    @Override
    public void write( byte[] buf, int off, int len ) {
	super.write( buf, off, len );
	appender.appendFromPrintErr( new String( buf, off, len ) );
    }

    public void clear() {
	this.out = new ByteArrayOutputStream();
    }
}
