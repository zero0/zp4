package org.zero0.zeropoint.tools.arc.compiler;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.EnumMap;

import org.zero0.zeropoint.tools.arc.Rendering;

public class ShaderCompiler extends ArcCompiler {

	static EnumMap<Rendering, String> shaderCompiler;
	
	@Override
	public void run() {
		Runtime r = Runtime.getRuntime();
		Process p = null;
		try {
			p = r.exec( new String[]{ "cmd.exe", "/C", "echo %DXSDK_DIR%" } );
			BufferedReader b = new BufferedReader( new InputStreamReader( p.getInputStream() ) );
			System.out.println( b.readLine() );
			p.waitFor();
		} catch( InterruptedException e ) {
			System.err.println();
		} catch( IOException e ) {
			e.printStackTrace();
		} finally {
			if( p != null ) p.destroy();
		}

		
	}

	

}
