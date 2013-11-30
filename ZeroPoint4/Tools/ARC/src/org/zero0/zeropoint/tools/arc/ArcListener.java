package org.zero0.zeropoint.tools.arc;

public interface ArcListener
{
	public void onArcProcessStart( int numTasks );
	public void onArcProcessUpdate( boolean success, int completeTime, int numTasks, int numSucces, int numError );
	public void onArcProcessEnd();
}
