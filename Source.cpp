#include <iostream>
#include <fstream>
#include <ctime>
#include <filesystem>
#include "GraphParser.h"

using namespace std;

void proceedFile(const filesystem::path& path) {
    clock_t time = clock();
    cout << "Proceeding the " << path.filename() << " file" << endl;
    unique_ptr<Graph> graph(GraphParser::parseFile(path.c_str()));
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
    cout << "computing time: " << (double)time / CLOCKS_PER_SEC << " s." << endl;
    cout << "------------------" << endl;
}

int main(int argc, char* argv[]) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            try {
                filesystem::path path(argv[i]);
                proceedFile(path);
            }
            catch (exception excep) {
                cout << "Unable to open file " << argv[1] << endl << excep.what() << endl;
            }
            catch (const char* msg) {
                cout << msg << endl;
            }
        }
    }
    else {
        filesystem::directory_iterator dir("./");
        for (auto& file : dir) {
            if (file.path().extension() != ".stp") {
                continue;
            }
            try {
                proceedFile(file.path());
            }
            catch (const char* msg) {
                std::cout << msg << endl;
            };
        }
    }
    return 0;
}