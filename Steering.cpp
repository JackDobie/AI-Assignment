#include "Steering.h"

Steering::Steering(Vehicle* veh)
{
	vehicle = veh;
}

void Steering::CalculateForce()
{
	switch (activeType)
	{
	case BehaviourType::none:
		break;
	case BehaviourType::seek:
		//steeringForce = Seek()
		break;
	case BehaviourType::flee:
		//steeringForce = Flee()
		break;
	case BehaviourType::arrive:
		//steeringForce = Flee()
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
	}
}

Vector2D Steering::Seek(Vector2D _target)
{
	// returns a steering force that directs the vehicle towards the target
	Vector2D desiredVelocity = Vec2DNormalize(_target - vehicle->GetPositionVector()) * vehicle->GetMaxSpeed();
	return (desiredVelocity - vehicle->GetVelocity())
}

Vector2D Steering::Arrive(Vector2D _target)
{

}

Vector2D Steering::Flee(Vector2D _target)
{

}

Vector2D Steering::Pursuit(Vector2D* _target)
{

}

Vector2D Steering::Wander()
{
	if (vehicle->GetPositionVector().Distance(vehicle->GetWanderTarget()) < 3.5f)
	{
		NewWanderTarget();
	}

	Vector2D seekWander = Seek(vehicle->GetWanderTarget());
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

}