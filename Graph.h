#pragma once
#include <vector>
#include <iostream>
#include <utility>
#include <unordered_map>

using namespace std;
typedef unsigned int uint;

class Graph {
private:
    unordered_map<uint, uint>* _graph = nullptr; //head : (tail, weight)
    vector<uint> _distance;
    Graph* _spt = nullptr;
    uint _size = 0;
    bool _directed = false;
public:
    Graph(uint size, bool directed);
    void setEdge(uint head, uint tail, uint weight);
    void removeEdge(uint head, uint tail);
    void print(ostream& outStream = cout);
    void buildSPT(uint root);
    Graph* getSPT();
    vector<uint>& getDistances();
    ~Graph();
};