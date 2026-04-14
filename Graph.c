#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>



struct Node {
  unsigned vertex;
  struct Node *next;
};

struct Node *addNeighbor(struct Node *node, unsigned vertex) {
  if (!node) {
    node = malloc(sizeof(*node));
    node->vertex = vertex;
    node->next = NULL;
  } else if (vertex != node->vertex) {
    node->next = addNeighbor(node->next, vertex);
  }
  return node;
}

void freeNeighbors(struct Node *node) {
  if (node) {
    freeNeighbors(node->next);
    free(node);
  }
}



struct Graph {
  unsigned size;
  struct Node **neighbors;
};

struct Graph *createGraph(unsigned size) {
  struct Graph *graph = malloc(sizeof(struct Graph));
  graph->size = size;
  graph->neighbors = calloc(size, sizeof(*graph->neighbors));
  return graph;
}

void freeGraph(struct Graph *graph) {
  for (unsigned i = 0; i < graph->size; i++) freeNeighbors(graph->neighbors[i]);
  free(graph->neighbors);
  free(graph);
}

void addDirectedEdge(struct Graph *graph, unsigned source, unsigned destination) {
  assert(graph);
  assert(source < graph->size);
  assert(destination < graph->size);
  graph->neighbors[source] = addNeighbor(graph->neighbors[source], destination);
}

void addUndirectedEdge(struct Graph *graph, unsigned source, unsigned destination) {
  addDirectedEdge(graph, source, destination);
  if (source != destination) addDirectedEdge(graph, destination, source);
}

static void depthFirstSearchRecursive(const struct Graph *graph, bool *visited, unsigned vertex) {
  assert(vertex < graph->size);
  visited[vertex] = true;
  printf("Visited %d\n", vertex);
  for (const struct Node *neighbors = graph->neighbors[vertex]; neighbors; neighbors = neighbors->next)
    if (!visited[neighbors->vertex]) depthFirstSearchRecursive(graph, visited, neighbors->vertex);
}

void depthFirstSearch(const struct Graph *graph, unsigned vertex) {
  bool *visited = calloc(graph->size, sizeof(*visited));
  depthFirstSearchRecursive(graph, visited, vertex);
  free(visited);
}

static bool isCyclicUndirectedRecursive(const struct Graph *graph, bool *visited, unsigned parent, unsigned current) {
  visited[current] = true;
  for (struct Node *neighbors = graph->neighbors[current]; neighbors; neighbors = neighbors->next)
    if (!visited[neighbors->vertex]) { if (isCyclicUndirectedRecursive(graph, visited, current, neighbors->vertex)) return true; }
    else if (neighbors->vertex != parent) return true;
  return false;
}

bool isCyclicUndirected(const struct Graph *graph) {
  bool *visited = calloc(graph->size, sizeof(*visited));
  bool cyclic = false;
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i] && isCyclicUndirectedRecursive(graph, visited, graph->size, i)) { cyclic = true; break; }
  free(visited);
  return cyclic;
}

static bool isCyclicDirectedRecursive(const struct Graph *graph, bool *visited, bool *stack, unsigned vertex) {
  visited[vertex] = true;
  stack[vertex] = true;
  for (const struct Node *neighbors = graph->neighbors[vertex]; neighbors; neighbors = neighbors->next)
    if (stack[neighbors->vertex] || (!visited[neighbors->vertex] && isCyclicDirectedRecursive(graph, visited, stack, neighbors->vertex)))
      return true;
  stack[vertex] = false;
  return false;
}

bool isCyclicDirected(const struct Graph *graph) {
  bool *visited = calloc(graph->size, sizeof(*visited));
  bool *stack = calloc(graph->size, sizeof(*stack));
  bool cyclic = false;
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i] && isCyclicDirectedRecursive(graph, visited, stack, i)) { cyclic = true; break; }
  free(visited);
  free(stack);
  return cyclic;
}

int main() {
  struct Graph *graph1 = createGraph(4);
  addUndirectedEdge(graph1, 0, 1);
  addUndirectedEdge(graph1, 0, 2);
  addUndirectedEdge(graph1, 1, 2);
  addUndirectedEdge(graph1, 2, 3);
  unsigned start = 2;
  printf("DFS starting from vertex %d:\n", start);
  depthFirstSearch(graph1, start);
  freeGraph(graph1);
  struct Graph *graph2 = createGraph(4);
  addDirectedEdge(graph2, 0, 1);
  addDirectedEdge(graph2, 1, 2);
  addDirectedEdge(graph2, 2, 0);
  addDirectedEdge(graph2, 2, 3);
  if (isCyclicDirected(graph2)) printf("Graph 2 contains a cycle.\n"); else printf("Graph 2 does not contain a cycle.\n");
  freeGraph(graph2);
  return 0;
}
