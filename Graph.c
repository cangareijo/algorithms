#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned vertex;
  unsigned priority;
} HeapNode;

void swapHeapNodes(HeapNode *a, HeapNode *b);

typedef struct {
  unsigned size;
  unsigned capacity;
  HeapNode *nodes;
} Heap;

Heap *createHeap();
void freeHeap(Heap *heap);
void insertInHeap(Heap *heap, unsigned vertex, unsigned priority);
unsigned getMinimumFromHeap(const Heap *heap);
void removeMinimumFromHeap(Heap *heap);

typedef struct {
  unsigned *parent;
  unsigned *rank;
} Dsu;

Dsu *createDsu(unsigned n);
void freeDsu(Dsu *dsu);
unsigned findDsu(Dsu *dsu, unsigned i);
void unionDsu(Dsu *dsu, unsigned i, unsigned j);

typedef struct {
  unsigned u, v;
  int weight;
} FlatEdge;

int compareEdges(const void *a, const void *b);

typedef struct Edge {
  unsigned destination;
  int weight;
  struct Edge *next;
} Edge;

typedef struct {
  unsigned size;
  Edge **edges;
} Graph;

Graph *createGraph(unsigned size);
void destroyGraph(Graph *graph);
void addEdgeToDirectedGraph(Graph *graph, unsigned source, unsigned destination, int weight);
void addEdgeToUndirectedGraph(Graph *graph, unsigned u, unsigned v, int weight);
unsigned countEdgesInDirectedGraph(const Graph *graph);
unsigned countEdgesInUndirectedGraph(const Graph *graph);
FlatEdge *getEdgeArrayFromUndirectedGraph(const Graph *graph);
void printGraph(const Graph *graph);
bool isDirectedCyclicGraphComponent(const Graph *graph, unsigned vertex, char *visited);
bool isDirectedCyclicGraph(const Graph *graph);
bool isUndirectedCyclicGraphComponent(const Graph *graph, unsigned vertex, unsigned parent, bool *visited);
bool isUndirectedCyclicGraph(const Graph *graph);
void depthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *depthFirstSortOfGraph(const Graph *graph, unsigned source);
void breadthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *breadthFirstSortOfGraph(const Graph *graph, unsigned source);
void topologicalSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *topologicalSortOfGraph(const Graph *graph);
int *bellmanFord(const Graph *graph, unsigned source);
int *dijkstra(const Graph *graph, unsigned source);
Graph *prim(const Graph *graph, unsigned source);
Graph *kruskal(const Graph *graph);

void testIsDirectedCyclicGraph();
void testIsUndirectedCyclicGraph();
void testDepthFirstSortOfGraph();
void testBreadthFirstSortOfGraph();
bool isValidTopologicalSort(const Graph *graph, const unsigned *ordering);
void testTopologicalSortOfGraph();
void testBellmanFord();
void testDijkstra();
void getUndirectedGraphStats(const Graph *graph, int *weight, unsigned *count);
void testPrim();
void testKruskal();



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

void insertInHeap(Heap *heap, unsigned vertex, unsigned priority) {
  if (heap->size >= heap->capacity) {
    heap->capacity *= 2;
    heap->nodes = realloc(heap->nodes, heap->capacity * sizeof(HeapNode));
  }
  unsigned i = heap->size++;
  heap->nodes[i].vertex = vertex;
  heap->nodes[i].priority = priority;
  while (i > 0 && heap->nodes[(i - 1) / 2].priority > heap->nodes[i].priority) {
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
    if (left < heap->size && heap->nodes[left].priority < heap->nodes[smallest].priority) smallest = left;
    if (right < heap->size && heap->nodes[right].priority < heap->nodes[smallest].priority) smallest = right;
    if (smallest == i) break;
    swapHeapNodes(&heap->nodes[i], &heap->nodes[smallest]);
    i = smallest;
  }
}



Dsu *createDsu(unsigned n) {
  Dsu *dsu = malloc(sizeof(Dsu));
  dsu->parent = malloc(n * sizeof(unsigned));
  dsu->rank = calloc(n, sizeof(unsigned));
  for (unsigned i = 0; i < n; i++) dsu->parent[i] = i;
  return dsu;
}

