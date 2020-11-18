#pragma once
#include "Graph.h"

/**
 * @brief Class for parsing Graph from a stp file (fully static)
*/
class GraphParser {
private:
    /**
     * @brief Method that replaces all comments in the string with spaces
     * @param str Null terminated string with file's data
    */
    static void deleteComments(char* str);
    /**
     * @brief Method that extracts the information about graph's edge out of the line
     * @param line Pointer to the line in the Graph section
     * @param head Pointer where the head node will be written
     * @param tail Pointer where the tail node will be written
     * @param weight Pointer where the weight of the edge will be written
    */
    static void readGraphRow(char*& line, uint* head, uint* tail, uint* weight);
    /**
     * @brief Method that handles Graph section and extracts all edges of the graph
     * @throw const char* type in case of the heap memory lack
     * @param buf Pointer to the Graph section
     * @return Pointer to the Graph
    */
    static Graph* proceedGraphSection(char* buf) noexcept(false);
    /**
     * @brief Method that handles Terminals section and extracts all terminals of the graph
     * @param graph Pointer to the graph after proceeding Graph section
     * @param buf Pointer to the Terminals section
    */
    static void proceedTerminalSection(Graph* graph, char* buf);
public:
    /**
     * @brief Static method that extracts Graph from the stp file
     * @throw const char* type in case of the heap memory lack or wrong file format
     * @param filePath Path to the stp file
     * @return Pointer to the Graph class
    */
    static Graph* parseFile(const char* filePath) noexcept(false);
};

