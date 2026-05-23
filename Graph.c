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
  unsigned *inDegree;
  unsigned *outDegree;
  Edge **edges;
} Graph;

bool isValidEdge(const Graph *graph);
bool isValidInDegree(const Graph *graph);
bool isValidOutDegree(const Graph *graph);
bool isValid(const Graph *graph);
bool isNull(const Graph *graph);
bool isTrivial(const Graph *graph);
bool isEmpty(const Graph *graph);
bool isRegular(const Graph *graph);
bool isComplete(const Graph *graph);
bool hasSelfLoops(const Graph *graph);
bool isBalanced(const Graph *graph);
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
bool allAreReachableFromVertexInGraph(const Graph *graph, unsigned vertex);
bool isAdjacent(const Graph *graph, unsigned u, unsigned v);
bool isReachable(const Graph *graph, unsigned start, unsigned target);
bool hasWeightedEdge(const Graph *graph, unsigned u, unsigned v, int weight);
bool isClique(const Graph *graph, const bool *subset);
bool isIndependentSet(const Graph *graph, const bool *subset);
bool isVertexCover(const Graph *graph, const bool *subset);
bool isWalk(const Graph *graph, const unsigned *sequence, unsigned length);
bool isPath(const Graph *graph, const unsigned *path, unsigned length);
bool isHamiltonianPath(const Graph *graph, const unsigned *path, unsigned length);
bool isTrail(const Graph *graph, const unsigned *sequence, unsigned length);
bool isDirectedCycle(const Graph *graph, const unsigned *path, unsigned length);
bool isSimpleCycle(const Graph *graph, const unsigned *path, unsigned length);
bool isHamiltonianCycle(const Graph *graph, const unsigned *path, unsigned length);
bool isCircuit(const Graph *graph, const unsigned *sequence, unsigned length);
bool isSubGraph(const Graph *sub, const Graph *main);

bool *graphCenter(const Graph *graph);
bool *graphPeriphery(const Graph *graph);
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

Graph *createGraph(unsigned size);
Graph *createPathGraph(unsigned size);
Graph *createCycleGraph(unsigned size);
Graph *createCompleteGraph(unsigned size);
Graph *copyGraph(const Graph *graph);
Graph *copyTranspose(const Graph *graph);
Graph *copyUnweighted(const Graph *graph);
Graph *copyUndirected(const Graph *graph);
Graph *copyComplement(const Graph *graph);
Graph *lineGraph(const Graph *graph);
Graph *underlyingGraph(const Graph *graph);
Graph *kruskal(const Graph *graph);
Graph *removeVertex(const Graph *graph, unsigned vertex);
Graph *prim(const Graph *graph, unsigned source);
Graph *mergeVertices(const Graph *graph, unsigned u, unsigned v);
Graph *copySubgraph(const Graph *graph, const bool *vertices);
Graph *graphUnion(const Graph *g1, const Graph *g2);
Graph *createDirectedGraphFromEdgeArray(unsigned size, const FlatEdge *edges, unsigned count);
Graph *createUndirectedGraphFromEdgeArray(unsigned size, const FlatEdge *edges, unsigned count);

void printGraph(const Graph *graph);
void destroyGraph(Graph *graph);
void removeDirectedEdge(Graph *graph, unsigned source, unsigned destination);
void removeUndirectedEdge(Graph *graph, unsigned u, unsigned v);
void addDirectedEdge(Graph *graph, unsigned source, unsigned destination, int weight);
void addUndirectedEdge(Graph *graph, unsigned u, unsigned v, int weight);

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

unsigned *outDegreeDistribution(const Graph *graph);
unsigned *inDegreeDistribution(const Graph *graph);
unsigned *coloring(const Graph *graph);
void topologicalSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *topologicalSortOfGraph(const Graph *graph);
unsigned *getNeighbors(const Graph *graph, unsigned vertex);
unsigned *unweightedDijkstra(const Graph *graph, unsigned source);
void depthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *depthFirstSortOfGraph(const Graph *graph, unsigned source);
void breadthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *index, bool *visited);
unsigned *breadthFirstSortOfGraph(const Graph *graph, unsigned source);

int sumWeights(const Graph *graph);
int graphRadius(const Graph *graph);
int graphDiameter(const Graph *graph);
int graphEccentricity(const Graph *graph, unsigned vertex);
int edgeWeight(const Graph *graph, unsigned source, unsigned destination);
int pathWeight(const Graph *graph, const unsigned *path, unsigned length);

int *bellmanFord(const Graph *graph, unsigned source);
int *weightedDijkstra(const Graph *graph, unsigned source);

int **toMatrix(const Graph *graph);
int **floydWarshall(const Graph *graph);

double density(const Graph *graph);
double averageClusteringCoefficient(const Graph *graph);
double normalizedDegree(const Graph *graph, unsigned vertex);
double localClusteringCoefficient(const Graph *graph, unsigned vertex);
double subgraphDensity(const Graph *graph, const bool *subset);

FlatEdge *getEdgeArrayFromDirectedGraph(const Graph *graph);
FlatEdge *getEdgeArrayFromUndirectedGraph(const Graph *graph);

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



bool isValidEdge(const Graph *graph) {
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      if (e->destination >= graph->size)
        return false;
  return true;
}

bool isValidInDegree(const Graph *graph) {
  unsigned *degree = calloc(graph->size, sizeof(unsigned));
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      degree[e->destination]++;
  bool valid = true;
  for (unsigned v = 0; v < graph->size; v++)
    if (graph->inDegree[v] != degree[v]) {
      valid = false;
      break;
    }
  free(degree);
  return valid;
}

bool isValidOutDegree(const Graph *graph) {
  for (unsigned v = 0; v < graph->size; v++) {
    unsigned degree = 0;
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      degree++;
    if (graph->outDegree[v] != degree)
      return false;
  }
  return true;
}

bool isValid(const Graph *graph) {
  if (graph == NULL || graph->inDegree == NULL || graph->outDegree == NULL || graph->edges == NULL) return false;
  return isValidEdge(graph) && isValidInDegree(graph) && isValidOutDegree(graph);
}

bool isNull(const Graph *graph) {
  return graph->size == 0;
}

bool isTrivial(const Graph *graph) {
  return graph->size == 1 && graph->edges[0] == NULL;
}

bool isEmpty(const Graph *graph) {
  for (unsigned v = 0; v < graph->size; v++)
    if (graph->edges[v] != NULL)
      return false;
  return true;
}

bool isRegular(const Graph *graph) {
  for (unsigned v = 1; v < graph->size; v++)
    if (outDegree(graph, v - 1) != outDegree(graph, v))
      return false;
  return true;
}

bool isComplete(const Graph *graph) {
  return countEdges(graph) == graph->size * (graph->size - 1);
}

bool hasSelfLoops(const Graph *graph) {
  for (unsigned v = 0; v < graph->size; v++)
    if (hasSelfLoopsAtVertex(graph, v))
      return true;
  return false;
}

bool isBalanced(const Graph *graph) {
  for (unsigned v = 0; v < graph->size; v++)
    if (inDegree(graph, v) != outDegree(graph, v))
      return false;
  return true;
}

bool isEulerianUndirected(const Graph *graph) {
  for (unsigned v = 0; v < graph->size; v++)
    if (outDegree(graph, v) % 2 != 0)
      return false;
  return isConnectedUndirected(graph);
}

bool isEulerianDirected(const Graph *graph) {
  return isBalanced(graph) && isStronglyConnectedDirected(graph);
}

bool isConnectedUndirected(const Graph *graph) {
  if (graph->size < 2) return true;
  return allAreReachableFromVertexInGraph(graph, 0);
}

bool isWeaklyConnectedDirected(const Graph *graph) {
  Graph *undirected = copyUndirected(graph);
  bool connected = isConnectedUndirected(undirected);
  destroyGraph(undirected);
  return connected;
}

