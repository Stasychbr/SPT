#include <iostream>
#include <fstream>
#include "GraphParser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Pass the file path as the command line argument\n";
        return 0;
    }
    try {
        Graph* graph = GraphParser::parseFile(argv[1]);
        ofstream output("output.txt");
        if (output.good()) {
            graph->print(output);
            graph->buildSPT(0);
            vector<uint>& test = graph->getDistances();
            for (int i = 0; i < test.size(); i++) {
                cout << "to " << i + 1 << ": " << test[i] << endl;
            }
            graph->getSPT()->print();
            auto& terminals = graph->getTerminals();
            cout << "Terminals " << terminals.size() << endl;
            for (auto it = terminals.begin(); it != terminals.end(); it++) {
                cout << "T " << *it + 1 << endl;
            }
        }
        delete graph;
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
    };
    return 0;
}