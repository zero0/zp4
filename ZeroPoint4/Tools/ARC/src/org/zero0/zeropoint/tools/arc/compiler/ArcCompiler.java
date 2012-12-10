package org.zero0.zeropoint.tools.arc.compiler;

import java.util.Properties;

import org.zero0.zeropoint.tools.arc.Platform;
import org.zero0.zeropoint.tools.arc.Rendering;

public abstract class ArcCompiler implements Runnable {

    public enum ArcCompilerPropertyType {
	Boolean, Int, Float, String
    };
    
    protected Properties properties = new Properties();
    protected String fileToCompile = "";
    protected ArcCompilerListener listener = null;
    protected Platform platform;
    protected Rendering rendering;

    public final void setFileToCompile( String filePath ) {
	fileToCompile = filePath;
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
