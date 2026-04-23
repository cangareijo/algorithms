#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned vertex;
  unsigned distance;
} HeapNode;

void swapHeapNodes(HeapNode *a, HeapNode *b);

typedef struct {
  unsigned size;
  unsigned capacity;
  HeapNode *nodes;
} Heap;

Heap *createHeap();
void freeHeap(Heap *heap);
void insertInHeap(Heap *heap, unsigned vertex, unsigned distance);
unsigned getMinimumFromHeap(const Heap *heap);
void removeMinimumFromHeap(Heap *heap);

typedef struct Edge {
  unsigned destination;
  unsigned distance;
  struct Edge *next;
} Edge;

typedef struct {
  unsigned size;
  Edge **edges;
} Graph;

Graph *createGraph(unsigned size);
void destroyGraph(Graph *graph);
void addEdgeToGraph(Graph *graph, unsigned source, unsigned destination, unsigned distance);
bool isCyclicGraphComponent(const Graph *graph, unsigned vertex, bool *visited, bool *visiting);
bool isCyclicGraph(const Graph *graph);
void depthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *depthFirstSortOfGraph(const Graph *graph, unsigned source);
void breadthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *breadthFirstSortOfGraph(const Graph *graph, unsigned source);
void topologicalSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *topologicalSortOfGraph(const Graph *graph);
unsigned *distancesInGraph(const Graph *graph, unsigned source);

bool isValidTopologicalSort(const Graph *graph, const unsigned *ordering);
void testIsCyclicGraph();
void testDepthFirstSortOfGraph();
void testBreadthFirstSortOfGraph();
void testTopologicalSortOfGraph();
void testDistancesInGraph();

void swapHeapNodes(HeapNode *a, HeapNode *b) {
  HeapNode c = *a;
  HeapNode d = *b;
  *a = d;
  *b = c;
}

Heap *createHeap() {
  Heap *heap = malloc(sizeof(Heap));
  heap->size = 0;
  heap->capacity = 1;
  heap->nodes = malloc(heap->capacity * sizeof(HeapNode));
  return heap;
}

void freeHeap(Heap *heap) {
  free(heap->nodes);
  free(heap);
}

void insertInHeap(Heap *heap, unsigned vertex, unsigned distance) {
  if (heap->size >= heap->capacity) {
    heap->capacity *= 2;
    heap->nodes = realloc(heap->nodes, heap->capacity * sizeof(HeapNode));
  }
  unsigned i = heap->size++;
  heap->nodes[i] = (HeapNode){vertex, distance};
  while (i > 0 && heap->nodes[(i - 1) / 2].distance > heap->nodes[i].distance) {
    swapHeapNodes(&heap->nodes[i], &heap->nodes[(i - 1) / 2]);
    i = (i - 1) / 2;
  }
}

unsigned getMinimumFromHeap(const Heap *heap) {
  return heap->nodes[0].vertex;
}

void removeMinimumFromHeap(Heap *heap) {
  heap->nodes[0] = heap->nodes[--heap->size];
  unsigned i = 0;
  while (true) {
    unsigned left = 2 * i + 1, right = 2 * i + 2, smallest = i;
    if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) smallest = left;
    if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) smallest = right;
    if (smallest != i) {
      swapHeapNodes(&heap->nodes[i], &heap->nodes[smallest]);
      i = smallest;
    } else {
      break;
    }
  }
}

Graph *createGraph(unsigned size) {
  Graph *graph = malloc(sizeof(Graph));
  graph->size = size;
  graph->edges = calloc(size, sizeof(Edge *));
  return graph;
}

void destroyGraph(Graph *graph) {
  for (unsigned i = 0; i < graph->size; i++) {
    Edge *current = graph->edges[i];
    while (current) {
      Edge *next = current->next;
      free(current);
      current = next;
    }
  }
  free(graph->edges);
  free(graph);
}

