#ifndef GRAPH_H
#define GRAPH_H

#include "vector.h"  // Assuming you have your custom Vector class defined elsewhere
#include "queue.h"
#include <string>
#include <fstream>

// Structure to represent an edge with a weight
struct Node {
  unsigned node;  // Destination node
  int weight;     // Weight of the edge
};

class Graph {
 public:
  Graph(const bool directed, const unsigned nodes);
  ~Graph() {};

  bool directed() const;  // `const` because this method doesn't modify the object
  void AddEdge(const unsigned start, const unsigned end, const int weight = 0);
  void PrintList() const;
  unsigned GetNodes() const;
  unsigned GetEdges() const;
  void PrintBFS(const unsigned initial_node) const;
  
 private:
 void BFS(const unsigned initial_node, Vector<int>& pred, Vector<bool>& visited) const;
   
  Vector<Vector<Node>> adj_list_;  // Adjacency list using the custom `Vector` class
  bool directed_;                  // Whether the graph is directed or undirected
  unsigned nodes_;                 // Number of nodes
  unsigned edges_;                 // Number of edges
};

// Constructor
Graph::Graph(const bool directed, const unsigned nodes) : directed_(directed), nodes_(nodes), edges_(0) {
  adj_list_.Resize(nodes_);  // Initialize adjacency list size based on the number of nodes
}

unsigned Graph::GetNodes() const {
  return nodes_;
}

unsigned Graph::GetEdges() const {
  return edges_;
}

// Check if the graph is directed
bool Graph::directed() const {
  return directed_;
}

// Add a new edge to the graph
void Graph::AddEdge(const unsigned start, const unsigned end, const int weight) {
  // Add edge from start to end
  // changes avoid repetitions, edges between non-existing nodes and duplication in unidirectional graphs
  if ((start < 1 || start > nodes_) || (end < 1 || end > nodes_)) {
    return;
  }
  for (int i{0}; i < adj_list_.at(start - 1).GetSize(); ++i) {
    if (adj_list_.at(start - 1).at(i).node == end - 1) {
      return;
    }
  }
  Node aux;
  aux.node = end - 1;
  aux.weight = weight;
  adj_list_.at(start - 1).PushBack(aux);

  // If the graph is undirected, add the reverse edge (from end to start)
  if (!directed_ && start != end) {
    aux.node = start - 1;  // Reverse the direction
    adj_list_.at(end - 1).PushBack(aux);
  }
  edges_++;
}

// Prints the adjacency list
void Graph::PrintList() const {
  for (unsigned i = 0; i < nodes_; ++i) {
    std::cout << "Node " << i + 1 << ": [";
    for (unsigned j = 0; j < adj_list_.at(i).GetSize(); ++j) {
      std::cout << "(" << adj_list_.at(i).at(j).node + 1
                << ", " << adj_list_.at(i).at(j).weight << ")";
      if (j < adj_list_.at(i).GetSize() - 1) {
        std::cout << ", ";
      }
    }
    std::cout << "]" << std::endl;
  }
}

void Graph::PrintBFS(const unsigned initial_node) const {
  Vector<int> pred(nodes_);
  Vector<bool> visited(nodes_);
  BFS(initial_node, pred, visited);
  std::cout << "BFS from node " << initial_node << ":\n";
  for (unsigned i = 0; i < nodes_; ++i) {
    if (visited.at(i)) {
      std::cout << "Node " << i + 1 << " visited, predecessor: " << (pred.at(i) + 1) << "\n";
    } else {
      std::cout << "Node " << i + 1 << " not reachable\n";
    }
  }
}

void Graph::BFS(const unsigned initial_node, Vector<int>& pred, Vector<bool>& visited) const {
  Queue<unsigned> queue;
  
  // Initialize visited and predecessor vectors
  visited.AllTo(false);
  pred.AllTo(-1);

  unsigned start_node = initial_node - 1;  // Convert to zero-indexed
  unsigned current_node;

  queue.PushBack(start_node);
  visited.at(start_node) = true;
  pred.at(start_node) = start_node;

  while (!queue.Empty()) {
    current_node = queue.PopFront();

    // Explore all the neighbors of the current node
    for (int i = 0; i < adj_list_.at(current_node).GetSize(); ++i) {
      unsigned neighbor = adj_list_.at(current_node).at(i).node;  // Get the neighbor node

      // If the neighbor has not been visited, add it to the queue
      if (!visited.at(neighbor)) {
        queue.PushBack(neighbor);
        visited.at(neighbor) = true;
        pred.at(neighbor) = current_node;  // Set the predecessor
      }
    }
  }
}

void Graph<T>::DFS(const unsigned initial_node) {

}


#endif
