#include "Steering.h"
#include "Vehicle.h"

Steering::Steering(Vehicle* veh) : vehicle(veh)
{
	activeType = BehaviourType::none;
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
		steeringForce = Pursuit(vehicle->GetOtherVehicle());
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

Vector2D Steering::Flee(Vector2D _target)
{
	//similar to seek but target and position are swapped
	Vector2D desiredVelocity = Vec2DNormalize(vehicle->GetPositionVector() - _target) * vehicle->GetMaxSpeed();
	return (desiredVelocity - vehicle->GetVelocity());
}

Vector2D Steering::Arrive(Vector2D _target)
{
	//calculate direction and distance to target
	Vector2D dir = _target - vehicle->GetPositionVector();
	float dist = dir.Length();
	if (dist > 0)
	{
		//decrease speed with distance
		float speed = dist / 1.5f;
		//ensure speed does not exceed max speed
		speed = min(speed, vehicle->GetMaxSpeed());
		Vector2D desiredVelocity = dir * speed / dist;
		return (desiredVelocity - vehicle->GetVelocity());
	}
	//if distance is less than or equal to 0, do not return a force
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

Vector2D Steering::Pursuit(Vehicle* _target)
{
	Vector2D toTarget = _target->GetPositionVector() - vehicle->GetPositionVector();

	float relativeForward = vehicle->GetForward().Dot(_target->GetForward());

	//if the target is ahead then can just seek towards position
	if ((toTarget.Dot(vehicle->GetForward()) > 0) && (relativeForward < -0.95f))
	{
		return Seek(_target->GetPositionVector());
	}

	//if target is not in front then it will be cut off
	float lookAheadTime = toTarget.Length() / (vehicle->GetMaxSpeed() + _target->GetCurrentSpeed());

	return Seek(_target->GetPositionVector() + _target->GetVelocity() * lookAheadTime);
}