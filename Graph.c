#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

unsigned minimumUnsigned(unsigned a, unsigned b);
unsigned maximumUnsigned(unsigned a, unsigned b);

void freeMatrix(int **matrix, unsigned n);

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

bool isValid(const Graph *graph);
bool isNull(const Graph *graph);
bool isTrivial(const Graph *graph);
bool isEmpty(const Graph *graph);
bool isRegular(const Graph *graph);
bool isComplete(const Graph *graph);
bool hasSelfLoops(const Graph *graph);
bool isBalancedDirected(const Graph *graph);
bool isEulerianUndirected(const Graph *graph);
bool isEulerianDirected(const Graph *graph);
bool isConnectedUndirected(const Graph *graph);
bool isWeaklyConnectedDirected(const Graph *graph);
bool isStronglyConnectedDirected(const Graph *graph);
bool isBipartite(const Graph *graph);
bool isUndirected(const Graph *graph);
bool isMultiGraph(const Graph *graph);
bool isForest(const Graph *graph);
bool isTree(const Graph *graph);
bool isStar(const Graph *graph);
bool isWheel(const Graph *graph);
bool hasIsolatedVertices(const Graph *graph);
bool isCyclicDirectedComponent(const Graph *graph, unsigned vertex, char *visited);
bool isCyclicDirected(const Graph *graph);
bool isCyclicUndirectedComponent(const Graph *graph, unsigned vertex, unsigned parent, bool *visited);
bool isCyclicUndirected(const Graph *graph);
bool isKRegular(const Graph *graph, unsigned k);
bool hasConstantWeights(const Graph *graph, int weight);
bool isDense(const Graph *graph, double threshold);
bool isIsolated(const Graph *graph, unsigned vertex);
bool isSource(const Graph *graph, unsigned vertex);
bool isSink(const Graph *graph, unsigned vertex);
bool isUniversalSink(const Graph *graph, unsigned vertex);
bool isDirectedLeaf(const Graph *graph, unsigned vertex);
bool isUndirectedLeaf(const Graph *graph, unsigned vertex);
bool hasSelfLoopsAtVertex(const Graph *graph, unsigned vertex);
bool isAdjacent(const Graph *graph, unsigned u, unsigned v);
bool isReachable(const Graph *graph, unsigned start, unsigned target);
bool hasWeightedEdge(const Graph *graph, unsigned u, unsigned v, int weight);
bool isClique(const Graph *graph, const bool *subset);
bool isIndependentSet(const Graph *graph, const bool *subset);
bool isVertexCover(const Graph *graph, const bool *subset);
bool isPath(const Graph *graph, const unsigned *path, unsigned length);
bool isDirectedCycle(const Graph *graph, const unsigned *path, unsigned length);
bool isSimpleCycle(const Graph *graph, const unsigned *path, unsigned length);
bool isHamiltonianCycle(const Graph *graph, const unsigned *path, unsigned length);
bool isHamiltonianPath(const Graph *graph, const unsigned *path, unsigned length);
bool isWalk(const Graph *graph, const unsigned *sequence, unsigned length);
bool isSubGraph(const Graph *sub, const Graph *main);

bool *graphCenter(const Graph *graph);
bool *graphPeriphery(const Graph *graph);

Graph *createGraph(unsigned size);
Graph *createPathGraph(unsigned size);
Graph *createCycleGraph(unsigned size);
Graph *copyGraph(const Graph *graph);
Graph *copyUnweighted(const Graph *graph);
Graph *copyUndirected(const Graph *graph);
Graph *copyComplement(const Graph *graph);
Graph *lineGraph(const Graph *graph);
Graph *underlyingGraph(const Graph *graph);
Graph *removeVertex(const Graph *graph, unsigned vertex);
Graph *copySubgraph(const Graph *graph, const bool *vertices);
Graph *graphUnion(const Graph *g1, const Graph *g2);
Graph *createDirectedGraphFromEdgeArray(unsigned size, const FlatEdge *edges, unsigned count);
Graph *createUndirectedGraphFromEdgeArray(unsigned size, const FlatEdge *edges, unsigned count);

void printGraph(const Graph *graph);
void destroyGraph(Graph *graph);
void removeSelfLoops(Graph *graph);
void removeParallelEdges(Graph *graph);
void transpose(Graph *graph);
void removeEdgeFromDirectedGraph(Graph *graph, unsigned source, unsigned destination);
void removeEdgeFromUndirectedGraph(Graph *graph, unsigned u, unsigned v);
void mergeVertices(Graph *graph, unsigned u, unsigned v);
void addEdgeToDirectedGraph(Graph *graph, unsigned source, unsigned destination, int weight);
void addEdgeToUndirectedGraph(Graph *graph, unsigned u, unsigned v, int weight);

unsigned countEdges(const Graph *graph);
unsigned countSelfLoops(const Graph *graph);
unsigned countTriangles(const Graph *graph);
unsigned minDegree(const Graph *graph);
unsigned maxDegree(const Graph *graph);
unsigned countDirectedLeaves(const Graph *graph);
unsigned countUndirectedLeaves(const Graph *graph);
unsigned countSources(const Graph *graph);
unsigned countSinks(const Graph *graph);
unsigned countParallelEdges(const Graph *graph);
unsigned countIsolatedVertices(const Graph *graph);
void traverseComponent(const Graph *graph, unsigned vertex, bool *visited);
unsigned countComponents(const Graph *graph);
unsigned outDegree(const Graph *graph, unsigned vertex);
unsigned inDegree(const Graph *graph, unsigned vertex);
unsigned countCommonNeighbors(const Graph *graph, unsigned u, unsigned v);

unsigned *degreeDistribution(const Graph *graph);
unsigned *outDegrees(const Graph *graph);
unsigned *inDegrees(const Graph *graph);

int sumWeights(const Graph *graph);
int graphRadius(const Graph *graph);
int graphDiameter(const Graph *graph);
int graphEccentricity(const Graph *graph, unsigned vertex);
int edgeWeight(const Graph *graph, unsigned source, unsigned destination);
int pathWeight(const Graph *graph, const unsigned *path, unsigned length);

int **toMatrix(const Graph *graph);

double density(const Graph *graph);
double averageClusteringCoefficient(const Graph *graph);
double normalizedDegree(const Graph *graph, unsigned vertex);
double localClusteringCoefficient(const Graph *graph, unsigned vertex);
double subgraphDensity(const Graph *graph, const bool *subset);

FlatEdge *getEdgeArrayFromDirectedGraph(const Graph *graph);
FlatEdge *getEdgeArrayFromUndirectedGraph(const Graph *graph);
bool allAreReachableFromVertexInGraph(const Graph *graph, unsigned vertex);
void depthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *depthFirstSortOfGraph(const Graph *graph, unsigned source);
void breadthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *breadthFirstSortOfGraph(const Graph *graph, unsigned source);
void topologicalSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *topologicalSortOfGraph(const Graph *graph);
unsigned *coloring(const Graph *graph);
int *bellmanFord(const Graph *graph, unsigned source);
unsigned *unweightedDijkstra(const Graph *graph, unsigned source);
int *weightedDijkstra(const Graph *graph, unsigned source);
int **floydWarshall(const Graph *graph);
Graph *prim(const Graph *graph, unsigned source);
Graph *kruskal(const Graph *graph);
void recursivelyFindArticulationPoints(
  const Graph *graph,
  unsigned vertex,
  bool *visited,
  unsigned *discovery,
  unsigned *low,
  unsigned *parent,
  bool *articulations,
  unsigned *timer);
bool *findArticulationPoints(const Graph *graph);
void recursivelyFindBridges(
  const Graph *graph,
  unsigned vertex,
  bool *visited,
  unsigned *discovery,
  unsigned *low,
  unsigned *parent,
  unsigned **bridges,
  unsigned *count,
  unsigned *timer);
unsigned **findBridges(const Graph *graph);

void testIsDirectedCyclicGraph();
void testIsUndirectedCyclicGraph();
void testIsConnectedUndirected();
void testIsWeaklyConnectedDirected();
void testIsStronglyConnectedDirected();
void testDepthFirstSortOfGraph();
void testBreadthFirstSortOfGraph();
bool isValidTopologicalSort(const Graph *graph, const unsigned *ordering);
void testTopologicalSortOfGraph();
void testBellmanFord();
void testUnweightedDijkstra();
void testWeightedDijkstra();
void testFloydWarshall();
void testPrim();
void testKruskal();
void testFindArticulationPoints();
bool hasBridge(unsigned *const *const bridges, unsigned u, unsigned v);
void freeBridgeResult(unsigned **bridges);
void testFindBridges();

int main();



unsigned minimumUnsigned(unsigned a, unsigned b) { return a <= b ? a : b; }

unsigned maximumUnsigned(unsigned a, unsigned b) { return a >= b ? a : b; }



void freeMatrix(int **matrix, unsigned n) {
  if (matrix == NULL) return;
  for (unsigned i = 0; i < n; i++) free(matrix[i]);
  free(matrix);
}



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



bool isValid(const Graph *graph) {
  if (graph == NULL) return false;
  if (graph->edges == NULL) return false;
  for (unsigned vertex = 0; vertex < graph->size; vertex++)
    for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next)
      if (edge->destination >= graph->size)
        return false;
  return true;
}

bool isNull(const Graph *graph) {
  return graph->size == 0;
}

bool isTrivial(const Graph *graph) {
  if (graph->size != 1) return false;
  return graph->edges[0] == NULL;
}

bool isEmpty(const Graph *graph) {
  if (graph->size == 0) return false;
  for (unsigned v = 0; v < graph->size; v++) if (graph->edges[v] != NULL) return false;
  return true;
}

