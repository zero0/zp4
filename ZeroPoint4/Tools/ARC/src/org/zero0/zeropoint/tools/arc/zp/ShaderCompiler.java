package org.zero0.zeropoint.tools.arc.zp;

import java.util.Arrays;
import java.util.List;

import org.zero0.zeropoint.tools.arc.Arc;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;

public class ShaderCompiler extends ArcCompiler
{
	protected List<String> getCompilerParams()
	{
		char option = 'd';
		switch( Arc.getInstance().getExecutableMode() )
		{
		case Release:
			option = '1';
			break;
		}

		return Arrays.asList( getRendering().name(), "-O" + option, getFileToCompile(), getOutputFile() );
	}
}
