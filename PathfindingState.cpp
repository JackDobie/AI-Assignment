#include "PathfindingState.h"
#include "Vehicle.h"

void PathfindingState::Start()
{
	_vehicle->Reset();
	_trackReader = new TrackReader();
	_trackReader->ReadFile("Resources/waypoints.txt");
	_waypoints = _trackReader->GetWaypoints();
	_waypointIndex = 0;
	_startNode = _waypoints[_waypointIndex];
	_endNode = _waypoints[++_waypointIndex];
	// move vehicle to the first node
	_vehicle->SetVehiclePosition(GetWaypoint(_startNode)->GetPositionVector());
	_pathfinder = new Pathfinder();
	_pathfinder->FindPath(_startNode, _endNode);
	_nodePath = _pathfinder->GetNodePath();
	_pathIndex = 0;
	_targetPos = GetWaypoint(_nodePath[_pathIndex])->GetPositionVector();
	// set the vehicle to seek to the first node in the path
	_vehicle->GetSteering()->activeType = Steering::BehaviourType::seek;
	_vehicle->SetPositionTo(_targetPos);
	GetWaypoint(_nodePath[_pathIndex])->draw = true;

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
		if (n != _nodePath[_pathIndex])
		{
			GetWaypoint(n)->draw = false;
		}
	}

	if (Vec2DDistance(_vehicle->GetPositionVector(), GetWaypoint(_nodePath[_pathIndex])->GetPositionVector()) < 10.0f)
	{
		if (_pathIndex < _nodePath.size() - 1)
		{
			_pathIndex++;
			_targetPos = GetWaypoint(_nodePath[_pathIndex])->GetPositionVector();
			_vehicle->SetPositionTo(_targetPos);
			GetWaypoint(_nodePath[_pathIndex])->draw = true;
		}
	}

	// compare vehicle position to end node
	//if (Vec2DDistance(_vehicle->GetPositionVector(), GetWaypoint(_endNode)->GetPositionVector()) < 100.0f)
	if(_pathIndex + 1 == _nodePath.size())
	{
		// go to the next waypoint, or loop around when at the end
		if (_waypointIndex < _waypoints.size() - 1)
		{
			_pathIndex = 0;
			_startNode = _waypoints[_waypointIndex];
			_endNode = _waypoints[++_waypointIndex];
		}
		else
		{
			_pathIndex = 0;
			_startNode = _waypoints[_waypointIndex];
			_waypointIndex = 0;
			_endNode = _waypoints[_waypointIndex];
		}

		_pathfinder->FindPath(_startNode, _endNode);
		_nodePath.clear();
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
	ImGui::End();
}

Waypoint* PathfindingState::GetWaypoint(node* n)
{
	return _vehicle->GetWaypoint(n->pos.x, n->pos.y);
}