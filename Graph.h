#pragma once
#include <vector>
#include <iostream>
#include <utility>
#include <map>
#include <list>
#include <memory>
#include <set>

typedef unsigned int uint;
/**
 * @brief Class that implements the graph
*/
class Graph {
private:
    /// A vector of adjacency lists for each node in the graph. Array index is the tail node of the edge, 
    /// each node in the list is pair (head_num, edge_weight)
    std::vector<std::list<std::pair<uint, uint>>> _graph;
    /// List of the terminals
    std::list<uint> _terminals;
    /// Root node for the directed STP
    uint _root = UINT_MAX;
    /// Smart pointer to the graph, which was builded within @ref Graph::buildSPT, solution of the SPT 
    std::shared_ptr<Graph> _spt;
    /// Flag, which marks if the graph directed
    bool _directed = false;
    /// Full length of the graph (sum of all edge weights)
    uint _length = 0;
    /// Method that implements Dijkstra's shortest path algorithm, used in @ref Graph::buildSPT
    void dijkstra(uint root, std::vector<std::pair<uint, uint>>& path, std::vector<uint>& distances);
    /// Method that finds the closest terminal and returns his iterator, used in @ref Graph::buildSPT
    std::list<uint>::iterator findMinDist(std::list<uint>& leftToInsert, std::vector<uint>& distances);
    /// Method that adds edges to the closest terminal in the SPT (which are not there yet), used in @ref Graph::buildSPT
    void addEdgesToSPT(uint nextRoot, std::vector<std::pair<uint, uint>>& path, std::set<std::pair<uint, uint>>& addedEdges);
    /// Method that prepares directed graph for the solution (the problem is that the root node may be not terminal)
    std::list<uint>::iterator prepareDirGraph(std::list<uint>& leftToInsert, bool& fFakeTerm);
public:
    /**
     * @brief Graph constructor, needs information about size (number of nodes) and if it directed or not
     * @throw std::runtime_error type in case of the heap memory lack
     * @param size
     * @param directed
    */
    Graph(uint size, bool directed) noexcept(false);
    /**
     * @brief Tells if the graph directed or not
     * @return bool type
    */
    bool isDirected() const;
    /**
     * @brief Method that inserts edge in the graph
     * @param tail Tail node of the edge (in x->y edge x is the tail)
     * @param head Head node of the edge (in x->y edge y is the head)
     * @param weight Weight of the edge
     * @details Note that nodes in the Graph are mapped in [0, Graph::_size), while in the stp file [1, Graph::_size].
     * That Method needs numbers in Graph's coordinates [0, Graph::_size).
    */
    void setEdge(uint tail, uint head, uint weight);
    /**
     * @brief Method that inserts the terminal in the graph
     * @param terminal Number of the node, that contains th terminal
     * @details Note that nodes in the Graph are mapped in [0, Graph::_size), while in the stp file [1, Graph::_size].
     * That Method needs numbers in Graph's coordinates [0, Graph::_size).
    */
    void setTerminal(uint terminal);
    /**
     * @brief Method that marks node of the graph as the root
     * @param root Number of the node
     * @details Note that nodes in the Graph are mapped in [0, Graph::_size), while in the stp file [1, Graph::_size].
     * That Method needs numbers in Graph's coordinates [0, Graph::_size). Also, you must use this function only in case of
     * directed Steiner Tree Problem, it will make no sense otherwise.
    */
    void setRoot(uint root);
    /**
     * @brief Method that prints graph as set of edges
     * @param outStream Stream, where the information will be printed. Default value is cout.
     * @details Format is the same as inside the Graph section in the stp file. If you want to print
     * graph in the file, create ofstream object, connected with your file, and pass it to the function.
    */
    void print(std::ostream& outStream = std::cout) const;
    /**
     * @brief Method that creates the solution of the STP based on the Shortest Path Tree
     * @throw std::runtime_error type in case of the heap memory lack
     * @details Builds SPT using Dijkstra's algorithm :
     * 1) Choose any terminal (or root in case of directed STP)
     * 2) Find out, which terminal is the closest to the current terminal
     * 3) Add path from that terminal to the current one in the SPT
     * 4) That terminal becomes current, repeat 2-4 until we run out of the terminals 
     * or we can't find closest terminal in case of directed STP
     * @return Smart pointer to the constant Graph class, containing builded SPT
    */
    std::shared_ptr<const Graph> buildSPT() noexcept(false);
    /**
     * @brief Query to get smart pointer to the solution, builded by @ref Graph::buildSPT function
     * @return Smart pointer to the constant Graph class 
    */
    std::shared_ptr<const Graph> getSPT();
    /**
     * @brief Query to get number of the nodes in the graph
     * @return Number of the nodes in the graph
    */
    uint getSize() const;
    /**
     * @brief Query to get the length of the graph (sum of all edge weights)
     * @return Length of the graph (sum of all edge weights)
    */
    uint getLenght() const;
    /**
     * @brief Query to get number of the root node (in Graph's coordinates [0, Graph::_size))
     * @return Number of the root node if the graph is directed or number of the first terminal in the
     * terminals list otherwise.
    */
    uint getRoot() const;
    /**
     * @brief Query to get the reference to the terminals list
     * @return Constant reference to the terminals set
     * @details Note that nodes in the Graph are mapped in [0, Graph::_size), while in the stp file [1, Graph::_size].
     * That Method returns terminals in Graph's coordinates [0, Graph::_size).
    */
    const std::list<uint>& getTerminals() const;
    /**
     * @brief Query to get the reference to the adjacency list for the node
     * @param node Number of the node in Graph's coordinates [0, Graph::_size)
     * @return Reference to the adjacency list of the requested node or of the first node
     * if the asked number was incorrect (>= Graph::_size)
     * @details Note that nodes in the Graph are mapped in [0, Graph::_size), while in the stp file [1, Graph::_size].
     * That Method returns nodes in Graph's coordinates [0, Graph::_size).
    */
    const std::list<std::pair<uint, uint>>& getAdjacencyList(uint node) const;
};