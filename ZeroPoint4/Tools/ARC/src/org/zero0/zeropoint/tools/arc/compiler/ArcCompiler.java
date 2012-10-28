package org.zero0.zeropoint.tools.arc.compiler;

import java.util.Properties;

import org.zero0.zeropoint.tools.arc.Platform;
import org.zero0.zeropoint.tools.arc.Rendering;

public abstract class ArcCompiler implements Runnable {

	protected String fileToCompile = "";
	protected Properties properties = new Properties();
	protected ArcCompilerListener listener = null;
	protected Platform platform;
	protected Rendering rendering;
	
	public final void setFileToCompile( String filePath ) {
		fileToCompile = filePath;
	}
	
	public final void setProperties( Properties properties ) {
		this.properties.putAll( properties );
	}
	
	public final Properties getProperties() {
		return properties;
	}
	
	public final void setListener( ArcCompilerListener listener ) {
		this.listener = listener;
	}

	public final Platform getPlatform() {
		return platform;
	}

	public final void setPlatform( Platform platform ) {
		this.platform = platform;
	}

	public final Rendering getRendering() {
		return rendering;
	}

	public final void setRendering( Rendering rendering ) {
		this.rendering = rendering;
	}
}
