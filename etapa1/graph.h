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

    Agent(int id)
    {
        this->id = id;

    }
    Agent() {
        this->id = -1;
    }
};

struct Node {
    int id;
    int x;
    int y;
    vector<Node> neighbors;
    vector<Edge> edges;
    bool free = true;
    };

struct Graph {
    vector<Node> nodes;
    priority_queue<pair<int, pair<int, int>>> closest;
};

#endif // GRAPH_H
