#include "Vehicle.h"

Vehicle::Vehicle(std::string name, Vector2D startPos, float maxSpeed) : _name(name), _maxSpeed(maxSpeed)
{
	SetVehiclePosition(startPos);
}

HRESULT	Vehicle::initMesh(ID3D11Device* pd3dDevice)
{
	m_scale = XMFLOAT3(30, 20, 1);
	setTextureName(L"Resources\\car_blue.dds");

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	_maxSpeed = MAX_SPEED;
	_currentSpeed = _maxSpeed;
	SetVehiclePosition(Vector2D(0, 0));

	_lastPosition = Vector2D(0, 0);

	return hr;
}

void Vehicle::Update(const float deltaTime)
{
	// consider replacing with force based acceleration / velocity calculations
	Vector2D vecTo = _positionTo - _currentPosition;
	float velocity = deltaTime * _currentSpeed;

	float length = (float)vecTo.Length();
	// if the distance to the end point is less than the car would move, then only move that distance.
	if (length > 0) {
		vecTo.Normalize();
		if(length > velocity)
			vecTo *= velocity;
		else
			vecTo *= length;

		_currentPosition += vecTo;
	}

	// rotate the object based on its last & current position
	Vector2D diff = _currentPosition - _lastPosition;
	if (diff.Length() > 0) { // if zero then don't update rotation
		diff.Normalize();
		m_radianRotation = atan2f(diff.y, diff.x); // this is used by DrawableGameObject to set the rotation
	}
	_lastPosition = _currentPosition;

	// set the current poistion for the drawablegameobject
	setPosition(XMFLOAT3((float)_currentPosition.x, (float)_currentPosition.y, 0));

	DrawableGameObject::update(deltaTime);
}

// a ratio: a value between 0 and 1 (1 being max speed)
void Vehicle::SetCurrentSpeed(const float speed)
{
	_currentSpeed = _maxSpeed * speed;
	_currentSpeed = max(0, _currentSpeed);
	_currentSpeed = min(1, _currentSpeed);
}

// a position to move to
void Vehicle::SetPositionTo(Vector2D position)
{
	_startPosition = _currentPosition;
	_positionTo = position;
}

// the current position
void Vehicle::SetVehiclePosition(Vector2D position)
{
	_currentPosition = position;
	_positionTo = position;
	_startPosition = position;
	setPosition(XMFLOAT3((float)position.x, (float)position.y, 0));
}

void Vehicle::DrawUI()
{
	ImGui::Begin((_name + " details").c_str());
	ImGui::Text("Position: ", _currentPosition.x, _currentPosition.y);

	ImGui::Text("\nSteering Type:");
	static int e = 0;
	if (ImGui::RadioButton("None", &e, 0))
	{
		_steering->activeType = Steering::BehaviourType::none;
	}
	if(ImGui::RadioButton("Seek", &e, 1))
	{
		_steering->activeType = Steering::BehaviourType::seek;
	}
	if(ImGui::RadioButton("Flee", &e, 2))
	{
		_steering->activeType = Steering::BehaviourType::flee;
	}
	if(ImGui::RadioButton("Arrive", &e, 3))
	{
		_steering->activeType = Steering::BehaviourType::arrive;
	}
	if (ImGui::RadioButton("Wander", &e, 4))
	{
		_steering->activeType = Steering::BehaviourType::wander;
		ImGui::Text("Wander target: ", _wanderTarget.x, _wanderTarget.y);
	}
	if(ImGui::RadioButton("Obstacle Avoidance", &e, 5))
	{
		_steering->activeType = Steering::BehaviourType::obstacle_avoidance;
	}
	if(ImGui::RadioButton("Pursuit", &e, 6))
	{
		_steering->activeType = Steering::BehaviourType::pursuit;
	}

	ImGui::End();
}