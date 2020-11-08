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
    Graph* graph = new Graph(nodes);
    if (!graph) {
        delete[] line;
        throw "Not enough heap memory";
    }
    file.getline(line, bufSize);
    uint head, tail, weight;
    while (strcmp(line, "END")) {
        sscanf_s(line, "%*c %i %i %i", &tail, &head, &weight);
        graph->setArc(head - 1, tail - 1, weight);
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
