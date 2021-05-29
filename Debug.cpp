#include "Debug.h"

void Debug::Print(std::string output)
{
	OutputDebugStringA(output.c_str());
}
void Debug::Print(int output)
{
	char sz[1024] = { 0 };
	sprintf_s(sz, "%d \n", output);
	OutputDebugStringA(sz);
}
void Debug::Print(float output)
{
	char sz[1024] = { 0 };
	sprintf_s(sz, "%f \n", output);
	OutputDebugStringA(sz);
}
void Debug::Print(DirectX::XMFLOAT3 output)
{
	std::string x = std::to_string(output.x);
	std::string y = std::to_string(output.y);
	std::string z = std::to_string(output.z);
	std::string out = "X:" + x + ", Y:" + y + ", Z:" + z + "\n";
	OutputDebugStringA(out.c_str());
}
void Debug::Print(Vector2D output)
{
	std::string x = std::to_string(output.x);
	std::string y = std::to_string(output.y);
	std::string out = "X:" + x + ", Y:" + y + "\n";
	OutputDebugStringA(out.c_str());
}
void Debug::Print(std::string str, DirectX::XMFLOAT3 output)
{
	std::string x = std::to_string(output.x);
	std::string y = std::to_string(output.y);
	std::string z = std::to_string(output.z);
	std::string out = str + " X:" + x + ", Y:" + y + ", Z:" + z + "\n";
	OutputDebugStringA(out.c_str());
}