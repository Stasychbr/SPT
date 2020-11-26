#include "Graph.h"
#include <limits>
#include <queue>
#include <functional>
#include <algorithm>
#include <stdexcept>

using namespace std;

Graph::Graph(uint size, bool directed) {
    _graph.resize(size);
    if (_graph.size() != size) {
        throw runtime_error("Not enough heap memory");
    }
    _directed = directed;
}

void Graph::setEdge(uint tail, uint head, uint weight) {
    if (head < _graph.size() && tail < _graph.size()) {
        _graph[tail].push_back(make_pair(head, weight));
        if (!_directed) {
            _graph[head].push_back(make_pair(tail, weight));
        }
        _length += weight;
    }
}

void Graph::setTerminal(uint terminal) {
    if (terminal < _graph.size()) {
        _terminals.push_back(terminal);
    }
}

void Graph::setRoot(uint root) {
    if (root < _graph.size()) {
        _root = root;
    }
    else {
        root = 0;
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
            outStream << mode << ' ' << i + 1 << ' ' << j->first + 1 << ' ' << j->second << endl;
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

list<uint>::iterator Graph::findMinDist(list<uint>& leftToInsert, vector<uint>& distances) {
    uint minDist = UINT_MAX;
    list<uint>::iterator root = leftToInsert.end();
    for (auto it = leftToInsert.begin(); it != leftToInsert.end(); it++) {
        if (distances[*it] < minDist) {
            minDist = distances[*it];
            root = it;
        }
    }
    return root;
}


void Graph::addEdgesToSPT(uint curRoot, uint nextRoot, vector<pair<uint, uint>>& path, set<pair<uint, uint>>& addedEdges) {
    uint curVert = nextRoot;
    while (curVert != curRoot) {
        if (addedEdges.find(make_pair(path[curVert].first, curVert)) == addedEdges.end()) {
            _spt->setEdge(path[curVert].first, curVert, path[curVert].second);
            addedEdges.emplace(make_pair(path[curVert].first, curVert));
            if (!_directed) {
                addedEdges.emplace(make_pair(curVert, path[curVert].first));
            }
        }
        curVert = path[curVert].first;
    }
}

shared_ptr<const Graph> Graph::buildSPT() {
    vector<pair<uint, uint>> path(_graph.size());
    vector<uint> distances(_graph.size());
    list<uint> leftToInsert = _terminals;;
    set<pair<uint, uint>> addedEdges;
    _spt = make_shared<Graph>(_graph.size(), _directed);
    list<uint>::iterator nextRoot; 
    uint curRoot;
    if (_directed) {
        curRoot = _root;
        list<uint>::iterator isTerm = find(leftToInsert.begin(), leftToInsert.end(), _root);
        if (isTerm != leftToInsert.end()) {
            leftToInsert.erase(isTerm);
            _spt->setTerminal(_root);
        }
    }
    else {
        curRoot = leftToInsert.front();
        leftToInsert.pop_front();
        _spt->setTerminal(curRoot);
    }
    while (!leftToInsert.empty()) {
        fill(distances.begin(), distances.end(), UINT_MAX);
        dijkstra(curRoot, path, distances);
        nextRoot = findMinDist(leftToInsert, distances);
        if (nextRoot == leftToInsert.end()) {
            if (_directed && curRoot != _root) {
                curRoot = _root;
                continue;
            }
            else {
                break;
            }
        }
        addEdgesToSPT(curRoot, *nextRoot, path, addedEdges);
        _spt->setTerminal(*nextRoot);
        curRoot = *nextRoot;
        leftToInsert.erase(nextRoot);
    }
    return _spt;
}

bool Graph::isDirected() const {
    return _directed;
}

uint Graph::getLenght() const {
    return _length;
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

const list<pair<uint, uint>>& Graph::getAdjacencyList(uint node) const {
    if (node >= 0 && node < _graph.size()) {
        return _graph[node];
    }
    else {
        return _graph[0];
    }
}