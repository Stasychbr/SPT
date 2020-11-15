#pragma once
#include "Graph.h"

class GraphParser {
private:
    static void deleteComments(char* str);
    static void skipEmptyLines(ifstream& file, char* buf, int bufSize);
    static Graph* proceedGraphSection(ifstream& file, char* buf, int bufSize);
    static void proceedTerminalSection(ifstream& file, Graph* graph, char* buf, int bufSize);
public:
    static Graph* parseFile(const char* filePath);
};

