#pragma once
#include <vector>
#include <iostream>
#include <utility>

using namespace std;
typedef unsigned int uint;

class Graph {
private:
    vector<pair<uint, uint>>* _graph = nullptr;
    vector<uint> _distance;
    Graph* _spt = nullptr;
    uint _size = 0;
    bool _directed = false;
public:
    Graph(uint size, bool directed);
    void setEdge(uint head, uint tail, uint weight);
    void removeEdge(uint head, uint tail);
    void print(FILE* file = stdout);
    void buildSPT(uint root);
    Graph* getSPT();
    vector<uint>& getDistances();
    ~Graph();
};