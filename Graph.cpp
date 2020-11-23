#include "Graph.h"
#include <limits>
#include <queue>
#include <list>
#include <functional>
#include <set>

using namespace std;

Graph::Graph(uint size, bool directed) {
    _graph.resize(size);
    if (_graph.size() != size) {
        throw "Not enough heap memory";
    }
    _directed = directed;
}

bool Graph::isDirected() const {
    return _directed;
}

void Graph::setEdge(uint head, uint tail, uint weight) {
    if (head < _graph.size() && tail < _graph.size()) {
        _graph[head][tail] = weight;
        if (!_directed) {
            _graph[tail][head] = weight;
        }
    }
}

void Graph::setTerminal(uint terminal) {
    if (terminal < _graph.size()) {
        _terminals.push_back(terminal);
    }
}

void Graph::removeTerminal(uint terminal) {
    _terminals.remove(terminal);
}

void Graph::setTermsNumber(uint number) {
    //_terminals.
}

void Graph::setRoot(uint root) {
    if (root < _graph.size()) {
        _root = root;
    }
    else {
        root = 0;
    }
}

void Graph::removeEdge(uint head, uint tail) {
    if (head < _graph.size() && tail < _graph.size()) {
        _graph[head].erase(tail);
        if (!_directed) {
            _graph[tail].erase(head);
        }
    }
}

void Graph::print(ostream& outStream) const {
    char mode = _directed ? 'A' : 'E';
    outStream << "Nodes " << _graph.size() << endl;
    for (uint i = 0; i < _graph.size(); i++) {
        for (auto j = _graph[i].begin(); j != _graph[i].end(); j++) {
            if (!_directed && j->first < i) {
                continue;
            }
            outStream << mode << ' ' << j->first + 1 << ' ' << i + 1 << ' ' << j->second << endl;
        }
    }
}

// distances (total_dist); path (parent node, dist)
void Graph::dijkstra(uint root, vector<pair<uint, uint>>& path, vector<uint>& distances) {
    auto comparator = [](pair<uint, uint>& e1, pair<uint, uint>& e2) { return e1.second > e2.second; };
    priority_queue <pair<uint, uint>, vector<pair<uint, uint>>, decltype(comparator)> q(comparator);
    distances[root] = 0;
    q.emplace(make_pair(root, 0));
    while (!q.empty()) {
        auto edge = q.top();
        q.pop();
        const uint& v = edge.first;
        const uint& dist = edge.second;
        if (dist <= distances[v]) {
            for (auto it = _graph[v].begin(); it != _graph[v].end(); it++) {
                const uint& u = it->first;
                const uint& weight = it->second;
                if (distances[u] > distances[v] + weight) {
                    distances[u] = distances[v] + weight;
                    path[u] = make_pair(v, weight);
                    q.emplace(make_pair(u, distances[u]));
                }
            }
        }
    }
}

shared_ptr<const Graph> Graph::buildSPT() {
    vector<pair<uint, uint>> path(_graph.size());
    vector<uint> distances(_graph.size());
    list<uint> leftToInsert(_terminals);
    _spt = make_shared<Graph>(_graph.size(), _directed);
    _spt->setTermsNumber(_terminals.size());
    auto root = leftToInsert.begin(); // DIRECTED PROBLEM!!!
    for (uint i = 1; i < _terminals.size(); i++) {
        for (uint j = 0; j < _graph.size(); j++) {
            distances[j] = UINT_MAX;
        }
        _spt->setTerminal(*root); 
        dijkstra(*root, path, distances);
        leftToInsert.erase(root);
        uint minDist = UINT_MAX;
        for (auto j = leftToInsert.begin(); j != leftToInsert.end(); j++) {
            if (distances[*j] < minDist) {
                minDist = distances[*j];
                root = j;
            }
        }
        uint prevRoot = _spt->_terminals.back();
        uint curVert = *root;
        while (curVert != prevRoot) {
            _spt->setEdge(path[curVert].first, curVert, path[curVert].second);
            curVert = path[curVert].first;
        }
    }
    return _spt;
}

uint Graph::getLenght() const {
    uint res = 0;
    for (uint i = 0; i < _graph.size(); i++) {
        for (auto j = _graph[i].begin(); j != _graph[i].end(); j++) {
            if (!_directed && j->first < i) {
                continue;
            }
            res += j->second;
        }
    }
    return res;
}

shared_ptr<const Graph> Graph::getSPT() {
    return _spt;
}

uint Graph::getSize() const {
    return _graph.size();
}

uint Graph::getRoot() const {
    return _directed ? _root : *(_terminals.begin());
}

const list<uint>& Graph::getTerminals() const {
    return _terminals;
}

const map<uint, uint>& Graph::getAdjacencyList(uint node) const {
    if (node >= 0 && node < _graph.size()) {
        return _graph[node];
    }
    else {
        return _graph[0];
    }
}

Graph::~Graph() {
}