void freeDsu(Dsu *dsu) {
  free(dsu->parent);
  free(dsu->rank);
  free(dsu);
}

unsigned findDsu(Dsu *dsu, unsigned i) {
  if (dsu->parent[i] == i) return i;
  return dsu->parent[i] = findDsu(dsu, dsu->parent[i]);
}

void unionDsu(Dsu *dsu, unsigned i, unsigned j) {
  unsigned root_i = findDsu(dsu, i);
  unsigned root_j = findDsu(dsu, j);
  if (root_i != root_j) {
    if (dsu->rank[root_i] < dsu->rank[root_j]) dsu->parent[root_i] = root_j;
    else if (dsu->rank[root_i] > dsu->rank[root_j]) dsu->parent[root_j] = root_i;
    else {
      dsu->parent[root_i] = root_j;
      dsu->rank[root_j]++;
    }
  }
}



int compareEdges(const void *a, const void *b) {
  return ((FlatEdge *)a)->weight - ((FlatEdge *)b)->weight;
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

void addEdgeToDirectedGraph(Graph *graph, unsigned u, unsigned v, int weight) {
  Edge *edge = malloc(sizeof(Edge));
  edge->destination = v;
  edge->weight = weight;
  edge->next = graph->edges[u];
  graph->edges[u] = edge;
}

void addEdgeToUndirectedGraph(Graph *graph, unsigned u, unsigned v, int weight) {
  addEdgeToDirectedGraph(graph, u, v, weight);
  addEdgeToDirectedGraph(graph, v, u, weight);
}

unsigned countEdgesInDirectedGraph(const Graph *graph) {
  unsigned count = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      count++;
  return count;
}

unsigned countEdgesInUndirectedGraph(const Graph *graph) {
  unsigned count = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      if (v < e->destination) count++;
  return count;
}

FlatEdge *getEdgeArrayFromUndirectedGraph(const Graph *graph) {
  FlatEdge *edges = malloc(countEdgesInUndirectedGraph(graph) * sizeof(FlatEdge));
  unsigned i = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      if (v < e->destination) {
        edges[i].u = v;
        edges[i].v = e->destination;
        edges[i].weight = e->weight;
        i++;
      }
  return edges;
}

void printGraph(const Graph *graph) {
  for (unsigned i = 0; i < graph->size; i++) {
    Edge *current = graph->edges[i];
    if (current) {
      while (current) {
        printf(" (%u, %u): %d,", i, current->destination, current->weight);
        current = current->next;
      }
      printf("\n");
    }
  }
}

bool isDirectedCyclicGraphComponent(const Graph *graph, unsigned vertex, char *visited) {
  visited[vertex] = 1;
  for (Edge *edge = graph->edges[vertex]; edge; edge = edge->next)
    if (visited[edge->destination] == 1 ||
      (visited[edge->destination] == 0 && isDirectedCyclicGraphComponent(graph, edge->destination, visited)))
        return true;
  visited[vertex] = 2;
  return false;
}

bool isDirectedCyclicGraph(const Graph *graph) {
  char visited[graph->size];
  for (unsigned vertex = 0; vertex < graph->size; vertex++)
    visited[vertex] = 0;
  for (unsigned vertex = 0; vertex < graph->size; vertex++)
    if (visited[vertex] == 0 && isDirectedCyclicGraphComponent(graph, vertex, visited))
      return true;
  return false;
}

bool isUndirectedCyclicGraphComponent(const Graph *graph, unsigned vertex, unsigned parent, bool *visited) {
  visited[vertex] = true;
  for (Edge *edge = graph->edges[vertex]; edge; edge = edge->next)
    if (visited[edge->destination]) {
      if (edge->destination != parent)
        return true;
    } else {
      if (isUndirectedCyclicGraphComponent(graph, edge->destination, vertex, visited))
        return true;
    }
  return false;
}

bool isUndirectedCyclicGraph(const Graph *graph) {
  bool *visited = calloc(graph->size, sizeof(bool));
  for (unsigned vertex = 0; vertex < graph->size; vertex++)
    if (!visited[vertex])
      if (isUndirectedCyclicGraphComponent(graph, vertex, graph->size, visited)) {
        free(visited);
        return true;
      }
  free(visited);
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

int *bellmanFord(const Graph *graph, unsigned source) {
  int *distance = malloc(graph->size * sizeof(int));
  for (unsigned v = 0; v < graph->size; v++) distance[v] = INT_MAX;
  distance[source] = 0;
  for (unsigned i = 1; i < graph->size; i++) {
    bool changed = false;
    for (unsigned v = 0; v < graph->size; v++)
      if (distance[v] < INT_MAX)
        for (Edge *e = graph->edges[v]; e; e = e->next)
          if (distance[v] + e->weight < distance[e->destination]) {
            distance[e->destination] = distance[v] + e->weight;
            changed = true;
          }
    if (!changed) break;
  }
  for (unsigned v = 0; v < graph->size; v++)
    if (distance[v] < INT_MAX)
      for (Edge *e = graph->edges[v]; e; e = e->next)
        if (distance[v] + (int)e->weight < distance[e->destination]) {
          free(distance);
          return NULL;
        }
  return distance;
}

int *dijkstra(const Graph *graph, unsigned source) {
  int *weights = malloc(graph->size * sizeof(int));
  for (unsigned v = 0; v < graph->size; v++) weights[v] = INT_MAX;
  weights[source] = 0;
  Heap *heap = createHeap();
  insertInHeap(heap, source, 0);
  while (heap->size > 0) {
    unsigned vertex = getMinimumFromHeap(heap);
    removeMinimumFromHeap(heap);
    for (Edge *edge = graph->edges[vertex]; edge; edge = edge->next)
      if (weights[vertex] + edge->weight < weights[edge->destination]) {
        weights[edge->destination] = weights[vertex] + edge->weight;
        insertInHeap(heap, edge->destination, weights[edge->destination]);
      }
  }
  freeHeap(heap);
  return weights;
}

Graph *prim(const Graph *graph, unsigned source) {
  unsigned parents[graph->size];
  int weights[graph->size];
  bool visited[graph->size];
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    parents[vertex] = UINT_MAX;
    weights[vertex] = INT_MAX;
    visited[vertex] = false;
  }
  weights[source] = 0;
  Heap *heap = createHeap();
  insertInHeap(heap, source, 0);
  Graph *tree = createGraph(graph->size);
  while (heap->size > 0) {
    unsigned vertex = getMinimumFromHeap(heap);
    removeMinimumFromHeap(heap);
    if (visited[vertex]) continue;
    visited[vertex] = true;
    if (parents[vertex] != UINT_MAX)
      addEdgeToUndirectedGraph(tree, parents[vertex], vertex, weights[vertex]);
    for (Edge *edge = graph->edges[vertex]; edge; edge = edge->next)
      if (!visited[edge->destination] && edge->weight < weights[edge->destination]) {
        parents[edge->destination] = vertex;
        weights[edge->destination] = edge->weight;
        insertInHeap(heap, edge->destination, edge->weight);
      }
  }
  freeHeap(heap);
  return tree;
}

Graph *kruskal(const Graph *graph) {
  unsigned edgeCount = countEdgesInUndirectedGraph(graph);
  FlatEdge *flatEdges = getEdgeArrayFromUndirectedGraph(graph);
  qsort(flatEdges, edgeCount, sizeof(FlatEdge), compareEdges);
  Dsu *dsu = createDsu(graph->size);
  Graph *mst = createGraph(graph->size);
  for (unsigned i = 0; i < edgeCount; i++)
    if (findDsu(dsu, flatEdges[i].u) != findDsu(dsu, flatEdges[i].v)) {
      unionDsu(dsu, flatEdges[i].u, flatEdges[i].v);
      addEdgeToUndirectedGraph(mst, flatEdges[i].u, flatEdges[i].v, flatEdges[i].weight);
    }
  free(flatEdges);
  freeDsu(dsu);
  return mst;
}

void testIsDirectedCyclicGraph() {
  Graph *g = createGraph(3);
  addEdgeToDirectedGraph(g, 0, 1, 1);
  addEdgeToDirectedGraph(g, 1, 2, 1);
  addEdgeToDirectedGraph(g, 2, 0, 1);
  if (isDirectedCyclicGraph(g))
    printf("Cycle Detection Test passed: Cycle found!\n");
  else
    printf("Cycle Detection Test failed: No cycle found.\n");
  destroyGraph(g);
}

void testIsUndirectedCyclicGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToUndirectedGraph(g1, 0, 1, 1);
  addEdgeToUndirectedGraph(g1, 1, 2, 1);
  if (!isUndirectedCyclicGraph(g1))
    printf("Undirected Test 1 passed: Tree is acyclic.\n");
  else
    printf("Undirected Test 1 failed: False positive in tree.\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addEdgeToUndirectedGraph(g2, 0, 1, 1);
  addEdgeToUndirectedGraph(g2, 1, 2, 1);
  addEdgeToUndirectedGraph(g2, 2, 0, 1);
  if (isUndirectedCyclicGraph(g2))
    printf("Undirected Test 2 passed: Triangle cycle detected.\n");
  else
    printf("Undirected Test 2 failed: Triangle cycle missed.\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(5);
  addEdgeToUndirectedGraph(g3, 0, 1, 1);
  addEdgeToUndirectedGraph(g3, 2, 3, 1);
  addEdgeToUndirectedGraph(g3, 3, 4, 1);
  addEdgeToUndirectedGraph(g3, 4, 2, 1);
  if (isUndirectedCyclicGraph(g3))
    printf("Undirected Test 3 passed: Cycle in disconnected component detected.\n");
  else
    printf("Undirected Test 3 failed: Missed cycle in disconnected component.\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(2);
  addEdgeToUndirectedGraph(g4, 0, 1, 1);
  if (!isUndirectedCyclicGraph(g4))
    printf("Undirected Test 4 passed: Simple edge is acyclic.\n");
  else
    printf("Undirected Test 4 failed: Parent incorrectly triggered cycle.\n");
  destroyGraph(g4);
}

void testDepthFirstSortOfGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToDirectedGraph(g1, 0, 1, 1);
  addEdgeToDirectedGraph(g1, 1, 2, 1);
  unsigned *order1 = depthFirstSortOfGraph(g1, 0);
  assert(order1[0] == 0);
  assert(order1[1] == 1);
  assert(order1[2] == 2);
  printf("DFS test 1 (Linear) passed!\n");
  destroyGraph(g1);
  free(order1);

  Graph *g2 = createGraph(3);
  addEdgeToDirectedGraph(g2, 0, 2, 1);
  addEdgeToDirectedGraph(g2, 0, 1, 1);
  unsigned *order2 = depthFirstSortOfGraph(g2, 0);
  assert(order2[0] == 0);
  assert(order2[1] == 1);
  assert(order2[2] == 2);
  printf("DFS test 2 (Fork) passed!\n");
  destroyGraph(g2);
  free(order2);

  Graph *g3 = createGraph(3);
  addEdgeToDirectedGraph(g3, 0, 1, 1);
  unsigned *order3 = depthFirstSortOfGraph(g3, 0);
  assert(order3[0] == 0);
  assert(order3[1] == 1);
  assert(order3[2] == 2);
  printf("DFS test 3 (Disconnected) passed!\n");
  destroyGraph(g3);
  free(order3);

  Graph *g4 = createGraph(2);
  addEdgeToDirectedGraph(g4, 0, 1, 1);
  addEdgeToDirectedGraph(g4, 1, 0, 1);
  unsigned *order4 = depthFirstSortOfGraph(g4, 0);
  assert(order4[0] == 0);
  assert(order4[1] == 1);
  printf("DFS test 4 (Cyclic) passed!\n");
  destroyGraph(g4);
  free(order4);
}

void testBreadthFirstSortOfGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToDirectedGraph(g1, 0, 1, 1);
  addEdgeToDirectedGraph(g1, 1, 2, 1);
  unsigned *order1 = breadthFirstSortOfGraph(g1, 0);
  assert(order1[0] == 0);
  assert(order1[1] == 1);
  assert(order1[2] == 2);
  printf("BFS test 1 (linear) passed!\n");
  destroyGraph(g1);
  free(order1);

  Graph *g2 = createGraph(3);
  addEdgeToDirectedGraph(g2, 0, 2, 1);
  addEdgeToDirectedGraph(g2, 0, 1, 1);
  unsigned *order2 = breadthFirstSortOfGraph(g2, 0);
  assert(order2[0] == 0);
  assert(order2[1] == 1);
  assert(order2[2] == 2);
  printf("BFS test 2 (fork) passed!\n");
  destroyGraph(g2);
  free(order2);

  Graph *g3 = createGraph(3);
  addEdgeToDirectedGraph(g3, 0, 1, 1);
  unsigned *order3 = breadthFirstSortOfGraph(g3, 0);
  assert(order3[0] == 0);
  assert(order3[1] == 1);
  assert(order3[2] == 2);
  printf("BFS test 3 (disconnected) passed!\n");
  destroyGraph(g3);
  free(order3);

  Graph *g4 = createGraph(2);
  addEdgeToDirectedGraph(g4, 0, 1, 1);
  addEdgeToDirectedGraph(g4, 1, 0, 1);
  unsigned *order4 = breadthFirstSortOfGraph(g4, 0);
  assert(order4[0] == 0);
  assert(order4[1] == 1);
  printf("BFS test 4 (cycle) passed!\n");
  destroyGraph(g4);
  free(order4);
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

void testTopologicalSortOfGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToDirectedGraph(g1, 0, 1, 1);
  addEdgeToDirectedGraph(g1, 1, 2, 1);
  unsigned *order1 = topologicalSortOfGraph(g1);
  assert(isValidTopologicalSort(g1, order1));
  printf("Topo test 1 (linear) passed!\n");
  destroyGraph(g1);
  free(order1);

  Graph *g2 = createGraph(4);
  addEdgeToDirectedGraph(g2, 0, 1, 1);
  addEdgeToDirectedGraph(g2, 0, 2, 1);
  addEdgeToDirectedGraph(g2, 1, 3, 1);
  addEdgeToDirectedGraph(g2, 2, 3, 1);
  unsigned *order2 = topologicalSortOfGraph(g2);
  assert(isValidTopologicalSort(g2, order2));
  printf("Topo test 2 (diamond) passed!\n");
  destroyGraph(g2);
  free(order2);

  Graph *g3 = createGraph(4);
  addEdgeToDirectedGraph(g3, 0, 1, 1);
  addEdgeToDirectedGraph(g3, 2, 3, 1);
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

void testBellmanFord() {
  Graph *g = createGraph(4);
  addEdgeToDirectedGraph(g, 0, 1, 5);
  addEdgeToDirectedGraph(g, 1, 2, 1);
  addEdgeToDirectedGraph(g, 0, 2, 10);
  addEdgeToDirectedGraph(g, 2, 3, 1);
  int *d1 = bellmanFord(g, 0);
  assert(d1 != NULL);
  assert(d1[3] == 7);
  printf("Bellman-Ford test 1 (positive) passed!\n");
  free(d1);

  addEdgeToDirectedGraph(g, 3, 1, -10);
  int *d2 = bellmanFord(g, 0);
  assert(d2 == NULL);
  printf("Bellman-Ford test 2 (negative cycle) passed!\n");
  destroyGraph(g);
}

void testDijkstra() {
  Graph *g1 = createGraph(3);
  addEdgeToDirectedGraph(g1, 0, 1, 5);
  addEdgeToDirectedGraph(g1, 1, 2, 10);
  int *dist1 = dijkstra(g1, 0);
  assert(dist1[0] == 0);
  assert(dist1[1] == 5);
  assert(dist1[2] == 15);
  printf("Test 1 passed: Simple path\n");
  destroyGraph(g1);
  free(dist1);

  Graph *g2 = createGraph(3);
  addEdgeToDirectedGraph(g2, 0, 2, 10);
  addEdgeToDirectedGraph(g2, 0, 1, 2);
  addEdgeToDirectedGraph(g2, 1, 2, 3);
  int *dist2 = dijkstra(g2, 0);
  assert(dist2[2] == 5);
  printf("Test 2 passed: Shortest path selection\n");
  destroyGraph(g2);
  free(dist2);

  Graph *g3 = createGraph(2);
  int *dist3 = dijkstra(g3, 0);
  assert(dist3[0] == 0);
  assert(dist3[1] == INT_MAX);
  printf("Test 3 passed: Unreachable vertex (INT_MAX)\n");
  destroyGraph(g3);
  free(dist3);

  Graph *g4 = createGraph(3);
  addEdgeToDirectedGraph(g4, 0, 1, 1);
  addEdgeToDirectedGraph(g4, 1, 2, 1);
  addEdgeToDirectedGraph(g4, 2, 0, 1);
  int *dist4 = dijkstra(g4, 0);
  assert(dist4[0] == 0);
  assert(dist4[1] == 1);
  assert(dist4[2] == 2);
  printf("Test 4 passed: Cyclic graph\n");
  destroyGraph(g4);
  free(dist4);
}

void getUndirectedGraphStats(const Graph *graph, int *weight, unsigned *count) {
  *weight = 0;
  *count = 0;
  for (unsigned vertex = 0; vertex < graph->size; vertex++)
    for (Edge *edge = graph->edges[vertex]; edge; edge = edge->next) {
      *weight += edge->weight;
      (*count)++;
    }
  *weight /= 2;
  *count /= 2;
}

void testPrim() {
  Graph *g1 = createGraph(3);
  addEdgeToUndirectedGraph(g1, 0, 1, 1);
  addEdgeToUndirectedGraph(g1, 1, 2, 3);
  addEdgeToUndirectedGraph(g1, 0, 2, 4);
  Graph *mst1 = prim(g1, 0);
  int w1;
  unsigned e1;
  getUndirectedGraphStats(mst1, &w1, &e1);
  assert(e1 == 2);
  assert(w1 == 4);
  printf("Prim test 1 (triangle) passed: weight %d\n", w1);
  destroyGraph(g1);
  destroyGraph(mst1);

  Graph *g2 = createGraph(5);
  addEdgeToUndirectedGraph(g2, 0, 1, 2);
  addEdgeToUndirectedGraph(g2, 0, 3, 6);
  addEdgeToUndirectedGraph(g2, 1, 2, 3);
  addEdgeToUndirectedGraph(g2, 1, 3, 8);
  addEdgeToUndirectedGraph(g2, 1, 4, 5);
  addEdgeToUndirectedGraph(g2, 2, 4, 7);
  addEdgeToUndirectedGraph(g2, 3, 4, 9);
  Graph *mst2 = prim(g2, 0);
  int w2;
  unsigned e2;
  getUndirectedGraphStats(mst2, &w2, &e2);
  assert(e2 == 4);
  assert(w2 == 16);
  printf("Prim test 2 (complex) passed: weight %d\n", w2);
  destroyGraph(g2);
  destroyGraph(mst2);

  Graph *g3 = createGraph(1);
  Graph *mst3 = prim(g3, 0);
  int w3;
  unsigned e3;
  getUndirectedGraphStats(mst3, &w3, &e3);
  assert(e3 == 0);
  assert(w3 == 0);
  printf("Prim test 3 (single vertex) passed!\n");
  destroyGraph(g3);
  destroyGraph(mst3);
}

void testKruskal() {
  Graph *g = createGraph(4);
  addEdgeToUndirectedGraph(g, 0, 1, 10);
  addEdgeToUndirectedGraph(g, 1, 3, 15);
  addEdgeToUndirectedGraph(g, 3, 2, 4);
  addEdgeToUndirectedGraph(g, 2, 0, 6);
  addEdgeToUndirectedGraph(g, 0, 3, 5);

  Graph *mst = kruskal(g);
  int weight;
  unsigned count;
  getUndirectedGraphStats(mst, &weight, &count);

  assert(count == 3);
  assert(weight == 19);
  printf("Kruskal test passed: weight %d\n", weight);

  destroyGraph(g);
  destroyGraph(mst);
}

int main() {
  testIsDirectedCyclicGraph();
  testIsUndirectedCyclicGraph();
  testDepthFirstSortOfGraph();
  testBreadthFirstSortOfGraph();
  testTopologicalSortOfGraph();
  testBellmanFord();
  testDijkstra();
  testPrim();
  testKruskal();
  return 0;
}
