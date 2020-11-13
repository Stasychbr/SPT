#include "Graph.h"
#include <limits>
#include <queue>
#include <functional>

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

void Graph::removeEdge(uint head, uint tail) {
    if (head < _size && tail < _size) {
        for (auto it = _graph[head].begin(); it != _graph[head].end(); it++) {
            if ((*it).first == tail) {
                _graph[head].erase(it); 
                break;
            }
        }
        if (!_directed) {
            for (auto it = _graph[tail].begin(); it != _graph[tail].end(); it++) {
                if ((*it).first == head) {
                    _graph[tail].erase(it);
                    return;
                }
            }
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

void Graph::buildSPT(uint root) {
    vector<uint> lastPath(_size, UINT_MAX);
    auto comparator = [](pair<uint, uint> e1, pair<uint, uint> e2) { return e1.second > e2.second; };
    priority_queue <pair<uint, uint>, vector<pair<uint, uint>>, decltype(comparator)> q(comparator);
    delete _spt;
    _spt = new Graph(_size, _directed);
    _distance.resize(_size);
    for (auto& n : _distance) {
        n = UINT_MAX;
    }
    if (!_spt || _distance.size() != _size || lastPath.size() != _size) {
        return;
    }
    _distance[root] = 0;
    q.emplace(make_pair(root, 0));
    while (!q.empty()) {
        auto edge = q.top();
        q.pop();
        uint v = edge.first;
        uint dist = edge.second;
        if (dist <= _distance[v]) {
            for (uint i = 0; i < _graph[v].size(); i++) {
                uint u = _graph[v][i].first;
                uint weight = _graph[v][i].second;
                if (_distance[u] > _distance[v] + weight) {
                    if (_distance[u] != UINT_MAX) {
                        _spt->removeEdge(lastPath[u], u);
                    }
                    _distance[u] = _distance[v] + weight;
                    lastPath[u] = v;
                    _spt->setEdge(v, u, weight);
                    q.emplace(make_pair(u, _distance[u]));
                }
            }
        }
    }
    //return res;
}

Graph* Graph::getSPT() {
    return _spt;
}

vector<uint>& Graph::getDistances() {
    return _distance;
}

Graph::~Graph() {
    delete[] _graph;
    delete _spt;
}