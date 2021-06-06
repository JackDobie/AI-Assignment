#pragma once

#include "State.h"
#include "Steering.h"
#include "Vehicle.h"

class SteeringState : public State
{
public:
	SteeringState(Vehicle* vehicle) : State(vehicle) {}

	void Start() override
	{
		_vehicle->Reset();
	}

	void Exit() override
	{

	}

	void Update(float deltaTime) override
	{

	}

	void DrawUI() override
	{
		ImGui::Begin((_vehicle->GetName() + " details").c_str());
		ImGui::Text("Position:\nX: %.3f, Y: %.3f", _vehicle->GetCurrentPosition().x, _vehicle->GetCurrentPosition().y);
		if (_vehicle->GetSteering()->activeType == Steering::BehaviourType::wander)
		{
			ImGui::Text("Wander target:\nX: %.3f, Y: %.3f", _vehicle->GetWanderTarget().x, _vehicle->GetWanderTarget().y);
		}

		ImGui::Text("Current Speed: %.3f", _vehicle->GetCurrentSpeed());
		ImGui::SliderFloat("Max Speed", _vehicle->GetMaxSpeed(), 0.0f, 400.0f);
		if (_vehicle->GetSpeedBoostTimer() > 0.0f)
		{
			ImGui::Text("Speed boost! Time remaining: %.3f", _vehicle->GetSpeedBoostTimer());
		}

		ImGui::Text("\nSteering Type:");
		int* type = (int*)&_vehicle->GetSteering()->activeType;

		if (ImGui::RadioButton("None", type, 0))
			_vehicle->GetVelocity()->Zero();

		ImGui::RadioButton("Seek", type, 1);
		ImGui::RadioButton("Flee", type, 2);
		ImGui::RadioButton("Arrive", type, 3);
		ImGui::RadioButton("Wander", type, 4);
		ImGui::RadioButton("Obstacle Avoidance", type, 5);
		ImGui::RadioButton("Pursuit", type, 6);

		if (ImGui::Button("Reset"))
			_vehicle->Reset();

		ImGui::End();
	}
};