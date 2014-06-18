package org.zero0.zeropoint.tools.arc.zp;

import java.util.Arrays;
import java.util.List;

import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompilerName;

public class TextureCompiler extends ArcCompiler
{
	@Override
	protected List<String> getCompilerParams()
	{
		String format = getProperties().getProperty( "format", "RGB" );
		String compression = getProperties().getProperty( "compression", "BC1" );
		
		return Arrays.asList( getRendering().name(), "-F" + format, "-C" + compression, getFileToCompile(), getOutputFile() );
	}
}
