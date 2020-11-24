#pragma once
#include <string>
#include <memory>
#include "Graph.h"

/**
 * @brief Class for parsing Graph from a stp file (fully static)
*/
class GraphParser {
private:
    /**
     * @brief Method that replaces all comments in the string with spaces
     * @param data String with file's data
    */
    static void deleteComments(std::string& data);
    /**
     * @brief Method that extracts the information about graph's edges out of the file data
     * @throw std::runtime_error type in case of error
     * @param graph Graph where the edges will be added
     * @param data String with file's data
    */
    static void readGraphEdges(std::unique_ptr<Graph>& graph, std::string& data) noexcept(false);
    /**
     * @brief Method that handles Graph section and creates the graph
     * @throw std::runtime_error type in case of error
     * @param data String with file's data
     * @return Unique pointer to the Graph
    */
    static std::unique_ptr<Graph> proceedGraphSection(std::string& data) noexcept(false);
    /**
     * @brief Method that handles Terminals section and extracts all terminals info from the file
     * @throw std::runtime_error type in case of error
     * @param graph Unique pointer to the graph after proceeding Graph section
     * @param data String with file's data
    */
    static void proceedTerminalSection(std::unique_ptr<Graph>& graph, std::string& data) noexcept(false);
public:
    /**
     * @brief Static method that extracts Graph from the stp file
     * @throw std::runtime_error type in case of the heap memory lack or wrong file format
     * @param filePath Path to the stp file
     * @return Unique pointer to the Graph class
    */
    static std::unique_ptr<Graph> parseFile(const std::wstring& filePath) noexcept(false);
};

