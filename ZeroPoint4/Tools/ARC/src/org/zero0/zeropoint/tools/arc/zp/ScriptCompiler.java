package org.zero0.zeropoint.tools.arc.zp;

import java.util.Arrays;
import java.util.List;

import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompilerName;

@ArcCompilerName( "" )
public class ScriptCompiler extends ArcCompiler
{
	@Override
	protected List<String> getCompilerParams()
	{
		return Arrays.asList( "cmd", "/c", "copy", "/Y", getFileToCompile(), getOutputFile() );
	}
}
