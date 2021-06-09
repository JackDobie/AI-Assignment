#include "PathfindingState.h"
#include "Vehicle.h"

PathfindingState::PathfindingState(Vehicle* vehicle) : State(vehicle)
{
	_startNode = nullptr;
	_endNode = nullptr;
	_pathIndex = 0;
	_waypointIndex = 0;
	_resetting = false;
	_drawPath = false;
	_pathfinder = new Pathfinder();
	_trackReader = new TrackReader();
	_trackReader->ReadFile("Resources/waypoints.txt");
	_distanceToHitNode = _distanceToHitNodeDefault;
}

void PathfindingState::Start()
{
	// reset vehicle position and movement
	_vehicle->Reset();
	_waypoints = _trackReader->GetWaypoints();
	// get start and end points between first two waypoints
	_waypointIndex = 0;
	_startNode = _waypoints[_waypointIndex];
	_endNode = _waypoints[++_waypointIndex];
	// move vehicle to the first waypoint
	Vector2D startPos = GetWaypoint(_startNode)->GetPositionVector();
	_vehicle->SetVehiclePosition(startPos);
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
	_vehicle->GetSteering()->activeType = Steering::BehaviourType::seek;
	_vehicle->SetPositionTo(_targetPos);
}

void PathfindingState::Exit()
{
	ResetNodes();

	_waypointIndex = 0;
	_pathIndex = 0;
	_distanceToHitNode = _distanceToHitNodeDefault;

	_nodePath.clear();
}

void PathfindingState::Update(float deltaTime)
{
	// if the other vehicle has crashed, increase the distance to hit a node to allow for easier overtakes
	if (_vehicle->GetOtherVehicle()->GetSpeedFactor() < 1.0f)
	{
		_distanceToHitNode = _distanceToHitNodeBoost;
	}
	else
	{
		_distanceToHitNode = _distanceToHitNodeDefault;
	}

	// compare distance to check if got in range of the next node
	if (Vec2DDistance(_vehicle->GetPositionVector(), GetWaypoint(_nodePath[_pathIndex])->GetPositionVector()) < _distanceToHitNode)
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
	if (Vec2DDistance(_vehicle->GetPositionVector(), GetWaypoint(_endNode)->GetPositionVector()) < _distanceToHitNode)
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

void PathfindingState::DrawUI()
{
	ImGui::Begin((_vehicle->GetName() + " pathfinding controls").c_str());
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
		_startNode = _nodePath[_pathIndex];
		_waypointIndex = 0;
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

		_pathIndex = 0;
		_targetPos = GetWaypoint(_nodePath[0])->GetPositionVector();
		_vehicle->SetPositionTo(_targetPos);
	}
	ImGui::End();
}

Waypoint* PathfindingState::GetWaypoint(node* n)
{
	return _vehicle->GetWaypoint(n->pos.x, n->pos.y);
}

void PathfindingState::ResetNodes()
{
	for (node* n : _trackReader->GetNodeVector())
	{
		n->visited = false;
		n->globalGoal = FLT_MAX;
		n->localGoal = FLT_MAX;
		n->parent = nullptr;
		if(_drawPath)
			GetWaypoint(n)->draw = false;
	}
}