bool isStronglyConnectedDirected(const Graph *graph) {
  if (graph->size < 2) return true;
  Graph *g = copyTranspose(graph);
  bool reachable = allAreReachableFromVertexInGraph(graph, 0) && allAreReachableFromVertexInGraph(g, 0);
  destroyGraph(g);
  return reachable;
}

bool isBipartite(const Graph *graph) {
  int *colors = calloc(graph->size, sizeof(int));
  unsigned *queue = malloc(graph->size * sizeof(unsigned));
  bool bipartite = true;
  for (unsigned u = 0; u < graph->size && bipartite; u++)
    if (colors[u] == 0) {
      colors[u] = 1;
      unsigned head = 0, tail = 0;
      queue[tail++] = u;
      while (head < tail && bipartite) {
        unsigned v = queue[head++];
        for (Edge *e = graph->edges[v]; e != NULL && bipartite; e = e->next)
          if (colors[e->destination] == 0) {
            colors[e->destination] = (colors[v] == 1) ? 2 : 1;
            queue[tail++] = e->destination;
          } else if (colors[e->destination] == colors[v]) {
            bipartite = false;
          }
      }
    }
  free(colors);
  free(queue);
  return bipartite;
}

bool isUndirected(const Graph *graph) {
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      if (!hasWeightedEdge(graph, e->destination, v, e->weight))
        return false;
  return true;
}

bool isMultiGraph(const Graph *graph) {
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
  for (unsigned v = 0; v < graph->size; v++)
    if (outDegree(graph, v) == graph->size - 1)
      count++;
    else if (outDegree(graph, v) != 1)
      return false;
  if (graph->size == 2)
    return count == 2;
  return count == 1;
}

bool isWheel(const Graph *graph) {
  if (graph->size < 4) return false;
  if (!isUndirected(graph)) return false;
  unsigned hub;
  unsigned hubCount = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (outDegree(graph, v) == graph->size - 1) {
      hub = v;
      hubCount++;
    } else if (outDegree(graph, v) != 3) {
      return false;
    }
  if (graph->size == 4 && hubCount != 4) return false;
  if (graph->size > 4 && hubCount != 1) return false;
  unsigned visitedCount = 0;
  unsigned previous = hub;
  unsigned current = hub;
  do {
    visitedCount++;
    unsigned next = UINT_MAX;
    for (Edge *e = graph->edges[current]; e != NULL; e = e->next)
      if (e->destination != hub && e->destination != previous) {
        next = e->destination;
        break;
      }
    previous = current;
    current = next;
  } while (current != UINT_MAX);
  return visitedCount == graph->size;
}

bool hasIsolatedVertices(const Graph *graph) {
  for (unsigned v = 0; v < graph->size; v++)
    if (isIsolated(graph, v))
      return true;
  return false;
}

bool isCyclicDirectedComponent(const Graph *graph, unsigned vertex, char *visited) {
  visited[vertex] = 1;
  for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next)
    if (visited[e->destination] == 1 || (visited[e->destination] == 0 && isCyclicDirectedComponent(graph, e->destination, visited)))
      return true;
  visited[vertex] = 2;
  return false;
}

bool isCyclicDirected(const Graph *graph) {
  char *visited = calloc(graph->size, sizeof(char));
  bool cyclic = false;
  for (unsigned v = 0; v < graph->size; v++)
    if (visited[v] == 0 && isCyclicDirectedComponent(graph, v, visited)) {
      cyclic = true;
      break;
    }
  free(visited);
  return cyclic;
}

bool isCyclicUndirectedComponent(const Graph *graph, unsigned vertex, unsigned parent, bool *visited) {
  visited[vertex] = true;
  for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next)
    if (visited[e->destination]) {
      if (e->destination != parent)
        return true;
    } else {
      if (isCyclicUndirectedComponent(graph, e->destination, vertex, visited))
        return true;
    }
  return false;
}

bool isCyclicUndirected(const Graph *graph) {
  bool *visited = calloc(graph->size, sizeof(bool));
  bool cyclic = false;
  for (unsigned v = 0; v < graph->size; v++)
    if (!visited[v])
      if (isCyclicUndirectedComponent(graph, v, graph->size, visited)) {
        cyclic = true;
        break;
      }
  free(visited);
  return cyclic;
}

bool isKRegular(const Graph *graph, unsigned k) {
  for (unsigned v = 0; v < graph->size; v++)
    if (outDegree(graph, v) != k)
      return false;
  return true;
}

bool hasConstantWeights(const Graph *graph, int weight) {
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      if (e->weight != weight)
        return false;
  return true;
}

bool isDense(const Graph *graph, double threshold) {
  return countEdges(graph) >= threshold * graph->size * (graph->size - 1);
}

bool isIsolated(const Graph *graph, unsigned vertex) {
  return outDegree(graph, vertex) == 0 && inDegree(graph, vertex) == 0;
}

bool isSource(const Graph *graph, unsigned vertex) {
  return inDegree(graph, vertex) == 0 && outDegree(graph, vertex) > 0;
}

bool isSink(const Graph *graph, unsigned vertex) {
  return outDegree(graph, vertex) == 0 && inDegree(graph, vertex) > 0;
}

bool isUniversalSource(const Graph *graph, unsigned vertex) {
  return inDegree(graph, vertex) == 0 && outDegree(graph, vertex) == graph->size - 1;
}

bool isUniversalSink(const Graph *graph, unsigned vertex) {
  return outDegree(graph, vertex) == 0 && inDegree(graph, vertex) == graph->size - 1;
}

bool isDirectedLeaf(const Graph *graph, unsigned vertex) {
  return outDegree(graph, vertex) + inDegree(graph, vertex) == 1;
}

bool isUndirectedLeaf(const Graph *graph, unsigned vertex) {
  return outDegree(graph, vertex) == 1;
}

bool hasSelfLoopsAtVertex(const Graph *graph, unsigned vertex) {
  for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next)
    if (e->destination == vertex)
      return true;
  return false;
}

bool allAreReachableFromVertexInGraph(const Graph *graph, unsigned vertex) {
  assert(isValid(graph));
  assert(vertex < graph->size);
  unsigned *distances = unweightedDijkstra(graph, vertex);
  bool b = true;
  for (unsigned v = 0; v < graph->size && b; v++)
    b = b && distances[v] < UINT_MAX;
  free(distances);
  return b;
}

bool isAdjacent(const Graph *graph, unsigned u, unsigned v) {
  for (Edge *e = graph->edges[u]; e != NULL; e = e->next)
    if (e->destination == v)
      return true;
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
  while (head < tail && !b) {
    unsigned u = queue[head++];
    for (Edge *e = graph->edges[u]; e != NULL; e = e->next) {
      if (e->destination == target) {
        b = true;
        break;
      }
      if (!visited[e->destination]) {
        visited[e->destination] = true;
        queue[tail++] = e->destination;
      }
    }
  }
  free(visited);
  free(queue);
  return b;
}

bool hasWeightedEdge(const Graph *graph, unsigned u, unsigned v, int weight) {
  for (Edge *e = graph->edges[u]; e != NULL; e = e->next)
    if (e->destination == v && e->weight == weight)
      return true;
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
      for (Edge *e = graph->edges[u]; e != NULL; e = e->next)
        if (!subset[e->destination])
          return false;
  return true;
}

bool isWalk(const Graph *graph, const unsigned *sequence, unsigned length) {
  bool b = true;
  for (unsigned i = 1; i < length && b; i++)
    b = b && isAdjacent(graph, sequence[i - 1], sequence[i]);
  return b;
}

