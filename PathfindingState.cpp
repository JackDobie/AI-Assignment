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
	_vehicle->GetSteering()->activeType = Steering::BehaviourType::seek;
	_vehicle->SetPositionTo(_targetPos);
}

void PathfindingState::Exit()
{
	ResetNodes();

	_waypointIndex = 0;
	_pathIndex = 0;

	_nodePath.clear();
}

void PathfindingState::Update(float deltaTime)
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
		_pathIndex = 0;

		// go to the next waypoint, or loop around when at the end
		_startNode = _waypoints[_waypointIndex];
		_waypointIndex = (_waypointIndex + 1) % (_waypoints.size() - 1);
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
	ImGui::Text("%.2f, %.2f", _targetPos.x, _targetPos.y);
	if (ImGui::Button("Toggle path drawing"))
	{
		_drawPath = !_drawPath;
		for (node* n : _nodePath)
		{
			GetWaypoint(n)->draw = _drawPath;
		}
	}
	/*ImGui::Text(("Waypoint index: " + to_string(_waypointIndex)).c_str());
	ImGui::Text(("Path index: " + to_string(_pathIndex)).c_str());*/
	if (ImGui::Button("Return to start"))
	{
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
		GetWaypoint(n)->draw = false;
	}
}