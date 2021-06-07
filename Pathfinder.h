#pragma once

#include <list>
#include <vector>
#include "Vector2D.h"
#include "Constants.h"
using namespace std;

struct node
{
    // is this node an obstacle?
    bool isObstacle = false;
    // has this node been visited previously?
    bool visited = false;
    // distance to goal
    float globalGoal = FLT_MAX;
    float localGoal = FLT_MAX;
    // position in 2d space
    Vector2D pos = Vector2D();
    // connecting nodes
    vector<node*> neighbours;
    // node connected to this node that has the shortest path
    node* parent = nullptr;
};

class Pathfinder
{
public:
    Pathfinder() {};

    void FindPath(node* startNode, node* destination);

    void Reset();

    vector<node*> GetNodePath() { return _nodePath; }
private:
    vector<node*> _nodePath;
};