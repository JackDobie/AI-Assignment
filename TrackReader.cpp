#include "TrackReader.h"

bool TrackReader::ReadFile(string filePath)
{
	// opens the stream
	ifstream stream(filePath.c_str());
	// cancels if the file cannot be opened
	if (!stream.is_open())
		return false;

	string currentline = "";
	int rows = 0;
	vector<string> map;
	while (getline(stream, currentline))
	{
		map.push_back(currentline);
		rows++;
	}
	mapWidth = currentline.length();
	mapHeight = rows;

	nodes = new node[mapWidth * mapHeight];

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			node* n = &nodes[y * mapWidth + x];

			n->pos = Vector2D(x, y);

			char c = map[y].c_str()[x];

			// avoid setting all cases as not obstacles, only need to set x as obstacle
			n->isObstacle = false;
			switch (c)
			{
			case 'x':
				n->isObstacle = true;
				break;
			case '.':
				break;
			case 'A':
				waypoints[0] = n;
				break;
			case 'B':
				waypoints[1] = n;
				break;
			case 'C':
				waypoints[2] = n;
				break;
			case 'D':
				waypoints[3] = n;
				break;
			case 'E':
				waypoints[4] = n;
				break;
			case 'F':
				waypoints[5] = n;
				break;
			case 'G':
				waypoints[6] = n;
				break;
			case 'H':
				waypoints[7] = n;
				break;
			case 'I':
				waypoints[8] = n;
				break;
			case 'J':
				waypoints[9] = n;
				break;
			case 'K':
				waypoints[10] = n;
				break;
			case 'L':
				waypoints[11] = n;
				break;
			case 'M':
				waypoints[12] = n;
				break;
			case 'N':
				waypoints[13] = n;
				break;
			case 'O':
				waypoints[14] = n;
				break;
			default:
				Debug::Print("Unknown character read from file: " + c);
				break;
			}
		}
	}
	// cancel if any of the waypoints are nullptr
	for (auto n : waypoints)
	{
		if (n == nullptr)
			return false;
	}

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			node* n = &nodes[y * mapWidth + x];
			vector<node*> nodeNeighbours = GetNeighbours(x, y);
			n->neighbours = nodeNeighbours;
		}
	}
	return true;
}

vector<node*> TrackReader::GetNeighbours(int x, int y)
{
	vector<node*> neighbours;
	if (y > 0)
	{
		neighbours.push_back(&nodes[(y - 1) * mapWidth + (x)]); //top

		if (x > 0)
		{
			neighbours.push_back(&nodes[(y - 1) * mapWidth + (x - 1)]); //top left
		}

		if (x < WAYPOINT_RESOLUTION)
		{
			neighbours.push_back(&nodes[(y - 1) * mapWidth + (x + 1)]); //top right
		}
	}

	if (x > 0)
	{
		neighbours.push_back(&nodes[(y) * mapWidth + (x - 1)]); //left
	}

	if (x < WAYPOINT_RESOLUTION)
	{
		neighbours.push_back(&nodes[(y) * mapWidth + (x + 1)]); //right
	}

	if (y < WAYPOINT_RESOLUTION)
	{
		neighbours.push_back(&nodes[(y + 1) * mapWidth + (x)]); //right

		if (x > 0)
		{
			neighbours.push_back(&nodes[(y + 1) * mapWidth + (x - 1)]); //top right
		}

		if (x < WAYPOINT_RESOLUTION)
		{
			neighbours.push_back(&nodes[(y + 1) * mapWidth + (x + 1)]); //bottom right
		}
	}

	return neighbours;
}