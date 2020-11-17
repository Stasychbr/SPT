#include "GraphParser.h"
#include <fstream>
#include <cstdlib>
#include <memory>

void GraphParser::deleteComments(char* str) {
    while (*str) {
        if (*str == '#') {
            while (*str && *str != '\n') {
                *str = ' ';
                str++;
            }
        }
        else {
            str++;
        }
    }
}

void GraphParser::readGraphRow(char*& line, uint* head, uint* tail, uint* weight) {
    uint* params[3] = { tail, head, weight };
    for (int i = 0; i < 3; i++) {
        *params[i] = 0;
        while (*line && !isdigit(*line)) {
            line++;
        }
        if (!*line) {
            break;
        }
        *params[i] = atol(line);
        while (*line && isdigit(*line)) {
            line++;
        }
    }
}

Graph* GraphParser::proceedGraphSection(char* buf) {
    uint nodes = 0;
    Graph* graph = nullptr;
    uint head, tail, weight;
    char* curLine, * endLine;
    bool directed = false;
    const char* exepMsg = "Wrong file format (Graph section)";
    curLine = strstr(buf, "Nodes");
    if (!curLine) {
        throw exepMsg;
    }
    nodes = (uint)atoi(curLine + sizeof("Nodes"));
    if (nodes == 0) {
        throw exepMsg;
    }
    curLine = strstr(curLine, "\nE ");
    if (curLine) {
        directed = false;
    }
    else {
        curLine = strstr(buf, "\nA ");
        if (curLine) {
            directed = true;
        }
        else {
            throw exepMsg;
        }
    }
    curLine++;
    endLine = strstr(curLine, "END");
    if (!endLine) {
        throw "Can't find the end of the Graph section";
    }
    try {
        graph = new Graph(nodes, directed);
        if (!graph) {
            throw "Not enough heap memory";
        }
    }
    catch (const char* msg) {
        throw msg;
    }
    while (curLine && curLine < endLine) {
        readGraphRow(curLine, &head, &tail, &weight);
        if (head != 0 && tail != 0 && weight != 0) {
            graph->setEdge(head - 1, tail - 1, weight);
        }
        while (curLine < endLine && !isdigit(*curLine)) {
            curLine++;
        }
    }
    return graph;
}

void GraphParser::proceedTerminalSection(Graph* graph, char* buf) {
    uint terminals;
    uint root;
    uint termNum;
    const char* exepMsg = "Wrong file format (Terminals section)";
    char* curLine, * endLine;
    curLine = strstr(buf, "\nTerminals");
    if (!curLine) {
        delete graph;
        throw exepMsg;
    }
    terminals = (uint)atoi(curLine + sizeof("\nTerminals"));
    if (terminals == 0) {
        delete graph;
        throw exepMsg;
    }
    graph->setTermsNumber(terminals);
    if (graph->isDirected()) {
        curLine = strstr(curLine, "Root");
        root = (uint)atoi(curLine + sizeof("Root"));
        if (root == 0) {
            delete graph;
            throw exepMsg;
        }
        graph->setRoot(root - 1);
    }
    endLine = strstr(curLine, "END");
    if (!endLine) {
        throw "Can't find the end of the Terminals section";
    }
    curLine = strstr(curLine, "T ");
    while (curLine && curLine < endLine) {
        while (curLine < endLine && !isdigit(*curLine)) {
            curLine++;
        }
        termNum = atol(curLine);
        graph->setTerminal(termNum - 1);
        while (isdigit(*curLine)) {
            curLine++;
        }
    }
}

Graph* GraphParser::parseFile(const char* filePath) {
    ifstream file(filePath);
    Graph* graph = nullptr;
    uint bufSize;
    char* buf, * graphSec, * termSec;
    file.seekg(0, ios_base::end);
    bufSize = (uint)file.tellg();
    file.seekg(0, ios_base::beg);
    buf = new char[bufSize + 1];
    if (!buf) {
        throw "Not enough heap memory";
    }
    unique_ptr<char[]> up(buf);
    if (!file.is_open()) {
        throw "Can't open file";
    }
    file.read(buf, bufSize);
    buf[bufSize] = 0;
    deleteComments(buf);
    graphSec = strstr(buf, "SECTION Graph");
    if (!graphSec) {
        throw "Wrong file format (can't find Graph section)";
    }
    graph = proceedGraphSection(graphSec);
    termSec = strstr(graphSec, "SECTION Terminals");
    if (!termSec) {
        throw "Wrong file format (can't find Terminals section)";
    }
    proceedTerminalSection(graph, termSec);
    return graph;
}
