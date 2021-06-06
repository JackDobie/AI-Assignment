#pragma once
#include <string>
#include "DrawableGameObject.h"
#include "imgui/imgui.h"
#include "Vector2D.h"
#include "Constants.h"
#include "Steering.h"
#include "StateMachine.h"

class Steering;

class Vehicle : public DrawableGameObject
{
protected:
	std::string _name;

	float _maxSpeed;
	float _currentSpeed;

	Vector2D _velocity;

	Vector2D _forward;
	Vector2D _side;

	Vector2D _currentPosition;
	Vector2D _startPosition;
	Vector2D _positionTo;
	Vector2D _lastPosition;

	Vector2D _wanderTarget = Vector2D();

	Steering* _steering;

	// the vehicle to persue
	Vehicle* _otherVehicle = nullptr;

	float _speedFactor;
	float _speedBoostTimer;

	StateMachine* _stateMachine;
public:
	Vehicle(std::string name, Vector2D startPos, float maxSpeed);
	virtual HRESULT initMesh(ID3D11Device* pd3dDevice, wstring texturePath);
	void InitStateMachine(State* startState);

	virtual void Update(float deltaTime);

	Vector2D GetCurrentPosition() { return _currentPosition; }

	float* GetMaxSpeed() { return &_maxSpeed; }
	void SetMaxSpeed(const float maxSpeed) { _maxSpeed = maxSpeed; }

	float GetCurrentSpeed() { return _currentSpeed; }
	void SetCurrentSpeed(const float speed); // a ratio: a value between 0 and 1 (1 being max speed)

	float GetSpeedBoostTimer() { return _speedBoostTimer; }

	Vector2D* GetVelocity() { return &_velocity; }
	void SetVelocity(Vector2D newVelocity) { _velocity = newVelocity; }

	Vector2D GetTarget() { return _positionTo; }
	void SetPositionTo(Vector2D positionTo); // a position to move to
	void SetVehiclePosition(Vector2D position); // the current position - this resets positionTo

	Vector2D GetWanderTarget() { return _wanderTarget; }
	void SetWanderTarget(Vector2D newTarget) { _wanderTarget = newTarget; }

	Vehicle* GetOtherVehicle() { return _otherVehicle; }
	void SetOtherVehicle(Vehicle* otherVeh) { _otherVehicle = otherVeh; }

	Vector2D GetForward() { return _forward; }
	Vector2D GetSide() { return _side; }

	void Boost();

	float GetSpeedFactor() { return _speedFactor; }

	void Reset();

	string GetName() { return _name; }
	void SetName(string newName) { _name = newName; }

	Steering* GetSteering() { return _steering; }

	StateMachine* GetStateMachine() { return _stateMachine; }
};

