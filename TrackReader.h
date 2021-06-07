#pragma once
#include <vector>
#include <fstream>
#include <string>
#include "Pathfinder.h"
#include "Vehicle.h"
#include "Debug.h"
using namespace std;

class TrackReader
{
public:
	TrackReader()
	{
		waypoints.resize(15, nullptr);
	}

	bool ReadFile(string filePath);

	node* GetNodes() { return nodes; }
	vector<node*> GetWaypoints() { return waypoints; }
private:
	vector<node*> GetNeighbours(int x, int y);

	int mapWidth = 0;
	int mapHeight = 0;
	node* nodes = nullptr;
	vector<node*> waypoints;
	//node* waypoints[15] = {};
};