bool isPath(const Graph *graph, const unsigned *sequence, unsigned length) {
  bool b = isWalk(graph, sequence, length);
  bool *visited = calloc(graph->size, sizeof(bool));
  for (unsigned i = 0; i < length && b; i++) {
    b = b && !visited[sequence[i]];
    visited[sequence[i]] = true;
  }
  free(visited);
  return b;
}

bool isHamiltonianPath(const Graph *graph, const unsigned *sequence, unsigned length) {
  return length == graph->size && isPath(graph, sequence, length);
}

bool isTrail(const Graph *graph, const unsigned *sequence, unsigned length) {
  bool b = isWalk(graph, sequence, length);
  unsigned count = countEdges(graph);
  FlatEdge *edges = getEdgeArrayFromDirectedGraph(graph);
  bool *used = calloc(count, sizeof(bool));
  for (unsigned i = 1; i < length && b; i++) {
    unsigned index = UINT_MAX;
    for (unsigned j = 0; j < count && index == UINT_MAX; j++)
      if ((edges[j].u == sequence[i - 1] && edges[j].v == sequence[i]) || (edges[j].v == sequence[i - 1] && edges[j].u == sequence[i]))
        if (!used[j])
          index = j;
    if (index == UINT_MAX)
      b = false;
    else
      used[index] = true;
  }
  free(edges);
  free(used);
  return b;
}

bool isDirectedCycle(const Graph *graph, const unsigned *sequence, unsigned length) {
  bool b = length >= 2 && sequence[0] == sequence[length - 1];
  for (unsigned i = 1; i < length && b; i++)
    b = b && isAdjacent(graph, sequence[i - 1], sequence[i]);
  return b;
}

bool isSimpleCycle(const Graph *graph, const unsigned *sequence, unsigned length) {
  bool b = length >= 4 && isDirectedCycle(graph, sequence, length);
  bool *visited = calloc(graph->size, sizeof(bool));
  for (unsigned i = 1; i < length && b; i++) {
    b = b && !visited[sequence[i]];
    visited[sequence[i]] = true;
  }
  free(visited);
  return b;
}

bool isHamiltonianCycle(const Graph *graph, const unsigned *sequence, unsigned length) {
  return length == graph->size + 1 && isSimpleCycle(graph, sequence, length);
}

bool isCircuit(const Graph *graph, const unsigned *sequence, unsigned length) {
  bool b = isDirectedCycle(graph, sequence, length);
  unsigned count = countEdges(graph);
  FlatEdge *edges = getEdgeArrayFromDirectedGraph(graph);
  bool *used = calloc(count, sizeof(bool));
  for (unsigned i = 1; i < length && b; i++) {
    unsigned index = UINT_MAX;
    for (unsigned j = 0; j < count && index == UINT_MAX; j++)
      if ((edges[j].u == sequence[i - 1] && edges[j].v == sequence[i]) || (edges[j].v == sequence[i - 1] && edges[j].u == sequence[i]))
        if (!used[j])
          index = j;
    if (index == UINT_MAX)
      b = false;
    else
      used[index] = true;
  }
  free(edges);
  free(used);
  return b;
}

bool isSubGraph(const Graph *sub, const Graph *main) {
  bool b = sub->size <= main->size;
  bool *edges = calloc(main->size, sizeof(bool));
  int *weights = malloc(main->size * sizeof(int));
  for (unsigned v = 0; v < sub->size && b; v++) {
    for (Edge *e = main->edges[v]; e != NULL; e = e->next) {
      edges[e->destination] = true;
      weights[e->destination] = e->weight;
    }
    for (Edge *e = sub->edges[v]; e != NULL && b; e = e->next)
      b = b && edges[e->destination] && e->weight == weights[e->destination];
    for (Edge *e = main->edges[v]; e != NULL; e = e->next)
      edges[e->destination] = false;
  }
  free(weights);
  free(edges);
  return b;
}



bool *graphCenter(const Graph *graph) {
  int *eccentricity = malloc(graph->size * sizeof(int));
  int radius = INT_MAX;
  for (unsigned v = 0; v < graph->size; v++) {
    eccentricity[v] = graphEccentricity(graph, v);
    if (eccentricity[v] < radius)
      radius = eccentricity[v];
  }
  bool *center = calloc(graph->size, sizeof(bool));
  for (unsigned v = 0; v < graph->size; v++)
    if (eccentricity[v] == radius)
      center[v] = true;
  free(eccentricity);
  return center;
}

bool *graphPeriphery(const Graph *graph) {
  int *eccentricity = malloc(graph->size * sizeof(int));
  int diameter = INT_MIN;
  for (unsigned v = 0; v < graph->size; v++) {
    eccentricity[v] = graphEccentricity(graph, v);
    if (eccentricity[v] > diameter)
      diameter = eccentricity[v];
  }
  bool *periphery = calloc(graph->size, sizeof(bool));
  for (unsigned v = 0; v < graph->size; v++)
    if (eccentricity[v] == diameter)
      periphery[v] = true;
  free(eccentricity);
  return periphery;
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
  for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next)
    if (!visited[e->destination]) {
      children++;
      parent[e->destination] = vertex;
      recursivelyFindArticulationPoints(graph, e->destination, visited, discovery, low, parent, articulations, timer);
      low[vertex] = minimumUnsigned(low[vertex], low[e->destination]);
      if ((parent[vertex] == UINT_MAX && children >= 2) || (parent[vertex] != UINT_MAX && low[e->destination] >= discovery[vertex]))
        articulations[vertex] = true;
    } else if (e->destination != parent[vertex]) {
      low[vertex] = minimumUnsigned(low[vertex], discovery[e->destination]);
    }
}

bool *findArticulationPoints(const Graph *graph) {
  bool *visited = calloc(graph->size, sizeof(bool));
  unsigned *discovery = calloc(graph->size, sizeof(unsigned));
  unsigned *low = calloc(graph->size, sizeof(unsigned));
  unsigned *parent = malloc(graph->size * sizeof(unsigned));
  bool *articulations = calloc(graph->size, sizeof(bool));
  unsigned timer = 0;
  for (unsigned v = 0; v < graph->size; v++)
    parent[v] = UINT_MAX;
  for (unsigned v = 0; v < graph->size; v++)
    if (!visited[v])
      recursivelyFindArticulationPoints(graph, v, visited, discovery, low, parent, articulations, &timer);
  free(visited);
  free(discovery);
  free(low);
  free(parent);
  return articulations;
}



Graph *createGraph(unsigned size) {
  Graph *g = malloc(sizeof(Graph));
  g->size = size;
  g->inDegree = calloc(size, sizeof(unsigned));
  g->outDegree = calloc(size, sizeof(unsigned));
  g->edges = calloc(size, sizeof(Edge *));
  return g;
}

Graph *createPathGraph(unsigned size) {
  Graph *g = createGraph(size);
  for (unsigned v = 1; v < size; v++)
    addUndirectedEdge(g, v - 1, v, 1);
  return g;
}

Graph *createCycleGraph(unsigned size) {
  Graph *g = createGraph(size);
  for (unsigned v = 0; v < size; v++)
    addUndirectedEdge(g, v, (v + 1) % size, 1);
  return g;
}

Graph *createCompleteGraph(unsigned size) {
  Graph *g = createGraph(size);
  for (unsigned u = 0; u < size; u++)
    for (unsigned v = 0; v < size; v++)
      if (u != v)
        addDirectedEdge(g, u, v, 1);
  return g;
}

Graph *copyGraph(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      addDirectedEdge(g, v, e->destination, e->weight);
  return g;
}

Graph *copyTranspose(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      addDirectedEdge(g, e->destination, v, e->weight);
  return g;
}

Graph *copyUnweighted(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      addDirectedEdge(g, v, e->destination, 1);
  return g;
}

Graph *copyUndirected(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      addUndirectedEdge(g, v, e->destination, e->weight);
  return g;
}

