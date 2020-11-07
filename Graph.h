#pragma once
#include <vector>
#include <iostream>
using namespace std;
typedef unsigned int uint;

class Graph {
private:
    vector<vector<uint>> _matrix;
public:
    Graph(uint size);
    void setArc(uint head, uint tail, uint weight);
    void print(FILE* file = stdout);
};

