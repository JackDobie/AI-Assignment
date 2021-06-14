#pragma once
#include <vector>
#include <string>
#include "Pathfinder.h"
#include "Vehicle.h"
using namespace std;

class TrackReader
{
public:
	TrackReader()
	{
		waypoints.resize(15, nullptr);
	}

	bool CreateNodes();

	node* GetNodes() { return nodes; }
	vector<node*> GetNodeVector();
	vector<node*> GetWaypoints() { return waypoints; }
private:
	vector<node*> GetNeighbours(int x, int y);

	int mapWidth = 20;
	int mapHeight = 20;
	node* nodes = nullptr;
	vector<node*> waypoints;

	vector<string> map = {
		"xx...xxxxxxxxxxxxxxx",
		"x.O....xxxxxxxxxxxxx",
		"x.........x.xxxxxxxx",
		"..xxxx.......xxxxxxx",
		".xxxxxxx...A...xxxxx",
		".xx...xxxxx.......xx",
		".xx.I...xxxxx......x",
		"N.x......xxxxxxx....",
		"x.xx.x.....xxxxxxx.B",
		"x..x..xx.H.......xx.",
		"xx.x..xxx........xx.",
		"xxMx...xxxxxxxxxGxx.",
		"xx.xx..xxxxxxx...x.C",
		"xx..xx..xxxxx....x.x",
		"xxx.xx...xxxx..xxx.x",
		"xxx..xxJ.xxxxFxxxx.x",
		"xxxx.xx..xxxx...x..x",
		"xxxxL..K.xxxx.E...Dx",
		"xxxxx...xxxxxxx...xx",
		"xxxxxxxxxxxxxxxxxxxx" };
};