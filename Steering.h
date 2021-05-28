#pragma once

#pragma warning (disable:4786)

class Vehicle;

class Steering
{
public:
    Steering(Vehicle* veh) : vehicle(veh) {}

    Vector2D CalculateForce();
    
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

    float seekWeight = 5.0f;
    float arriveWeight = 2.0f;
    float fleeWeight = 10.0f;
private:
    // the vehicle using this steering
	Vehicle* vehicle;

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