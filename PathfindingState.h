#pragma once

#include "State.h"
#include "Pathfinder.h"

class Vehicle;

class PathfindingState : public State
{
public:
	PathfindingState(Vehicle* vehicle) : State(vehicle) {}

	void Start() override;

	void Exit() override;

	void Update(float deltaTime) override;

	void DrawUI() override;
};