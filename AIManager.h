#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include <vector>
#include "imgui/imgui.h"
#include "main.h"
#include "Debug.h"
#include "SteeringState.h"
#include "PathfindingState.h"

using namespace std;

class Vehicle;
class DrawableGameObject;
class PickupItem;
class Waypoint;

typedef vector<DrawableGameObject*> vecDrawables;
typedef vector<Waypoint*> vecWaypoints;

class AIManager
{
public:

	HRESULT					initialise(ID3D11Device* pd3dDevice);
	void					update(const float fDeltaTime);
	void					mouseUp(int x, int y);
	void					keyPress(WPARAM param);

	Waypoint*				GetWaypoint(int x, int y);
	vector<Waypoint*>		GetNeighbours(int x, int y);

	void					DrawUI();
private:
	bool					checkForCollisions();

	vecWaypoints            m_waypoints;
	PickupItem*				pPickup;
	Vehicle*				pCar = nullptr;
	Vehicle*				AICar = nullptr;

	bool					displayWaypoints = false;

	SteeringState*			_steeringState;
	PathfindingState*		_pathfindingState;
};

