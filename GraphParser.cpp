#include "GraphParser.h"
#include <fstream>
#include <cstdlib>

Graph* GraphParser::parseFile(const char* filePath) {
    ifstream file(filePath);
    const int bufSize = 100;
    uint nodes = 0;
    char* line = new char[bufSize];
    if (!line) {
        throw "Not enough heap memory";
    }
    if (!file.is_open()) {
        delete[] line;
        throw "Can't open file";
    }
    do {
        file.getline(line, bufSize);
    } while (file.good() && strcmp(line, "SECTION Graph"));
    if (!file.good()) {
        delete[] line;
        throw "Wrong file format";
    }
    file.getline(line, bufSize);
    nodes = (uint)atoi(line + sizeof("Nodes"));
    if (nodes == 0) {
        delete[] line;
        throw "Wrong file format";
    }
    file.getline(line, bufSize);
    file.getline(line, bufSize);
    bool directed = false;
    if (line[0] == 'E') {
        directed = false;
    }
    else if (line[0] == 'A') {
        directed = true;
    }
    else {
        delete[] line;
        throw "Wrong file format";
    }
    Graph* graph = new Graph(nodes, directed);
    uint head, tail, weight;
    while (strcmp(line, "END")) {
        sscanf_s(line, "%*c %i %i %i", &tail, &head, &weight);
        graph->setEdge(head - 1, tail - 1, weight);
        if (!file.good()) {
            delete graph;
            delete[] line;
            throw "Wrong file format";
        }
        file.getline(line, bufSize);
    }
    delete[] line;
    return graph;
}
