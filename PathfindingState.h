#pragma once

#include "State.h"
#include "Pathfinder.h"
#include "TrackReader.h"

class Vehicle;

class PathfindingState : public State
{
public:
	PathfindingState(Vehicle* vehicle);// : State(vehicle);

	void Start() override;

	void Exit() override;

	void Update(float deltaTime) override;

	void DrawUI() override;

	int GetCurrentState() override
	{
		return 1;
	}
private:
	Waypoint* GetWaypoint(node* n);
	void ResetNodes();

	vector<node*> _waypoints;
	int _waypointIndex;
	int _pathIndex;
	node* _startNode;
	node* _endNode;
	vector<node*> _nodePath;
	Vector2D _targetPos;

	Pathfinder* _pathfinder;
	TrackReader* _trackReader;

	bool _drawPath;
	bool _resetting;

	float _distanceToHitNodeDefault = 30.0f;
	float _distanceToHitNodeBoost = 90.0f;
	float _distanceToHitNode;

	float _distToEndNode = FLT_MAX;
};