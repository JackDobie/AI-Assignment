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
	float _crashTimer;

	StateMachine* _stateMachine;

	vector<Waypoint*> _waypoints;

	BoundingSphere* _boundingSphere;

	bool _overtaking = false;

	PickupItem* _pickUpItem;

	bool _auto;
	int _autoPosIndex;
	Vector2D _autoPos0 = Vector2D(SCREEN_WIDTH - (SCREEN_WIDTH * 1.5f) + 50.0f, 0);
	Vector2D _autoPos1 = Vector2D(SCREEN_WIDTH - (SCREEN_WIDTH * 0.5f) - 50.0f, 0);
	vector<Vector2D> _autoPos = { _autoPos0, _autoPos1 };
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
	float GetCrashTimer() { return _crashTimer; }

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
	Vector2D GetSide() { return _side; }

	void Boost();

	float GetSpeedFactor() { return _speedFactor; }

	void Reset();

	string GetName() { return _name; }
	void SetName(string newName) { _name = newName; }

	Steering* GetSteering() { return _steering; }

	StateMachine* GetStateMachine() { return _stateMachine; }

	Waypoint* GetWaypoint(int x, int y);
	vector<Waypoint*> GetWaypoints() { return _waypoints; }
	void SetWaypoints(vector<Waypoint*> newWaypoints) { _waypoints = newWaypoints; }

	int _waypointCount = 0;
	int _pathIndex = 0;

	XMFLOAT3 GetScale() { return m_scale; }

	BoundingSphere* GetBoundingSphere() { return _boundingSphere; }

	void Collide();

	bool GetOvertaking() { return _overtaking; }
	void SetOvertaking(bool overtaking) { _overtaking = overtaking; }

	PickupItem* GetPickUpItem() { return _pickUpItem; }
	void SetPickUpItem(PickupItem* pickup) { _pickUpItem = pickup; }

	bool GetAuto() { return _auto; }
	void SetAuto(bool isAuto) { _auto = isAuto; }
};

