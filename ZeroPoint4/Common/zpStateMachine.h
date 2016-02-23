#pragma once
#ifndef ZP_STATE_MACHINE_H
#define ZP_STATE_MACHINE_H

ZP_PURE_INTERFACE zpIState
{

};

class zpStateMachine
{
public:
	void setup();
	void teardown();

	void update( zp_float dt, zp_float rt );

	void addState();

	void swapState( const zp_char* stateName );
	void pushState( const zp_char* stateName );
	void popState();

	zpIState* getCurrentState() const;

private:
	zpArrayList< zpIState* > m_allStates;
	zpArrayList< zpIState* > m_stateStack;
};

#endif
