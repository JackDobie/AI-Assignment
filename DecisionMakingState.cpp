#include "DecisionMakingState.h"

// used to count how many vehicles have decisionmakingstate
// static and added to when a new vehicle uses this state, then taken from on exit.
static int carsWithThis = 0;

DecisionMakingState::DecisionMakingState(Vehicle* v) : State(v)
{
	_distanceToHitNode = _distanceToHitNodeDefault;
	_movingToPickUp = false;
	_overtaking = false;
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
	_trackReader->CreateNodes();
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
	Vector2D startPos = GetWaypoint(_startNode)->GetPositionVector();
	// add a offset to avoid multiple vehicles starting in exact same position
	if (++carsWithThis == 1)
	{
		startPos.x -= 200.0f;
		startPos.y += 40.0f;
	}
	else
	{
		startPos.y -= 50.0f;
	}
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
	_vehicle->GetSteering()->activeType = Steering::BehaviourType::obstacle_avoidance;
	_vehicle->SetPositionTo(_targetPos);
}

void DecisionMakingState::Exit()
{
	carsWithThis--;

	ResetNodes();

	_waypointIndex = 0;
	_pathIndex = 0;
	_lapCount = 0;
	_finished = false;
	_overtaking = false;
	_movingToPickUp = false;
	_distanceToHitNode = _distanceToHitNodeDefault;

	_nodePath.clear();
}

void DecisionMakingState::Update(float deltaTime)
{
	_vehicle->_pathIndex = _pathIndex;

	_distToEndNode = Vec2DDistance(_vehicle->GetPositionVector(), GetWaypoint(_endNode)->GetPositionVector());
	_vehicle->SetDistToEndNode(_distToEndNode);

	//// if this vehicle is trying to overtake the other vehicle, increase the distance required to hit a node
	//if (_overtaking)
	//	_distanceToHitNode = _distanceToHitNodeOvertaking;
	//else
	//	_distanceToHitNode = _distanceToHitNodeDefault;

	// if done five laps
	if (_finished)
	{

	}
	else
	{
		if (_vehicle->_waypointCount < _vehicle->GetOtherVehicle()->_waypointCount ||
			_distToEndNode > _vehicle->GetOtherVehicle()->GetDistToEndNode())
		{
			// check if in range before overtaking
			float dist = Vec2DDistance(_vehicle->GetPositionVector(), _vehicle->GetOtherVehicle()->GetPositionVector());
			if (dist < 150)
				_overtaking = true;
		}
		/*else if (_vehicle->_waypointCount == _vehicle->GetOtherVehicle()->_waypointCount)
		{
			if (_distToEndNode < _vehicle->GetOtherVehicle()->GetDistToEndNode())
			{
				float dist = Vec2DDistance(_vehicle->GetPositionVector(), _vehicle->GetOtherVehicle()->GetPositionVector());
				if (dist < (_vehicle->GetScale().x * 3))
					_overtaking = true;
			}
			else
			{
				_overtaking = false;
			}
		}*/
		else
			_overtaking = false;

		if (_movingToPickUp)
		{
			if (Vec2DDistance(_vehicle->GetPositionVector(), _targetPos) < 50.0f)
			{
				_movingToPickUp = false;

				_targetPos = GetWaypoint(_endNode)->GetPositionVector();
				_vehicle->SetPositionTo(_targetPos);
			}
		}
		else
		{
			if (Vec2DDistance(_vehicle->GetPositionVector(), _vehicle->GetPickUpItem()->GetPositionVector()) < _distToEndNode)
			{
				_movingToPickUp = true;
			}

			if (_overtaking)
			{
				_distanceToHitNode = _distanceToHitNodeOvertaking;
				// check if in range to overtake
				if (Vec2DDistance(_vehicle->GetPositionVector(), _vehicle->GetOtherVehicle()->GetPositionVector()) < _distanceToHitNode)
				{
					Vector2D rightside = (_vehicle->GetOtherVehicle()->GetSide() * -(_vehicle->GetScale().x * 2));
					Vector2D leftside = Vector2D(-rightside.x, -rightside.y);
					// target the side of the other vehicle
					if (Vec2DDistance(_vehicle->GetPositionVector(), rightside) < Vec2DDistance(_vehicle->GetPositionVector(), leftside))
					{
						_targetPos = _vehicle->GetOtherVehicle()->GetPositionVector() + rightside;
					}
					else
					{
						_targetPos = _vehicle->GetOtherVehicle()->GetPositionVector() + leftside;
					}
					//_targetPos = _vehicle->GetOtherVehicle()->GetPositionVector() + (_vehicle->GetOtherVehicle()->GetSide() * -(_vehicle->GetScale().x * 2));
					_vehicle->SetPositionTo(_targetPos);
					// check if hit the target
					// distance to hit node will be increased while overtaking to allow it to hit while at the side of the other vehicle
					if (Vec2DDistance(_vehicle->GetPositionVector(), _targetPos) < _distanceToHitNode)
					{
						// set the target to the end node to overtake
						_targetPos = GetWaypoint(_endNode)->GetPositionVector();
						_vehicle->SetPositionTo(_targetPos);
						// set overtaking to false to avoid 
						_overtaking = false;
					}
				}
				else
				{
					// if not in range to overtake, just set the target to the end node
					_targetPos = GetWaypoint(_endNode)->GetPositionVector();
					_vehicle->SetPositionTo(_targetPos);
				}
			}
			else
			{
				// compare distance to check if got in range of the next node
				if (Vec2DDistance(_vehicle->GetPositionVector(), GetWaypoint(_nodePath[_pathIndex])->GetPositionVector()) < _distanceToHitNode)
				{
					NextNode();
				}
			}

			// compare distance to check if got in range of the waypoint
			if (_distToEndNode < _distanceToHitNode)
			{
				NextWaypoint();
			}
		}
	}
}

