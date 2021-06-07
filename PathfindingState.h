#pragma once

#include "State.h"
#include "Pathfinder.h"
#include "TrackReader.h"

class Vehicle;

class PathfindingState : public State
{
public:
	PathfindingState(Vehicle* vehicle) : State(vehicle) {}

	void Start() override;

	void Exit() override;

	void Update(float deltaTime) override;

	void DrawUI() override;
private:
	vector<node*> waypoints;
	int _waypointIndex;
	node* _currentNode;
	vector<node*> _currentNodePath;
	Vector2D _targetPos;
	Pathfinder* _pathfinder;
	TrackReader* _trackReader;
};