Graph *copyComplement(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned u = 0; u < graph->size; u++)
    for (unsigned v = 0; v < graph->size; v++)
      if (u != v && !isAdjacent(graph, u, v))
        addDirectedEdge(g, u, v, 1);
  return g;
}

Graph *lineGraph(const Graph *graph) {
  unsigned count = countEdges(graph) / 2;
  FlatEdge *edges = getEdgeArrayFromUndirectedGraph(graph);
  Graph *g = createGraph(count);
  for (unsigned i = 0; i < count; i++)
    for (unsigned j = i + 1; j < count; j++)
      if (edges[i].u == edges[j].u || edges[i].u == edges[j].v || edges[i].v == edges[j].u || edges[i].v == edges[j].v)
        addUndirectedEdge(g, i, j, 1);
  free(edges);
  return g;
}

Graph *underlyingGraph(const Graph *graph) {
  bool *seen = calloc(graph->size, sizeof(bool));
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++) {
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      if (v != e->destination && !seen[e->destination]) {
        addUndirectedEdge(g, v, e->destination, 1);
        seen[e->destination] = true;
      }
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      seen[e->destination] = false;
  }
  free(seen);
  return g;
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
      addUndirectedEdge(mst, edges[i].u, edges[i].v, edges[i].weight);
    }
  free(edges);
  freeDsu(dsu);
  return mst;
}

Graph *removeVertex(const Graph *graph, unsigned vertex) {
  Graph *g = createGraph(graph->size - 1);
  for (unsigned u = 0; u < graph->size; u++)
    if (u != vertex) {
      unsigned v = (u > vertex) ? u - 1 : u;
      for (Edge *e = graph->edges[u]; e != NULL; e = e->next)
        if (e->destination != vertex) {
          unsigned w = (e->destination > vertex) ? e->destination - 1 : e->destination;
          addDirectedEdge(g, v, w, e->weight);
        }
    }
  return g;
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
  Graph *g = createGraph(graph->size);
  while (heap->size > 0) {
    unsigned vertex = getMinimumFromHeap(heap);
    removeMinimumFromHeap(heap);
    if (!visited[vertex]) {
      visited[vertex] = true;
      if (parents[vertex] != UINT_MAX)
        addUndirectedEdge(g, parents[vertex], vertex, weights[vertex]);
      for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next)
        if (!visited[e->destination] && e->weight < weights[e->destination]) {
          parents[e->destination] = vertex;
          weights[e->destination] = e->weight;
          insertInHeap(heap, e->destination, e->weight);
        }
    }
  }
  freeHeap(heap);
  return g;
}

Graph *mergeVertices(const Graph *graph, unsigned u, unsigned v) {
  Graph *g = createGraph(graph->size);
  for (unsigned x = 0; x < graph->size; x++) {
    unsigned y = x > v ? x - 1 : x == v ? u : x;
    for (Edge *e = graph->edges[x]; e != NULL; e = e->next) {
      unsigned z = e->destination > v ? e->destination - 1 : e->destination == v ? u : e->destination;
      addDirectedEdge(g, y, z, e->weight);
    }
  }
  return g;
}

Graph *copySubgraph(const Graph *graph, const bool *vertices) {
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    if (vertices[v])
      for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
        if (vertices[e->destination])
          addDirectedEdge(g, v, e->destination, e->weight);
  return g;
}

Graph *graphUnion(const Graph *g1, const Graph *g2) {
  Graph *g3 = createGraph(maximumUnsigned(g1->size, g2->size));
  for (unsigned v = 0; v < g1->size; v++)
    for (Edge *e = g1->edges[v]; e; e = e->next)
      addDirectedEdge(g3, v, e->destination, e->weight);
  for (unsigned v = 0; v < g2->size; v++)
    for (Edge *e = g2->edges[v]; e; e = e->next)
      if (!isAdjacent(g3, v, e->destination))
        addDirectedEdge(g3, v, e->destination, e->weight);
  return g3;
}

Graph *createDirectedGraphFromEdgeArray(unsigned size, const FlatEdge *edges, unsigned count) {
  Graph *g = createGraph(size);
  for (unsigned i = 0; i < count; i++)
    addDirectedEdge(g, edges[i].u, edges[i].v, edges[i].weight);
  return g;
}

Graph *createUndirectedGraphFromEdgeArray(unsigned size, const FlatEdge *edges, unsigned count) {
  Graph *g = createGraph(size);
  for (unsigned i = 0; i < count; i++)
    addUndirectedEdge(g, edges[i].u, edges[i].v, edges[i].weight);
  return g;
}



void printGraph(const Graph *graph) {
  printf("{");
  unsigned i = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next) {
      if (i++ > 0)
        printf(", ");
      printf("(%u, %u, %d)", v, e->destination, e->weight);
    }
  printf("}\n");
}

void destroyGraph(Graph *graph) {
  for (unsigned v = 0; v < graph->size; v++) {
    Edge *e = graph->edges[v];
    while (e != NULL) {
      Edge *next = e->next;
      free(e);
      e = next;
    }
  }
  free(graph->inDegree);
  free(graph->outDegree);
  free(graph->edges);
  free(graph);
}

void removeDirectedEdge(Graph *graph, unsigned source, unsigned destination) {
  Edge **e = &graph->edges[source];
  while (*e != NULL)
    if ((*e)->destination == destination) {
      Edge *temporary = *e;
      *e = (*e)->next;
      free(temporary);
      graph->inDegree[destination]--;
      graph->outDegree[source]--;
    } else {
      e = &(*e)->next;
    }
}

void removeUndirectedEdge(Graph *graph, unsigned u, unsigned v) {
  removeDirectedEdge(graph, u, v);
  removeDirectedEdge(graph, v, u);
}

void addDirectedEdge(Graph *graph, unsigned source, unsigned destination, int weight) {
  Edge *e = malloc(sizeof(Edge));
  e->destination = destination;
  e->weight = weight;
  e->next = graph->edges[source];
  graph->edges[source] = e;
  graph->inDegree[destination]++;
  graph->outDegree[source]++;
}

void addUndirectedEdge(Graph *graph, unsigned u, unsigned v, int weight) {
  addDirectedEdge(graph, u, v, weight);
  addDirectedEdge(graph, v, u, weight);
}



unsigned countEdges(const Graph *graph) {
  unsigned count = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      count++;
  return count;
}

unsigned countSelfLoops(const Graph *graph) {
  unsigned count = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      if (e->destination == v)
        count++;
  return count;
}

unsigned countTriangles(const Graph *graph) {
  unsigned count = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *d = graph->edges[v]; d != NULL; d = d->next)
      if (d->destination != v)
        for (Edge *e = graph->edges[d->destination]; e != NULL; e = e->next)
          if (e->destination != v && e->destination != d->destination)
            for (Edge *f = graph->edges[e->destination]; f != NULL; f = f->next)
              if (f->destination == v)
                count++;
  return count;
}

unsigned minDegree(const Graph *graph) {
  unsigned minimum = UINT_MAX;
  for (unsigned v = 0; v < graph->size; v++)
    if (inDegree(graph, v) + outDegree(graph, v) < minimum)
      minimum = inDegree(graph, v) + outDegree(graph, v);
  return minimum;
}

unsigned maxDegree(const Graph *graph) {
  unsigned maximum = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (inDegree(graph, v) + outDegree(graph, v) > maximum)
      maximum = inDegree(graph, v) + outDegree(graph, v);
  return maximum;
}

unsigned countDirectedLeaves(const Graph *graph) {
  unsigned n = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (inDegree(graph, v) + outDegree(graph, v) == 1)
      n++;
  return n;
}

unsigned countUndirectedLeaves(const Graph *graph) {
  unsigned n = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (outDegree(graph, v) == 1)
      n++;
  return n;
}

