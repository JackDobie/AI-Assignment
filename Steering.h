#pragma once

#pragma warning (disable:4786)

#include "Vehicle.h"

class Steering
{
public:
	Steering(Vehicle* veh);

    void CalculateForce();
    
    enum class BehaviourType
    {
        none,
        seek,
        flee,
        arrive,
        wander,
        obstacle_avoidance,
        pursuit
    };

    BehaviourType activeType;

private:
    // the vehicle using this steering
	Vehicle* vehicle;

    Vector2D steeringForce;
    Vector2D target;
private:
    bool IsActive(BehaviourType type) { return (type == activeType); }

    Vector2D Seek(Vector2D _target);

    Vector2D Arrive(Vector2D _target);

    Vector2D Flee(Vector2D _target);

    Vector2D Pursuit(Vector2D* _target);

    Vector2D Wander();
    void NewWanderTarget();

    Vector2D ObstacleAvoidance();
};