bool isRegular(const Graph *graph) {
  if (graph->size < 2) return true;
  unsigned degree = outDegree(graph, 0);
  for (unsigned i = 1; i < graph->size; i++) if (outDegree(graph, i) != degree) return false;
  return true;
}

bool isComplete(const Graph *graph) {
  assert(isValid(graph));
  if (graph->size < 2) return true;
  return countEdges(graph) == graph->size * (graph->size - 1);
}

bool hasSelfLoops(const Graph *graph) {
  assert(isValid(graph));
  for (unsigned vertex = 0; vertex < graph->size; vertex++) if (hasSelfLoopsAtVertex(graph, vertex)) return true;
  return false;
}

bool isBalancedDirected(const Graph *graph) {
  assert(isValid(graph));
  unsigned *a = inDegrees(graph);
  bool b = true;
  for (unsigned v = 0; v < graph->size; v++) if (a[v] != outDegree(graph, v)) { b = false; break; }
  free(a);
  return b;
}

bool isEulerianUndirected(const Graph *graph) {
  assert(isValid(graph));
  if (graph->size < 2) return true;
  for (unsigned vertex = 0; vertex < graph->size; vertex++) if (outDegree(graph, vertex) % 2 != 0) return false;
  return isConnectedUndirected(graph);
}

bool isEulerianDirected(const Graph *graph) {
  assert(isValid(graph));
  if (graph->size < 2) return true;
  return isBalancedDirected(graph) && isStronglyConnectedDirected(graph);
}

bool isConnectedUndirected(const Graph *graph) {
  assert(isValid(graph));
  if (graph->size < 2) return true;
  return allAreReachableFromVertexInGraph(graph, 0);
}

bool isWeaklyConnectedDirected(const Graph *graph) {
  if (graph->size < 2) return true;
  Graph *undirected = copyUndirected(graph);
  bool reachable = allAreReachableFromVertexInGraph(undirected, 0);
  destroyGraph(undirected);
  return reachable;
}

bool isStronglyConnectedDirected(const Graph *graph) {
  if (graph->size < 2) return true;
  Graph *copy = copyGraph(graph);
  transpose(copy);
  bool reachable = allAreReachableFromVertexInGraph(graph, 0) && allAreReachableFromVertexInGraph(copy, 0);
  destroyGraph(copy);
  return reachable;
}

bool isBipartite(const Graph *graph) {
  int *colors = calloc(graph->size, sizeof(int));
  unsigned *queue = malloc(graph->size * sizeof(unsigned));
  bool bipartite = true;
  for (unsigned u = 0; u < graph->size; u++)
    if (colors[u] == 0) {
      colors[u] = 1;
      unsigned head = 0, tail = 0;
      queue[tail++] = u;
      while (head < tail) {
        unsigned v = queue[head++];
        for (Edge *edge = graph->edges[v]; edge; edge = edge->next)
          if (colors[edge->destination] == 0) {
            colors[edge->destination] = (colors[v] == 1) ? 2 : 1;
            queue[tail++] = edge->destination;
          } else if (colors[edge->destination] == colors[v]) {
            bipartite = false;
          }
      }
    }
  free(colors);
  free(queue);
  return bipartite;
}

bool isUndirected(const Graph *graph) {
  for (unsigned u = 0; u < graph->size; u++)
    for (Edge *e = graph->edges[u]; e != NULL; e = e->next)
      if (!hasWeightedEdge(graph, e->destination, u, e->weight))
        return false;
  return true;
}

bool isMultiGraph(const Graph *graph) {
  assert(isValid(graph));
  bool multi = false;
  bool *seen = malloc(graph->size * sizeof(bool));
  for (unsigned v = 0; v < graph->size && !multi; v++) {
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next) seen[e->destination] = false;
    for (Edge *e = graph->edges[v]; e != NULL && !multi; e = e->next) {
      if (seen[e->destination]) multi = true;
      seen[e->destination] = true;
    }
  }
  free(seen);
  return multi;
}

bool isForest(const Graph *graph) {
  return isUndirected(graph) && !isCyclicUndirected(graph);
}

bool isTree(const Graph *graph) {
  return isUndirected(graph) && !isCyclicUndirected(graph) && isConnectedUndirected(graph);
}

bool isStar(const Graph *graph) {
  if (graph->size < 2) return false;
  if (!isUndirected(graph)) return false;
  unsigned count = 0;
  for (unsigned v = 0; v < graph->size; v++) {
    unsigned degree = outDegree(graph, v);
    if (degree == graph->size - 1) count++; else if (degree != 1) return false;
  }
  if (graph->size == 2) return count == 2;
  return count == 1;
}

bool isWheel(const Graph *graph) {
  if (graph->size < 4) return false;
  if (!isUndirected(graph)) return false;
  unsigned hub;
  unsigned hubCount = 0;
  for (unsigned v = 0; v < graph->size; v++) {
    unsigned degree = outDegree(graph, v);
    if (degree == graph->size - 1) {
      hub = v;
      hubCount++;
    } else if (degree != 3) {
      return false;
    }
  }
  if (graph->size == 4 && hubCount != 4) return false;
  if (graph->size > 4 && hubCount != 1) return false;
  bool *visited = calloc(graph->size, sizeof(bool));
  unsigned visitedCount = 0;
  unsigned current = hub;
  do {
    visited[current] = true;
    visitedCount++;
    unsigned next = UINT_MAX;
    for (Edge *edge = graph->edges[current]; edge != NULL; edge = edge->next)
      if (!visited[edge->destination]) {
        next = edge->destination;
        break;
      }
    current = next;
  } while (current != UINT_MAX);
  free(visited);
  return visitedCount == graph->size;
}

bool hasIsolatedVertices(const Graph *graph) {
  assert(isValid(graph));
  bool *a = calloc(graph->size, sizeof(bool));
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      a[e->destination] = true;
  bool b = false;
  for (unsigned v = 0; v < graph->size; v++)
    if (graph->edges[v] == NULL && !a[v]) {
      b = true;
      break;
    }
  free(a);
  return b;
}

bool isCyclicDirectedComponent(const Graph *graph, unsigned vertex, char *visited) {
  visited[vertex] = 1;
  for (Edge *edge = graph->edges[vertex]; edge; edge = edge->next)
    if (visited[edge->destination] == 1 ||
      (visited[edge->destination] == 0 && isCyclicDirectedComponent(graph, edge->destination, visited)))
        return true;
  visited[vertex] = 2;
  return false;
}

bool isCyclicDirected(const Graph *graph) {
  char *visited = calloc(graph->size, sizeof(char));
  bool cyclic = false;
  for (unsigned vertex = 0; vertex < graph->size && !cyclic; vertex++)
    cyclic = (visited[vertex] == 0 && isCyclicDirectedComponent(graph, vertex, visited));
  free(visited);
  return cyclic;
}

bool isCyclicUndirectedComponent(const Graph *graph, unsigned vertex, unsigned parent, bool *visited) {
  visited[vertex] = true;
  for (Edge *edge = graph->edges[vertex]; edge; edge = edge->next)
    if (visited[edge->destination]) {
      if (edge->destination != parent)
        return true;
    } else {
      if (isCyclicUndirectedComponent(graph, edge->destination, vertex, visited))
        return true;
    }
  return false;
}

bool isCyclicUndirected(const Graph *graph) {
  assert(isValid(graph));
  assert(isUndirected(graph));
  bool *visited = calloc(graph->size, sizeof(bool));
  for (unsigned vertex = 0; vertex < graph->size; vertex++)
    if (!visited[vertex])
      if (isCyclicUndirectedComponent(graph, vertex, graph->size, visited)) {
        free(visited);
        return true;
      }
  free(visited);
  return false;
}

bool isKRegular(const Graph *graph, unsigned k) {
  assert(isValid(graph));
  for (unsigned v = 0; v < graph->size; v++) if (outDegree(graph, v) != k) return false;
  return true;
}

bool hasConstantWeights(const Graph *graph, int weight) {
  assert(isValid(graph));
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      if (e->weight != weight)
        return false;
  return true;
}

bool isDense(const Graph *graph, double threshold) {
  assert(isValid(graph));
  assert(!isMultiGraph(graph));
  assert(!hasSelfLoops(graph));
  assert(threshold >= 0);
  assert(threshold <= 1);
  if (graph->size <= 1) return false;
  return (double)countEdges(graph) / graph->size * (graph->size - 1) >= threshold;
}

bool isIsolated(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  return outDegree(graph, vertex) == 0 && inDegree(graph, vertex) == 0;
}

bool isSource(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  return inDegree(graph, vertex) == 0 && outDegree(graph, vertex) > 0;
}

bool isSink(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  return outDegree(graph, vertex) == 0 && inDegree(graph, vertex) > 0;
}

bool isUniversalSink(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  return outDegree(graph, vertex) == 0 && inDegree(graph, vertex) == graph->size - 1;
}

bool isDirectedLeaf(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  return outDegree(graph, vertex) + inDegree(graph, vertex) == 1;
}

bool isUndirectedLeaf(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(isUndirected(graph));
  assert(vertex < graph->size);
  return outDegree(graph, vertex) == 1;
}

bool hasSelfLoopsAtVertex(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next) if (e->destination == vertex) return true;
  return false;
}

bool isAdjacent(const Graph *graph, unsigned u, unsigned v) {
  assert(isValid(graph));
  if (u >= graph->size || v >= graph->size) return false;
  for (Edge *e = graph->edges[u]; e != NULL; e = e->next) if (e->destination == v) return true;
  return false;
}