unsigned countSources(const Graph *graph) {
  unsigned n = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (isSource(graph, v))
      n++;
  return n;
}

unsigned countSinks(const Graph *graph) {
  unsigned n = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (isSink(graph, v))
      n++;
  return n;
}

unsigned countParallelEdges(const Graph *graph) {
  bool *seen = calloc(graph->size, sizeof(bool));
  unsigned count = 0;
  for (unsigned v = 0; v < graph->size; v++) {
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      if (seen[e->destination])
        count++;
      else
        seen[e->destination] = true;
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      seen[e->destination] = false;
  }
  free(seen);
  return count;
}

unsigned countIsolatedVertices(const Graph *graph) {
  unsigned n = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (isIsolated(graph, v))
      n++;
  return n;
}

void traverseComponent(const Graph *graph, unsigned vertex, bool *visited) {
  visited[vertex] = true;
  for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next)
    if (!visited[e->destination])
      traverseComponent(graph, e->destination, visited);
}

unsigned countComponents(const Graph *graph) {
  bool *visited = calloc(graph->size, sizeof(bool));
  unsigned n = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (!visited[v]) {
      n++;
      traverseComponent(graph, v, visited);
    }
  free(visited);
  return n;
}

unsigned outDegree(const Graph *graph, unsigned vertex) {
  return graph->outDegree[vertex];
}

unsigned inDegree(const Graph *graph, unsigned vertex) {
  return graph->inDegree[vertex];
}

unsigned countCommonNeighbors(const Graph *graph, unsigned u, unsigned v) {
  bool *neighbors = calloc(graph->size, sizeof(bool));
  for (Edge *e = graph->edges[u]; e != NULL; e = e->next)
    neighbors[e->destination] = true;
  unsigned n = 0;
  for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
    if (neighbors[e->destination]) {
      n++;
      neighbors[e->destination] = false; 
    }
  free(neighbors);
  return n;
}



unsigned *inDegreeDistribution(const Graph *graph) {
  unsigned *distribution = calloc(graph->size, sizeof(unsigned));
  for (unsigned v = 0; v < graph->size; v++)
    distribution[inDegree(graph, v)]++;
  return distribution;
}

unsigned *outDegreeDistribution(const Graph *graph) {
  unsigned *distribution = calloc(graph->size, sizeof(unsigned));
  for (unsigned v = 0; v < graph->size; v++)
    distribution[outDegree(graph, v)]++;
  return distribution;
}

unsigned *coloring(const Graph *graph) {
  unsigned *colors = malloc(graph->size  *sizeof(unsigned));
  colors[0] = 0;
  for (unsigned v = 1; v < graph->size; v++)
    colors[v] = UINT_MAX;
  bool *taken = calloc(graph->size, sizeof(bool));
  for (unsigned v = 1; v < graph->size; v++) {
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      if (colors[e->destination] < UINT_MAX)
        taken[colors[e->destination]] = true;
    unsigned color = 0;
    while (taken[color])
      color++;
    colors[v] = color;
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      if (colors[e->destination] < UINT_MAX)
        taken[colors[e->destination]] = false;
  }
  free(taken);
  return colors;
}

void topologicalSortOfGraphComponent(const Graph *graph, unsigned vertex, unsigned *ordering, unsigned *index, bool *visited) {
  visited[vertex] = true;
  for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next)
    if (!visited[e->destination])
      topologicalSortOfGraphComponent(graph, e->destination, ordering, index, visited);
  ordering[--(*index)] = vertex;
}

unsigned *topologicalSortOfGraph(const Graph *graph) {
  unsigned *ordering = malloc(graph->size * sizeof(unsigned));
  unsigned index = graph->size;
  bool *visited = calloc(graph->size, sizeof(bool));
  for (unsigned v = 0; v < graph->size; v++)
    if (!visited[v])
      topologicalSortOfGraphComponent(graph, v, ordering, &index, visited);
  free(visited);
  return ordering;
}

unsigned *getNeighbors(const Graph *graph, unsigned vertex) {
  unsigned *neighbors = malloc((outDegree(graph, vertex) + 1) * sizeof(unsigned));
  unsigned i = 0;
  for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next)
    neighbors[i++] = e->destination;
  neighbors[i] = UINT_MAX;
  return neighbors;
}

unsigned *unweightedDijkstra(const Graph *graph, unsigned source) {
  unsigned *distances = malloc(graph->size * sizeof(unsigned));
  distances[source] = 0;
  for (unsigned v = 1; v < graph->size; v++)
    distances[v] = INT_MAX;
  unsigned *queue = malloc(graph->size * sizeof(unsigned));
  unsigned head = 0, tail = 0;
  queue[tail++] = source;
  while (head < tail) {
    unsigned u = queue[head++];
    for (Edge *e = graph->edges[u]; e != NULL; e = e->next)
      if (distances[e->destination] == INT_MAX) {
        distances[e->destination] = distances[u] + 1;
        queue[tail++] = e->destination;
      }
  }
  free(queue);
  return distances;
}

void depthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *count, bool *visited) {
  ordering[(*count)++] = source;
  visited[source] = true;
  for (Edge *e = graph->edges[source]; e != NULL; e = e->next)
    if (!visited[e->destination])
      depthFirstSortOfGraphComponent(graph, e->destination, ordering, count, visited);
}

unsigned *depthFirstSortOfGraph(const Graph *graph, unsigned source) {
  unsigned *ordering = malloc(graph->size * sizeof(unsigned));
  unsigned count = 0;
  bool *visited = calloc(graph->size, sizeof(bool));
  depthFirstSortOfGraphComponent(graph, source, ordering, &count, visited);
  for (unsigned v = 0; v < graph->size; v++)
    if (!visited[v])
      depthFirstSortOfGraphComponent(graph, v, ordering, &count, visited);
  free(visited);
  return ordering;
}

void breadthFirstSortOfGraphComponent(const Graph *graph, unsigned source, unsigned *ordering, unsigned *count, bool *visited) {
  unsigned *queue = malloc(graph->size * sizeof(unsigned));
  unsigned head = 0, tail = 0;
  visited[source] = true;
  queue[tail++] = source;
  while (head < tail) {
    unsigned current = queue[head++];
    ordering[(*count)++] = current;
    for (Edge *e = graph->edges[current]; e != NULL; e = e->next)
      if (!visited[e->destination]) {
        visited[e->destination] = true;
        queue[tail++] = e->destination;
      }
  }
  free(queue);
}

unsigned *breadthFirstSortOfGraph(const Graph *graph, unsigned source) {
  unsigned *ordering = malloc(graph->size * sizeof(unsigned));
  unsigned count = 0;
  bool *visited = calloc(graph->size, sizeof(bool));
  breadthFirstSortOfGraphComponent(graph, source, ordering, &count, visited);
  for (unsigned v = 0; v < graph->size; v++)
    if (!visited[v])
      breadthFirstSortOfGraphComponent(graph, v, ordering, &count, visited);
  free(visited);
  return ordering;
}



int sumWeights(const Graph *graph) {
  int weight = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      weight += e->weight;
  return weight;
}

int graphRadius(const Graph *graph) {
  int radius = INT_MAX;
  for (unsigned v = 0; v < graph->size; v++) {
    int eccentricity = graphEccentricity(graph, v);
    if (eccentricity < radius)
      radius = eccentricity;
  }
  return radius;
}

int graphDiameter(const Graph *graph) {
  int diameter = INT_MIN;
  for (unsigned v = 0; v < graph->size; v++) {
    int eccentricity = graphEccentricity(graph, v);
    if (eccentricity > diameter)
      diameter = eccentricity;
  }
  return diameter;
}

