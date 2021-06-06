#include "StateMachine.h"

StateMachine::StateMachine(State* startState)
{
	currentState = startState;
}

void StateMachine::Update(float deltaTime)
{
	if(currentState != nullptr)
		currentState->Update(deltaTime);
}

void StateMachine::ChangeState(State* newState)
{
	if (newState != nullptr)
	{
		// do not change to the same state
		if (newState != currentState)
		{
			currentState->Exit();
			currentState = newState;
			currentState->Start();
		}
	}
}

void StateMachine::DrawUI()
{
	if (currentState != nullptr)
		currentState->DrawUI();
}