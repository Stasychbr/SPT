#include <iostream>
#include <fstream>
#include <ctime>
#include "GraphParser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Pass the file path as the command line argument\n";
        return 0;
    }
    try {
        clock_t time = clock();
        Graph* graph = GraphParser::parseFile(argv[1]);
        cout << "reading time: " << (double)(clock() - time) / CLOCKS_PER_SEC << " s." << endl;
        time = clock();
        graph->buildSPT();
        time = clock() - time;
        //graph->getSPT()->print();
        auto& terminals = graph->getTerminals();
        cout << "Terminals " << terminals.size() << endl;
        /*for (auto it = terminals.begin(); it != terminals.end(); it++) {
            cout << "T " << *it + 1 << endl;
        }*/
        cout << "Opt: " << graph->getSPTLength() << endl;
        cout << "time: " << (double)time / CLOCKS_PER_SEC << " s." << endl;
        delete graph;
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
    };
    return 0;
}