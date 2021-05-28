#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include <vector>

using namespace std;

class Vehicle;
class DrawableGameObject;
class PickupItem;
class Waypoint;

typedef vector<DrawableGameObject*> vecDrawables;
typedef vector<Waypoint*> vecWaypoints;
typedef vector<PickupItem*> vecPickups;

class AIManager
{
public:

	HRESULT initialise(ID3D11Device* pd3dDevice);
	void	update(const float fDeltaTime);
	void	mouseUp(int x, int y);
	void	keyPress(WPARAM param);

	Waypoint* GetWaypoint(int x, int y);
	vector<Waypoint*> GetNeighbours(int x, int y);

	void DrawUI();

protected:
	bool	checkForCollisions();

private:
	vecWaypoints            m_waypoints;
	vecPickups              m_pickups;
	Vehicle*				m_pCar = nullptr;
	Vehicle*				AICar = nullptr;
	vector<Vehicle*>			vehicles;
};

