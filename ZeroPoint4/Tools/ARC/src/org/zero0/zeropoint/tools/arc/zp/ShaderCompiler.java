package org.zero0.zeropoint.tools.arc.zp;

import java.util.Arrays;
import java.util.List;

import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;

public class ShaderCompiler extends ArcCompiler
{
	protected List<String> getCompilerParams()
	{
		return Arrays.asList( getRendering().name(), "-Od", getFileToCompile(), getOutputFile() );
	}
}