void addEdgeToGraph(Graph *graph, unsigned source, unsigned destination, unsigned distance) {
  Edge *edge = malloc(sizeof(Edge));
  edge->destination = destination;
  edge->distance = distance;
  edge->next = graph->edges[source];
  graph->edges[source] = edge;
}

bool isCyclicGraphComponent(const Graph *graph, unsigned vertex, bool *visited, bool *visiting) {
  visited[vertex] = true;
  visiting[vertex] = true;
  for (Edge *edge = graph->edges[vertex]; edge; edge = edge->next)
    if (visiting[edge->destination] || (!visited[edge->destination] && isCyclicGraphComponent(graph, edge->destination, visited, visiting)))
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

void depthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *count, bool *visited) {
  assert(source < graph->size);
  ordering[(*count)++] = source;
  visited[source] = true;
  for (Edge *edge = graph->edges[source]; edge; edge = edge->next)
    if (!visited[edge->destination])
      depthFirstSortOfGraphComponent(graph, edge->destination, ordering, count, visited);
}

unsigned *depthFirstSortOfGraph(const Graph *graph, unsigned source) {
  assert(source < graph->size);
  unsigned *ordering = malloc(graph->size * sizeof(unsigned));
  unsigned count = 0;
  bool visited[graph->size];
  for (unsigned i = 0; i < graph->size; i++)
    visited[i] = false;
  depthFirstSortOfGraphComponent(graph, source, ordering, &count, visited);
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i])
      depthFirstSortOfGraphComponent(graph, i, ordering, &count, visited);
  return ordering;
}

void breadthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *count, bool *visited) {
  assert(source < graph->size);
  unsigned queue[graph->size], head = 0, tail = 0;
  visited[source] = true;
  queue[tail++] = source;
  while (head < tail) {
    unsigned current = queue[head++];
    ordering[(*count)++] = current;
    for (Edge *edge = graph->edges[current]; edge; edge = edge->next)
      if (!visited[edge->destination]) {
        visited[edge->destination] = true;
        queue[tail++] = edge->destination;
      }
  }
}

unsigned *breadthFirstSortOfGraph(const Graph *graph, unsigned source) {
  assert(source < graph->size);
  unsigned *ordering = malloc(graph->size * sizeof(unsigned));
  unsigned count = 0;
  bool visited[graph->size];
  for (unsigned i = 0; i < graph->size; i++)
    visited[i] = false;
  breadthFirstSortOfGraphComponent(graph, source, ordering, &count, visited);
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i])
      breadthFirstSortOfGraphComponent(graph, i, ordering, &count, visited);
  return ordering;
}

void topologicalSortOfGraphComponent(const Graph *graph, unsigned vertex, unsigned *ordering, unsigned *index, bool *visited) {
  visited[vertex] = true;
  for (Edge *edge = graph->edges[vertex]; edge; edge = edge->next)
    if (!visited[edge->destination])
      topologicalSortOfGraphComponent(graph, edge->destination, ordering, index, visited);
  ordering[--(*index)] = vertex;
}

unsigned *topologicalSortOfGraph(const Graph *graph) {
  unsigned *ordering = malloc(graph->size * sizeof(unsigned));
  unsigned index = graph->size;
  bool visited[graph->size];
  for (unsigned i = 0; i < graph->size; i++)
    visited[i] = false;
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i])
      topologicalSortOfGraphComponent(graph, i, ordering, &index, visited);
  return ordering;
}

unsigned *distancesInGraph(const Graph *graph, unsigned source) {
  unsigned *distances = malloc(graph->size * sizeof(unsigned));
  for (unsigned i = 0; i < graph->size; i++) distances[i] = UINT_MAX;
  distances[source] = 0;
  Heap *heap = createHeap();
  insertInHeap(heap, source, 0);
  while (heap->size > 0) {
    unsigned vertex = getMinimumFromHeap(heap);
    removeMinimumFromHeap(heap);
    for (Edge *edge = graph->edges[vertex]; edge; edge = edge->next)
      if (distances[vertex] + edge->distance < distances[edge->destination]) {
        distances[edge->destination] = distances[vertex] + edge->distance;
        insertInHeap(heap, edge->destination, distances[edge->destination]);
      }
  }
  freeHeap(heap);
  return distances;
}

