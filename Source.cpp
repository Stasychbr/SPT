#include <iostream>
#include <fstream>
#include <ctime>
#include <filesystem>
#include "GraphParser.h"

using namespace std;

void proceedFile(const filesystem::path& path, ostream& out) {
    out << "Proceeding the " << path.filename() << " file" << endl;
    clock_t time = clock();
    unique_ptr<Graph> graph(GraphParser::parseFile(path.generic_wstring()));
    out << "reading time: " << (float)(clock() - time) / CLOCKS_PER_SEC << " s." << endl;
    out << "Full graph len: " << graph->getLenght() << endl;
    time = clock();
    shared_ptr<const Graph> spt = graph->buildSPT();
    time = clock() - time;
    out << "Opt: " << spt->getLenght() << endl;
    out << "computing time: " << (float)time / CLOCKS_PER_SEC << " s." << endl;
    out << "------------------" << endl;
}

int main(int argc, char* argv[]) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            try {
                filesystem::path path(argv[i]);
                proceedFile(path, cout);
            }
            catch (exception excep) {
                cout << excep.what() << endl;
            }
        }
    }
    else {
        filesystem::directory_iterator dir("./");
        for (auto& file : dir) {
            try {
                if (file.path().extension() != ".stp") {
                    continue;
                }
                proceedFile(file.path(), cout);
            }
            catch (exception excep) {
                cout << excep.what() << endl;
            }
        }
    }
    return 0;
}