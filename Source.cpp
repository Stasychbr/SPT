#include <iostream>
#include <fstream>
#include <ctime>
#include <filesystem>
#include "GraphParser.h"

using namespace std;

void proceedFile(const filesystem::path& path, ofstream& out) {
    cout << "Proceeding the " << path.filename() << " file" << endl;
    out << "Proceeding the " << path.filename() << " file" << endl; // dbg
    clock_t time = clock();
    unique_ptr<Graph> graph(GraphParser::parseFile(path.generic_wstring()));
    cout << "reading time: " << (double)(clock() - time) / CLOCKS_PER_SEC << " s." << endl;
    time = clock();
    shared_ptr<const Graph> spt = graph->buildSPT();
    time = clock() - time;
    cout << "Opt: " << spt->getLenght() << endl;
    out << "Opt: " << spt->getLenght() << endl; // dbg
    cout << "computing time: " << (double)time / CLOCKS_PER_SEC << " s." << endl;
    cout << "------------------" << endl;


    out << "computing time: " << (double)time / CLOCKS_PER_SEC << " s." << endl;
    out << "------------------" << endl;
}

int main(int argc, char* argv[]) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    ofstream out("output_lists.txt"); //dbg

    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            try {
                filesystem::path path(argv[i]);
                proceedFile(path, out);
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
                proceedFile(file.path(), out);
            }
            catch (const char* msg) {
                std::cout << msg << endl;
            };
        }
    }
    return 0;
}