package org.zero0.singularity;

import java.io.File;

public interface ISingularityFileSetFilter
{
	boolean acceptFile( File f );
}