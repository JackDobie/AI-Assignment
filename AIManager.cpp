#include "AIManager.h"
#include "Vehicle.h"
#include "DrawableGameObject.h"
#include "PickupItem.h"
#include "Waypoint.h"

HRESULT AIManager::initialise(ID3D11Device* pd3dDevice)
{
    // create a pickup item ----------------------------------------------

    PickupItem* pPickup = new PickupItem();
    HRESULT hr = pPickup->initMesh(pd3dDevice);
    m_pickups.push_back(pPickup);
    if (FAILED(hr))
        return hr;

    // create the vehicles ------------------------------------------------
    m_pCar = new Vehicle("Car", Vector2D(0.0f,0.0f), 200.0f);
    hr = m_pCar->initMesh(pd3dDevice);
    if (FAILED(hr))
        return hr;

    AICar = new Vehicle("AICar", Vector2D(0.0f, 225.0f), 200.0f);
    hr = AICar->initMesh(pd3dDevice);
    m_pCar->SetOtherVehicle(AICar);
    if (FAILED(hr))
        return hr;

    // create the waypoints
    float xGap = SCREEN_WIDTH / WAYPOINT_RESOLUTION;
    float yGap = SCREEN_HEIGHT / WAYPOINT_RESOLUTION;
    float xStart = -(SCREEN_WIDTH / 2) + (xGap / 2);
    float yStart = -(SCREEN_HEIGHT / 2) + (yGap / 2);

    unsigned int index = 0;
    for (unsigned int j = 0; j < WAYPOINT_RESOLUTION; j++)
    {
        for (unsigned int i = 0; i < WAYPOINT_RESOLUTION; i++)
        {
            Waypoint* wp = new Waypoint();
            hr = wp->initMesh(pd3dDevice, index++);
            wp->setPosition(XMFLOAT3(xStart + (xGap * i), yStart + (yGap * j), 0));
            m_waypoints.push_back(wp);
        }
    }

    return hr;
}

void AIManager::update(const float fDeltaTime)
{
    Waypoint* w = GetWaypoint(5, 1);
    if (w != nullptr)
    {
        w->update(fDeltaTime);
        AddItemToDrawList(w);
    }

    //for (unsigned int i = 0; i < m_waypoints.size(); i++)
    //{
    //    m_waypoints[i]->update(fDeltaTime);
    //    //AddItemToDrawList(m_waypoints[i]); // if you comment this in, it will display the waypoints
    //}

    for (unsigned int i = 0; i < m_pickups.size(); i++)
    {
        m_pickups[i]->update(fDeltaTime);
        AddItemToDrawList(m_pickups[i]);
    }

    m_pCar->Update(fDeltaTime);
    AICar->Update(fDeltaTime);

    checkForCollisions();

    AddItemToDrawList(m_pCar);
    AddItemToDrawList(AICar);
}

void AIManager::mouseUp(int x, int y)
{
    m_pCar->SetPositionTo(Vector2D(x, y));
}

void AIManager::keyPress(WPARAM param)
{
    switch (param)
    {
        case VK_NUMPAD0:
        {
            OutputDebugStringA("0 pressed \n");
            break;
        }
        case VK_NUMPAD1:
        {
            OutputDebugStringA("1 pressed \n");
            break;
        }
        case VK_NUMPAD2:
        {
            break;
        }
        // etc
        default:
            break;
    }
}

bool AIManager::checkForCollisions()
{
    if (m_pickups.size() == 0)
        return false;

    XMVECTOR dummy;

    // the car
    XMVECTOR carPos;
    XMVECTOR carScale;
    XMMatrixDecompose(
        &carScale,
        &dummy,
        &carPos,
        XMLoadFloat4x4(m_pCar->getTransform())
    );

    XMFLOAT3 scale;
    XMStoreFloat3(&scale, carScale);
    BoundingSphere boundingSphereCar;
    XMStoreFloat3(&boundingSphereCar.Center, carPos);
    boundingSphereCar.Radius = scale.x;

    // a pickup - !! NOTE it is only referring the first one in the list !!
    XMVECTOR puPos;
    XMVECTOR puScale;
    XMMatrixDecompose(
        &puScale,
        &dummy,
        &puPos,
        XMLoadFloat4x4(m_pickups[0]->getTransform())
    );

    XMStoreFloat3(&scale, puScale);
    BoundingSphere boundingSpherePU;
    XMStoreFloat3(&boundingSpherePU.Center, puPos);
    boundingSpherePU.Radius = scale.x;

    // test
    if (boundingSphereCar.Intersects(boundingSpherePU))
    {
        OutputDebugStringA("Collision!\n");
        return true;
    }

    return false;
}

Waypoint* AIManager::GetWaypoint(int x, int y)
{
    int index = y * WAYPOINT_RESOLUTION + x;
    Waypoint* w = nullptr;
    if (m_waypoints.size() > index)
    {
        w = m_waypoints[index];
    }
    return w;
}

vector<Waypoint*> AIManager::GetNeighbours(int x, int y)
{
    vector<Waypoint*> neighbours;
    if (y > 0)
    {
        neighbours.push_back(GetWaypoint(x, y - 1)); //top

        if (x > 0)
        {
            neighbours.push_back(GetWaypoint(x - 1, y - 1)); //top left
        }

        if (x < WAYPOINT_RESOLUTION)
        {
            neighbours.push_back(GetWaypoint(x + 1, y - 1)); //top right
        }
    }

    if (x > 0)
    {
        neighbours.push_back(GetWaypoint(x - 1, y)); //left
    }

    if (x < WAYPOINT_RESOLUTION)
    {
        neighbours.push_back(GetWaypoint(x + 1, y)); //right
    }

    if (y < WAYPOINT_RESOLUTION)
    {
        neighbours.push_back(GetWaypoint(x, y + 1)); //right

        if (x > 0)
        {
            neighbours.push_back(GetWaypoint(x - 1, y + 1)); //top right
        }

        if (x < WAYPOINT_RESOLUTION)
        {
            neighbours.push_back(GetWaypoint(x + 1, y + 1)); //bottom right
        }
    }

    return neighbours;
}

void AIManager::DrawUI()
{
    ImGui::Begin("Control window");
    ImGui::Text("Hello world!");
    ImGui::End();

    m_pCar->DrawUI();
}