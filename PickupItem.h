#pragma once
#include "DrawableGameObject.h"
#include "Constants.h"

class PickupItem : public DrawableGameObject
{
public:
    virtual HRESULT initMesh(ID3D11Device* pd3dDevice);

    void Collide();
    void Hide();
    void GetNewPosition();

    std::vector<Vector2D> placeablePositions;
private:
    Vector2D pos;

    bool pickedUp;

    void SetPos(Vector2D newPos);
};