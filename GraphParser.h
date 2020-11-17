#pragma once
#include "Graph.h"

class GraphParser {
private:
    static void deleteComments(char* str);
    static void readGraphRow(char*& line, uint* head, uint* tail, uint* weight);
    static Graph* proceedGraphSection(char* buf);
    static void proceedTerminalSection(Graph* graph, char* buf);
public:
    static Graph* parseFile(const char* filePath) noexcept(false);
};

