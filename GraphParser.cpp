#include "GraphParser.h"
#include <fstream>
#include <cstdlib>

void GraphParser::deleteComments(char* str) {
    int i;
    for (i = 0; str[i]; i++) {
        if (str[i] == '#') {
            break;
        }
    }
    str[i] = 0;
}

void GraphParser::skipEmptyLines(ifstream& file, char* buf, int bufSize) {
    do {
        file.getline(buf, bufSize);
        deleteComments(buf);
    } while (file.good() && *buf == 0);
}

Graph* GraphParser::proceedGraphSection(ifstream& file, char* buf, int bufSize) {
    uint nodes = 0;
    Graph* graph = nullptr;
    uint head, tail, weight;
    int read;
    bool directed = false;
    const char* exepMsg = "Wrong file format (Graph section)";
    skipEmptyLines(file, buf, bufSize);
    nodes = (uint)atoi(buf + sizeof("Nodes"));
    if (nodes == 0) {
        delete[] buf;
        throw exepMsg;
    }
    do {
        file.getline(buf, bufSize);
    } while (file.good() && *buf != 'E' && *buf != 'A');
    if (*buf == 'E') {
        directed = false;
    }
    else if (*buf == 'A') {
        directed = true;
    }
    else {
        delete[] buf;
        throw exepMsg;
    }
    try {
        graph = new Graph(nodes, directed);
        if (!graph) {
            throw "Not enough heap memory";
        }
    }
    catch (const char* msg) {
        delete[] buf;
        throw msg;
    }
    while (strcmp(buf, "END")) {
        deleteComments(buf);
        read = sscanf_s(buf, "%*c %i %i %i", &tail, &head, &weight);
        if (read == 3) {
            graph->setEdge(head - 1, tail - 1, weight);
        }
        if (!file.good()) {
            delete graph;
            delete[] buf;
            throw exepMsg;
        }
        file.getline(buf, bufSize);
    }
    return graph;
}

void GraphParser::proceedTerminalSection(ifstream& file, Graph* graph, char* buf, int bufSize) {
    uint terminals;
    uint root;
    uint termNum;
    int read;
    const char* exepMsg = "Wrong file format (Terminals section)";
    skipEmptyLines(file, buf, bufSize);
    terminals = (uint)atoi(buf + sizeof("Terminals"));
    if (terminals == 0) {
        delete graph;
        delete[] buf;
        throw exepMsg;
    }
    graph->setTermsNumber(terminals);
    skipEmptyLines(file, buf, bufSize);
    if (graph->isDirected()) {
        root = (uint)atoi(buf + sizeof("Root"));
        if (root == 0) {
            delete graph;
            delete[] buf;
            throw exepMsg;
        }
        graph->setRoot(root);
    }
    while (strcmp(buf, "END")) {
        deleteComments(buf);
        read = sscanf_s(buf, "%*c %i", &termNum);
        if (read == 1) {
            graph->setTerminal(termNum - 1);
        }
        if (!file.good()) {
            delete graph;
            delete[] buf;
            throw exepMsg;
        }
        file.getline(buf, bufSize);
    }
}

Graph* GraphParser::parseFile(const char* filePath) {
    ifstream file(filePath);
    const int bufSize = 128;
    char* buf = new char[bufSize];
    Graph* graph = nullptr;
    if (!buf) {
        throw "Not enough heap memory";
    }
    if (!file.is_open()) {
        delete[] buf;
        throw "Can't open file";
    }
    do {
        file.getline(buf, bufSize);
    } while (file.good() && strcmp(buf, "SECTION Graph"));
    if (!file.good()) {
        delete[] buf;
        throw "Wrong file format";
    }
    graph = proceedGraphSection(file, buf, bufSize);
    do {
        file.getline(buf, bufSize);
    } while (file.good() && strcmp(buf, "SECTION Terminals"));
    if (!file.good()) {
        delete[] buf;
        throw "Wrong file format";
    }
    proceedTerminalSection(file, graph, buf, bufSize);
    delete[] buf;
    return graph;
}
