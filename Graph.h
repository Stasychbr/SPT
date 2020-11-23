#pragma once
#include <vector>
#include <iostream>
#include <utility>
#include <map>
#include <unordered_set>
#include <list>
#include <memory>

typedef unsigned int uint;
/**
 * @brief Class that implements the graph
*/
class Graph {
private:
    /// An array of adjacency lists for each node in the graph. Array index is the head node of the edge, 
    /// the hash map key is the tail node and the value is weight of the edge
    std::vector<std::map<uint, uint>> _graph;
    /// Set of the terminals
    std::list<uint> _terminals;
    /// Root node for the directed STP
    uint _root = UINT_MAX;
    /// Pointer to the graph, which was builded within @ref Graph::buildSPT, solution of the SPT 
    std::shared_ptr<Graph> _spt;
    /// Length of the solution, counted within Graph::buildSPT
    //uint _sptLenght = 0;
    /// Number of nodes in the graph 
    //uint _size = 0;
    /// Flag, which marks if the graph directed
    bool _directed = false;
    
    void dijkstra(uint root, std::vector<std::pair<uint, uint>>& path, std::vector<uint>& distances);
public:
    /**
     * @brief Graph constructor, needs information about size (number of nodes) and if it directed or not
     * @throw const char* type in case of the heap memory lack
     * @param size 
     * @param directed 
     * @return 
    */
    Graph(uint size, bool directed) noexcept(false);
    /**
     * @brief Tells if the graph directed or not
     * @return bool type 
    */
    bool isDirected() const;
    /**
     * @brief Procedure that inserts edge in the graph
     * @param head Head node of the edge
     * @param tail Tail node of the edge
     * @param weight Weight of the edge
     * @details Note that nodes in the Graph are mapped in [0, Graph::_size), while in the stp file [1, Graph::_size].
     * That procedure needs numbers in Graph's coordinates [0, Graph::_size).
    */
    void setEdge(uint head, uint tail, uint weight);
    /**
     * @brief Procedure that inserts the terminal in the graph
     * @param terminal Number of the node, that contains th terminal
     * @details Note that nodes in the Graph are mapped in [0, Graph::_size), while in the stp file [1, Graph::_size].
     * That procedure needs numbers in Graph's coordinates [0, Graph::_size).
    */
    void setTerminal(uint terminal);
    /**
     * @brief Procedure that removes the terminal from the graph
     * @param terminal Number of the node, which no longer contains the terminal
     * @details Note that nodes in the Graph are mapped in [0, Graph::_size), while in the stp file [1, Graph::_size].
     * That procedure needs numbers in Graph's coordinates [0, Graph::_size).
    */
    void removeTerminal(uint terminal);
    /**
     * @brief Procedure that reservates memory for the terminals set
     * @param number Number of the terminals which will be inserted in the Graph
    */
    void setTermsNumber(uint number);
    /**
     * @brief Procedure that marks node of the graph as the root 
     * @param root Number of the node
     * @details Note that nodes in the Graph are mapped in [0, Graph::_size), while in the stp file [1, Graph::_size].
     * That procedure needs numbers in Graph's coordinates [0, Graph::_size). Also, you must use this function only in case of 
     * directed Steiner Tree Problem, it will make no sense otherwise.
    */
    void setRoot(uint root);
    /**
     * @brief Procedure that removes edge from the graph
     * @param head Head node of the edge
     * @param tail Tail node of the edge
     * @details Note that nodes in the Graph are mapped in [0, Graph::_size), while in the stp file [1, Graph::_size].
     * That procedure needs numbers in Graph's coordinates [0, Graph::_size).
    */
    void removeEdge(uint head, uint tail);
    /**
     * @brief Procedure that prints graph as set of edges 
     * @param outStream Stream, where the information will be printed. Default value is cout.
     * @details Format is the same as inside the Graph section in the stp file. If you want to print 
     * graph in the file, create ofstream object, connected with your file, and pass it to the function.
    */
    void print(std::ostream& outStream = std::cout) const;
    /**
     * @brief Procedure that creates the solution of the STP based on the Shortest Path Tree
     * @throw const char* type in case of the heap memory lack
     * @details Runs Dijkstra's algorithm inside and then calls Graph::removeExcessNodes.
     * In case of graph rebuilding (insertion of some edges, f.e.) you need to call this method again.
    */
    std::shared_ptr<const Graph> buildSPT() noexcept(false);
    /**
     * @brief Query to get pointer to the solution, builded by @ref Graph::buildSPT function
     * @return pointer to the Graph class or nullptr if the SPT doesn't exist (@ref Graph::buildSPT wasn't called yet)
    */
    std::shared_ptr<const Graph> getSPT();
    /**
     * @brief Query to get number of the nodes in the graph
     * @return number of the nodes in the graph
    */
    uint getSize() const;
    /**
     * @brief Query to get number of the root node (in Graph's coordinates [0, Graph::_size))
     * @return Number of the root node if the graph is directed and number of the first terminal in the 
     * terminals set otherwise.
    */
    uint getRoot() const;
    /**
     * @brief Query to get the full length of the STP solution
     * @return Length or 0 if the SPT doesn't exist (@ref Graph::buildSPT wasn't called yet)
    */
    //uint getSPTLength() const;
    /**
     * @brief Query to get the reference to the terminals set
     * @return Reference to the terminals set
    */
    const std::list<uint>& getTerminals() const;
    /**
     * @brief Query to get the reference to the adjacency list for the node
     * @param node Number of the node in Graph's coordinates [0, Graph::_size)
     * @return Reference to the adjacency list of the requested node or of the first node 
     * if the asked number was incorrect (>= Graph::_size, f.e.) 
    */
    const std::map<uint, uint>& getAdjacencyList(uint node) const;

    uint getLenght() const;
    ~Graph();
};