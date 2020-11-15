#include "Graph.h"
#include <limits>
#include <queue>
#include <functional>

Graph::Graph(uint size, bool directed) {
    _graph = new unordered_map<uint, uint>[size];
    if (!_graph) {
        throw "Not enough heap memory";
    }
    _size = size;
    _directed = directed;
}

bool Graph::isDirected() {
    return _directed;
}

void Graph::setEdge(uint head, uint tail, uint weight) {
    if (head < _size && tail < _size) {
        _graph[head][tail] = weight;
        if (!_directed) {
            _graph[tail][head] = weight;
        }
    }
}

void Graph::setTerminal(uint terminal) {
    if (terminal < _size) {
        _terminals.insert(terminal);
    }
}

void Graph::removeTerminal(uint terminal) {
    _terminals.erase(terminal);
}

void Graph::setTermsNumber(uint number) {
    _terminals.reserve(number);
}

void Graph::setRoot(uint root) {
    _root = root;
}

void Graph::removeEdge(uint head, uint tail) {
    if (head < _size && tail < _size) {
        _graph[head].erase(tail);
        if (!_directed) {
            _graph[tail].erase(head);
        }
    }
}

void Graph::print(ostream& outStream) {
    char mode = _directed ? 'A' : 'E';
    outStream << "Nodes " << _size << endl;
    for (uint i = 0; i < _size; i++) {
        for (auto j = _graph[i].begin(); j != _graph[i].end(); j++) {
            if (!_directed && j->first < i) {
                continue;
            }
            outStream << mode << ' ' << j->first + 1 << ' ' << i + 1 << ' ' << j->second << endl;
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
            for (auto it = _graph[v].begin(); it != _graph[v].end(); it++) {
                const uint& u = it->first;
                const uint& weight = it->second;
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
}

Graph* Graph::getSPT() {
    return _spt;
}

vector<uint>& Graph::getDistances() {
    return _distance;
}

unordered_set<uint>& Graph::getTerminals() {
    return _terminals;
}

Graph::~Graph() {
    delete[] _graph;
    delete _spt;
}