bool isReachable(const Graph *graph, unsigned start, unsigned target) {
  if (start >= graph->size) return false;
  if (target >= graph->size) return false;
  if (start == target) return true;
  bool *visited = calloc(graph->size, sizeof(bool));
  unsigned *queue = malloc(graph->size * sizeof(unsigned));
  unsigned head = 0, tail = 0;
  visited[start] = true;
  queue[tail++] = start;
  bool b = false;
  while (head < tail) {
    unsigned u = queue[head++];
    for (Edge *edge = graph->edges[u]; edge != NULL; edge = edge->next) {
      if (edge->destination == target) {
        b = true;
        break;
      }
      if (!visited[edge->destination]) {
        visited[edge->destination] = true;
        queue[tail++] = edge->destination;
      }
    }
  }
  free(visited);
  free(queue);
  return b;
}

bool hasWeightedEdge(const Graph *graph, unsigned u, unsigned v, int weight) {
  if (u >= graph->size || v >= graph->size) return false;
  for (Edge *e = graph->edges[u]; e != NULL; e = e->next) if (e->destination == v && e->weight == weight) return true;
  return false;
}

bool isClique(const Graph *graph, const bool *subset) {
  for (unsigned u = 0; u < graph->size; u++)
    if (subset[u])
      for (unsigned v = u + 1; v < graph->size; v++)
        if (subset[v])
          if (!isAdjacent(graph, u, v))
            return false;
  return true;
}

bool isIndependentSet(const Graph *graph, const bool *subset) {
  for (unsigned u = 0; u < graph->size; u++)
    if (subset[u])
      for (unsigned v = u + 1; v < graph->size; v++)
        if (subset[v])
          if (isAdjacent(graph, u, v))
            return false;
  return true;
}

bool isVertexCover(const Graph *graph, const bool *subset) {
  for (unsigned u = 0; u < graph->size; u++)
    if (!subset[u])
      for (Edge *edge = graph->edges[u]; edge != NULL; edge = edge->next)
        if (!subset[edge->destination])
          return false;
  return true;
}

bool isPath(const Graph *graph, const unsigned *path, unsigned length) {
  for (unsigned i = 1; i < length; i++) if (!isAdjacent(graph, path[i - 1], path[i])) return false;
  return true;
}

bool isDirectedCycle(const Graph *graph, const unsigned *path, unsigned length) {
  bool cycle = true;
  if (length < 2 || path[0] != path[length - 1]) cycle = false;
  for (unsigned i = 1; i < length; i++) if (!isAdjacent(graph, path[i - 1], path[i])) cycle = false;
  return cycle;
}

bool isSimpleCycle(const Graph *graph, const unsigned *path, unsigned length) {
  bool cycle = true;
  if (length < 4 || path[0] != path[length - 1]) cycle = false;
  bool *visited = calloc(graph->size, sizeof(bool));
  for (unsigned i = 1; i < length; i++) {
    if (visited[path[i]]) cycle = false;
    visited[path[i]] = true;
  }
  free(visited);
  for (unsigned i = 1; i < length; i++) if (!isAdjacent(graph, path[i - 1], path[i])) cycle = false;
  return cycle;
}

bool isHamiltonianCycle(const Graph *graph, const unsigned *path, unsigned length) {
  bool cycle = true;
  if (length != graph->size + 1 || path[0] != path[length - 1]) cycle = false;
  bool *visited = calloc(graph->size, sizeof(bool));
  for (unsigned i = 1; i < length; i++) {
    if (visited[path[i]]) cycle = false;
    visited[path[i]] = true;
  }
  free(visited);
  for (unsigned i = 1; i < length; i++) if (!isAdjacent(graph, path[i - 1], path[i])) cycle = false;
  return cycle;
}

bool isHamiltonianPath(const Graph *graph, const unsigned *path, unsigned length) {
  assert(isValid(graph));
  assert(path != NULL);
  bool b = true;
  if (length != graph->size) b = false;
  bool *visited = calloc(graph->size, sizeof(bool));
  for (unsigned i = 0; i < length; i++) {
    if (visited[path[i]]) b = false;
    visited[path[i]] = true;
  }
  free(visited);
  for (unsigned i = 1; i < length; i++) if (!isAdjacent(graph, path[i - 1], path[i])) b = false;
  return b;
}

bool isWalk(const Graph *graph, const unsigned *sequence, unsigned length) {
  assert(isValid(graph));
  assert(sequence != NULL);
  if (length == 0) return false;
  for (unsigned i = 0; i < length; i++) if (sequence[i] >= graph->size) return false;
  for (unsigned i = 1; i < length; i++) if (!isAdjacent(graph, sequence[i - 1], sequence[i])) return false;
  return true;
}

bool isSubGraph(const Graph *sub, const Graph *main) {
  if (sub->size > main->size) return false;
  bool *edges = calloc(main->size, sizeof(bool));
  int *weights = malloc(main->size * sizeof(int));
  bool match = true;
  for (unsigned u = 0; u < sub->size; u++) {
    for (Edge *e = main->edges[u]; e != NULL; e = e->next) {
      edges[e->destination] = true;
      weights[e->destination] = e->weight;
    }
    for (Edge *e = sub->edges[u]; e != NULL; e = e->next)
      if (!edges[e->destination] || e->weight != weights[e->destination]) {
        match = false;
        break;
      }
    if (!match) break;
    for (Edge *e = main->edges[u]; e != NULL; e = e->next) edges[e->destination] = false;
  }
  free(weights);
  free(edges);
  return match;
}



bool *graphCenter(const Graph *graph) {
  int *eccentricity = malloc(graph->size * sizeof(int));
  int radius = INT_MAX;
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    eccentricity[vertex] = graphEccentricity(graph, vertex);
    if (eccentricity[vertex] < radius) radius = eccentricity[vertex];
  }
  bool *center = calloc(graph->size, sizeof(bool));
  for (unsigned vertex = 0; vertex < graph->size; vertex++) if (eccentricity[vertex] == radius) center[vertex] = true;
  free(eccentricity);
  return center;
}

bool *graphPeriphery(const Graph *graph) {
  int *eccentricity = malloc(graph->size * sizeof(int));
  int diameter = INT_MIN;
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    eccentricity[vertex] = graphEccentricity(graph, vertex);
    if (eccentricity[vertex] > diameter) diameter = eccentricity[vertex];
  }
  bool *periphery = calloc(graph->size, sizeof(bool));
  for (unsigned vertex = 0; vertex < graph->size; vertex++) if (eccentricity[vertex] == diameter) periphery[vertex] = true;
  free(eccentricity);
  return periphery;
}



Graph *createGraph(unsigned size) {
  Graph *graph = malloc(sizeof(Graph));
  graph->size = size;
  graph->edges = calloc(size, sizeof(Edge *));
  return graph;
}

Graph *createPathGraph(unsigned size) {
  Graph *g = createGraph(size);
  for (unsigned i = 1; i < size; i++) addEdgeToUndirectedGraph(g, i - 1, i, 1);
  return g;
}

Graph *createCycleGraph(unsigned size) {
  if (size < 3) return createGraph(size);
  Graph *g = createGraph(size);
  for (unsigned i = 0; i < size; i++) addEdgeToUndirectedGraph(g, i, (i + 1) % size, 1);
  return g;
}

Graph *copyGraph(const Graph *graph) {
  assert(isValid(graph));
  Graph *copy = createGraph(graph->size);
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next) {
      addEdgeToDirectedGraph(copy, vertex, edge->destination, edge->weight);
    }
  }
  return copy;
}

Graph *copyUnweighted(const Graph *graph) {
  assert(isValid(graph));
  Graph *copy = createGraph(graph->size);
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next) {
      addEdgeToDirectedGraph(copy, vertex, edge->destination, 1);
    }
  }
  return copy;
}

Graph *copyUndirected(const Graph *graph) {
  assert(isValid(graph));
  Graph *undirected = createGraph(graph->size);
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next) {
      addEdgeToUndirectedGraph(undirected, vertex, edge->destination, edge->weight);
    }
  }
  return undirected;
}

Graph *copyComplement(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned u = 0; u < graph->size; u++)
    for (unsigned v = 0; v < graph->size; v++)
      if (u != v)
        if (!isAdjacent(graph, u, v))
          addEdgeToDirectedGraph(g, u, v, 1);
  return g;
}

Graph *lineGraph(const Graph *graph) {
  unsigned count = countEdges(graph) / 2;
  FlatEdge *edges = getEdgeArrayFromUndirectedGraph(graph);
  Graph *line = createGraph(count);
  for (unsigned i = 0; i < count; i++)
    for (unsigned j = i + 1; j < count; j++)
      if (edges[i].u == edges[j].u || edges[i].u == edges[j].v || edges[i].v == edges[j].u || edges[i].v == edges[j].v)
        addEdgeToUndirectedGraph(line, i, j, 1);
  free(edges);
  return line;
}

Graph *underlyingGraph(const Graph *graph) {
  assert(isValid(graph));
  Graph *simple = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      if (v != e->destination && !isAdjacent(simple, v, e->destination))
        addEdgeToUndirectedGraph(simple, v, e->destination, 1);
  return simple;
}

Graph *removeVertex(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  Graph *removed = createGraph(graph->size - 1);
  assert(isValid(removed));
  for (unsigned source = 0; source < graph->size; source++)
    if (source != vertex) {
      unsigned newSource = (source > vertex) ? source - 1 : source;
      for (Edge *current = graph->edges[source]; current != NULL; current = current->next) {
        if (current->destination != vertex) {
          unsigned newDestination = (current->destination > vertex) ? current->destination - 1 : current->destination;
          addEdgeToDirectedGraph(removed, newSource, newDestination, current->weight);
        }
      }
    }
  return removed;
}

