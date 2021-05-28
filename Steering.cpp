#include "Steering.h"
#include "Vehicle.h"

Steering::Steering(Vehicle* veh) : vehicle(veh)
{
	activeType = BehaviourType::wander;
}

Vector2D Steering::CalculateForce()
{
	Vector2D steeringForce;

	switch (activeType)
	{
	case BehaviourType::seek:
		steeringForce = Seek(vehicle->GetTarget()) * seekWeight;
		break;
	case BehaviourType::flee:
		steeringForce = Flee(vehicle->GetTarget()) * fleeWeight;
		break;
	case BehaviourType::arrive:
		steeringForce = Arrive(vehicle->GetTarget()) * arriveWeight;
		break;
	case BehaviourType::wander:
		steeringForce = Wander();
		break;
	case BehaviourType::obstacle_avoidance:
		steeringForce = ObstacleAvoidance();;
		break;
	case BehaviourType::pursuit:
		//steeringForce = Pursuit()
		break;
	default:
		steeringForce = Vector2D();
	}

	return steeringForce;
}

Vector2D Steering::Seek(Vector2D _target)
{
	// returns a steering force that directs the vehicle towards the target
	Vector2D desiredVelocity = Vec2DNormalize(_target - vehicle->GetPositionVector()) * vehicle->GetMaxSpeed();
	return (desiredVelocity - vehicle->GetVelocity());
}

Vector2D Steering::Arrive(Vector2D _target)
{
	return Vector2D();
}

Vector2D Steering::Flee(Vector2D _target)
{
	return Vector2D();
}

Vector2D Steering::Pursuit(Vector2D* _target)
{
	return Vector2D();
}

Vector2D Steering::Wander()
{
	if (vehicle->GetPositionVector().Distance(vehicle->GetWanderTarget()) < 3.5f)
	{
		NewWanderTarget();
	}

	Vector2D seekWander = Seek(vehicle->GetWanderTarget());
	return seekWander;
}
void Steering::NewWanderTarget()
{
	int width = 1024;
	int height = 768;

	//get random vector2d by width and height of screen
	Vector2D randTarget = Vector2D(rand() % width, rand() % height);

	randTarget.x -= width / 2;
	randTarget.y -= height / 2;

	vehicle->SetWanderTarget(randTarget);
}

Vector2D Steering::ObstacleAvoidance()
{
	return Vector2D();
}