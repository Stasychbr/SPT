#pragma once
#include "Graph.h"

class GraphParser {
public:
    static Graph* parseFile(const char* filePath);
};

