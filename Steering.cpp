#include "Steering.h"

Steering::Steering(Vehicle* veh)
{
	vehicle = veh;
}

void Steering::CalculateForce()
{
	steeringForce.Zero();

	switch (activeType)
	{
	case BehaviourType::none:
		break;
	case BehaviourType::seek:
		//steeringForce += Seek()
		break;
	case BehaviourType::flee:
		//steeringForce += Flee()
		break;
	case BehaviourType::arrive:
		//steeringForce += Flee()
		break;
	case BehaviourType::wander:
		steeringForce += Wander();
		break;
	case BehaviourType::obstacle_avoidance:
		steeringForce += ObstacleAvoidance();;
		break;
	case BehaviourType::pursuit:
		//steeringForce += Pursuit()
		break;
	}
}