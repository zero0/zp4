package org.zero0.zeropoint.tools.arc.compiler;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

@Retention(RetentionPolicy.RUNTIME)
public @interface ArcCompilerName
{
	String value();
}
