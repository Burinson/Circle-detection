#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <iostream>
#include <queue>
using namespace std;


struct Edge {
    vector<pair<int, int>> line;
};

struct Agent {
    int id;
    int x;
    int y;
    Agent(int id, int x, int y)
    {
        this->id = id;
        this->x = x;
        this->y = y;
    }
    Agent() {
        this->id = -1;
    };
};

struct Node {
    int id;
    int x;
    int y;
    vector<Node> neighbors;
    vector<Edge> edges;
    bool free = true;
    Agent agent;
    };

struct Graph {
    vector<Node> nodes;
    priority_queue<pair<int, pair<int, int>>> closest;
};

#endif // GRAPH_H
