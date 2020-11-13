#include <iostream>
#include "GraphParser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Pass the file path as the command line argument\n";
        return 0;
    }
    try {
        Graph* graph = GraphParser::parseFile(argv[1]);
        FILE* output;
        fopen_s(&output, "output.txt", "w");
        if (output) {
            graph->print(output);
            graph->buildSPT(0);
            vector<uint> test = graph->getDistances();
            for (int i = 0; i < test.size(); i++) {
                printf("to %i: %i\n", i + 1, test[i]);
            }
            graph->getSPT()->print();
            fclose(output);
        }
        delete graph;
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
    };
    return 0;
}