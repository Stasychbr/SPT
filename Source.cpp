#include <iostream>
#include "GraphParser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Pass the file path as the command line argument\n";
        return 0;
    }
    try {
        Graph* graph = GraphParser::parseFile(argv[1]);
        graph->print();
        delete graph;
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
    };
    return 0;
}