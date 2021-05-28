#pragma once
#include "DrawableGameObject.h"
#include "Vector2D.h"
#include "imgui/imgui.h"

#include "Steering.h"

#define MAX_SPEED 200

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

	void SetPositionTo(Vector2D positionTo); // a position to move to
	void SetVehiclePosition(Vector2D position); // the current position - this resets positionTo

	Vector2D GetWanderTarget() { return _wanderTarget; }
	void SetWanderTarget(Vector2D newTarget) { _wanderTarget = newTarget; }
protected:
	std::string _name;

	float _maxSpeed;
	float _currentSpeed;

	Vector2D _velocity;

	Vector2D _currentPosition;
	Vector2D _startPosition;
	Vector2D _positionTo;
	Vector2D _lastPosition;

	Vector2D _wanderTarget;

	Steering* _steering;
private:
	void DrawUI();
};

