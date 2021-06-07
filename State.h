#pragma once

// forward declare as state will be used by vehicle
class Vehicle;

class State
{
public:
	State(Vehicle* vehicle) { _vehicle = vehicle; }

	virtual void Start() = 0;
	virtual void Exit() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void DrawUI() = 0;
	virtual int GetCurrentState() = 0;
protected:
	Vehicle* _vehicle;
};