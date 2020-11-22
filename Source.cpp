#include <iostream>
#include <fstream>
#include <ctime>
#include "GraphParser.h"

using namespace std;

int main(int argc, char* argv[]) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    if (argc < 2) {
        std::cout << "Pass the file path as the command line argument\n";
        return 0;
    }
    try {
        clock_t time = clock();
        unique_ptr<Graph> graph(GraphParser::parseFile(argv[1]));
        cout << "reading time: " << (double)(clock() - time) / CLOCKS_PER_SEC << " s." << endl;
        time = clock();
        graph->buildSPT();
        time = clock() - time;
        Graph* spt = graph->getSPT();
        auto& terms = graph->getTerminals();
        for (auto it = terms.cbegin(); it != terms.cend(); it++) {
            if (spt->getAdjacencyList(*it).empty()) {
                cout << "troubles with terminal " << *it + 1 << endl;
            }
        }
        cout << "Opt: " << graph->getSPTLength() << endl;
        cout << "time: " << (double)time / CLOCKS_PER_SEC << " s." << endl;
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
    };
    return 0;
}