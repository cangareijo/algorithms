#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  unsigned vertex;
  struct Node *next;
} Node;

static Node *createNode(unsigned vertex, Node *next);

typedef struct {
  unsigned size;
  Node **adjacency;
} Graph;

Graph *createGraph(unsigned size);
void destroyGraph(Graph *graph);
void addEdgeToGraph(Graph *graph, unsigned source, unsigned destination);
bool isCyclicGraphComponent(const Graph *graph, unsigned vertex, bool *visited, bool *visiting);
bool isCyclicGraph(const Graph *graph);
void depthFirstSortOfGraphComponent(const Graph *graph, unsigned start, unsigned *ordering, unsigned *index, bool *visited);
void depthFirstSortOfGraph(const Graph *graph, unsigned start, unsigned *ordering);
void breadthFirstSortOfGraphComponent(const Graph *graph, unsigned start, unsigned *ordering, unsigned *index, bool *visited);
void breadthFirstSortOfGraph(const Graph *graph, unsigned start, unsigned *ordering);
void topologicalSortOfGraphComponent(const Graph *graph, unsigned start, unsigned *ordering, unsigned *index, bool *visited);
void topologicalSortOfGraph(const Graph *graph, unsigned *ordering);

bool isValidTopologicalSort(const Graph *graph, const unsigned *ordering);
void testIsCyclicGraph();
void testDepthFirstSortOfGraph();
void testBreadthFirstSortOfGraph();


static Node *createNode(unsigned vertex, Node *next) {
  Node *node = malloc(sizeof(Node));
  node->vertex = vertex;
  node->next = next;
  return node;
}

Graph *createGraph(unsigned size) {
  Graph *graph = malloc(sizeof(Graph));
  graph->size = size;
  graph->adjacency = calloc(size, sizeof(Node *));
  return graph;
}

void destroyGraph(Graph *graph) {
  for (unsigned i = 0; i < graph->size; i++) {
    Node *node = graph->adjacency[i];
    while (node) {
      Node *next = node->next;
      free(node);
      node = next;
    }
  }
  free(graph->adjacency);
  free(graph);
}

void addEdgeToGraph(Graph *graph, unsigned source, unsigned destination) {
  graph->adjacency[source] = createNode(destination, graph->adjacency[source]);
}

bool isCyclicGraphComponent(const Graph *graph, unsigned vertex, bool *visited, bool *visiting) {
  visited[vertex] = true;
  visiting[vertex] = true;
  for (Node *adjacency = graph->adjacency[vertex]; adjacency; adjacency = adjacency->next)
    if (visiting[adjacency->vertex] || (!visited[adjacency->vertex] && isCyclicGraphComponent(graph, adjacency->vertex, visited, visiting)))
      return true;
  visiting[vertex] = false;
  return false;
}

bool isCyclicGraph(const Graph *graph) {
  bool visited[graph->size];
  bool visiting[graph->size];
  for (unsigned i = 0; i < graph->size; i++) {
    visited[i] = false;
    visiting[i] = false;
  }
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i] && isCyclicGraphComponent(graph, i, visited, visiting))
      return true;
  return false;
}

void depthFirstSortOfGraphComponent(const Graph *graph, unsigned start, unsigned *ordering, unsigned *count, bool *visited) {
  assert(start < graph->size);
  ordering[(*count)++] = start;
  visited[start] = true;
  for (Node *adjacency = graph->adjacency[start]; adjacency; adjacency = adjacency->next)
    if (!visited[adjacency->vertex])
      depthFirstSortOfGraphComponent(graph, adjacency->vertex, ordering, count, visited);
}

void depthFirstSortOfGraph(const Graph *graph, unsigned start, unsigned *ordering) {
  assert(start < graph->size);
  unsigned count = 0;
  bool visited[graph->size];
  for (unsigned i = 0; i < graph->size; i++)
    visited[i] = false;
  depthFirstSortOfGraphComponent(graph, start, ordering, &count, visited);
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i])
      depthFirstSortOfGraphComponent(graph, i, ordering, &count, visited);
}

