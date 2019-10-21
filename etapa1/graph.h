#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <iostream>
#include <queue>
#include <map>
using namespace std;


struct Edge {
    vector<pair<int, int>> line;
    bool operator==(Edge e) {
        return (this->line == e.line);
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
    bool operator==(Point p) {
        return (p.x == this->x && p.y == this->y);
    }
};

struct Lure {
    Point pos;
    bool active;
    Lure(Point pos) {
        this->pos = pos;
    }
    Lure(){this->active = false;}
    void activate(){
        this->active = true;
    }
};

struct Node {
    int id;
    Point top;
    Point right;
    Point bot;
    Point left;
    Point center;
    int radius;
    vector<pair<Node, Edge>> adj;
    Node() {}
};

struct Agent {
    int id;
    int speed;
    Point pos;
    vector<pair<Node, Edge>> adj;

    Agent(int id, Point pos, vector<pair<Node, Edge>>  adj)
    {
        this->id = id;
        this->pos = pos;
        this->adj = adj;

    }
    Agent() {
        this->id = -1;
        this->speed = 1;
    }
};

struct Graph {
    vector<Node> nodes;
    vector<Agent> agents;
    Lure lure;
    priority_queue<pair<int, pair<int, int>>> closest;
};

#endif // GRAPH_H

