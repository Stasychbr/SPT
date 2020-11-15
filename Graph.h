#pragma once
#include <vector>
#include <iostream>
#include <utility>
#include <unordered_map>
#include <unordered_set>

using namespace std;
typedef unsigned int uint;

class Graph {
private:
    unordered_map<uint, uint>* _graph = nullptr; //head : (tail, weight)
    vector<uint> _distance;
    unordered_set<uint> _terminals;
    uint _root = UINT_MAX;
    Graph* _spt = nullptr;
    uint _size = 0;
    bool _directed = false;
public:
    Graph(uint size, bool directed);
    bool isDirected();
    void setEdge(uint head, uint tail, uint weight);
    void setTerminal(uint terminal);
    void removeTerminal(uint terminal);
    void setTermsNumber(uint number);
    void setRoot(uint root);
    void removeEdge(uint head, uint tail);
    void print(ostream& outStream = cout);
    void buildSPT(uint root);
    Graph* getSPT();
    vector<uint>& getDistances();
    unordered_set<uint>& getTerminals();
    ~Graph();
};