#include "Graph.h"
#include <limits>
#include <queue>
#include <stack>
#include <deque>
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

void Graph::buildSPT() {
    uint root = getRoot();
    vector<uint> lastPath(_size, UINT_MAX);
    vector<uint> distance(_size, UINT_MAX);
    auto comparator = [](pair<uint, uint> e1, pair<uint, uint> e2) { return e1.second > e2.second; };
    priority_queue <pair<uint, uint>, vector<pair<uint, uint>>, decltype(comparator)> q(comparator);
    delete _spt;
    _spt = new Graph(_size, _directed);
    if (!_spt || distance.size() != _size || lastPath.size() != _size) {
        return;
    }
    distance[root] = 0;
    q.emplace(make_pair(root, 0));
    while (!q.empty()) {
        auto edge = q.top();
        q.pop();
        uint& v = edge.first;
        uint& dist = edge.second;
        if (dist <= distance[v]) {
            for (auto it = _graph[v].begin(); it != _graph[v].end(); it++) {
                const uint& u = it->first;
                const uint& weight = it->second;
                if (distance[u] > distance[v] + weight) {
                    if (distance[u] != UINT_MAX) {
                        _spt->removeEdge(lastPath[u], u);
                    }
                    distance[u] = distance[v] + weight;
                    lastPath[u] = v;
                    _spt->setEdge(v, u, weight);
                    q.emplace(make_pair(u, distance[u]));
                }
            }
        }
    }
    removeExcessNodes();
}

void Graph::removeExcessNodes() {
    stack<uint> vertStack;
    deque<pair<uint, uint>> pathStack;
    uint lastVert = UINT_MAX;
    vector<bool> visited(_size, false);
    vector<bool> leadToTerminal(_size, false);
    _sptLenght = 0;
    vertStack.push(getRoot());
    while (!vertStack.empty()) {
        uint v = vertStack.top();
        if (lastVert != UINT_MAX) {
            pathStack.push_back(make_pair(lastVert, v));
        }
        vertStack.pop();
        lastVert = v;
        visited[v] = true;
        if (pathStack.size() > 0 && (!_directed && _spt->_graph[v].size() == 1 || _directed && _spt->_graph[v].empty())) {
            bool foundTerminal = false;
            pair<uint, uint> edge;
            while (!pathStack.empty() && (vertStack.empty() ||
                _spt->_graph[pathStack.back().second].find(vertStack.top()) == _spt->_graph[pathStack.back().second].end())) {
                edge = pathStack.back();
                pathStack.pop_back();
                if (!foundTerminal && !leadToTerminal[edge.second]) {
                    if (_terminals.find(edge.second) != _terminals.end()) {
                        foundTerminal = true;
                        for (auto it = pathStack.rbegin(); it != pathStack.rend(); it++) {
                            if (leadToTerminal[it->first]) {
                                break;
                            }
                            leadToTerminal[it->first] = true;
                        }
                        continue;
                    }
                    _sptLenght -= _spt->_graph[edge.first][edge.second];
                    _spt->removeEdge(edge.first, edge.second);
                }
            }
            lastVert = edge.first;
        }
        else {
            for (auto it = _spt->_graph[v].begin(); it != _spt->_graph[v].end(); it++) {
                if (!visited[it->first]) {
                    vertStack.push(it->first);
                    _sptLenght += it->second;
                }
            }
        }
    }
}

Graph* Graph::getSPT() {
    return _spt;
}

uint Graph::getRoot() {
    return _directed ? _root : *(_terminals.begin());
}

uint Graph::getSPTLength() {
    return _sptLenght;
}

unordered_set<uint>& Graph::getTerminals() {
    return _terminals;
}

Graph::~Graph() {
    delete[] _graph;
    delete _spt;
}