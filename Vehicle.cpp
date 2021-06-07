#include "Vehicle.h"
#include "Steering.h"

Vehicle::Vehicle(std::string name, Vector2D startPos, float maxSpeed) : _name(name), _startPosition(startPos), _maxSpeed(maxSpeed)
{
	SetVehiclePosition(startPos);
	_steering = new Steering(this);
	_currentSpeed = _maxSpeed;
	_lastPosition = startPos;
	_speedFactor = 1.0f;
}

HRESULT	Vehicle::initMesh(ID3D11Device* pd3dDevice, wstring texturePath)
{
	m_scale = XMFLOAT3(30, 20, 1);
	setTextureName(texturePath);

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	return hr;
}

void Vehicle::InitStateMachine(State* startState)
{
	if (startState != nullptr)
		_stateMachine = new StateMachine(startState);
}

void Vehicle::Update(float deltaTime)
{
	_stateMachine->Update(deltaTime);

	if (_speedFactor != 1.0f)
	{
		if (_speedBoostTimer > 0.0f)
		{
			_speedBoostTimer -= deltaTime;
		}
		else
		{
			_speedBoostTimer = 0.0f;
			_speedFactor = 1.0f;
		}
	}

	//calculate acceleration based on steering force
	Vector2D acceleration = _steering->CalculateForce(deltaTime);
	//add acceleration to velocity
	_velocity += acceleration * deltaTime;
	_velocity *= _speedFactor;
	//stops velocity from exceeding max speed
	_velocity.Truncate(_maxSpeed * _speedFactor);
	_currentSpeed = _velocity.Length();
	//add to position with velocity
	_currentPosition += _velocity * deltaTime;

	// rotate the object based on its last & current position
	Vector2D diff = _currentPosition - _lastPosition;
	// if zero then don't update rotation
	if (diff.Length() > 0) 
	{
		diff.Normalize();
		m_radianRotation = atan2f(diff.y, diff.x); // this is used by DrawableGameObject to set the rotation
	}
	_lastPosition = _currentPosition;

	_forward = Vector2D(cosf(m_radianRotation), sinf(m_radianRotation));
	_side = _forward.Perp();

	//loop around screen
	int Wmin = (SCREEN_WIDTH * 0.5f) - SCREEN_WIDTH;
	int Wmax = SCREEN_WIDTH - (SCREEN_WIDTH * 0.5f);
	int Hmin = (SCREEN_HEIGHT * 0.5f) - SCREEN_HEIGHT;
	int Hmax = SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.5f);

	if (_currentPosition.x < Wmin)
		_currentPosition.x = Wmax;
	else if (_currentPosition.x > Wmax)
		_currentPosition.x = Wmin;

	if (_currentPosition.y < Hmin)
		_currentPosition.y = Hmax;
	else if (_currentPosition.y > Hmax)
		_currentPosition.y = Hmin;

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
	ImGui::Text("Position:\nX: %.3f, Y: %.3f", _currentPosition.x, _currentPosition.y);
	if (_steering->activeType == Steering::BehaviourType::wander)
	{
		ImGui::Text("Wander target:\nX: %.3f, Y: %.3f", _wanderTarget.x, _wanderTarget.y);
	}

	ImGui::Text("Current Speed: %.3f", _currentSpeed);
	ImGui::SliderFloat("Max Speed", &_maxSpeed, 0.0f, 400.0f);
	if (_speedBoostTimer > 0.0f)
	{
		ImGui::Text("Speed boost! Time remaining: %.3f", _speedBoostTimer);
	}
	ImGui::End();

	_stateMachine->DrawUI();
}

void Vehicle::Boost()
{
	_speedFactor = 1.3f;
	_speedBoostTimer = 3.0f;
}

void Vehicle::Reset()
{
	SetVehiclePosition(_startPosition);
	_velocity.Zero();
	_steering->activeType = Steering::BehaviourType::none;
	_speedFactor = 1.0f;
}

Waypoint* Vehicle::GetWaypoint(int x, int y)
{
	y = WAYPOINT_RESOLUTION - y;
	int index = y * WAYPOINT_RESOLUTION + x;
	Waypoint* w = nullptr;
	if (_waypoints.size() > index)
	{
		w = _waypoints[index];
	}
	return w;
}