#include "DecisionMakingState.h"

DecisionMakingState::DecisionMakingState(Vehicle* v) : State(v)
{
	_finished = false;
	_startNode = nullptr;
	_endNode = nullptr;
	_pathIndex = 0;
	_waypointIndex = 0;
	_lapCount = 0;
	_resetting = false;
	_drawPath = false;
	_otherVehicle = _vehicle->GetOtherVehicle();
	_pathfinder = new Pathfinder();
	_trackReader = new TrackReader();
	_trackReader->ReadFile("Resources/waypoints.txt");
}

void DecisionMakingState::Start()
{
	// reset vehicle position and movement
	_vehicle->Reset();
	_waypoints = _trackReader->GetWaypoints();
	// get start and end points between first two waypoints
	_waypointIndex = 0;
	_startNode = _waypoints[_waypointIndex];
	_endNode = _waypoints[++_waypointIndex];
	// move vehicle to the first waypoint
	_vehicle->SetVehiclePosition(GetWaypoint(_startNode)->GetPositionVector());
	// calculate a path to the next waypoint
	_pathfinder->FindPath(_startNode, _endNode);
	_nodePath = _pathfinder->GetNodePath();
	if (_drawPath)
	{
		for (node* n : _nodePath)
		{
			GetWaypoint(n)->draw = true;
		}
	}

	// set the target
	_pathIndex = 0;
	_targetPos = GetWaypoint(_nodePath[_pathIndex])->GetPositionVector();
	// set the vehicle to seek to the first node in the path
	_vehicle->GetSteering()->activeType = Steering::BehaviourType::obstacle_avoidance;
	_vehicle->SetPositionTo(_targetPos);
}

void DecisionMakingState::Exit()
{
	ResetNodes();

	_waypointIndex = 0;
	_pathIndex = 0;
	_lapCount = 0;
	_finished = false;

	_nodePath.clear();
}

void DecisionMakingState::Update(float deltaTime)
{
	// if done five laps
	if (_finished)
	{
		_targetPos.x = _vehicle->GetPositionVector().x + 200.0f;
		_targetPos.y = _vehicle->GetPositionVector().y;
		// slow down to a stop
		_vehicle->GetSteering()->activeType = Steering::BehaviourType::arrive;
	}
	else
	{
		// compare distance to check if got in range of the next node
		if (Vec2DDistance(_vehicle->GetPositionVector(), GetWaypoint(_nodePath[_pathIndex])->GetPositionVector()) < 20.0f)
		{
			// if this isn't the last node in the path, set target to the next node
			if (_pathIndex < _nodePath.size() - 1)
			{
				_pathIndex++;
				_targetPos = GetWaypoint(_nodePath[_pathIndex])->GetPositionVector();
				_vehicle->SetPositionTo(_targetPos);
			}
		}

		// compare distance to check if got in range of the waypoint
		if (Vec2DDistance(_vehicle->GetPositionVector(), GetWaypoint(_endNode)->GetPositionVector()) < 20.0f)
		{
			// if vehicle went from the end waypoint hit the starting waypoint to complete a lap
			if (_startNode == _waypoints[14] && _endNode == _waypoints[0])
			{
				if (++_lapCount == 5)
				{
					_finished = true;
				}
			}

			if(_lapCount != 5)
			{
				_pathIndex = 0;

				// go to the next waypoint, or loop around when at the end
				_startNode = _waypoints[_waypointIndex];
				_waypointIndex = (_waypointIndex + 1) % (_waypoints.size());
				_endNode = _waypoints[_waypointIndex];

				ResetNodes();
				_pathfinder->FindPath(_startNode, _endNode);
				_nodePath = _pathfinder->GetNodePath();
				if (_drawPath)
				{
					for (node* n : _nodePath)
					{
						GetWaypoint(n)->draw = true;
					}
				}

				_targetPos = GetWaypoint(_nodePath[_pathIndex])->GetPositionVector();
				_vehicle->SetPositionTo(_targetPos);
			}
		}
	}
}

void DecisionMakingState::DrawUI()
{
	ImGui::Begin((_vehicle->GetName() + " decision making controls").c_str());
	ImGui::Text(("Waypoint index: " + to_string(_waypointIndex)).c_str());
	ImGui::Text(("Laps: " + to_string(_lapCount)).c_str());
	if (ImGui::Button("Toggle path drawing"))
	{
		_drawPath = !_drawPath;
		for (node* n : _nodePath)
		{
			GetWaypoint(n)->draw = _drawPath;
		}
	}
	if (ImGui::Button("Return to start"))
	{
		_lapCount = 0;
		_finished = false;

		_startNode = _waypoints[_waypointIndex];
		_waypointIndex = 0;
		_endNode = _waypoints[_waypointIndex];

		_pathfinder->FindPath(_startNode, _endNode);
		_nodePath = _pathfinder->GetNodePath();

		_pathIndex = 0;
		_targetPos = GetWaypoint(_nodePath[0])->GetPositionVector();
		_vehicle->SetPositionTo(_targetPos);
	}
	ImGui::End();
}

Waypoint* DecisionMakingState::GetWaypoint(node* n)
{
	return _vehicle->GetWaypoint(n->pos.x, n->pos.y);
}

void DecisionMakingState::ResetNodes()
{
	for (node* n : _trackReader->GetNodeVector())
	{
		n->visited = false;
		n->globalGoal = FLT_MAX;
		n->localGoal = FLT_MAX;
		n->parent = nullptr;
		if (_drawPath)
			GetWaypoint(n)->draw = false;
	}
}