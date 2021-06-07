#pragma once
#include "State.h"

class StateMachine
{
public:
	StateMachine(State* startState);

	virtual void Update(float deltaTime);
	void ChangeState(State* newState);
	void DrawUI();
	State* GetCurrentState() { return currentState; }
protected:
	State* currentState;
};