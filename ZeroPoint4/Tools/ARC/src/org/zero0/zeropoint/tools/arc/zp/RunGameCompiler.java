package org.zero0.zeropoint.tools.arc.zp;

import java.io.File;
import java.util.Arrays;
import java.util.List;

import org.zero0.zeropoint.tools.arc.ExecutableMode;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompilerName;

@ArcCompilerName( "" )
public class RunGameCompiler extends ArcCompiler
{
	@Override
	protected List<String> getCompilerParams()
	{
		String d = getExecutableMode() == ExecutableMode.Debug ? "_d" : "";
		return Arrays.asList( getOutputDirectory() + File.separator + getProject() + d + ".exe" );
	}
	
	@Override
	public String getWorkingDirectory()
	{
		return getOutputDirectory();
	}
}