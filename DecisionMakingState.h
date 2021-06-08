#pragma once

#include "State.h"
#include "Vehicle.h"

class DecisionMakingState : public State
{
public:
	DecisionMakingState(Vehicle* vehicle);

	void Start() override;

	void Exit() override;

	void Update(float deltaTime) override;

	void DrawUI() override;

	int GetCurrentState() override
	{
		return 2;
	}
private:

};