Graph *copySubgraph(const Graph *graph, const bool *vertices) {
  assert(isValid(graph));
  assert(vertices != NULL);
  Graph *subgraph = createGraph(graph->size);
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    if (vertices[vertex]) {
      for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next) {
        if (vertices[edge->destination]) {
          addEdgeToDirectedGraph(subgraph, vertex, edge->destination, edge->weight);
        }
      }
    }
  }
  return subgraph;
}

Graph *graphUnion(const Graph *g1, const Graph *g2) {
  Graph *g3 = createGraph(maximumUnsigned(g1->size, g2->size));
  for (unsigned v = 0; v < g1->size; v++)
    for (Edge *e = g1->edges[v]; e; e = e->next)
      addEdgeToDirectedGraph(g3, v, e->destination, e->weight);
  for (unsigned v = 0; v < g2->size; v++)
    for (Edge *e = g2->edges[v]; e; e = e->next)
      if (!isAdjacent(g3, v, e->destination))
        addEdgeToDirectedGraph(g3, v, e->destination, e->weight);
  return g3;
}

Graph *createDirectedGraphFromEdgeArray(unsigned size, const FlatEdge *edges, unsigned count) {
  Graph *graph = createGraph(size);
  for (unsigned i = 0; i < count; i++) addEdgeToDirectedGraph(graph, edges[i].u, edges[i].v, edges[i].weight);
  return graph;
}

Graph *createUndirectedGraphFromEdgeArray(unsigned size, const FlatEdge *edges, unsigned count) {
  Graph *graph = createGraph(size);
  for (unsigned i = 0; i < count; i++) addEdgeToUndirectedGraph(graph, edges[i].u, edges[i].v, edges[i].weight);
  return graph;
}



void printGraph(const Graph *graph) {
  unsigned i = 0;
  printf("{");
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next) {
      if (i++ > 0) printf(", ");
      printf("(%u, %u, %d)", v, e->destination, e->weight);
    }
  printf("}\n");
}

void destroyGraph(Graph *graph) {
  for (unsigned i = 0; i < graph->size; i++) {
    Edge *e = graph->edges[i];
    while (e) {
      Edge *next = e->next;
      free(e);
      e = next;
    }
  }
  free(graph->edges);
  free(graph);
}

void removeSelfLoops(Graph *graph) {
  for (unsigned v = 0; v < graph->size; v++) {
    Edge **current = &graph->edges[v];
    while (*current != NULL)
      if ((*current)->destination == v) {
        Edge *duplicate = *current;
        *current = (*current)->next;
        free(duplicate);
      } else {
        current = &(*current)->next;
      }
  }
}

void removeParallelEdges(Graph *graph) {
  int *minimum = malloc(graph->size * sizeof(int));
  bool *seen = calloc(graph->size, sizeof(bool));
  for (unsigned v = 0; v < graph->size; v++) {
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next) {
      if (!seen[e->destination] || e->weight < minimum[e->destination]) minimum[e->destination] = e->weight;
      seen[e->destination] = true;
    }
    Edge **e = &graph->edges[v];
    while (*e != NULL) {
      if (seen[(*e)->destination] && minimum[(*e)->destination] == (*e)->weight) {
        seen[(*e)->destination] = false;
        e = &(*e)->next;
      } else {
        Edge *parallel = *e;
        *e = (*e)->next;
        free(parallel);
      }
    }
  }
  free(minimum);
  free(seen);
}

void transpose(Graph *graph) {
  Edge **edges = calloc(graph->size, sizeof(Edge *));
  for (unsigned v = 0; v < graph->size; v++) {
    Edge *current = graph->edges[v];
    while (current != NULL) {
      unsigned destination = current->destination;
      Edge *next = current->next;
      current->destination = v;
      current->next = edges[destination];
      edges[destination] = current;
      current = next;
    }
  }
  free(graph->edges);
  graph->edges = edges;
}

void removeEdgeFromDirectedGraph(Graph *graph, unsigned source, unsigned destination) {
  Edge *previous = NULL;
  for (Edge *current = graph->edges[source]; current != NULL; current = current->next) {
    if (current->destination == destination) {
      if (previous == NULL) graph->edges[source] = current->next; else previous->next = current->next;
      free(current);
      return;
    }
    previous = current;
  }
}

void removeEdgeFromUndirectedGraph(Graph *graph, unsigned u, unsigned v) {
  removeEdgeFromDirectedGraph(graph, u, v);
  removeEdgeFromDirectedGraph(graph, v, u);
}

void mergeVertices(Graph *graph, unsigned u, unsigned v) {
  assert(u < graph->size);
  assert(v < graph->size);
  assert(u != v);
  for (unsigned w = 0; w < graph->size; w++)
    for (Edge *e = graph->edges[w]; e != NULL; e = e->next)
      if (e->destination == v)
        e->destination = u;
  for (unsigned w = 0; w < graph->size; w++)
    for (Edge *e = graph->edges[w]; e != NULL; e = e->next)
      if (e->destination > v)
        e->destination = e->destination - 1;
  while (graph->edges[v] != NULL) {
    Edge *e = graph->edges[v];
    graph->edges[v] = e->next;
    e->next = graph->edges[u];
    graph->edges[u] = e;
  }
  for (unsigned w = v + 1; w < graph->size; w++)
    graph->edges[w - 1] = graph->edges[w];
  graph->size--;
}

void addEdgeToDirectedGraph(Graph *graph, unsigned source, unsigned destination, int weight) {
  Edge *edge = malloc(sizeof(Edge));
  edge->destination = destination;
  edge->weight = weight;
  edge->next = graph->edges[source];
  graph->edges[source] = edge;
}

void addEdgeToUndirectedGraph(Graph *graph, unsigned u, unsigned v, int weight) {
  addEdgeToDirectedGraph(graph, u, v, weight);
  addEdgeToDirectedGraph(graph, v, u, weight);
}



unsigned countEdges(const Graph *graph) {
  unsigned count = 0;
  for (unsigned vertex = 0; vertex < graph->size; vertex++)
    for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next)
      count++;
  return count;
}

unsigned countSelfLoops(const Graph *graph) {
  unsigned count = 0;
  for (unsigned v = 0; v < graph->size; v++) for (Edge *e = graph->edges[v]; e != NULL; e = e->next) if (e->destination == v) count++;
  return count;
}

unsigned countTriangles(const Graph *graph) {
  unsigned triangles = 0;
  for (unsigned vertex = 0; vertex < graph->size; vertex++)
    for (Edge *d = graph->edges[vertex]; d != NULL; d = d->next)
      if (d->destination > vertex)
        for (Edge *e = graph->edges[d->destination]; e != NULL; e = e->next)
          if (e->destination > d->destination)
            for (Edge *f = graph->edges[e->destination]; f != NULL; f = f->next)
              if (f->destination == vertex)
                triangles++;
  return triangles;
}

unsigned minDegree(const Graph *graph) {
  if (graph->size == 0) return 0;
  unsigned *in = inDegrees(graph);
  unsigned *out = outDegrees(graph);
  unsigned minimum = in[0] + out[0];
  for (unsigned v = 1; v < graph->size; v++)
    if (in[v] + out[v] < minimum)
      minimum = in[v] + out[v];
  free(in);
  free(out);
  return minimum;
}

unsigned maxDegree(const Graph *graph) {
  unsigned *in = inDegrees(graph);
  unsigned *out = outDegrees(graph);
  unsigned maximum = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (in[v] + out[v] > maximum)
      maximum = in[v] + out[v];
  free(in);
  free(out);
  return maximum;
}

unsigned countDirectedLeaves(const Graph *graph) {
  assert(isValid(graph));
  unsigned *a = inDegrees(graph);
  unsigned n = 0;
  for (unsigned v = 0; v < graph->size; v++) if (a[v] + outDegree(graph, v) == 1) n++;
  free(a);
  return n;
}

unsigned countUndirectedLeaves(const Graph *graph) {
  assert(isValid(graph));
  assert(isUndirected(graph));
  unsigned n = 0;
  for (unsigned v = 0; v < graph->size; v++) if (outDegree(graph, v) == 1) n++;
  return n;
}

unsigned countSources(const Graph *graph) {
  assert(isValid(graph));
  unsigned count = 0;
  for (unsigned v = 0; v < graph->size; v++) if (isSource(graph, v)) count++;
  return count;
}

unsigned countSinks(const Graph *graph) {
  assert(isValid(graph));
  unsigned count = 0;
  for (unsigned v = 0; v < graph->size; v++) if (isSink(graph, v)) count++;
  return count;
}

unsigned countParallelEdges(const Graph *graph) {
  assert(isValid(graph));
  bool *seen = calloc(graph->size, sizeof(bool));
  unsigned count = 0;
  for (unsigned u = 0; u < graph->size; u++) {
    for (Edge *e = graph->edges[u]; e != NULL; e = e->next) {
      if (seen[e->destination]) count++;
      seen[e->destination] = true;
    }
    for (Edge *e = graph->edges[u]; e != NULL; e = e->next) seen[e->destination] = false;
  }
  free(seen);
  return count;
}

unsigned countIsolatedVertices(const Graph *graph) {
  assert(isValid(graph));
  unsigned count = 0;
  unsigned *in = inDegrees(graph);
  for (unsigned v = 0; v < graph->size; v++) if (graph->edges[v] == NULL && in[v] == 0) count++;
  free(in);
  return count;
}

