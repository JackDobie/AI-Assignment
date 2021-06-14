#include "Steering.h"
#include "Vehicle.h"
#include "Debug.h"

Steering::Steering(Vehicle* veh) : vehicle(veh)
{
	activeType = BehaviourType::none;
}

Vector2D Steering::CalculateForce(float deltaTime)
{
	Vector2D steeringForce;

	switch (activeType)
	{
	case BehaviourType::seek:
		steeringForce = Seek(vehicle->GetTarget()) * seekWeight;
		break;
	case BehaviourType::flee:
		steeringForce = Flee(vehicle->GetOtherVehicle()->GetPositionVector()) * fleeWeight;
		break;
	case BehaviourType::arrive:
		steeringForce = Arrive(vehicle->GetTarget()) * arriveWeight;
		break;
	case BehaviourType::wander:
		steeringForce = Wander(deltaTime);
		break;
	case BehaviourType::obstacle_avoidance:
		steeringForce = ObstacleAvoidance(vehicle->GetTarget()) * obstacleAvoidWeight;
		break;
	case BehaviourType::pursuit:
		steeringForce = Pursuit(vehicle->GetOtherVehicle()) * pursuitWeight;
		break;
	default:
		steeringForce = Vector2D();
	}

	// divide the max speed by 200 to get how much faster or slower the car is to the default
	float turnFactor = *vehicle->GetMaxSpeed() / 200.0f;
	// multiply steeringforce by turnfactor to speed up or slow down turning with speed
	return steeringForce * turnFactor;
}

Vector2D Steering::Seek(Vector2D _target)
{
	// returns a steering force that directs the vehicle towards the target
	Vector2D desiredVelocity = Vec2DNormalize(_target - vehicle->GetPositionVector()) * *vehicle->GetMaxSpeed();
	return (desiredVelocity - vehicle->GetVelocity());
}

Vector2D Steering::Flee(Vector2D _target)
{
	float dist = (_target - vehicle->GetPositionVector()).Length();
	if (dist < 400.0f)
	{
		//similar to seek but target and position are swapped
		Vector2D desiredVelocity = Vec2DNormalize(vehicle->GetPositionVector() - _target) * *vehicle->GetMaxSpeed();
		Vector2D out = desiredVelocity - vehicle->GetVelocity();
		return out;
	}
	else
	{
		return Vector2D(-vehicle->GetVelocity().x, -vehicle->GetVelocity().y);
	}
}

Vector2D Steering::Arrive(Vector2D _target)
{
	//calculate direction and distance to target
	Vector2D dir = _target - vehicle->GetPositionVector();
	float dist = dir.Length();
	if (dist > 0.1f || vehicle->GetCurrentSpeed() > 0.0001f)
	{
		//decrease speed with distance
		float speed = dist / 1.5f;
		//ensure speed does not exceed max speed
		speed = min(speed, *vehicle->GetMaxSpeed());
		Vector2D desiredVelocity = dir * speed / dist;
		return (desiredVelocity - vehicle->GetVelocity());
	}
	//if distance is less than or equal to 0, do not return a force
	return Vector2D();
}

Vector2D Steering::Wander(float deltaTime)
{
	float jitter = 60.0f * deltaTime;

	Vector2D wanderTarget = vehicle->GetWanderTarget();
	//add a small random vector to the target position
	wanderTarget += Vector2D(((rand()) / (RAND_MAX + 1.0)) - ((rand()) / (RAND_MAX + 1.0)) * jitter,
							((rand()) / (RAND_MAX + 1.0)) - ((rand()) / (RAND_MAX + 1.0)) * jitter);
	wanderTarget.Normalize();
	//update the wander target
	vehicle->SetWanderTarget(wanderTarget);
	//use wandertarget as an offset from position
	Vector2D target = wanderTarget + vehicle->GetPositionVector();
	//move towards target
	return Seek(target);
}

Vector2D Steering::ObstacleAvoidance(Vector2D _target)
{
	// creates a box in front of the vehicle to detect obstacles
	float minDetectionBoxLength = vehicle->GetScale().x;
	float detectionBoxLength = minDetectionBoxLength + (vehicle->GetCurrentSpeed() / (*vehicle->GetMaxSpeed() * vehicle->GetSpeedFactor()) * minDetectionBoxLength);

	Vector2D vehPos = vehicle->GetPositionVector();
	Vehicle* otherVeh = vehicle->GetOtherVehicle();

	Vehicle* closestObstacle = nullptr;
	Vector2D closestObstacleLocalPos = Vector2D();

	float avoidScale = 1.1f;

	float aRadius = vehicle->GetScale().x * avoidScale;
	float bRadius = vehicle->GetOtherVehicle()->GetScale().x * avoidScale;

	Vector2D otherPos = otherVeh->GetPositionVector();

	Vector2D localPos;
	localPos.x = (vehicle->GetForward().x * otherPos.x) + (vehicle->GetForward().y * otherPos.y) + (-vehPos.Dot(vehicle->GetForward()));
	localPos.y = (vehicle->GetSide().x * otherPos.x) + (vehicle->GetSide().y * otherPos.y) + (-vehPos.Dot(vehicle->GetSide()));

	float expandedRadius = aRadius + bRadius;

	//if local position has a negative x then it must be behind the vehicle, and can be ignored
	if (localPos.x >= 0.0f)
	{
		if (fabs(localPos.y) < expandedRadius)
		{
			if (localPos.LengthSq() < expandedRadius * expandedRadius)
			{
				closestObstacle = otherVeh;
				closestObstacleLocalPos = localPos;
			}
		}
	}
	if (closestObstacle != nullptr)
	{
		Vector2D steeringForce = Vector2D();

		double multiplier = 1.0 + (detectionBoxLength - closestObstacleLocalPos.x) / detectionBoxLength;

		steeringForce.y = bRadius - closestObstacleLocalPos.y * multiplier;

		double brakeWeight = 0.2;

		steeringForce.x = (bRadius - closestObstacleLocalPos.x) * brakeWeight;

		Vector2D out;
		out.x = (vehicle->GetForward().x * steeringForce.x) + (vehicle->GetSide().x * steeringForce.y);
		out.y = (vehicle->GetForward().y * steeringForce.x) + (vehicle->GetSide().y * steeringForce.y);

		// if there is no steering force, do not return and go to the next waypoint
		return out;
	}

	// seek to the mouse click when not colliding
	return Seek(_target);
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
	float lookAheadTime = toTarget.Length() / (*vehicle->GetMaxSpeed() + _target->GetCurrentSpeed());

	return Seek(_target->GetPositionVector() + _target->GetVelocity() * lookAheadTime);
}