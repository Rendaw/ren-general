#include "statemachine.h"

#include <cstdlib>
#include <cassert>
#include <iostream>

namespace State
{

Machine::Machine(void) : CurrentState(NULL), NextState(NULL) {}
Machine::~Machine(void)
{
	if (CurrentState != NULL) delete CurrentState;
	if ((NextState != CurrentState) && (NextState != NULL)) delete NextState;
}

void Machine::Update(void)
{
	if (CurrentState != NULL)
	{
		assert(CurrentState != NULL);
		CurrentState->Update();
	}
	
	if (NextState != CurrentState)
	{
		if (CurrentState != NULL) delete CurrentState;
		CurrentState = NextState;
	}
}

bool Machine::IsDone(void)
	{ return (CurrentState == NULL) && (NextState == NULL); }

void Machine::End(void)
	{ SetState(NULL); }

void Machine::SetState(Base *NewState)
{
	if ((NewState == CurrentState) || (NewState == NextState)) return;
	if ((NextState != NULL) && (NextState != CurrentState)) delete NextState;
	NextState = NewState;
}

}