void breadthFirstSortOfGraphComponent(const Graph *graph, unsigned start, unsigned *ordering, unsigned *count, bool *visited) {
  assert(start < graph->size);
  unsigned queue[graph->size], head = 0, tail = 0;
  visited[start] = true;
  queue[tail++] = start;
  while (head < tail) {
    unsigned current = queue[head++];
    ordering[(*count)++] = current;
    for (Node *adjacency = graph->adjacency[current]; adjacency; adjacency = adjacency->next)
      if (!visited[adjacency->vertex]) {
        visited[adjacency->vertex] = true;
        queue[tail++] = adjacency->vertex;
      }
  }
}

void breadthFirstSortOfGraph(const Graph *graph, unsigned start, unsigned *ordering) {
  assert(start < graph->size);
  unsigned count = 0;
  bool visited[graph->size];
  for (unsigned i = 0; i < graph->size; i++)
    visited[i] = false;
  breadthFirstSortOfGraphComponent(graph, start, ordering, &count, visited);
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i])
      breadthFirstSortOfGraphComponent(graph, i, ordering, &count, visited);
}

void topologicalSortOfGraphComponent(const Graph *graph, unsigned vertex, unsigned *ordering, unsigned *index, bool *visited) {
  visited[vertex] = true;
  for (Node *adjacency = graph->adjacency[vertex]; adjacency; adjacency = adjacency->next)
    if (!visited[adjacency->vertex])
      topologicalSortOfGraphComponent(graph, adjacency->vertex, ordering, index, visited);
  ordering[--(*index)] = vertex;
}

void topologicalSortOfGraph(const Graph *graph, unsigned *ordering) {
  assert(!isCyclicGraph(graph));
  unsigned index = graph->size;
  bool visited[graph->size];
  for (unsigned i = 0; i < graph->size; i++)
    visited[i] = false;
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i])
      topologicalSortOfGraphComponent(graph, i, ordering, &index, visited);
}

bool isValidTopologicalSort(const Graph *graph, const unsigned *ordering) {
  unsigned position[graph->size];
  for (unsigned i = 0; i < graph->size; i++)
    position[ordering[i]] = i;
  for (unsigned v = 0; v < graph->size; v++)
    for (Node *adjacency = graph->adjacency[v]; adjacency; adjacency = adjacency->next)
      if (position[v] >= position[adjacency->vertex])
        return false;
  return true;
}

void testIsCyclicGraph() {
  Graph *g = createGraph(3);
  addEdgeToGraph(g, 0, 1);
  addEdgeToGraph(g, 1, 2);
  addEdgeToGraph(g, 2, 0);
  if (isCyclicGraph(g))
    printf("Cycle Detection Test passed: Cycle found!\n");
  else
    printf("Cycle Detection Test failed: No cycle found.\n");
  destroyGraph(g);
}

void testDepthFirstSortOfGraph() {
  Graph *graph1 = createGraph(3);
  addEdgeToGraph(graph1, 0, 1);
  addEdgeToGraph(graph1, 1, 2);
  unsigned order1[3];
  depthFirstSortOfGraph(graph1, 0, order1);
  destroyGraph(graph1);
  assert(order1[0] == 0);
  assert(order1[1] == 1);
  assert(order1[2] == 2);
  printf("DFS test 1 (Linear) passed!\n");

  Graph *g2 = createGraph(3);
  addEdgeToGraph(g2, 0, 2);
  addEdgeToGraph(g2, 0, 1); 
  unsigned order2[3];
  depthFirstSortOfGraph(g2, 0, order2);
  assert(order2[0] == 0);
  assert(order2[1] == 1); 
  assert(order2[2] == 2);
  destroyGraph(g2);
  printf("DFS test 2 (Fork) passed!\n");

  Graph *graph2 = createGraph(3);
  addEdgeToGraph(graph2, 0, 1);
  unsigned order3[3];
  depthFirstSortOfGraph(graph2, 0, order3);
  destroyGraph(graph2);
  assert(order3[0] == 0);
  assert(order3[1] == 1);
  assert(order3[2] == 2);
  printf("DFS test 3 (Disconnected) passed!\n");

  Graph *graph3 = createGraph(2);
  addEdgeToGraph(graph3, 0, 1);
  addEdgeToGraph(graph3, 1, 0);
  unsigned order4[2];
  depthFirstSortOfGraph(graph3, 0, order4);
  destroyGraph(graph3);
  assert(order4[0] == 0);
  assert(order4[1] == 1);
  printf("DFS test 4 (Cyclic) passed!\n");
}

void testBreadthFirstSortOfGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToGraph(g1, 0, 1);
  addEdgeToGraph(g1, 1, 2);
  unsigned order1[3];
  breadthFirstSortOfGraph(g1, 0, order1);
  assert(order1[0] == 0);
  assert(order1[1] == 1);
  assert(order1[2] == 2);
  destroyGraph(g1);
  printf("BFS test 1 (Linear) passed!\n");

  Graph *g2 = createGraph(3);
  addEdgeToGraph(g2, 0, 2);
  addEdgeToGraph(g2, 0, 1); 
  unsigned order2[3];
  breadthFirstSortOfGraph(g2, 0, order2);
  assert(order2[0] == 0);
  assert(order2[1] == 1); 
  assert(order2[2] == 2);
  destroyGraph(g2);
  printf("BFS test 2 (Fork) passed!\n");

  Graph *g3 = createGraph(3);
  addEdgeToGraph(g3, 0, 1);
  unsigned order3[3];
  breadthFirstSortOfGraph(g3, 0, order3);
  assert(order3[0] == 0);
  assert(order3[1] == 1);
  assert(order3[2] == 2);
  destroyGraph(g3);
  printf("BFS test 3 (Disconnected) passed!\n");

  Graph *g4 = createGraph(2);
  addEdgeToGraph(g4, 0, 1);
  addEdgeToGraph(g4, 1, 0);
  unsigned order4[2];
  breadthFirstSortOfGraph(g4, 0, order4);
  assert(order4[0] == 0);
  assert(order4[1] == 1);
  destroyGraph(g4);
  printf("BFS test 4 (Cycle) passed!\n");
}

void testTopologicalSortOfGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToGraph(g1, 0, 1);
  addEdgeToGraph(g1, 1, 2);
  unsigned order1[3];
  topologicalSortOfGraph(g1, order1);
  assert(isValidTopologicalSort(g1, order1));
  destroyGraph(g1);
  printf("Topo test 1 (Linear) passed!\n");

  Graph *g2 = createGraph(4);
  addEdgeToGraph(g2, 0, 1);
  addEdgeToGraph(g2, 0, 2);
  addEdgeToGraph(g2, 1, 3);
  addEdgeToGraph(g2, 2, 3);
  unsigned order2[4];
  topologicalSortOfGraph(g2, order2);
  assert(isValidTopologicalSort(g2, order2));
  destroyGraph(g2);
  printf("Topo test 2 (Diamond) passed!\n");

  Graph *g3 = createGraph(4);
  addEdgeToGraph(g3, 0, 1);
  addEdgeToGraph(g3, 2, 3);
  unsigned order3[4];
  topologicalSortOfGraph(g3, order3);
  assert(isValidTopologicalSort(g3, order3));
  destroyGraph(g3);
  printf("Topo test 3 (Disconnected) passed!\n");

  Graph *g4 = createGraph(1);
  unsigned order4[1];
  topologicalSortOfGraph(g4, order4);
  assert(order4[0] == 0);
  destroyGraph(g4);
  printf("Topo test 4 (Single Node) passed!\n");
}

int main() {
  testIsCyclicGraph();
  testDepthFirstSortOfGraph();
  testBreadthFirstSortOfGraph();
  testTopologicalSortOfGraph();
  return 0;
}
