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
    unordered_set<uint> _terminals;
    uint _root = UINT_MAX;
    Graph* _spt = nullptr;
    uint _sptLenght = 0;
    uint _size = 0;
    bool _directed = false;
    void removeExcessNodes();
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
    void buildSPT();
    Graph* getSPT();
    uint getSPTLength();
    unordered_set<uint>& getTerminals();
    ~Graph();
};