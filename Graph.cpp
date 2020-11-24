#include "Graph.h"
#include <limits>
#include <queue>
#include <functional>
#include <algorithm>

using namespace std;

Graph::Graph(uint size, bool directed) {
    _graph.resize(size);
    if (_graph.size() != size) {
        throw "Not enough heap memory";
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


void Graph::addEdgesToSPT(uint nextRoot, vector<pair<uint, uint>>& path, set<pair<uint, uint>>& addedEdges) {
    uint prevRoot = _spt->_terminals.back();
    uint curVert = nextRoot;
    while (curVert != prevRoot) {
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

list<uint>::iterator Graph::prepareDirGraph(std::list<uint>& leftToInsert, bool& fFakeTerm) {
    list<uint>::iterator root;
    _spt->setRoot(_root);
    root = find(leftToInsert.begin(), leftToInsert.end(), _root);
    if (root == leftToInsert.end()) {
        leftToInsert.push_front(_root);
        _spt->setTerminal(_root);
        root = leftToInsert.begin();
        fFakeTerm = true;
    }
    else {
        _spt->setTerminal(*root);
    }
    return root;
}

shared_ptr<const Graph> Graph::buildSPT() {
    vector<pair<uint, uint>> path(_graph.size());
    vector<uint> distances(_graph.size());
    list<uint> leftToInsert(_terminals);
    set<pair<uint, uint>> addedEdges;
    _spt = make_shared<Graph>(_graph.size(), _directed);
    list<uint>::iterator root; 
    bool fakeTerminal = false;
    if (_directed) {
        root = prepareDirGraph(leftToInsert, fakeTerminal);
    }
    else {
        root = leftToInsert.begin();
        _spt->setTerminal(*root);
    }
    while (_spt->_terminals.size() != _terminals.size()) {
        for (uint& dist: distances) {
            dist = UINT_MAX;
        }
        dijkstra(*root, path, distances);
        leftToInsert.erase(root);
        root = findMinDist(leftToInsert, distances);
        if (root == leftToInsert.end()) {
            break;
        }
        addEdgesToSPT(*root, path, addedEdges);
        _spt->setTerminal(*root);
    }
    if (fakeTerminal) {
        _spt->_terminals.pop_front();
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