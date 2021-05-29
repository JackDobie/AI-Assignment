#pragma once
#include <string>
#include "DrawableGameObject.h"
#include "imgui/imgui.h"
#include "Vector2D.h"
#include "Constants.h"

class Steering;

class Vehicle : public DrawableGameObject
{
public:
	Vehicle(std::string name, Vector2D startPos, float maxSpeed);

	virtual HRESULT initMesh(ID3D11Device* pd3dDevice);
	virtual void Update(const float deltaTime);

	float GetMaxSpeed() { return _maxSpeed; }
	void SetMaxSpeed(const float maxSpeed) { _maxSpeed = maxSpeed; }

	float GetCurrentSpeed() { return _currentSpeed; }
	void SetCurrentSpeed(const float speed); // a ratio: a value between 0 and 1 (1 being max speed)

	Vector2D GetVelocity() { return _velocity; }
	void SetVelocity(Vector2D newVelocity) { _velocity = newVelocity; }

	Vector2D GetTarget() { return _positionTo; }
	void SetPositionTo(Vector2D positionTo); // a position to move to
	void SetVehiclePosition(Vector2D position); // the current position - this resets positionTo

	Vector2D GetWanderTarget() { return _wanderTarget; }
	void SetWanderTarget(Vector2D newTarget) { _wanderTarget = newTarget; }

	void DrawUI();

	Vehicle* GetOtherVehicle() { return _otherVehicle; }
	void SetOtherVehicle(Vehicle* otherVeh) { _otherVehicle = otherVeh; }

	Vector2D GetForward() { return _forward; }
protected:
	std::string _name;

	float _maxSpeed;
	float _currentSpeed;

	Vector2D _velocity;

	Vector2D _forward;

	Vector2D _currentPosition;
	Vector2D _startPosition;
	Vector2D _positionTo;
	Vector2D _lastPosition;

	Vector2D _wanderTarget = Vector2D();

	Steering* _steering;

	// the vehicle to persue
	Vehicle* _otherVehicle = nullptr;
};

