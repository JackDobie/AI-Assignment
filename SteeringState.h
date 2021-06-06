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
		int* type = (int*)&_vehicle->GetSteering()->activeType;
		type = 0;
	}

	void Update(float deltaTime) override
	{

	}

	void DrawUI() override
	{
		ImGui::Begin((_vehicle->GetName() + " steering controls").c_str());
		ImGui::Text("Steering Type:");
		int* type = (int*)&_vehicle->GetSteering()->activeType;

		if (ImGui::RadioButton("None", type, 0))
			_vehicle->GetVelocity()->Zero();

		ImGui::RadioButton("Seek", type, 1);
		ImGui::SameLine();
		ImGui::RadioButton("Flee", type, 2);
		ImGui::SameLine();
		ImGui::RadioButton("Arrive", type, 3);
		ImGui::SameLine();
		ImGui::RadioButton("Wander", type, 4);
		ImGui::RadioButton("Obstacle Avoidance", type, 5);
		ImGui::SameLine();
		ImGui::RadioButton("Pursuit", type, 6);

		if (ImGui::Button("Reset"))
			_vehicle->Reset();

		ImGui::End();
	}
};