int graphEccentricity(const Graph *graph, unsigned vertex) {
  int *distance = weightedDijkstra(graph, vertex);
  int eccentricity = INT_MIN;
  for (unsigned v = 0; v < graph->size; v++)
    if (distance[v] > eccentricity)
      eccentricity = distance[v];
  free(distance);
  return eccentricity;
}

int edgeWeight(const Graph *graph, unsigned u, unsigned v) {
  for (Edge *e = graph->edges[u]; e != NULL; e = e->next)
    if (e->destination == v)
      return e->weight;
  return -1; 
}

int pathWeight(const Graph *graph, const unsigned *path, unsigned length) {
  int weight = 0;
  for (unsigned i = 1; i < length; i++)
    weight += edgeWeight(graph, path[i - 1], path[i]);
  return weight;
}



int *bellmanFord(const Graph *graph, unsigned source) {
  int *distance = malloc(graph->size * sizeof(int));
  for (unsigned v = 0; v < graph->size; v++)
    distance[v] = INT_MAX;
  distance[source] = 0;
  for (unsigned i = 1; i < graph->size; i++)
    for (unsigned v = 0; v < graph->size; v++)
      if (distance[v] < INT_MAX)
        for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
          if (distance[v] + e->weight < distance[e->destination])
            distance[e->destination] = distance[v] + e->weight;
  for (unsigned v = 0; v < graph->size && distance != NULL; v++)
    if (distance[v] < INT_MAX)
      for (Edge *e = graph->edges[v]; e != NULL && distance != NULL; e = e->next)
        if (distance[v] + e->weight < distance[e->destination]) {
          free(distance);
          distance = NULL;
        }
  return distance;
}

int *weightedDijkstra(const Graph *graph, unsigned source) {
  int *weights = malloc(graph->size * sizeof(int));
  weights[source] = 0;
  for (unsigned v = 1; v < graph->size; v++)
    weights[v] = INT_MAX;
  Heap *heap = createHeap();
  insertInHeap(heap, source, 0);
  while (heap->size > 0) {
    unsigned v = getMinimumFromHeap(heap);
    removeMinimumFromHeap(heap);
    for (Edge *e = graph->edges[v]; e; e = e->next)
      if (weights[v] + e->weight < weights[e->destination]) {
        weights[e->destination] = weights[v] + e->weight;
        insertInHeap(heap, e->destination, weights[e->destination]);
      }
  }
  freeHeap(heap);
  return weights;
}



int **toMatrix(const Graph *graph) {
  int **matrix = malloc(graph->size * sizeof(int *));
  for (unsigned v = 0; v < graph->size; v++)
    matrix[v] = calloc(graph->size, sizeof(int));
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      matrix[v][e->destination] = e->weight;
  return matrix;
}

int **floydWarshall(const Graph *graph) {
  int **distance = malloc(graph->size * sizeof(int *));
  for (unsigned i = 0; i < graph->size; i++) {
    distance[i] = malloc(graph->size * sizeof(int));
    for (unsigned j = 0; j < graph->size; j++)
      if (i == j)
        distance[i][j] = 0;
      else
        distance[i][j] = INT_MAX;
  }
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      if (e->weight < distance[v][e->destination])
        distance[v][e->destination] = e->weight;
  for (unsigned k = 0; k < graph->size; k++)
    for (unsigned i = 0; i < graph->size; i++)
      for (unsigned j = 0; j < graph->size; j++)
        if (distance[i][k] < INT_MAX && distance[k][j] < INT_MAX && distance[i][k] + distance[k][j] < distance[i][j])
          distance[i][j] = distance[i][k] + distance[k][j];
  return distance;
}



double density(const Graph *graph) {
  if (graph->size < 2)
    return 0;
  return (double)countEdges(graph) / (graph->size * (graph->size - 1));
}

double averageClusteringCoefficient(const Graph *graph) {
  if (graph->size == 0)
    return 0;
  double total = 0;
  for (unsigned v = 0; v < graph->size; v++)
    total += localClusteringCoefficient(graph, v);
  return total / graph->size;
}

double normalizedDegree(const Graph *graph, unsigned vertex) {
  if (graph->size < 2)
    return 0;
  return (double)outDegree(graph, vertex) / (graph->size - 1);
}

double localClusteringCoefficient(const Graph *graph, unsigned vertex) {
  if (outDegree(graph, vertex) < 2)
    return 0;
  unsigned edgesBetweenNeighbours = 0;
  for (Edge *d = graph->edges[vertex]; d != NULL; d = d->next)
    for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next)
      if (isAdjacent(graph, d->destination, e->destination))
        edgesBetweenNeighbours++;
  return (double)edgesBetweenNeighbours / (outDegree(graph, vertex) * (outDegree(graph, vertex) - 1));
}

double subgraphDensity(const Graph *graph, const bool *subset) {
  unsigned vertices = 0;
  unsigned edges = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (subset[v]) {
      vertices++;
      for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
        if (subset[e->destination])
          edges++;
    }
  if (vertices < 2)
    return 0;
  return (double)edges / (vertices * (vertices - 1));
}



