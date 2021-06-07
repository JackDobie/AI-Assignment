#include "PathfindingState.h"
#include "Vehicle.h"

void PathfindingState::Start()
{
	_vehicle->Reset();
	_waypointIndex = 0;
	_trackReader = new TrackReader();
	if (!_trackReader->ReadFile("Resources/waypoints.txt"))
	{
		Debug::Print("ERROR: Could not read file!");
	}
	else
	{
		_waypoints = _trackReader->GetWaypoints();
		_pathfinder = new Pathfinder(_waypoints[0], _waypoints[1]);
		_nodePath = _pathfinder->GetNodePath();
	}
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
	//Vector2D wpPos = _vehicle->GetWaypoint(/*currentnode pos*/)->GetPositionVector()
	//if(Vec2DDistance(_vehicle->GetPositionVector(), ))
}

void PathfindingState::DrawUI()
{

}