#include "vector.h"
#include "graph.h"
#include "queue.h"
#include <vector>

int main () {
  Graph grafo(true, 7);
  grafo.AddEdge(1, 3);
  grafo.AddEdge(1, 2);
  grafo.AddEdge(2, 5);
  grafo.AddEdge(5, 4);
  grafo.AddEdge(1, 5);
  grafo.AddEdge(4, 6);
  grafo.PrintBFS(7);
}