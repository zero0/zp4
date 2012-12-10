package org.zero0.zeropoint.tools.arc.compiler;

public interface ArcCompilerListener {
    void onCompileMessage( String filePath, String msg );

    void onCompileSuccess( String filePath, String msg );

    void onCompileFailed( String filePath, String msg );
}
