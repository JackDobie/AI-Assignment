#pragma once

#include <string>
#include <Windows.h>
#include <DirectXMath.h>
#include "Vector2D.h"

class Debug
{
public:
	Debug() {};
	static void Print(std::string output);
	static void Print(int output);
	static void Print(float output);
	static void Print(DirectX::XMFLOAT3 output);
	static void Print(Vector2D output);
	static void Print(std::string str, DirectX::XMFLOAT3 output);
};