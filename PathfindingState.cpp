#include "PathfindingState.h"
#include "Vehicle.h"

void PathfindingState::Start()
{
	_vehicle->Reset();
	_waypointIndex = 0;
	_pathfinder = new Pathfinder();
	_trackReader = new TrackReader(_vehicle);
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