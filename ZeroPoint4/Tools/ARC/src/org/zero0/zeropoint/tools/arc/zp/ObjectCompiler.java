package org.zero0.zeropoint.tools.arc.zp;

import java.util.Arrays;
import java.util.List;

import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;

public class ObjectCompiler extends ArcCompiler
{
	@Override
	protected List<String> getCompilerParams()
	{
		return Arrays.asList( getFileToCompile(), getOutputFile() );
	}
}