void traverseComponent(const Graph *graph, unsigned vertex, bool *visited) {
  visited[vertex] = true;
  for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next)
    if (!visited[edge->destination])
      traverseComponent(graph, edge->destination, visited);
}

unsigned countComponents(const Graph *graph) {
  bool *visited = calloc(graph->size, sizeof(bool));
  unsigned count = 0;
  for (unsigned vertex = 0; vertex < graph->size; vertex++)
    if (!visited[vertex]) {
      count++;
      traverseComponent(graph, vertex, visited);
    }
  free(visited);
  return count;
}

unsigned outDegree(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  unsigned count = 0;
  for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next) {
    count++;
  }
  return count;
}

unsigned inDegree(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  unsigned *a = inDegrees(graph);
  unsigned n = a[vertex];
  free(a);
  return n;
}

unsigned countCommonNeighbors(const Graph *graph, unsigned u, unsigned v) {
  assert(isValid(graph));
  assert(u < graph->size);
  assert(v < graph->size);
  if (u >= graph->size || v >= graph->size) return 0;
  bool *neighbors = calloc(graph->size, sizeof(bool));
  for (Edge *e = graph->edges[u]; e != NULL; e = e->next)
    neighbors[e->destination] = true;
  unsigned count = 0;
  for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
    if (neighbors[e->destination]) {
      count++;
      neighbors[e->destination] = false; 
    }
  free(neighbors);
  return count;
}



unsigned *degreeDistribution(const Graph *graph) {
  unsigned *distribution = calloc(graph->size, sizeof(unsigned));
  for (unsigned vertex = 0; vertex < graph->size; vertex++) distribution[outDegree(graph, vertex)]++;
  return distribution;
}

unsigned *outDegrees(const Graph *graph) {
  unsigned *degrees = calloc(graph->size, sizeof(unsigned));
  for (unsigned vertex = 0; vertex < graph->size; vertex++)
    for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next)
      degrees[vertex]++;
  return degrees;
}

unsigned *inDegrees(const Graph *graph) {
  unsigned *degrees = calloc(graph->size, sizeof(unsigned));
  for (unsigned vertex = 0; vertex < graph->size; vertex++)
    for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next)
      degrees[edge->destination]++;
  return degrees;
}



int sumWeights(const Graph *graph) {
  assert(isValid(graph));
  int weight = 0;
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    for (Edge *edge = graph->edges[vertex]; edge; edge = edge->next) {
      weight += edge->weight;
    }
  }
  return weight;
}

int graphRadius(const Graph *graph) {
  int radius = INT_MAX;
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    int eccentricity = graphEccentricity(graph, vertex);
    if (eccentricity < radius) radius = eccentricity;
  }
  return radius;
}

int graphDiameter(const Graph *graph) {
  int diameter = INT_MIN;
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    int eccentricity = graphEccentricity(graph, vertex);
    if (eccentricity > diameter) diameter = eccentricity;
  }
  return diameter;
}

int graphEccentricity(const Graph *graph, unsigned vertex) {
  int *distance = weightedDijkstra(graph, vertex);
  int eccentricity = INT_MIN;
  for (unsigned vertex = 0; vertex < graph->size; vertex++) if (distance[vertex] > eccentricity) eccentricity = distance[vertex];
  free(distance);
  return eccentricity;
}

int edgeWeight(const Graph *graph, unsigned u, unsigned v) {
  for (Edge *edge = graph->edges[u]; edge != NULL; edge = edge->next) if (edge->destination == v) return edge->weight;
  return -1; 
}

int pathWeight(const Graph *graph, const unsigned *path, unsigned length) {
  int weight = 0;
  for (unsigned i = 1; i < length; i++) weight += edgeWeight(graph, path[i - 1], path[i]);
  return weight;
}



int **toMatrix(const Graph *graph) {
  int **matrix = malloc(graph->size * sizeof(int *));
  for (unsigned v = 0; v < graph->size; v++)
    matrix[v] = malloc(graph->size * sizeof(int));
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      matrix[v][e->destination] = e->weight;
  return matrix;
}



double density(const Graph *graph) {
  assert(isValid(graph));
  if (graph->size < 2) return 0;
  return (double)countEdges(graph) / (graph->size * (graph->size - 1));
}

double averageClusteringCoefficient(const Graph *graph) {
  assert(isValid(graph));
  if (graph->size == 0) return 0;
  double total = 0;
  for (unsigned vertex = 0; vertex < graph->size; vertex++) total += localClusteringCoefficient(graph, vertex);
  return total / graph->size;
}

double normalizedDegree(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  if (graph->size < 2) return 0;
  return (double)outDegree(graph, vertex) / (graph->size - 1);
}

double localClusteringCoefficient(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  unsigned k = outDegree(graph, vertex);
  if (k < 2) return 0;
  unsigned edgesBetweenNeighbours = 0;
  for (Edge *d = graph->edges[vertex]; d != NULL; d = d->next)
    for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next)
      if (isAdjacent(graph, d->destination, e->destination))
        edgesBetweenNeighbours++;
  return (double)edgesBetweenNeighbours / (k * (k - 1));
}

double subgraphDensity(const Graph *graph, const bool *subset) {
  assert(isValid(graph));
  assert(subset != NULL);
  unsigned vertices = 0;
  unsigned edges = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (subset[v]) {
      vertices++;
      for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
        if (subset[e->destination])
          edges++;
    }
  if (vertices <= 1) return 0;
  return (double)edges / (vertices * (vertices - 1));
}



FlatEdge *getEdgeArrayFromDirectedGraph(const Graph *graph) {
  FlatEdge *edges = malloc(countEdges(graph) * sizeof(FlatEdge));
  unsigned i = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next) {
      edges[i].u = v;
      edges[i].v = e->destination;
      edges[i].weight = e->weight;
      i++;
    }
  return edges;
}

FlatEdge *getEdgeArrayFromUndirectedGraph(const Graph *graph) {
  FlatEdge *edges = malloc(countEdges(graph) / 2 * sizeof(FlatEdge));
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

bool allAreReachableFromVertexInGraph(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  unsigned *distances = unweightedDijkstra(graph, vertex);
  bool b = true;
  for (unsigned v = 0; v < graph->size; v++)
    if (distances[v] == UINT_MAX) {
      b = false;
      break;
    }
  free(distances);
  return b;
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
  bool *visited = calloc(graph->size, sizeof(bool));
  depthFirstSortOfGraphComponent(graph, source, ordering, &count, visited);
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i])
      depthFirstSortOfGraphComponent(graph, i, ordering, &count, visited);
  free(visited);
  return ordering;
}

void breadthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *count, bool *visited) {
  assert(source < graph->size);
  unsigned *queue = malloc(graph->size * sizeof(unsigned));
  unsigned head = 0, tail = 0;
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
  free(queue);
}

unsigned *breadthFirstSortOfGraph(const Graph *graph, unsigned source) {
  assert(source < graph->size);
  unsigned *ordering = malloc(graph->size * sizeof(unsigned));
  unsigned count = 0;
  bool *visited = calloc(graph->size, sizeof(bool));
  breadthFirstSortOfGraphComponent(graph, source, ordering, &count, visited);
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i])
      breadthFirstSortOfGraphComponent(graph, i, ordering, &count, visited);
  free(visited);
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
  assert(!isCyclicDirected(graph));
  unsigned *ordering = malloc(graph->size * sizeof(unsigned));
  unsigned index = graph->size;
  bool *visited = calloc(graph->size, sizeof(bool));
  for (unsigned i = 0; i < graph->size; i++)
    if (!visited[i])
      topologicalSortOfGraphComponent(graph, i, ordering, &index, visited);
  free(visited);
  return ordering;
}

unsigned *coloring(const Graph *graph) {
  unsigned *colors = malloc(graph->size  *sizeof(unsigned));
  colors[0] = 0;
  for (unsigned vertex = 1; vertex < graph->size; vertex++) colors[vertex] = UINT_MAX;
  bool *taken = calloc(graph->size, sizeof(bool));
  for (unsigned vertex = 1; vertex < graph->size; vertex++) {
    for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next)
      if (colors[edge->destination] != UINT_MAX)
        taken[colors[edge->destination]] = true;
    unsigned color = 1;
    while (taken[color]) color++;
    colors[vertex] = color;
    for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next)
      if (colors[edge->destination] != UINT_MAX)
        taken[colors[edge->destination]] = false;
  }
  free(taken);
  return colors;
}

int *bellmanFord(const Graph *graph, unsigned source) {
  assert(isValid(graph));
  assert(source < graph->size);
  int *distance = malloc(graph->size * sizeof(int));
  for (unsigned v = 0; v < graph->size; v++) distance[v] = INT_MAX;
  distance[source] = 0;
  for (unsigned i = 1; i < graph->size; i++)
    for (unsigned v = 0; v < graph->size; v++)
      if (distance[v] < INT_MAX)
        for (Edge *e = graph->edges[v]; e; e = e->next)
          if (distance[v] + e->weight < distance[e->destination])
            distance[e->destination] = distance[v] + e->weight;
  bool hasNegativeCycle = false;
  for (unsigned v = 0; v < graph->size; v++)
    if (distance[v] < INT_MAX)
      for (Edge *e = graph->edges[v]; e; e = e->next)
        if (distance[v] + e->weight < distance[e->destination])
          hasNegativeCycle = true;
  if (hasNegativeCycle) {
    free(distance);
    distance = NULL;
  }
  return distance;
}

