#include "Graph.h"
#include <limits>

Graph::Graph(uint size, bool directed) {
    _graph = new vector<pair<uint, uint>>[size];
    if (!_graph) {
        throw "Not enough heap memory";
    }
    _size = size;
    _directed = directed;
}

void Graph::setEdge(uint head, uint tail, uint weight) {
    if (head < _size && tail < _size) {
        _graph[head].emplace_back(make_pair(tail, weight));
        if (!_directed) {
            _graph[tail].emplace_back(make_pair(head, weight));
        }
    }
}

void Graph::print(FILE* file) {
    char mode = _directed ? 'A' : 'E';
    fprintf(file, "Nodes %i\n", _size);
    for (uint i = 0; i < _size; i++) {
        for (uint j = 0; j < _graph[i].size(); j++) {
            if (!_directed && _graph[i][j].first < i) {
                continue;
            }
            fprintf(file, "%c %i %i %i\n", mode, _graph[i][j].first + 1, i + 1, _graph[i][j].second);
        }
    }
}

Graph::~Graph() {
    delete[] _graph;
}