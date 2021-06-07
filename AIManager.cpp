#include "AIManager.h"
#include "Vehicle.h"
#include "DrawableGameObject.h"
#include "PickupItem.h"
#include "Waypoint.h"

HRESULT AIManager::initialise(ID3D11Device* pd3dDevice)
{
    HRESULT hr;

    // create the waypoints
    float xGap = SCREEN_WIDTH / WAYPOINT_RESOLUTION;
    float yGap = SCREEN_HEIGHT / WAYPOINT_RESOLUTION;
    float xStart = -(SCREEN_WIDTH / 2) + (xGap / 2);
    float yStart = -(SCREEN_HEIGHT / 2) + (yGap / 2);

    vector<Vector2D> placeablePositions;
    unsigned int index = 0;
    for (unsigned int j = 0; j < WAYPOINT_RESOLUTION; j++)
    {
        for (unsigned int i = 0; i < WAYPOINT_RESOLUTION; i++)
        {
            Waypoint* wp = new Waypoint();
            hr = wp->initMesh(pd3dDevice, index++);
            if (FAILED(hr))
                return hr;
            wp->setPosition(XMFLOAT3(xStart + (xGap * i), yStart + (yGap * j), 0));
            m_waypoints.push_back(wp);

            if (wp->isOnTrack())
            {
                if (wp->isCheckpoint())
                {
                    placeablePositions.push_back(wp->GetPositionVector());
                }
            }
        }
    }

    // create a pickup item ----------------------------------------------
    pPickup = new PickupItem();
    hr = pPickup->initMesh(pd3dDevice);
    if (FAILED(hr))
        return hr;
    pPickup->placeablePositions = placeablePositions;
    pPickup->GetNewPosition();

    // create the vehicles ------------------------------------------------
    pCar = new Vehicle("Car", Vector2D(0.0f,0.0f), 200.0f);
    hr = pCar->initMesh(pd3dDevice, L"Resources\\car_blue.dds");
    if (FAILED(hr))
        return hr;
    _steeringState = new SteeringState(pCar);
    _pathfindingState = new PathfindingState(pCar);
    pCar->SetWaypoints(m_waypoints);
    pCar->InitStateMachine(_steeringState);

    /*AICar = new Vehicle("AICar", Vector2D(0.0f, 225.0f), 200.0f);
    hr = AICar->initMesh(pd3dDevice, L"Resources\\car_red.dds");
    if (FAILED(hr))
        return hr;
    AICar->SetWaypoints(m_waypoints);
    AICar->InitStateMachine(new PathfindingState(nullptr));
    pCar->SetOtherVehicle(AICar);*/

    return hr;
}

void AIManager::update(const float fDeltaTime)
{
    /*Waypoint* w = GetWaypoint(5, 1);
    if (w != nullptr)
    {
        w->update(fDeltaTime);
        AddItemToDrawList(w);
    }*/

    if (displayWaypoints)
    {
        for (Waypoint* w : m_waypoints)
        {
            w->update(fDeltaTime);
            AddItemToDrawList(w);
        }
    }

    for (Waypoint* w : m_waypoints)
    {
        /*if (w->isOnTrack())
        {
            if (w->GetPositionVector() != pPickup->GetPositionVector())
            {
                w->update(fDeltaTime);
                AddItemToDrawList(w);
            }
        }*/
        if (w->draw)
        {
            w->update(fDeltaTime);
            AddItemToDrawList(w);
        }
    }

    pPickup->update(fDeltaTime);
    AddItemToDrawList(pPickup);

    pCar->Update(fDeltaTime);
    //AICar->Update(fDeltaTime);

    checkForCollisions();

    AddItemToDrawList(pCar);
    //AddItemToDrawList(AICar);
}

void AIManager::mouseUp(int x, int y)
{
    // only click to move if steering
    if(pCar->GetStateMachine()->GetCurrentState()->GetCurrentState() == 0)
        pCar->SetPositionTo(Vector2D(x, y));
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

    XMVECTOR dummy;

    // the car
    XMVECTOR carPos;
    XMVECTOR carScale;
    XMMatrixDecompose(
        &carScale,
        &dummy,
        &carPos,
        XMLoadFloat4x4(pCar->getTransform())
    );

    XMFLOAT3 scale;
    XMStoreFloat3(&scale, carScale);
    BoundingSphere boundingSphereCar;
    XMStoreFloat3(&boundingSphereCar.Center, carPos);
    boundingSphereCar.Radius = scale.x;

    // check for collision with pickups
    XMVECTOR puPos;
    XMVECTOR puScale;
    XMMatrixDecompose(
        &puScale,
        &dummy,
        &puPos,
        XMLoadFloat4x4(pPickup->getTransform())
    );

    XMStoreFloat3(&scale, puScale);
    BoundingSphere boundingSpherePU;
    XMStoreFloat3(&boundingSpherePU.Center, puPos);
    boundingSpherePU.Radius = scale.x;

    // if the two bounding spheres collide
    if (boundingSphereCar.Intersects(boundingSpherePU))
    {
        pPickup->Collide();
        pCar->Boost();
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
    
    static int radioctrl = 0;
    if (ImGui::RadioButton("Steering", &radioctrl, 0))
    {
        pCar->GetStateMachine()->ChangeState(_steeringState);
    }
    if (ImGui::RadioButton("Pathfinding", &radioctrl, 1))
    {
        pCar->GetStateMachine()->ChangeState(_pathfindingState);
    }

    if (ImGui::Button("Toggle waypoints"))
    {
        displayWaypoints = !displayWaypoints;
    }

    ImGui::End();

    pCar->DrawUI();
}