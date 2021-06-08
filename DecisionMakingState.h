#pragma once

#include "State.h"
#include "Vehicle.h"
#include "Pathfinder.h"
#include "TrackReader.h"

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
	Vehicle* _otherVehicle;

	int _lapCount;
	bool _finished;
};