#include "PathfindingState.h"
#include "Vehicle.h"

void PathfindingState::Start()
{
	// reset vehicle position and movement
	_vehicle->Reset();
	// read the waypoints file
	_trackReader = new TrackReader();
	_trackReader->ReadFile("Resources/waypoints.txt");
	_waypoints = _trackReader->GetWaypoints();
	// get start and end points between first two waypoints
	_waypointIndex = 0;
	_startNode = _waypoints[_waypointIndex];
	_endNode = _waypoints[++_waypointIndex];
	// move vehicle to the first waypoint
	_vehicle->SetVehiclePosition(GetWaypoint(_startNode)->GetPositionVector());
	// calculate a path to the next waypoint
	_pathfinder = new Pathfinder();
	_pathfinder->FindPath(_startNode, _endNode);
	_nodePath = _pathfinder->GetNodePath();
	// set the target
	_pathIndex = 0;
	_targetPos = GetWaypoint(_nodePath[_pathIndex])->GetPositionVector();
	// set the vehicle to seek to the first node in the path
	_vehicle->GetSteering()->activeType = Steering::BehaviourType::seek;
	_vehicle->SetPositionTo(_targetPos);
}

void PathfindingState::Exit()
{
	_waypointIndex = 0;

	delete(_pathfinder);
	_pathfinder = nullptr;
	delete(_trackReader);
	_trackReader = nullptr;
}

void PathfindingState::Update(float deltaTime)
{
	for (node* n : _nodePath)
	{
		GetWaypoint(n)->draw = true;
	}

	static int pathCount = 1;
	if (Vec2DDistance(_vehicle->GetPositionVector(), GetWaypoint(_nodePath[_pathIndex])->GetPositionVector()) < 7.5f)
	{
		if (_pathIndex < _nodePath.size() - 1)
		{
			pathCount++;
			_pathIndex++;
			_targetPos = GetWaypoint(_nodePath[_pathIndex])->GetPositionVector();
			_vehicle->SetPositionTo(_targetPos);
		}
	}
	ImGui::Text(to_string(pathCount).c_str());

	// check if got to the end of the path
	//if (Vec2DDistance(_vehicle->GetPositionVector(), GetWaypoint(_endNode)->GetPositionVector()) < 50.0f)
	if(_pathIndex == _nodePath.size() - 1)
	{
		_pathIndex = 0;

		// go to the next waypoint, or loop around when at the end
		_startNode = _waypoints[_waypointIndex];
		_waypointIndex = (_waypointIndex + 1) % (_waypoints.size() - 1);
		_endNode = _waypoints[_waypointIndex];

		_pathfinder->FindPath(_startNode, _endNode);
		_nodePath = _pathfinder->GetNodePath();
		_targetPos = GetWaypoint(_nodePath[_pathIndex])->GetPositionVector();
		_vehicle->SetPositionTo(_targetPos);
	}
}

void PathfindingState::DrawUI()
{
	ImGui::Begin((_vehicle->GetName() + " pathfinding controls").c_str());
	if (ImGui::Button("Reset pathfinding"))
	{
		_waypointIndex = 0;
		_startNode = _waypoints[_waypointIndex];
		_endNode = _waypoints[++_waypointIndex];
		_targetPos = GetWaypoint(_endNode)->GetPositionVector();
		_vehicle->SetPositionTo(_targetPos);
	}
	ImGui::Text("%.2f, %.2f", _targetPos.x, _targetPos.y);
	ImGui::Text(("Waypoint index: " + to_string(_waypointIndex)).c_str());
	ImGui::Text(("Path index: " + to_string(_pathIndex)).c_str());
	ImGui::End();
}

Waypoint* PathfindingState::GetWaypoint(node* n)
{
	return _vehicle->GetWaypoint(n->pos.x, n->pos.y);
}