package org.zero0.zeropoint.tools.arc.zp;

import java.util.Arrays;
import java.util.List;

import org.zero0.zeropoint.tools.arc.Arc;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompilerName;

@ArcCompilerName( "" )
public class BuildToolsCompiler extends ArcCompiler
{
	@Override
	protected List<String> getCompilerParams()
	{
		String vsDir = getProperties().getProperty( "vs.dir" );
		String vsSln = getProperties().getProperty( "vs.sln" );
		return Arrays.asList( "cmd", "/c", vsDir + "/vcvarsall.bat", getPlatform().getArch(), "&&", "devenv", vsSln, "/Build", getExecutableMode().name() + "_Tools" );
	}
	
	@Override
	public String getWorkingDirectory()
	{
		return Arc.getInstance().getBaseDirectory();
	}
}
