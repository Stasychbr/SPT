#include "GraphParser.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <memory>
#include <array>

using namespace std;

void GraphParser::deleteComments(string& data) {
    size_t pos = 0;
    while ((pos = data.find('#', pos)) != data.npos) {
        while (pos < data.npos && data[pos] != '\n') {
            data[pos] = ' ';
            pos++;
        }
    }
}

void GraphParser::readGraphEdges(unique_ptr<Graph>& graph, string& data) {
    const string lineStart = graph->isDirected() ? "\nA " : "\nE ";
    size_t pos = 0, eolPos = 0;
    size_t prefixLen = lineStart.length();
    array<uint, 3> params;
    try {
        while ((pos = data.find(lineStart,  pos)) != data.npos) {
            pos += prefixLen;
            eolPos = data.find('\n', pos);
            string curLine = data.substr(pos, eolPos - pos);
            size_t chRead = 0;
            for (uint& param : params) {
                param = (uint)stoul(curLine, &chRead);
                curLine.erase(0, chRead);
                pos += chRead;
            }
            graph->setEdge(params[0] - 1, params[1] - 1, params[2]);
        }
    }
    catch (exception excep) {
        throw "Unable to read graph's edge";
    }
}

unique_ptr<Graph> GraphParser::proceedGraphSection(string& data) {
    const string sectionName = "SECTION Graph";
    string lineStart = "\nE ";
    const char* excepMsg = "Wrong format of graph section";
    size_t pos = 0, endPos = 0;
    uint nodesNum = 0;
    bool directed = false;
    pos = data.find(sectionName);
    pos = data.find("Nodes", pos + sectionName.length());
    if (pos == data.npos) {
        throw excepMsg;
    }
    data.erase(0, pos + sizeof("Nodes"));
    try {
        nodesNum = (uint)stoul(data, &pos);
    }
    catch (exception) {
        throw excepMsg;
    }
    pos = data.find(lineStart);
    if (pos == data.npos) {
        directed = true;
        lineStart = "\nA ";
    }
    unique_ptr<Graph> graph = make_unique<Graph>(nodesNum, directed);
    readGraphEdges(graph, data);
    return graph;
}

void GraphParser::proceedTerminalSection(unique_ptr<Graph>& graph, string& data) {
    const string sectionName = "SECTION Terminals";
    const string lineStart = "\nT ";
    const char* excepMsg = "Wrong format of terminals section";
    uint termNum = 0;
    uint terminal = 0;
    uint root = 0;
    size_t pos = data.find(sectionName);
    if (graph->isDirected()) {
        try {
            pos = data.find("Root");
            data.erase(0, pos + sizeof("Root"));
            root = stoul(data);
            graph->setRoot(root - 1);
        }
        catch (exception excep) {
            throw excepMsg;
        }
    }
    try {
        while ((pos = data.find(lineStart)) != data.npos) {
            data.erase(0, pos + lineStart.length());
            terminal = stoul(data, &pos);
            graph->setTerminal(terminal - 1);
            data.erase(0, pos);
        }
    }
    catch (exception excep) {
        throw excepMsg;
    }
}

unique_ptr<Graph> GraphParser::parseFile(const wstring& filePath) {
    ifstream file(filePath);
    string data;
    size_t  fileSize;
    if (!file.good()) {
        throw "Unable to open file";
    }
    file.seekg(0, ios_base::end);
    fileSize = (size_t)file.tellg();
    file.seekg(0, ios_base::beg);
    data.resize(fileSize);
    file.read(&data[0], fileSize);
    deleteComments(data);
    unique_ptr<Graph> graph = proceedGraphSection(data);
    proceedTerminalSection(graph, data);
    return graph;
}
