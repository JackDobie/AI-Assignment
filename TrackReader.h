#pragma once
#include <vector>
#include <fstream>
#include <string>
#include "Pathfinder.h"
#include "Vehicle.h"
using namespace std;

class TrackReader
{
public:
	TrackReader(Vehicle* v) : vehicle(v) {}

	bool ReadFile(string filePath);
private:
	int mapWidth = 0;
	int mapHeight = 0;
	node* nodes = nullptr;
	node* waypoints[15] = {};

	Vehicle* vehicle;
};