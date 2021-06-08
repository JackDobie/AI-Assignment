#include "DecisionMakingState.h"

DecisionMakingState::DecisionMakingState(Vehicle* v) : State(v)
{

}

void DecisionMakingState::Start()
{
	_vehicle->Reset();
}

void DecisionMakingState::Exit()
{

}

void DecisionMakingState::Update(float deltaTime)
{

}

void DecisionMakingState::DrawUI()
{

}