bool isValidTopologicalSort(const Graph *graph, const unsigned *ordering) {
  unsigned position[graph->size];
  for (unsigned i = 0; i < graph->size; i++)
    position[ordering[i]] = i;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *edge = graph->edges[v]; edge; edge = edge->next)
      if (position[v] >= position[edge->destination])
        return false;
  return true;
}

void testIsCyclicGraph() {
  Graph *g = createGraph(3);
  addEdgeToGraph(g, 0, 1, 1);
  addEdgeToGraph(g, 1, 2, 1);
  addEdgeToGraph(g, 2, 0, 1);
  if (isCyclicGraph(g))
    printf("Cycle Detection Test passed: Cycle found!\n");
  else
    printf("Cycle Detection Test failed: No cycle found.\n");
  destroyGraph(g);
}

void testDepthFirstSortOfGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToGraph(g1, 0, 1, 1);
  addEdgeToGraph(g1, 1, 2, 1);
  unsigned *order1 = depthFirstSortOfGraph(g1, 0);
  assert(order1[0] == 0);
  assert(order1[1] == 1);
  assert(order1[2] == 2);
  printf("DFS test 1 (Linear) passed!\n");
  destroyGraph(g1);
  free(order1);

  Graph *g2 = createGraph(3);
  addEdgeToGraph(g2, 0, 2, 1);
  addEdgeToGraph(g2, 0, 1, 1); 
  unsigned *order2 = depthFirstSortOfGraph(g2, 0);
  assert(order2[0] == 0);
  assert(order2[1] == 1); 
  assert(order2[2] == 2);
  printf("DFS test 2 (Fork) passed!\n");
  destroyGraph(g2);
  free(order2);

  Graph *g3 = createGraph(3);
  addEdgeToGraph(g3, 0, 1, 1);
  unsigned *order3 = depthFirstSortOfGraph(g3, 0);
  assert(order3[0] == 0);
  assert(order3[1] == 1);
  assert(order3[2] == 2);
  printf("DFS test 3 (Disconnected) passed!\n");
  destroyGraph(g3);
  free(order3);

  Graph *g4 = createGraph(2);
  addEdgeToGraph(g4, 0, 1, 1);
  addEdgeToGraph(g4, 1, 0, 1);
  unsigned *order4 = depthFirstSortOfGraph(g4, 0);
  assert(order4[0] == 0);
  assert(order4[1] == 1);
  printf("DFS test 4 (Cyclic) passed!\n");
  destroyGraph(g4);
  free(order4);
}

void testBreadthFirstSortOfGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToGraph(g1, 0, 1, 1);
  addEdgeToGraph(g1, 1, 2, 1);
  unsigned *order1 = breadthFirstSortOfGraph(g1, 0);
  assert(order1[0] == 0);
  assert(order1[1] == 1);
  assert(order1[2] == 2);
  printf("BFS test 1 (linear) passed!\n");
  destroyGraph(g1);
  free(order1);

  Graph *g2 = createGraph(3);
  addEdgeToGraph(g2, 0, 2, 1);
  addEdgeToGraph(g2, 0, 1, 1); 
  unsigned *order2 = breadthFirstSortOfGraph(g2, 0);
  assert(order2[0] == 0);
  assert(order2[1] == 1); 
  assert(order2[2] == 2);
  printf("BFS test 2 (fork) passed!\n");
  destroyGraph(g2);
  free(order2);

  Graph *g3 = createGraph(3);
  addEdgeToGraph(g3, 0, 1, 1);
  unsigned *order3 = breadthFirstSortOfGraph(g3, 0);
  assert(order3[0] == 0);
  assert(order3[1] == 1);
  assert(order3[2] == 2);
  printf("BFS test 3 (disconnected) passed!\n");
  destroyGraph(g3);
  free(order3);

  Graph *g4 = createGraph(2);
  addEdgeToGraph(g4, 0, 1, 1);
  addEdgeToGraph(g4, 1, 0, 1);
  unsigned *order4 = breadthFirstSortOfGraph(g4, 0);
  assert(order4[0] == 0);
  assert(order4[1] == 1);
  printf("BFS test 4 (cycle) passed!\n");
  destroyGraph(g4);
  free(order4);
}

void testTopologicalSortOfGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToGraph(g1, 0, 1, 1);
  addEdgeToGraph(g1, 1, 2, 1);
  unsigned *order1 = topologicalSortOfGraph(g1);
  assert(isValidTopologicalSort(g1, order1));
  printf("Topo test 1 (linear) passed!\n");
  destroyGraph(g1);
  free(order1);

  Graph *g2 = createGraph(4);
  addEdgeToGraph(g2, 0, 1, 1);
  addEdgeToGraph(g2, 0, 2, 1);
  addEdgeToGraph(g2, 1, 3, 1);
  addEdgeToGraph(g2, 2, 3, 1);
  unsigned *order2 = topologicalSortOfGraph(g2);
  assert(isValidTopologicalSort(g2, order2));
  printf("Topo test 2 (diamond) passed!\n");
  destroyGraph(g2);
  free(order2);

  Graph *g3 = createGraph(4);
  addEdgeToGraph(g3, 0, 1, 1);
  addEdgeToGraph(g3, 2, 3, 1);
  unsigned *order3 = topologicalSortOfGraph(g3);
  assert(isValidTopologicalSort(g3, order3));
  printf("Topo test 3 (disconnected) passed!\n");
  destroyGraph(g3);
  free(order3);

  Graph *g4 = createGraph(1);
  unsigned *order4 = topologicalSortOfGraph(g4);
  assert(order4[0] == 0);
  printf("Topo test 4 (single vertex) passed!\n");
  destroyGraph(g4);
  free(order4);
}

void testDistancesInGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToGraph(g1, 0, 1, 5);
  addEdgeToGraph(g1, 1, 2, 10);
  unsigned *dist1 = distancesInGraph(g1, 0);
  assert(dist1[0] == 0);
  assert(dist1[1] == 5);
  assert(dist1[2] == 15);
  printf("Test 1 passed: Simple path\n");
  destroyGraph(g1);
  free(dist1);

  Graph *g2 = createGraph(3);
  addEdgeToGraph(g2, 0, 2, 10);
  addEdgeToGraph(g2, 0, 1, 2);
  addEdgeToGraph(g2, 1, 2, 3);
  unsigned *dist2 = distancesInGraph(g2, 0);
  assert(dist2[2] == 5);
  printf("Test 2 passed: Shortest path selection\n");
  destroyGraph(g2);
  free(dist2);

  Graph *g3 = createGraph(2);
  unsigned *dist3 = distancesInGraph(g3, 0);
  assert(dist3[0] == 0);
  assert(dist3[1] == UINT_MAX);
  printf("Test 3 passed: Unreachable vertex (UINT_MAX)\n");
  destroyGraph(g3);
  free(dist3);

  Graph *g4 = createGraph(3);
  addEdgeToGraph(g4, 0, 1, 1);
  addEdgeToGraph(g4, 1, 2, 1);
  addEdgeToGraph(g4, 2, 0, 1);
  unsigned *dist4 = distancesInGraph(g4, 0);
  assert(dist4[0] == 0);
  assert(dist4[1] == 1);
  assert(dist4[2] == 2);
  printf("Test 4 passed: Cyclic graph\n");
  destroyGraph(g4);
  free(dist4);
}

int main() {
  testIsCyclicGraph();
  testDepthFirstSortOfGraph();
  testBreadthFirstSortOfGraph();
  testTopologicalSortOfGraph();
  testDistancesInGraph();
  return 0;
}