FlatEdge *getEdgeArrayFromDirectedGraph(const Graph *graph) {
  FlatEdge *edges = malloc(countEdges(graph) * sizeof(FlatEdge));
  unsigned i = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next) {
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
    for (Edge *e = graph->edges[v]; e != NULL; e = e->next)
      if (v < e->destination) {
        edges[i].u = v;
        edges[i].v = e->destination;
        edges[i].weight = e->weight;
        i++;
      }
  return edges;
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
  for (Edge *e = graph->edges[vertex]; e != NULL; e = e->next)
    if (!visited[e->destination]) {
      parent[e->destination] = vertex;
      recursivelyFindBridges(graph, e->destination, visited, discovery, low, parent, bridges, count, timer);
      low[vertex] = minimumUnsigned(low[vertex], low[e->destination]);
      if (low[e->destination] > discovery[vertex]) {
        bridges[*count] = malloc(2 * sizeof(unsigned));
        bridges[*count][0] = vertex;
        bridges[*count][1] = e->destination;
        (*count)++;
      }
    } else if (e->destination != parent[vertex]) {
      low[vertex] = minimumUnsigned(low[vertex], discovery[e->destination]);
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
  addDirectedEdge(g1, 0, 1, 1);
  addDirectedEdge(g1, 1, 2, 1);
  addDirectedEdge(g1, 2, 0, 1);
  assert(isCyclicDirected(g1) == true);
  printf("Directed cyclic test 1 passed: Simple cycle found.\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addDirectedEdge(g2, 0, 1, 1);
  addDirectedEdge(g2, 1, 2, 1);
  addDirectedEdge(g2, 0, 2, 1);
  assert(isCyclicDirected(g2) == false);
  printf("Directed cyclic test 2 passed: DAG correctly identified as acyclic.\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(1);
  addDirectedEdge(g3, 0, 0, 1);
  assert(isCyclicDirected(g3) == true);
  printf("Directed cyclic test 3 passed: Self-loop detected.\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(4);
  addDirectedEdge(g4, 0, 1, 1);
  addDirectedEdge(g4, 2, 3, 1);
  addDirectedEdge(g4, 3, 2, 1);
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
  addUndirectedEdge(g1, 0, 1, 1);
  addUndirectedEdge(g1, 1, 2, 1);
  if (!isCyclicUndirected(g1))
    printf("Undirected Test 1 passed: Tree is acyclic.\n");
  else
    printf("Undirected Test 1 failed: False positive in tree.\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addUndirectedEdge(g2, 0, 1, 1);
  addUndirectedEdge(g2, 1, 2, 1);
  addUndirectedEdge(g2, 2, 0, 1);
  if (isCyclicUndirected(g2))
    printf("Undirected Test 2 passed: Triangle cycle detected.\n");
  else
    printf("Undirected Test 2 failed: Triangle cycle missed.\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(5);
  addUndirectedEdge(g3, 0, 1, 1);
  addUndirectedEdge(g3, 2, 3, 1);
  addUndirectedEdge(g3, 3, 4, 1);
  addUndirectedEdge(g3, 4, 2, 1);
  if (isCyclicUndirected(g3))
    printf("Undirected Test 3 passed: Cycle in disconnected component detected.\n");
  else
    printf("Undirected Test 3 failed: Missed cycle in disconnected component.\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(2);
  addUndirectedEdge(g4, 0, 1, 1);
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
  addUndirectedEdge(g2, 0, 1, 1);
  addUndirectedEdge(g2, 1, 2, 1);
  assert(isConnectedUndirected(g2) == true);
  printf("Undirected connected test 2 passed: Simple line graph\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(4);
  addUndirectedEdge(g3, 0, 1, 1);
  addUndirectedEdge(g3, 2, 3, 1);
  assert(isConnectedUndirected(g3) == false);
  printf("Undirected connected test 3 passed: Disconnected components\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(3);
  addUndirectedEdge(g4, 0, 1, 1);
  assert(isConnectedUndirected(g4) == false);
  printf("Undirected connected test 4 passed: Isolated vertex\n");
  destroyGraph(g4);

  Graph *g5 = createGraph(4);
  addUndirectedEdge(g5, 0, 1, 1);
  addUndirectedEdge(g5, 0, 2, 1);
  addUndirectedEdge(g5, 0, 3, 1);
  addUndirectedEdge(g5, 1, 2, 1);
  addUndirectedEdge(g5, 1, 3, 1);
  addUndirectedEdge(g5, 2, 3, 1);
  assert(isConnectedUndirected(g5) == true);
  printf("Undirected connected test 5 passed: Complete graph\n");
  destroyGraph(g5);
}

void testIsWeaklyConnectedDirected() {
  Graph *g1 = createGraph(3);
  addDirectedEdge(g1, 0, 1, 1);
  addDirectedEdge(g1, 1, 2, 1);
  assert(isWeaklyConnectedDirected(g1) == true);
  printf("Weakly Test 1 passed: Simple chain\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addDirectedEdge(g2, 0, 1, 1);
  addDirectedEdge(g2, 2, 1, 1);
  assert(isWeaklyConnectedDirected(g2) == true);
  printf("Weakly Test 2 passed: Source/Sink structure\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(4);
  addDirectedEdge(g3, 0, 1, 1);
  addDirectedEdge(g3, 2, 3, 1);
  assert(isWeaklyConnectedDirected(g3) == false);
  printf("Weakly Test 3 passed: Truly disconnected components\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(4);
  addDirectedEdge(g4, 0, 1, 1);
  addDirectedEdge(g4, 0, 2, 1);
  addDirectedEdge(g4, 0, 3, 1);
  assert(isWeaklyConnectedDirected(g4) == true);
  printf("Weakly Test 4 passed: Star pattern\n");
  destroyGraph(g4);

  Graph *g5 = createGraph(2);
  addDirectedEdge(g5, 0, 0, 1);
  assert(isWeaklyConnectedDirected(g5) == false);
  printf("Weakly Test 5 passed: Isolated vertex with self-loop\n");
  destroyGraph(g5);
}

void testIsStronglyConnectedDirected() {
  Graph *g1 = createGraph(3);
  addDirectedEdge(g1, 0, 1, 1);
  addDirectedEdge(g1, 1, 2, 1);
  addDirectedEdge(g1, 2, 0, 1);
  assert(isStronglyConnectedDirected(g1) == true);
  printf("Strongly Test 1 passed: Simple cycle\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addDirectedEdge(g2, 0, 1, 1);
  addDirectedEdge(g2, 1, 2, 1);
  assert(isStronglyConnectedDirected(g2) == false);
  printf("Strongly Test 2 passed: Linear chain (not strong)\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(4);
  addDirectedEdge(g3, 0, 1, 1);
  addDirectedEdge(g3, 1, 0, 1);
  addDirectedEdge(g3, 2, 3, 1);
  addDirectedEdge(g3, 3, 2, 1);
  assert(isStronglyConnectedDirected(g3) == false);
  printf("Strongly Test 3 passed: Disconnected cycles\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(3);
  addDirectedEdge(g4, 0, 1, 1);
  addDirectedEdge(g4, 1, 0, 1);
  addDirectedEdge(g4, 1, 2, 1);
  addDirectedEdge(g4, 2, 1, 1);
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
  addDirectedEdge(g1, 0, 1, 1);
  addDirectedEdge(g1, 1, 2, 1);
  unsigned *order1 = depthFirstSortOfGraph(g1, 0);
  assert(order1[0] == 0);
  assert(order1[1] == 1);
  assert(order1[2] == 2);
  printf("DFS test 1 (Linear) passed!\n");
  destroyGraph(g1);
  free(order1);

  Graph *g2 = createGraph(3);
  addDirectedEdge(g2, 0, 2, 1);
  addDirectedEdge(g2, 0, 1, 1);
  unsigned *order2 = depthFirstSortOfGraph(g2, 0);
  assert(order2[0] == 0);
  assert(order2[1] == 1);
  assert(order2[2] == 2);
  printf("DFS test 2 (Fork) passed!\n");
  destroyGraph(g2);
  free(order2);

  Graph *g3 = createGraph(3);
  addDirectedEdge(g3, 0, 1, 1);
  unsigned *order3 = depthFirstSortOfGraph(g3, 0);
  assert(order3[0] == 0);
  assert(order3[1] == 1);
  assert(order3[2] == 2);
  printf("DFS test 3 (Disconnected) passed!\n");
  destroyGraph(g3);
  free(order3);

  Graph *g4 = createGraph(2);
  addDirectedEdge(g4, 0, 1, 1);
  addDirectedEdge(g4, 1, 0, 1);
  unsigned *order4 = depthFirstSortOfGraph(g4, 0);
  assert(order4[0] == 0);
  assert(order4[1] == 1);
  printf("DFS test 4 (Cyclic) passed!\n");
  destroyGraph(g4);
  free(order4);
}

void testBreadthFirstSortOfGraph() {
  Graph *g1 = createGraph(3);
  addDirectedEdge(g1, 0, 1, 1);
  addDirectedEdge(g1, 1, 2, 1);
  unsigned *order1 = breadthFirstSortOfGraph(g1, 0);
  assert(order1[0] == 0);
  assert(order1[1] == 1);
  assert(order1[2] == 2);
  printf("BFS test 1 (linear) passed!\n");
  destroyGraph(g1);
  free(order1);

  Graph *g2 = createGraph(3);
  addDirectedEdge(g2, 0, 2, 1);
  addDirectedEdge(g2, 0, 1, 1);
  unsigned *order2 = breadthFirstSortOfGraph(g2, 0);
  assert(order2[0] == 0);
  assert(order2[1] == 1);
  assert(order2[2] == 2);
  printf("BFS test 2 (fork) passed!\n");
  destroyGraph(g2);
  free(order2);

  Graph *g3 = createGraph(3);
  addDirectedEdge(g3, 0, 1, 1);
  unsigned *order3 = breadthFirstSortOfGraph(g3, 0);
  assert(order3[0] == 0);
  assert(order3[1] == 1);
  assert(order3[2] == 2);
  printf("BFS test 3 (disconnected) passed!\n");
  destroyGraph(g3);
  free(order3);

  Graph *g4 = createGraph(2);
  addDirectedEdge(g4, 0, 1, 1);
  addDirectedEdge(g4, 1, 0, 1);
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
  addDirectedEdge(g1, 0, 1, 1);
  addDirectedEdge(g1, 1, 2, 1);
  unsigned *order1 = topologicalSortOfGraph(g1);
  assert(isValidTopologicalSort(g1, order1));
  printf("Topo test 1 (linear) passed!\n");
  destroyGraph(g1);
  free(order1);

  Graph *g2 = createGraph(4);
  addDirectedEdge(g2, 0, 1, 1);
  addDirectedEdge(g2, 0, 2, 1);
  addDirectedEdge(g2, 1, 3, 1);
  addDirectedEdge(g2, 2, 3, 1);
  unsigned *order2 = topologicalSortOfGraph(g2);
  assert(isValidTopologicalSort(g2, order2));
  printf("Topo test 2 (diamond) passed!\n");
  destroyGraph(g2);
  free(order2);

  Graph *g3 = createGraph(4);
  addDirectedEdge(g3, 0, 1, 1);
  addDirectedEdge(g3, 2, 3, 1);
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
  addDirectedEdge(g, 0, 1, 5);
  addDirectedEdge(g, 1, 2, 1);
  addDirectedEdge(g, 0, 2, 10);
  addDirectedEdge(g, 2, 3, 1);
  int *d1 = bellmanFord(g, 0);
  assert(d1 != NULL);
  assert(d1[3] == 7);
  printf("Bellman-Ford test 1 (positive) passed!\n");
  free(d1);

  addDirectedEdge(g, 3, 1, -10);
  int *d2 = bellmanFord(g, 0);
  assert(d2 == NULL);
  printf("Bellman-Ford test 2 (negative cycle) passed!\n");
  destroyGraph(g);
}

void testUnweightedDijkstra() {
  printf("Running unweightedDijkstra tests...\n");

  {
    Graph *g = createGraph(3);
    addDirectedEdge(g, 0, 1, 10);
    addDirectedEdge(g, 1, 2, 20);

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
    addDirectedEdge(g, 0, 2, 100);
    addDirectedEdge(g, 0, 1, 1);
    addDirectedEdge(g, 1, 2, 1);

    unsigned *distances = unweightedDijkstra(g, 0);
    assert(distances[2] == 1);

    printf("Passed: Shortest hops selection\n");
    free(distances);
    destroyGraph(g);
  }

  {
    Graph *g = createGraph(3);
    addDirectedEdge(g, 0, 1, 1);

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
    addDirectedEdge(g, 0, 1, 1);
    addDirectedEdge(g, 1, 2, 1);
    addDirectedEdge(g, 2, 0, 1);

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
  addDirectedEdge(g1, 0, 1, 5);
  addDirectedEdge(g1, 1, 2, 10);
  int *dist1 = weightedDijkstra(g1, 0);
  assert(dist1[0] == 0);
  assert(dist1[1] == 5);
  assert(dist1[2] == 15);
  printf("Dijkstra test 1 passed: Simple path\n");
  destroyGraph(g1);
  free(dist1);

  Graph *g2 = createGraph(3);
  addDirectedEdge(g2, 0, 2, 10);
  addDirectedEdge(g2, 0, 1, 2);
  addDirectedEdge(g2, 1, 2, 3);
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
  addDirectedEdge(g4, 0, 1, 1);
  addDirectedEdge(g4, 1, 2, 1);
  addDirectedEdge(g4, 2, 0, 1);
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
    addDirectedEdge(g, 0, 1, 10);
    addDirectedEdge(g, 1, 2, 5);
    addDirectedEdge(g, 0, 2, 20);

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
    addDirectedEdge(g, 0, 1, 4);
    addDirectedEdge(g, 0, 2, 5);
    addDirectedEdge(g, 1, 2, -2);

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
    addDirectedEdge(g, 0, 1, 1);
    addDirectedEdge(g, 2, 3, 1);

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

    addDirectedEdge(g, 0, 1, 1);
    addDirectedEdge(g, 1, 2, 1);
    addDirectedEdge(g, 2, 0, -5);

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
    addUndirectedEdge(g, 0, 1, 1);
    addUndirectedEdge(g, 1, 2, 3);
    addUndirectedEdge(g, 0, 2, 4);
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
    addUndirectedEdge(g, 0, 1, 2);
    addUndirectedEdge(g, 0, 3, 6);
    addUndirectedEdge(g, 1, 2, 3);
    addUndirectedEdge(g, 1, 3, 8);
    addUndirectedEdge(g, 1, 4, 5);
    addUndirectedEdge(g, 2, 4, 7);
    addUndirectedEdge(g, 3, 4, 9);
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
  addUndirectedEdge(g, 0, 1, 10);
  addUndirectedEdge(g, 1, 3, 15);
  addUndirectedEdge(g, 3, 2, 4);
  addUndirectedEdge(g, 2, 0, 6);
  addUndirectedEdge(g, 0, 3, 5);
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
    addUndirectedEdge(g, 0, 1, 1);
    addUndirectedEdge(g, 1, 2, 1);
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
    addUndirectedEdge(g, 0, 1, 1);
    addUndirectedEdge(g, 1, 2, 1);
    addUndirectedEdge(g, 2, 0, 1);
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
    addUndirectedEdge(g, 0, 1, 1);
    addUndirectedEdge(g, 0, 2, 1);
    addUndirectedEdge(g, 0, 3, 1);
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
    addUndirectedEdge(g, 0, 1, 1); addUndirectedEdge(g, 1, 2, 1); addUndirectedEdge(g, 2, 0, 1);
    addUndirectedEdge(g, 2, 3, 1);
    addUndirectedEdge(g, 3, 4, 1); addUndirectedEdge(g, 4, 5, 1); addUndirectedEdge(g, 5, 3, 1);
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
    addUndirectedEdge(g, 0, 1, 1);
    addUndirectedEdge(g, 2, 3, 1);
    addUndirectedEdge(g, 3, 4, 1);
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
  addUndirectedEdge(line, 0, 1, 1);
  addUndirectedEdge(line, 1, 2, 1);

  unsigned **b1 = findBridges(line);
  assert(hasBridge(b1, 0, 1));
  assert(hasBridge(b1, 1, 2));
  freeBridgeResult(b1);
  destroyGraph(line);
  printf("Passed: Line Graph\n");

  Graph *cycle = createGraph(3);
  addUndirectedEdge(cycle, 0, 1, 1);
  addUndirectedEdge(cycle, 1, 2, 1);
  addUndirectedEdge(cycle, 2, 0, 1);

  unsigned **b2 = findBridges(cycle);
  assert(b2[0] == NULL);
  freeBridgeResult(b2);
  destroyGraph(cycle);
  printf("Passed: Cycle (No Bridges)\n");

  Graph *dumbbell = createGraph(6);
  addUndirectedEdge(dumbbell, 0, 1, 1);
  addUndirectedEdge(dumbbell, 1, 2, 1);
  addUndirectedEdge(dumbbell, 2, 0, 1);
  addUndirectedEdge(dumbbell, 1, 3, 1);
  addUndirectedEdge(dumbbell, 3, 4, 1);
  addUndirectedEdge(dumbbell, 4, 5, 1);
  addUndirectedEdge(dumbbell, 5, 3, 1);

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

//

void _removeSelfLoops(Graph *graph) {
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

void _removeParallelEdges(Graph *graph) {
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

void _transpose(Graph *graph) {
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

void _mergeVertices(Graph *graph, unsigned u, unsigned v) {
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

unsigned *inDegrees(const Graph *graph) {
  unsigned *degrees = calloc(graph->size, sizeof(unsigned));
  for (unsigned v = 0; v < graph->size; v++)
    degrees[v] = inDegree(graph, v);
  return degrees;
}

unsigned *outDegrees(const Graph *graph) {
  unsigned *degrees = calloc(graph->size, sizeof(unsigned));
  for (unsigned v = 0; v < graph->size; v++)
    degrees[v] = outDegree(graph, v);
  return degrees;
}