void DecisionMakingState::DrawUI()
{
	ImGui::Begin((_vehicle->GetName() + " decision making controls").c_str());
	ImGui::Text(("Laps: " + to_string(_lapCount)).c_str());
	string overtaking = (_overtaking ? "true" : "false");
	ImGui::Text(("Overtaking: " + overtaking).c_str());
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
		_vehicle->GetSteering()->activeType = Steering::BehaviourType::obstacle_avoidance;
	}
	ImGui::End();
}

Waypoint* DecisionMakingState::GetWaypoint(node* n)
{
	return _vehicle->GetWaypoint(n->pos.x, n->pos.y);
}

void DecisionMakingState::ResetNodes()
{
	vector<node*> nodevec = _trackReader->GetNodeVector();
	for (node* n : nodevec)
	{
		n->visited = false;
		n->globalGoal = FLT_MAX;
		n->localGoal = FLT_MAX;
		n->parent = nullptr;
		if (_drawPath)
			GetWaypoint(n)->draw = false;
	}
}

void DecisionMakingState::NextNode()
{
	_distanceToHitNode = _distanceToHitNodeDefault;
	// if this isn't the last node in the path, set target to the next node
	if (_pathIndex < _nodePath.size() - 1)
	{
		_pathIndex++;
		_targetPos = GetWaypoint(_nodePath[_pathIndex])->GetPositionVector();
		_vehicle->SetPositionTo(_targetPos);
	}
}

void DecisionMakingState::NextWaypoint()
{
	_distanceToHitNode = _distanceToHitNodeDefault;
	// if vehicle went from the end waypoint hit the starting waypoint to complete a lap
	if (_startNode == _waypoints[14] && _endNode == _waypoints[0])
	{
		// increase lap count then finish when at 5 laps
		if (++_lapCount == 5)
		{
			_finished = true;

			// set a stopping position with a random offset
			_targetPos.x = GetWaypoint(_waypoints[0])->GetPositionVector().x + (rand() % (500 - 300 + 1) + 300);
			_targetPos.y = GetWaypoint(_waypoints[0])->GetPositionVector().y - (rand() % (100 - 50 + 1) + 50);
			_vehicle->SetVelocity(_vehicle->GetVelocity() * 0.4f);
			// slow down to a stop
			_vehicle->GetSteering()->activeType = Steering::BehaviourType::arrive;
			_vehicle->SetPositionTo(_targetPos);
		}
	}

	if (_lapCount != 5)
	{
		_overtaking = false;

		_pathIndex = 0;

		// go to the next waypoint, or loop around when at the end
		_startNode = _waypoints[_waypointIndex];
		_waypointIndex = (_waypointIndex + 1) % (_waypoints.size());
		_vehicle->_waypointCount = _waypointIndex;
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