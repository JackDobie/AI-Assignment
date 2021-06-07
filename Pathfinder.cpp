#include "Pathfinder.h"

void Pathfinder::FindPath(node* startNode, node* destination)
{
	Reset();

	// the distance between two nodes
	auto distance = [](node* a, node* b)
	{
		return Vec2DDistance(a->pos, b->pos);
	};

	node* currentNode = new node();
	currentNode->localGoal = 0.0f;
	currentNode->globalGoal = distance(startNode, destination);
	currentNode->isObstacle = startNode->isObstacle;
	currentNode->visited = startNode->visited;
	currentNode->neighbours = startNode->neighbours;

	list<node*> untestedNodes;
	untestedNodes.push_back(currentNode);
	while (!untestedNodes.empty())
	{
		// sort all remaining untested nodes by distance to destination
		untestedNodes.sort([](const node* lhs, const node* rhs) {return lhs->globalGoal < rhs->globalGoal; });

		// pop the top node if it has been visited
		while (!untestedNodes.empty() && untestedNodes.front()->visited)
			untestedNodes.pop_front();

		// break if the last node was just popped
		if (untestedNodes.empty())
			break;

		// set the current node to the one closest to the destination
		currentNode = untestedNodes.front();
		currentNode->visited = true;

		// add neighbours that have not been visited and are not obstacles
		for (auto neighbour : currentNode->neighbours)
		{
			if (!neighbour->visited && !neighbour->isObstacle)
			{
				untestedNodes.push_back(neighbour);
			}

			// calculate the potential lowest parent distance
			float possibleLowerGoal = currentNode->localGoal + distance(currentNode, neighbour);

			// if the distance between current node and neighbour is lower than the neighbour's current localgoal,
			// then it would be faster to get to neighbour from currentnode instead of what it had before
			if (possibleLowerGoal < neighbour->localGoal)
			{
				neighbour->parent = currentNode;
				neighbour->localGoal = possibleLowerGoal;
				neighbour->globalGoal = neighbour->localGoal + distance(neighbour, destination);
			}
		}
	}

	// get the parent of the final node and go back to the start node to find the correct path
	// put them all into a vector so that they can be compared to get the direction

	if (destination->parent != nullptr)
	{
		node* n = destination;
		while (n->parent != nullptr)
		{
			_nodePath.push_back(n);
			n = n->parent;
		}

		// reverse the vector to get the nodes in order from start to end
		reverse(_nodePath.begin(), _nodePath.end());
	}
}

void Pathfinder::Reset()
{
	for (node* n : _nodePath)
	{
		n->parent = nullptr;
		n->globalGoal = FLT_MAX;
		n->localGoal = FLT_MAX;
		n->visited = false;
	}
	_nodePath.clear();
}