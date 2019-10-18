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
    int speed;

    Agent(int id)
    {
        this->id = id;

    }
    Agent() {
        this->id = -1;
        this->speed = 1;
    }
};
struct Point {
    int x;
    int y;
    Point(pair<int, int> p) {
        this->x = p.first;
        this->y = p.second;
    }
    Point() {}
};

struct Node {
    int id;
    Point top;
    Point right;
    Point bot;
    Point left;
    Point center;
    bool free;
    int radius;
    vector<Node> neighbors;
    vector<Edge> edges;
    vector<Agent> agents;
    Node() {free = true; }
};

struct Graph {
    vector<Node> nodes;
    priority_queue<pair<int, pair<int, int>>> closest;
};

#endif // GRAPH_H

