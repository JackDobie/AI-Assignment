#include "PickupItem.h"

HRESULT	PickupItem::initMesh(ID3D11Device* pd3dDevice)
{
	//setTextureName(L"Resources\\yellow.dds");

	m_scale = XMFLOAT3(20, 20, 1);
	setTextureName(L"Resources\\pickup.dds");

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	//pos.x = SCREEN_WIDTH + 100.0f;
	//pos.y = SCREEN_HEIGHT + 100.0f;
	SetPos(Vector2D(100.0f, 100.0f));

	return hr;
}

void PickupItem::Collide()
{
	GetNewPosition();
}

void PickupItem::GetNewPosition()
{
	int index = rand() % placeablePositions.size();
	SetPos(placeablePositions[index]);
}

void PickupItem::SetPos(Vector2D newPos)
{
	pos = newPos;
	setPosition(XMFLOAT3(pos.x, pos.y, 0.0f));
}