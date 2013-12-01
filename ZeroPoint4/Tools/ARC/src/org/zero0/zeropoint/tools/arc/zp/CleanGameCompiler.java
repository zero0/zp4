package org.zero0.zeropoint.tools.arc.zp;

import java.util.Arrays;
import java.util.List;

import org.zero0.zeropoint.tools.arc.Arc;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompilerName;

@ArcCompilerName( "" )
public class CleanGameCompiler extends ArcCompiler
{
	@Override
	protected List<String> getCompilerParams()
	{
		return Arrays.asList( "cmd", "/c", getProperties().getProperty( "vs.dir" ) + "/vcvarsall.bat", getPlatform().getArch(), "&&", "devenv", getProperties().getProperty( "vs.sln" ), "/Clean", getExecutableMode().name() );
	}
	
	@Override
	public String getWorkingDirectory()
	{
		return Arc.getInstance().getBaseDirectory();
	}
}
