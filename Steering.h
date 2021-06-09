#pragma once

#pragma warning (disable:4786)

#include "Vector2D.h"
#include "Constants.h"
#include <vector>
#include "Waypoint.h"
using namespace std;

class Vehicle;

class Steering
{
public:
    Steering(Vehicle* veh);

    Vector2D CalculateForce(float deltaTime);
    
    enum class BehaviourType
    {
        none = 0,
        seek,
        flee,
        arrive,
        wander,
        obstacle_avoidance,
        pursuit
    };

    BehaviourType activeType;
private:
    Vector2D Seek(Vector2D _target);
    float seekWeight = 10.0f;

    Vector2D Flee(Vector2D _target);
    float fleeWeight = 20.0f;

    Vector2D Arrive(Vector2D _target);
    float arriveWeight = 10.0f;

    Vector2D Wander(float deltaTime);

    Vector2D ObstacleAvoidance(Vector2D _target);
    float obstacleAvoidWeight = 25.0f;

    Vector2D Pursuit(Vehicle* _target);
    float pursuitWeight = 2.0f;

    // the vehicle using this steering
    Vehicle* vehicle;

    Vector2D target;
};