unsigned *unweightedDijkstra(const Graph *graph, unsigned source) {
  assert(isValid(graph));
  assert(source < graph->size);
  unsigned *distances = malloc(graph->size * sizeof(unsigned));
  for (unsigned i = 0; i < graph->size; i++) distances[i] = INT_MAX;
  unsigned *queue = malloc(graph->size * sizeof(unsigned));
  unsigned head = 0, tail = 0;
  distances[source] = 0;
  queue[tail++] = source;
  while (head < tail) {
    unsigned u = queue[head++];
    for (Edge *edge = graph->edges[u]; edge != NULL; edge = edge->next) {
      if (distances[edge->destination] == INT_MAX) {
        distances[edge->destination] = distances[u] + 1;
        queue[tail++] = edge->destination;
      }
    }
  }
  free(queue);
  return distances;
}

int *weightedDijkstra(const Graph *graph, unsigned source) {
  assert(isValid(graph));
  assert(source < graph->size);
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

int **floydWarshall(const Graph *graph) {
  int **distance = malloc(graph->size * sizeof(int *));
  for (unsigned i = 0; i < graph->size; i++) {
    distance[i] = malloc(graph->size * sizeof(int));
    for (unsigned j = 0; j < graph->size; j++) {
      if (i == j) distance[i][j] = 0;
      else distance[i][j] = INT_MAX;
    }
  }
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      if (e->weight < distance[v][e->destination])
        distance[v][e->destination] = e->weight;
  for (unsigned k = 0; k < graph->size; k++)
    for (unsigned i = 0; i < graph->size; i++)
      for (unsigned j = 0; j < graph->size; j++)
        if (distance[i][k] < INT_MAX && distance[k][j] < INT_MAX)
          if (distance[i][k] + distance[k][j] < distance[i][j])
            distance[i][j] = distance[i][k] + distance[k][j];
  return distance;
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
  unsigned count = countEdges(graph) / 2;
  FlatEdge *edges = getEdgeArrayFromUndirectedGraph(graph);
  qsort(edges, count, sizeof(FlatEdge), compareEdges);
  Dsu *dsu = createDsu(graph->size);
  Graph *mst = createGraph(graph->size);
  for (unsigned i = 0; i < count; i++)
    if (findDsu(dsu, edges[i].u) != findDsu(dsu, edges[i].v)) {
      unionDsu(dsu, edges[i].u, edges[i].v);
      addEdgeToUndirectedGraph(mst, edges[i].u, edges[i].v, edges[i].weight);
    }
  free(edges);
  freeDsu(dsu);
  return mst;
}

void recursivelyFindArticulationPoints(
  const Graph *graph,
  unsigned vertex,
  bool *visited,
  unsigned *discovery,
  unsigned *low,
  unsigned *parent,
  bool *articulations,
  unsigned *timer)
{
  unsigned children = 0;
  visited[vertex] = true;
  discovery[vertex] = low[vertex] = ++(*timer);
  for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next) {
    if (!visited[edge->destination]) {
      children++;
      parent[edge->destination] = vertex;
      recursivelyFindArticulationPoints(graph, edge->destination, visited, discovery, low, parent, articulations, timer);
      low[vertex] = minimumUnsigned(low[vertex], low[edge->destination]);
      if (parent[vertex] == UINT_MAX && children >= 2) {
        articulations[vertex] = true;
      }
      if (parent[vertex] != UINT_MAX && low[edge->destination] >= discovery[vertex]) {
        articulations[vertex] = true;
      }
    } else if (edge->destination != parent[vertex]) {
      low[vertex] = minimumUnsigned(low[vertex], discovery[edge->destination]);
    }
  }
}

bool *findArticulationPoints(const Graph *graph) {
  bool *visited = calloc(graph->size, sizeof(bool));
  unsigned *discovery = calloc(graph->size, sizeof(unsigned));
  unsigned *low = calloc(graph->size, sizeof(unsigned));
  unsigned *parent = malloc(graph->size * sizeof(unsigned));
  bool *articulations = calloc(graph->size, sizeof(bool));
  unsigned timer = 0;
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    parent[vertex] = UINT_MAX;
  }
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    if (!visited[vertex]) {
      recursivelyFindArticulationPoints(graph, vertex, visited, discovery, low, parent, articulations, &timer);
    }
  }
  free(visited);
  free(discovery);
  free(low);
  free(parent);
  return articulations;
}

void recursivelyFindBridges(
  const Graph *graph,
  unsigned vertex,
  bool *visited,
  unsigned *discovery,
  unsigned *low,
  unsigned *parent,
  unsigned **bridges,
  unsigned *count,
  unsigned *timer)
{
  visited[vertex] = true;
  discovery[vertex] = low[vertex] = ++(*timer);
  for (Edge *edge = graph->edges[vertex]; edge != NULL; edge = edge->next) {
    if (!visited[edge->destination]) {
      parent[edge->destination] = vertex;
      recursivelyFindBridges(graph, edge->destination, visited, discovery, low, parent, bridges, count, timer);
      low[vertex] = minimumUnsigned(low[vertex], low[edge->destination]);
      if (low[edge->destination] > discovery[vertex]) {
        bridges[*count] = malloc(2 * sizeof(unsigned));
        bridges[*count][0] = vertex;
        bridges[*count][1] = edge->destination;
        (*count)++;
      }
    } else if (edge->destination != parent[vertex]) {
      low[vertex] = minimumUnsigned(low[vertex], discovery[edge->destination]);
    }
  }
}

unsigned **findBridges(const Graph *graph) {
  unsigned *discovery = calloc(graph->size, sizeof(unsigned));
  unsigned *low = calloc(graph->size, sizeof(unsigned));
  unsigned *parent = malloc(graph->size * sizeof(unsigned));
  bool *visited = calloc(graph->size, sizeof(bool));
  unsigned **bridges = calloc(graph->size, sizeof(unsigned *));
  unsigned count = 0;
  unsigned timer = 0;
  for (unsigned vertex = 0; vertex < graph->size; vertex++) parent[vertex] = UINT_MAX;
  for (unsigned vertex = 0; vertex < graph->size; vertex++) {
    if (!visited[vertex]) {
      recursivelyFindBridges(graph, vertex, visited, discovery, low, parent, bridges, &count, &timer);
    }
  }
  free(discovery);
  free(low);
  free(parent);
  free(visited);
  return bridges;
}



void testIsDirectedCyclicGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToDirectedGraph(g1, 0, 1, 1);
  addEdgeToDirectedGraph(g1, 1, 2, 1);
  addEdgeToDirectedGraph(g1, 2, 0, 1);
  assert(isCyclicDirected(g1) == true);
  printf("Directed cyclic test 1 passed: Simple cycle found.\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addEdgeToDirectedGraph(g2, 0, 1, 1);
  addEdgeToDirectedGraph(g2, 1, 2, 1);
  addEdgeToDirectedGraph(g2, 0, 2, 1);
  assert(isCyclicDirected(g2) == false);
  printf("Directed cyclic test 2 passed: DAG correctly identified as acyclic.\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(1);
  addEdgeToDirectedGraph(g3, 0, 0, 1);
  assert(isCyclicDirected(g3) == true);
  printf("Directed cyclic test 3 passed: Self-loop detected.\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(4);
  addEdgeToDirectedGraph(g4, 0, 1, 1);
  addEdgeToDirectedGraph(g4, 2, 3, 1);
  addEdgeToDirectedGraph(g4, 3, 2, 1);
  assert(isCyclicDirected(g4) == true);
  printf("Directed cyclic test 4 passed: Cycle in disconnected component found.\n");
  destroyGraph(g4);

  Graph *g5 = createGraph(0);
  assert(isCyclicDirected(g5) == false);
  printf("Directed cyclic test 5 passed: Empty graph is acyclic.\n");
  destroyGraph(g5);
}

void testIsUndirectedCyclicGraph() {
  Graph *g1 = createGraph(3);
  addEdgeToUndirectedGraph(g1, 0, 1, 1);
  addEdgeToUndirectedGraph(g1, 1, 2, 1);
  if (!isCyclicUndirected(g1))
    printf("Undirected Test 1 passed: Tree is acyclic.\n");
  else
    printf("Undirected Test 1 failed: False positive in tree.\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addEdgeToUndirectedGraph(g2, 0, 1, 1);
  addEdgeToUndirectedGraph(g2, 1, 2, 1);
  addEdgeToUndirectedGraph(g2, 2, 0, 1);
  if (isCyclicUndirected(g2))
    printf("Undirected Test 2 passed: Triangle cycle detected.\n");
  else
    printf("Undirected Test 2 failed: Triangle cycle missed.\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(5);
  addEdgeToUndirectedGraph(g3, 0, 1, 1);
  addEdgeToUndirectedGraph(g3, 2, 3, 1);
  addEdgeToUndirectedGraph(g3, 3, 4, 1);
  addEdgeToUndirectedGraph(g3, 4, 2, 1);
  if (isCyclicUndirected(g3))
    printf("Undirected Test 3 passed: Cycle in disconnected component detected.\n");
  else
    printf("Undirected Test 3 failed: Missed cycle in disconnected component.\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(2);
  addEdgeToUndirectedGraph(g4, 0, 1, 1);
  if (!isCyclicUndirected(g4))
    printf("Undirected Test 4 passed: Simple edge is acyclic.\n");
  else
    printf("Undirected Test 4 failed: Parent incorrectly triggered cycle.\n");
  destroyGraph(g4);
}

void testIsConnectedUndirected() {
  Graph *g1 = createGraph(1);
  assert(isConnectedUndirected(g1) == true);
  printf("Undirected connected test 1 passed: Single vertex\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addEdgeToUndirectedGraph(g2, 0, 1, 1);
  addEdgeToUndirectedGraph(g2, 1, 2, 1);
  assert(isConnectedUndirected(g2) == true);
  printf("Undirected connected test 2 passed: Simple line graph\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(4);
  addEdgeToUndirectedGraph(g3, 0, 1, 1);
  addEdgeToUndirectedGraph(g3, 2, 3, 1);
  assert(isConnectedUndirected(g3) == false);
  printf("Undirected connected test 3 passed: Disconnected components\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(3);
  addEdgeToUndirectedGraph(g4, 0, 1, 1);
  assert(isConnectedUndirected(g4) == false);
  printf("Undirected connected test 4 passed: Isolated vertex\n");
  destroyGraph(g4);

  Graph *g5 = createGraph(4);
  addEdgeToUndirectedGraph(g5, 0, 1, 1);
  addEdgeToUndirectedGraph(g5, 0, 2, 1);
  addEdgeToUndirectedGraph(g5, 0, 3, 1);
  addEdgeToUndirectedGraph(g5, 1, 2, 1);
  addEdgeToUndirectedGraph(g5, 1, 3, 1);
  addEdgeToUndirectedGraph(g5, 2, 3, 1);
  assert(isConnectedUndirected(g5) == true);
  printf("Undirected connected test 5 passed: Complete graph\n");
  destroyGraph(g5);
}

void testIsWeaklyConnectedDirected() {
  Graph *g1 = createGraph(3);
  addEdgeToDirectedGraph(g1, 0, 1, 1);
  addEdgeToDirectedGraph(g1, 1, 2, 1);
  assert(isWeaklyConnectedDirected(g1) == true);
  printf("Weakly Test 1 passed: Simple chain\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addEdgeToDirectedGraph(g2, 0, 1, 1);
  addEdgeToDirectedGraph(g2, 2, 1, 1);
  assert(isWeaklyConnectedDirected(g2) == true);
  printf("Weakly Test 2 passed: Source/Sink structure\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(4);
  addEdgeToDirectedGraph(g3, 0, 1, 1);
  addEdgeToDirectedGraph(g3, 2, 3, 1);
  assert(isWeaklyConnectedDirected(g3) == false);
  printf("Weakly Test 3 passed: Truly disconnected components\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(4);
  addEdgeToDirectedGraph(g4, 0, 1, 1);
  addEdgeToDirectedGraph(g4, 0, 2, 1);
  addEdgeToDirectedGraph(g4, 0, 3, 1);
  assert(isWeaklyConnectedDirected(g4) == true);
  printf("Weakly Test 4 passed: Star pattern\n");
  destroyGraph(g4);

  Graph *g5 = createGraph(2);
  addEdgeToDirectedGraph(g5, 0, 0, 1);
  assert(isWeaklyConnectedDirected(g5) == false);
  printf("Weakly Test 5 passed: Isolated vertex with self-loop\n");
  destroyGraph(g5);
}

void testIsStronglyConnectedDirected() {
  Graph *g1 = createGraph(3);
  addEdgeToDirectedGraph(g1, 0, 1, 1);
  addEdgeToDirectedGraph(g1, 1, 2, 1);
  addEdgeToDirectedGraph(g1, 2, 0, 1);
  assert(isStronglyConnectedDirected(g1) == true);
  printf("Strongly Test 1 passed: Simple cycle\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addEdgeToDirectedGraph(g2, 0, 1, 1);
  addEdgeToDirectedGraph(g2, 1, 2, 1);
  assert(isStronglyConnectedDirected(g2) == false);
  printf("Strongly Test 2 passed: Linear chain (not strong)\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(4);
  addEdgeToDirectedGraph(g3, 0, 1, 1);
  addEdgeToDirectedGraph(g3, 1, 0, 1);
  addEdgeToDirectedGraph(g3, 2, 3, 1);
  addEdgeToDirectedGraph(g3, 3, 2, 1);
  assert(isStronglyConnectedDirected(g3) == false);
  printf("Strongly Test 3 passed: Disconnected cycles\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(3);
  addEdgeToDirectedGraph(g4, 0, 1, 1);
  addEdgeToDirectedGraph(g4, 1, 0, 1);
  addEdgeToDirectedGraph(g4, 1, 2, 1);
  addEdgeToDirectedGraph(g4, 2, 1, 1);
  assert(isStronglyConnectedDirected(g4) == true);
  printf("Strongly Test 4 passed: Bidirectional chain\n");
  destroyGraph(g4);

  Graph *g5 = createGraph(1);
  assert(isStronglyConnectedDirected(g5) == true);
  printf("Strongly Test 5 passed: Single vertex\n");
  destroyGraph(g5);
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

void testUnweightedDijkstra() {
  printf("Running unweightedDijkstra tests...\n");

  {
    Graph *g = createGraph(3);
    addEdgeToDirectedGraph(g, 0, 1, 10);
    addEdgeToDirectedGraph(g, 1, 2, 20);

    unsigned *distances = unweightedDijkstra(g, 0);
    assert(distances[0] == 0);
    assert(distances[1] == 1);
    assert(distances[2] == 2);

    printf("Passed: Simple path (weights ignored)\n");
    free(distances);
    destroyGraph(g);
  }

  {
    Graph *g = createGraph(3);
    addEdgeToDirectedGraph(g, 0, 2, 100);
    addEdgeToDirectedGraph(g, 0, 1, 1);
    addEdgeToDirectedGraph(g, 1, 2, 1);

    unsigned *distances = unweightedDijkstra(g, 0);
    assert(distances[2] == 1);

    printf("Passed: Shortest hops selection\n");
    free(distances);
    destroyGraph(g);
  }

  {
    Graph *g = createGraph(3);
    addEdgeToDirectedGraph(g, 0, 1, 1);

    unsigned *distances = unweightedDijkstra(g, 0);
    assert(distances[0] == 0);
    assert(distances[1] == 1);
    assert(distances[2] == INT_MAX);

    printf("Passed: Unreachable node (INT_MAX)\n");
    free(distances);
    destroyGraph(g);
  }

  {
    Graph *g = createGraph(3);
    addEdgeToDirectedGraph(g, 0, 1, 1);
    addEdgeToDirectedGraph(g, 1, 2, 1);
    addEdgeToDirectedGraph(g, 2, 0, 1);

    unsigned *distances = unweightedDijkstra(g, 0);
    assert(distances[0] == 0);
    assert(distances[1] == 1);
    assert(distances[2] == 2);

    printf("Passed: Cyclic graph\n");
    free(distances);
    destroyGraph(g);
  }
}

void testWeightedDijkstra() {
  Graph *g1 = createGraph(3);
  addEdgeToDirectedGraph(g1, 0, 1, 5);
  addEdgeToDirectedGraph(g1, 1, 2, 10);
  int *dist1 = weightedDijkstra(g1, 0);
  assert(dist1[0] == 0);
  assert(dist1[1] == 5);
  assert(dist1[2] == 15);
  printf("Dijkstra test 1 passed: Simple path\n");
  destroyGraph(g1);
  free(dist1);

  Graph *g2 = createGraph(3);
  addEdgeToDirectedGraph(g2, 0, 2, 10);
  addEdgeToDirectedGraph(g2, 0, 1, 2);
  addEdgeToDirectedGraph(g2, 1, 2, 3);
  int *dist2 = weightedDijkstra(g2, 0);
  assert(dist2[2] == 5);
  printf("Dijkstra test 2 passed: Shortest path selection\n");
  destroyGraph(g2);
  free(dist2);

  Graph *g3 = createGraph(2);
  int *dist3 = weightedDijkstra(g3, 0);
  assert(dist3[0] == 0);
  assert(dist3[1] == INT_MAX);
  printf("Dijkstra test 3 passed: Unreachable vertex (INT_MAX)\n");
  destroyGraph(g3);
  free(dist3);

  Graph *g4 = createGraph(3);
  addEdgeToDirectedGraph(g4, 0, 1, 1);
  addEdgeToDirectedGraph(g4, 1, 2, 1);
  addEdgeToDirectedGraph(g4, 2, 0, 1);
  int *dist4 = weightedDijkstra(g4, 0);
  assert(dist4[0] == 0);
  assert(dist4[1] == 1);
  assert(dist4[2] == 2);
  printf("Dijkstra test 4 passed: Cyclic graph\n");
  destroyGraph(g4);
  free(dist4);
}

void testFloydWarshall() {
  {
    printf("Floyd-Warshall: Testing basic shortest path... ");
    Graph *g = createGraph(3);
    addEdgeToDirectedGraph(g, 0, 1, 10);
    addEdgeToDirectedGraph(g, 1, 2, 5);
    addEdgeToDirectedGraph(g, 0, 2, 20);

    int **distances = floydWarshall(g);

    assert(distances[0][1] == 10);
    assert(distances[1][2] == 5);
    assert(distances[0][2] == 15);
    assert(distances[2][0] == INT_MAX);

    freeMatrix(distances, 3);
    destroyGraph(g);
    printf("Passed!\n");
  }
  {
    printf("Floyd-Warshall: Testing negative weight (no cycle)... ");
    Graph *g = createGraph(3);
    addEdgeToDirectedGraph(g, 0, 1, 4);
    addEdgeToDirectedGraph(g, 0, 2, 5);
    addEdgeToDirectedGraph(g, 1, 2, -2);

    int **distances = floydWarshall(g);

    assert(distances[0][2] == 2);
    assert(distances[1][2] == -2);

    freeMatrix(distances, 3);
    destroyGraph(g);
    printf("Passed!\n");
  }
  {
    printf("Floyd-Warshall: Testing disconnected components... ");
    Graph *g = createGraph(4);
    addEdgeToDirectedGraph(g, 0, 1, 1);
    addEdgeToDirectedGraph(g, 2, 3, 1);

    int **distances = floydWarshall(g);

    assert(distances[0][1] == 1);
    assert(distances[0][2] == INT_MAX);
    assert(distances[3][0] == INT_MAX);

    freeMatrix(distances, 4);
    destroyGraph(g);
    printf("Passed!\n");
  }
  {
    printf("Floyd-Warshall: Testing negative cycle detection... ");
    unsigned n = 3;
    Graph *g = createGraph(n);

    addEdgeToDirectedGraph(g, 0, 1, 1);
    addEdgeToDirectedGraph(g, 1, 2, 1);
    addEdgeToDirectedGraph(g, 2, 0, -5);

    int **distances = floydWarshall(g);

    bool hasNegativeCycle = false;
    for (unsigned i = 0; i < n; i++)
      if (distances[i][i] < 0)
        hasNegativeCycle = true;

    assert(hasNegativeCycle == true);
    assert(distances[0][0] == -3);

    freeMatrix(distances, n);
    destroyGraph(g);
    printf("Passed!\n");
  }
}

void testPrim() {
  {
    Graph *g = createGraph(3);
    addEdgeToUndirectedGraph(g, 0, 1, 1);
    addEdgeToUndirectedGraph(g, 1, 2, 3);
    addEdgeToUndirectedGraph(g, 0, 2, 4);
    Graph *mst = prim(g, 0);
    int w = sumWeights(mst) / 2;
    assert(countEdges(mst) == 4);
    assert(w == 4);
    printf("Prim test 1 (triangle) passed: weight %d\n", w);
    destroyGraph(g);
    destroyGraph(mst);
  }
  {
    Graph *g = createGraph(5);
    addEdgeToUndirectedGraph(g, 0, 1, 2);
    addEdgeToUndirectedGraph(g, 0, 3, 6);
    addEdgeToUndirectedGraph(g, 1, 2, 3);
    addEdgeToUndirectedGraph(g, 1, 3, 8);
    addEdgeToUndirectedGraph(g, 1, 4, 5);
    addEdgeToUndirectedGraph(g, 2, 4, 7);
    addEdgeToUndirectedGraph(g, 3, 4, 9);
    Graph *mst = prim(g, 0);
    int w = sumWeights(mst) / 2;
    assert(countEdges(mst) == 8);
    assert(w == 16);
    printf("Prim test 2 (complex) passed: weight %d\n", w);
    destroyGraph(g);
    destroyGraph(mst);
  }
  {
    Graph *g = createGraph(1);
    Graph *mst = prim(g, 0);
    int w = sumWeights(mst);
    assert(countEdges(mst) == 0);
    assert(w == 0);
    printf("Prim test 3 (single vertex) passed!\n");
    destroyGraph(g);
    destroyGraph(mst);
  }
}

void testKruskal() {
  Graph *g = createGraph(4);
  addEdgeToUndirectedGraph(g, 0, 1, 10);
  addEdgeToUndirectedGraph(g, 1, 3, 15);
  addEdgeToUndirectedGraph(g, 3, 2, 4);
  addEdgeToUndirectedGraph(g, 2, 0, 6);
  addEdgeToUndirectedGraph(g, 0, 3, 5);
  Graph *mst = kruskal(g);
  int weight = sumWeights(mst) / 2;
  assert(countEdges(mst) == 6);
  assert(weight == 19);
  printf("Kruskal test passed: weight %d\n", weight);
  destroyGraph(g);
  destroyGraph(mst);
}

void testFindArticulationPoints() {
  {
    unsigned n = 3;
    Graph *g = createGraph(n);
    addEdgeToUndirectedGraph(g, 0, 1, 1);
    addEdgeToUndirectedGraph(g, 1, 2, 1);
    bool *articulations = findArticulationPoints(g);
    bool expected[] = {false, true, false};
    for (unsigned i = 0; i < n; i++) assert(articulations[i] == expected[i]);
    printf("Articulation test passed: linear path\n");
    destroyGraph(g);
    free(articulations);
  }
  {
    unsigned n = 3;
    Graph *g = createGraph(n);
    addEdgeToUndirectedGraph(g, 0, 1, 1);
    addEdgeToUndirectedGraph(g, 1, 2, 1);
    addEdgeToUndirectedGraph(g, 2, 0, 1);
    bool *articulations = findArticulationPoints(g);
    bool expected[] = {false, false, false};
    for (unsigned i = 0; i < n; i++) assert(articulations[i] == expected[i]);
    printf("Articulation test passed: simple cycle\n");
    destroyGraph(g);
    free(articulations);
  }
  {
    unsigned n = 4;
    Graph *g = createGraph(n);
    addEdgeToUndirectedGraph(g, 0, 1, 1);
    addEdgeToUndirectedGraph(g, 0, 2, 1);
    addEdgeToUndirectedGraph(g, 0, 3, 1);
    bool *articulations = findArticulationPoints(g);
    bool expected[] = {true, false, false, false};
    for (unsigned i = 0; i < n; i++) assert(articulations[i] == expected[i]);
    printf("Articulation test passed: star graph\n");
    destroyGraph(g);
    free(articulations);
  }
  {
    unsigned n = 6;
    Graph *g = createGraph(n);
    addEdgeToUndirectedGraph(g, 0, 1, 1); addEdgeToUndirectedGraph(g, 1, 2, 1); addEdgeToUndirectedGraph(g, 2, 0, 1);
    addEdgeToUndirectedGraph(g, 2, 3, 1);
    addEdgeToUndirectedGraph(g, 3, 4, 1); addEdgeToUndirectedGraph(g, 4, 5, 1); addEdgeToUndirectedGraph(g, 5, 3, 1);
    bool *articulations = findArticulationPoints(g);
    bool expected[] = {false, false, true, true, false, false};
    for (unsigned i = 0; i < n; i++) assert(articulations[i] == expected[i]);
    printf("Articulation test passed: two triangles connected by a bridge\n");
    destroyGraph(g);
    free(articulations);
  }
  {
    unsigned n = 5;
    Graph *g = createGraph(n);
    addEdgeToUndirectedGraph(g, 0, 1, 1);
    addEdgeToUndirectedGraph(g, 2, 3, 1);
    addEdgeToUndirectedGraph(g, 3, 4, 1);
    bool *articulations = findArticulationPoints(g);
    bool expected[] = {false, false, false, true, false};
    for (unsigned i = 0; i < n; i++) assert(articulations[i] == expected[i]);
    printf("Articulation test passed: disconnected components\n");
    destroyGraph(g);
    free(articulations);
  }
}

bool hasBridge(unsigned *const *const bridges, unsigned u, unsigned v) {
  if (!bridges) return false;
  for (int i = 0; bridges[i] != NULL; i++) {
    if ((bridges[i][0] == u && bridges[i][1] == v) || (bridges[i][0] == v && bridges[i][1] == u)) {
      return true;
    }
  }
  return false;
}

void freeBridgeResult(unsigned **bridges) {
  if (!bridges) return;
  for (int i = 0; bridges[i] != NULL; i++) {
    free(bridges[i]);
  }
  free(bridges);
}

void testFindBridges() {
  printf("Running Bridge Detection Tests...\n");

  Graph *line = createGraph(3);
  addEdgeToUndirectedGraph(line, 0, 1, 1);
  addEdgeToUndirectedGraph(line, 1, 2, 1);

  unsigned **b1 = findBridges(line);
  assert(hasBridge(b1, 0, 1));
  assert(hasBridge(b1, 1, 2));
  freeBridgeResult(b1);
  destroyGraph(line);
  printf("Passed: Line Graph\n");

  Graph *cycle = createGraph(3);
  addEdgeToUndirectedGraph(cycle, 0, 1, 1);
  addEdgeToUndirectedGraph(cycle, 1, 2, 1);
  addEdgeToUndirectedGraph(cycle, 2, 0, 1);

  unsigned **b2 = findBridges(cycle);
  assert(b2[0] == NULL);
  freeBridgeResult(b2);
  destroyGraph(cycle);
  printf("Passed: Cycle (No Bridges)\n");

  Graph *dumbbell = createGraph(6);
  addEdgeToUndirectedGraph(dumbbell, 0, 1, 1);
  addEdgeToUndirectedGraph(dumbbell, 1, 2, 1);
  addEdgeToUndirectedGraph(dumbbell, 2, 0, 1);
  addEdgeToUndirectedGraph(dumbbell, 1, 3, 1);
  addEdgeToUndirectedGraph(dumbbell, 3, 4, 1);
  addEdgeToUndirectedGraph(dumbbell, 4, 5, 1);
  addEdgeToUndirectedGraph(dumbbell, 5, 3, 1);

  unsigned **b3 = findBridges(dumbbell);
  assert(hasBridge(b3, 1, 3));
  assert(!hasBridge(b3, 0, 1));
  freeBridgeResult(b3);
  destroyGraph(dumbbell);
  printf("Passed: Dumbbell Graph\n");
}

int main() {
  testIsDirectedCyclicGraph();
  testIsUndirectedCyclicGraph();
  testIsConnectedUndirected();
  testIsWeaklyConnectedDirected();
  testIsStronglyConnectedDirected();
  testDepthFirstSortOfGraph();
  testBreadthFirstSortOfGraph();
  testTopologicalSortOfGraph();
  testBellmanFord();
  testUnweightedDijkstra();
  testWeightedDijkstra();
  testFloydWarshall();
  testPrim();
  testKruskal();
  testFindArticulationPoints();
  testFindBridges();
  printf("All tests passed!\n");
  return 0;
}
