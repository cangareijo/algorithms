#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

unsigned minimumUnsigned(unsigned a, unsigned b);
unsigned maximumUnsigned(unsigned a, unsigned b);

void freeMatrix(double **matrix, unsigned n);

typedef struct Edge {
  unsigned destination;
  double weight;
  struct Edge *next;
} Edge;

typedef struct {
  unsigned size;
  Edge **edges;
} Graph;

bool isValid(const Graph *g);
bool isNull(const Graph *g);
bool isTrivial(const Graph *g);
bool isEmpty(const Graph *g);
bool isRegular(const Graph *g);
bool isComplete(const Graph *g);
bool hasSelfLoops(const Graph *g);
bool isBalanced(const Graph *g);
bool allOutDegreesAreEven(const Graph *g);
bool isEulerianUndirected(const Graph *g);
bool isEulerianDirected(const Graph *g);
bool isConnectedUndirected(const Graph *g);
bool isWeaklyConnected(const Graph *g);
bool isWeaklyConnectedAlternative(const Graph *g);
bool isStronglyConnectedDirected(const Graph *g);
bool isBipartite(const Graph *g);
bool isUndirected(const Graph *g);
bool isMultiGraph(const Graph *g);
bool isDirectedForest(const Graph *g);
bool isUndirectedForest(const Graph *g);
bool isDirectedTree(const Graph *g);
bool isUndirectedTree(const Graph *g);
bool isPathGraph(const Graph *g);
bool isCycleGraph(const Graph *g);
bool isStarGraph(const Graph *g);
bool isWheelGraph(const Graph *g);
bool hasIsolatedVertices(const Graph *g);
bool isTournament(const Graph *g);
bool hasNegativeWeights(const Graph *g);
bool isCubic(const Graph *g);
bool hasNegativeCycle(const Graph *g);
bool isCyclicDirected(const Graph *g);
bool isCyclicUndirected(const Graph *g);
bool isKRegular(const Graph *g, unsigned k);
bool isProperColoring(const Graph *g, const unsigned *coloring);
bool hasConstantWeights(const Graph *g, double x);
bool isDense(const Graph *g, double threshold);
bool isIsolated(const Graph *g, unsigned v);
bool isSource(const Graph *g, unsigned v);
bool isSink(const Graph *g, unsigned v);
bool isUniversalSink(const Graph *g, unsigned v);
bool isDirectedLeaf(const Graph *g, unsigned v);
bool isUndirectedLeaf(const Graph *g, unsigned v);
bool hasSelfLoopsAtVertex(const Graph *g, unsigned v);
bool allAreReachableFromVertexInGraph(const Graph *g, unsigned v);
bool isArticulationVertex(const Graph *g, unsigned v);
bool hasDirectedEdge(const Graph *g, unsigned u, unsigned v);
bool hasUndirectedEdge(const Graph *g, unsigned u, unsigned v);
bool hasPath(const Graph *g, unsigned u, unsigned v);
bool shareNeighbor(const Graph *g, unsigned u, unsigned v);
bool isDirectedBridge(const Graph *g, unsigned u, unsigned v);
bool isUndirectedBridge(const Graph *g, unsigned u, unsigned v);
bool hasWeightedDirectedEdge(const Graph *g, unsigned u, unsigned v, double x);
bool hasWeightedUndirectedEdge(const Graph *g, unsigned u, unsigned v, double x);
bool isTriangle(const Graph *g, unsigned u, unsigned v, unsigned w);
bool isClique(const Graph *g, const bool *subset);
bool isIndependentSet(const Graph *g, const bool *subset);
bool isVertexCover(const Graph *g, const bool *subset);
bool hasDirectedEdges(const Graph *g, unsigned v, const bool *subset);
bool hasUndirectedEdges(const Graph *g, unsigned v, const bool *subset);
bool isTopologicalSort(const Graph *g, const unsigned *ordering);
bool isWalk(const Graph *g, const unsigned *sequence, unsigned length);
bool isPath(const Graph *g, const unsigned *sequence, unsigned length);
bool isHamiltonianPath(const Graph *g, const unsigned *sequence, unsigned length);
bool isDirectedTrail(const Graph *g, const unsigned *sequence, unsigned length);
bool isUndirectedTrail(const Graph *g, const unsigned *sequence, unsigned length);
bool isDirectedCycle(const Graph *g, const unsigned *sequence, unsigned length);
bool isSimpleCycle(const Graph *g, const unsigned *sequence, unsigned length);
bool isHamiltonianCycle(const Graph *g, const unsigned *sequence, unsigned length);
bool isDirectedCircuit(const Graph *g, const unsigned *sequence, unsigned length);
bool isUndirectedCircuit(const Graph *g, const unsigned *sequence, unsigned length);
bool isPerfectMatching(const Graph *g, const unsigned (*pairs)[2], unsigned n);
bool isSubGraph(const Graph *subgraph, const Graph *g);
bool isSpanningUndirectedTree(const Graph *subgraph, const Graph *g);
bool isSpanningDirectedTree(const Graph *subgraph, const Graph *g);

bool *graphCenter(const Graph *g);
bool *graphPeriphery(const Graph *g);
bool *findArticulationPoints(const Graph *g);
bool *findMaximalClique(const Graph *g);
bool *findMaximumClique(const Graph *g);

Graph *createGraph(unsigned n);
Graph *createPathGraph(unsigned size);
Graph *createCycleGraph(unsigned size);
Graph *createCompleteGraph(unsigned size);
Graph *createStarGraph(unsigned size);
Graph *createWheelGraph(unsigned size);
Graph *generateRandomGraph(unsigned n, double p, bool directed, bool weighted);
Graph *copyGraph(const Graph *graph);
Graph *copyTranspose(const Graph *graph);
Graph *copyUnweighted(const Graph *graph);
Graph *copyUndirected(const Graph *graph);
Graph *copyComplement(const Graph *graph);
Graph *lineGraph(const Graph *graph);
Graph *underlyingGraph(const Graph *graph);
Graph *kruskal(const Graph *g);
Graph *directedSubdivisionGraph(const Graph *g);
Graph *undirectedSubdivisionGraph(const Graph *g);
Graph *graphPower(const Graph *g, unsigned k);
Graph *removeVertex(const Graph *graph, unsigned vertex);
Graph *prim(const Graph *g, unsigned v);
Graph *contractVertices(const Graph *graph, unsigned u, unsigned v);
Graph *copySubgraph(const Graph *graph, const bool *subset);
Graph *subgraphInducedByEdges(const Graph *graph, const bool *subset);
Graph *graphUnion(const Graph *g1, const Graph *g2);
Graph *cartesianProduct(const Graph *g1, const Graph *g2);
Graph *disjointUnion(const Graph *g1, const Graph *g2);
Graph *tensorProduct(const Graph *g1, const Graph *g2);

void destroyGraph(Graph *g);
void addVertex(Graph *graph);
void printGraph(const Graph *graph);
void removeDirectedEdgeByIndex(Graph *g, unsigned i);
void removeFirstDirectedEdge(Graph *graph, unsigned source, unsigned destination);
void removeFirstUndirectedEdge(Graph *graph, unsigned u, unsigned v);
void subdivideEdge(Graph *graph, unsigned u, unsigned v);
void addDirectedEdge(Graph *g, unsigned u, unsigned v, double x);
void addUndirectedEdge(Graph *g, unsigned u, unsigned v, double x);
void removeFirstWeightedDirectedEdge(Graph *graph, unsigned u, unsigned v, double weight);
void removeFirstWeightedUndirectedEdge(Graph *graph, unsigned u, unsigned v, double weight);

unsigned countEdges(const Graph *g);
unsigned countSelfLoops(const Graph *g);
unsigned countTriangles(const Graph *g);
unsigned minimumInDegree(const Graph *g);
unsigned maximumInDegree(const Graph *g);
unsigned minimumOutDegree(const Graph *g);
unsigned maximumOutDegree(const Graph *g);
unsigned minimumDegree(const Graph *g);
unsigned maximumDegree(const Graph *g);
unsigned countDirectedLeaves(const Graph *g);
unsigned countUndirectedLeaves(const Graph *g);
unsigned countSources(const Graph *g);
unsigned countSinks(const Graph *g);
unsigned countParallelEdges(const Graph *g);
unsigned countIsolatedVertices(const Graph *g);
unsigned getSize(const Graph *g);
unsigned countComponents(const Graph *g);
unsigned firstActiveVertex(const Graph *g);
unsigned wienerIndex(const Graph *g);
unsigned inDegree(const Graph *g, unsigned v);
unsigned outDegree(const Graph *g, unsigned v);
unsigned degree(const Graph *g, unsigned v);
unsigned getNeighbor(const Graph *g, unsigned v, unsigned i);
unsigned countCommonNeighbors(const Graph *g, unsigned u, unsigned v);
unsigned countShortestPaths(const Graph *g, unsigned u, unsigned v);
unsigned distance(const Graph *g, unsigned u, unsigned v);
unsigned countMatchingEdges(const Graph *g, unsigned u, unsigned v);
unsigned countMatchingWeightedEdges(const Graph *g, unsigned u, unsigned v, double x);

unsigned *inDegrees(const Graph *g);
unsigned *outDegrees(const Graph *g);
unsigned *outDegreeDistribution(const Graph *graph);
unsigned *inDegreeDistribution(const Graph *graph);
unsigned *undirectedColoring(const Graph *g);
unsigned *stronglyConnectedComponents(const Graph *g);
unsigned *topologicalSortOfGraph(const Graph *g);
unsigned *findBridges(const Graph *g);
unsigned *unweightedDijkstra(const Graph *graph, unsigned source);
unsigned *getInNeighbors(const Graph *graph, unsigned vertex);
unsigned *getOutNeighbors(const Graph *graph, unsigned vertex);
unsigned *preOrderSort(const Graph *g, unsigned v);
unsigned *postOrderSort(const Graph *g, unsigned v);
unsigned *breadthFirstSort(const Graph *g, unsigned v);
unsigned *shortestPath(const Graph *graph, unsigned source, unsigned target, unsigned *length);

unsigned **allPairsShortestPathsUnweighted(const Graph *graph);

double sumWeights(const Graph *graph);
double graphRadius(const Graph *graph);
double graphDiameter(const Graph *graph);
double graphDensity(const Graph *graph);
double averageClusteringCoefficient(const Graph *graph);
double graphGirth(const Graph *graph);
double minimumEdgeWeight(const Graph *graph);
double maximumEdgeWeight(const Graph *graph);
double graphEccentricity(const Graph *graph, unsigned vertex);
double normalizedDegree(const Graph *graph, unsigned vertex);
double localClusteringCoefficient(const Graph *graph, unsigned vertex);
double edgeWeight(const Graph *graph, unsigned u, unsigned v);
double maxFlowEdmondsKarp(const Graph *g, unsigned u, unsigned v);
double subgraphDensity(const Graph *graph, const bool *subset);
double pathWeight(const Graph *graph, const unsigned *path, unsigned length);

double *closenessCentrality(const Graph *graph);
double *bellmanFord(const Graph *graph, unsigned source);
double *weightedDijkstra(const Graph *g, unsigned v);

double **toMatrix(const Graph *graph);
double **floydWarshall(const Graph *graph);

Edge **inNeighbors(const Graph *graph);
Edge **outNeighbors(const Graph *graph);

void testIsDirectedCyclicGraph();
void testIsUndirectedCyclicGraph();
void testIsConnectedUndirected();
void testIsWeaklyConnected();
void testIsStronglyConnectedDirected();
void testTopologicalSortOfGraph();
void testBellmanFord();
void testUnweightedDijkstra();
void testWeightedDijkstra();
void testFloydWarshall();
void testPrim();
void testKruskal();
void testFindArticulationPoints();
void testGraphDensity();

int main();



unsigned minimumUnsigned(unsigned a, unsigned b) { return a <= b ? a : b; }

unsigned maximumUnsigned(unsigned a, unsigned b) { return a >= b ? a : b; }



void freeMatrix(double **matrix, unsigned n) {
  if (!matrix) return;
  for (unsigned i = 0; i < n; i++)
    free(matrix[i]);
  free(matrix);
}



bool isValid(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return false;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination >= g->size)
        return false;
  return true;
}

bool isNull(const Graph *g) {
  return !g || g->size == 0;
}

bool isTrivial(const Graph *g) {
  return g && g->size == 1 && (!g->edges || !g->edges[0]);
}

bool isEmpty(const Graph *g) {
  if (!g || !g->edges) return true;
  for (unsigned v = 0; v < g->size; v++)
    if (g->edges[v])
      return false;
  return true;
}

bool isRegular(const Graph *graph) {
  for (unsigned v = 1; v < graph->size; v++)
    if (inDegree(graph, v - 1) != inDegree(graph, v) || outDegree(graph, v - 1) != outDegree(graph, v))
      return false;
  return true;
}

bool isComplete(const Graph *graph) {
  return !hasSelfLoops(graph) && !isMultiGraph(graph) && countEdges(graph) == graph->size * (graph->size - 1);
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

bool allOutDegreesAreEven(const Graph *g) {
  if (!g) return true;
  for (unsigned v = 0; v < g->size; v++)
    if (outDegree(g, v) % 2 != 0)
      return false;
  return true;
}

bool isEulerianUndirected(const Graph *g) {
  if (isEmpty(g)) return true;
  if (!allOutDegreesAreEven(g)) return false;
  unsigned v = firstActiveVertex(g);
  unsigned *distances = unweightedDijkstra(g, v);
  if (!distances) return false;
  for (unsigned v = 0; v < g->size; v++)
    if (distances[v] == UINT_MAX && g->edges[v])
      return false;
  free(distances);
  return true;
}

bool isEulerianDirected(const Graph *graph) {
  if (!graph || (graph->size < 2 && !graph->edges)) return true;
  if (graph->size >= 2 && !graph->edges) return false;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      if (e->destination >= graph->size)
        return false;
  unsigned *inDegree = calloc(graph->size, sizeof(unsigned));
  unsigned *outDegree = calloc(graph->size, sizeof(unsigned));
  bool *visited = calloc(graph->size, sizeof(bool));
  unsigned *stack = malloc(graph->size * sizeof(unsigned));
  if (!inDegree || !outDegree || !visited || !stack) {
    free(inDegree);
    free(outDegree);
    free(visited);
    free(stack);
    return false;
  }
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      inDegree[e->destination]++;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      outDegree[v]++;
  for (unsigned v = 0; v < graph->size; v++)
    if (inDegree[v] != outDegree[v]) {
      free(inDegree);
      free(outDegree);
      free(stack);
      free(visited);
      return false;
    }
  unsigned start = UINT_MAX;
  for (unsigned v = 0; v < graph->size; v++)
    if (outDegree[v] > 0) {
      start = v;
      break;
    }
  if (start == UINT_MAX)
    return graph->size <= 1;
  unsigned size = 0;
  stack[size++] = start;
  visited[start] = true;
  while (size > 0) {
    unsigned v = stack[--size];
    for (Edge *e = graph->edges[v]; e; e = e->next)
      if (!visited[e->destination]) {
        stack[size++] = e->destination;
        visited[e->destination] = true;
      }
  }
  for (unsigned v = 0; v < graph->size; v++)
    if ((inDegree[v] > 0 || outDegree[v] > 0) && !visited[v]) {
      free(inDegree);
      free(outDegree);
      free(stack);
      free(visited);
      return false;
    }
  free(inDegree);
  free(outDegree);
  free(stack);
  free(visited);
  return true;
}

bool isConnectedUndirected(const Graph *graph) {
  return graph->size < 2 || allAreReachableFromVertexInGraph(graph, 0);
}

bool isWeaklyConnected(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return false;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination >= g->size)
        return false;
  if (g->size < 2) return true;
  bool *visited = calloc(g->size, sizeof(bool));
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  if (!visited || !queue) {
    free(visited);
    free(queue);
    return false;
  }
  unsigned head = 0;
  unsigned tail = 0;
  visited[0] = true;
  queue[tail++] = 0;
  while (head < tail) {
    unsigned u = queue[head++];
    for (Edge *e = g->edges[u]; e; e = e->next)
      if (!visited[e->destination]) {
        visited[e->destination] = true;
        queue[tail++] = e->destination;
      }
    for (unsigned v = 0; v < g->size; v++)
      if (!visited[v])
        for (Edge *e = g->edges[v]; e; e = e->next)
          if (e->destination == u) {
            visited[v] = true;
            queue[tail++] = v;
            break;
          }
  }
  bool connected = true;
  for (unsigned v = 0; v < g->size; v++)
    if (!visited[v]) {
      connected = false;
      break;
    }
  free(visited);
  free(queue);
  return connected;
}

bool isWeaklyConnectedAlternative(const Graph *g) {
  Graph *undirected = copyUndirected(g);
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
  unsigned *colors = calloc(graph->size, sizeof(unsigned));
  unsigned *queue = malloc(graph->size * sizeof(unsigned));
  bool bipartite = true;
  for (unsigned u = 0; u < graph->size && bipartite; u++)
    if (colors[u] == 0) {
      colors[u] = 1;
      unsigned head = 0, tail = 0;
      queue[tail++] = u;
      while (head < tail && bipartite) {
        unsigned v = queue[head++];
        for (Edge *e = graph->edges[v]; e && bipartite; e = e->next)
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

bool isUndirected(const Graph *g) {
  if (!g || !g->edges) return true;
  for (unsigned u = 0; u < g->size; u++)
    for (Edge *e = g->edges[u]; e; e = e->next)
      if (countMatchingWeightedEdges(g, u, e->destination, e->weight) != countMatchingWeightedEdges(g, e->destination, u, e->weight))
        return false;
  return true;
}

bool isMultiGraph(const Graph *graph) {
  bool b = false;
  bool *seen = malloc(graph->size * sizeof(bool));
  for (unsigned v = 0; v < graph->size && !b; v++) {
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      seen[e->destination] = false;
    for (Edge *e = graph->edges[v]; e != nullptr && !b; e = e->next)
      if (seen[e->destination])
        b = true;
      else
        seen[e->destination] = true;
  }
  free(seen);
  return b;
}

bool isDirectedForest(const Graph *g) {
  if (!g || isCyclicDirected(g)) return false;
  unsigned *degree = inDegrees(g);
  if (g->size > 0 && !degree) return false;
  for (unsigned v = 0; v < g->size; v++)
    if (degree[v] >= 2) {
      free(degree);
      return false;
    }
  free(degree);
  return true;
}

bool isUndirectedForest(const Graph *g) {
  return isUndirected(g) && !isCyclicUndirected(g);
}

bool isDirectedTree(const Graph *g) {
  if (!g || g->size == 0 || isCyclicDirected(g)) return false;
  unsigned *degree = inDegrees(g);
  if (!degree) return false;
  unsigned roots = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (degree[v] == 0) {
      roots++;
    } else if (degree[v] >= 2) {
      free(degree);
      return false;
    }
  free(degree);
  return roots == 1;
}

bool isUndirectedTree(const Graph *g) {
  return isUndirected(g) && !isCyclicUndirected(g) && isConnectedUndirected(g);
}

bool isPathGraph(const Graph *g) {
  if (getSize(g) == 0) return false;
  if (getSize(g) == 1) return isEmpty(g);
  if (hasSelfLoops(g)) return false;
  if (isMultiGraph(g)) return false;
  if (!isConnectedUndirected(g)) return false;
  unsigned endpoints = 0;
  unsigned internal = 0;
  for (unsigned v = 0; v < getSize(g); v++)
    if (inDegree(g, v) == 1 && outDegree(g, v) == 1)
      endpoints++;
    else if (inDegree(g, v) == 2 && outDegree(g, v) == 2)
      internal++;
    else
      return false;
  if (endpoints != 2) return false;
  if (internal != getSize(g) - 2) return false;
  return true;
}

bool isCycleGraph(const Graph *g) {
  if (getSize(g) < 3) return false;
  if (hasSelfLoops(g)) return false;
  if (isMultiGraph(g)) return false;
  if (!isConnectedUndirected(g)) return false;
  if (!isKRegular(g, 2)) return false;
  return true;
}

bool isStarGraph(const Graph *graph) {
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

bool isWheelGraph(const Graph *graph) {
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
    for (Edge *e = graph->edges[current]; e != nullptr && next == UINT_MAX; e = e->next)
      if (e->destination != hub && e->destination != previous)
        next = e->destination;
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

bool isTournament(const Graph *graph) {
  unsigned **m = malloc(graph->size * sizeof(unsigned *));
  for (unsigned v = 0; v < graph->size; v++)
    m[v] = calloc(graph->size, sizeof(unsigned));
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      m[v][e->destination] += 1;
  bool b = true;
  for (unsigned v = 0; v < graph->size && b; v++)
    b = b && m[v][v] == 0;
  for (unsigned u = 0; u < graph->size && b; u++)
    for (unsigned v = u + 1; v < graph->size && b; v++)
      b = b && m[u][v] + m[v][u] == 1;
  for (unsigned v = 0; v < graph->size; v++)
    free(m[v]);
  free(m);
  return b;
}

bool hasNegativeWeights(const Graph *graph) {
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      if (e->weight < 0)
        return true;
  return false;
}

bool isCubic(const Graph *g) {
  return isKRegular(g, 3);
}

bool hasNegativeCycle(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return false;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination >= g->size)
        return false;
  double *distances = malloc(g->size * sizeof(double));
  if (!distances) return false;
  for (unsigned v = 0; v < g->size; v++) distances[v] = 0;
  for (unsigned i = 1; i < g->size; i++)
    for (unsigned v = 0; v < g->size; v++)
      for (Edge *e = g->edges[v]; e; e = e->next)
        if (distances[v] + e->weight < distances[e->destination])
          distances[e->destination] = distances[v] + e->weight;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (distances[v] + e->weight < distances[e->destination]) {
        free(distances);
        return true;
      }
  free(distances);
  return false;
}

bool isCyclicDirectedComponent(const Graph *graph, unsigned vertex, char *visited) {
  visited[vertex] = 1;
  for (Edge *e = graph->edges[vertex]; e != nullptr; e = e->next)
    if (visited[e->destination] == 1 || (visited[e->destination] == 0 && isCyclicDirectedComponent(graph, e->destination, visited)))
      return true;
  visited[vertex] = 2;
  return false;
}

bool isCyclicDirected(const Graph *graph) {
  char *visited = calloc(graph->size, sizeof(char));
  bool b = false;
  for (unsigned v = 0; v < graph->size && !b; v++)
    b = b || (visited[v] == 0 && isCyclicDirectedComponent(graph, v, visited));
  free(visited);
  return b;
}

bool isCyclicUndirectedComponent(const Graph *graph, unsigned vertex, unsigned parent, bool *visited) {
  visited[vertex] = true;
  for (Edge *e = graph->edges[vertex]; e != nullptr; e = e->next)
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
    if (inDegree(graph, v) != k || outDegree(graph, v) != k)
      return false;
  return true;
}

bool isProperColoring(const Graph *graph, const unsigned *coloring) {
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      if (coloring[v] == coloring[e->destination])
        return false;
  return true;
}

bool hasConstantWeights(const Graph *graph, double weight) {
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
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
  for (Edge *e = graph->edges[vertex]; e != nullptr; e = e->next)
    if (e->destination == vertex)
      return true;
  return false;
}

bool allAreReachableFromVertexInGraph(const Graph *graph, unsigned vertex) {
  unsigned *distances = unweightedDijkstra(graph, vertex);
  bool b = true;
  for (unsigned v = 0; v < graph->size && b; v++)
    b = b && distances[v] < UINT_MAX;
  free(distances);
  return b;
}

bool isArticulationVertex(const Graph *graph, unsigned vertex) {
  Graph *g = removeVertex(graph, vertex);
  unsigned n = countComponents(g);
  destroyGraph(g);
  return n > countComponents(graph);
}

bool hasDirectedEdge(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size) return false;
  for (Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination == v)
      return true;
  return false;
}

bool hasUndirectedEdge(const Graph *graph, unsigned u, unsigned v) {
  return hasDirectedEdge(graph, u, v) && hasDirectedEdge(graph, v, u);
}

bool hasPath(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size) return false;
  bool *visited = calloc(g->size, sizeof(bool));
  unsigned *stack = malloc(g->size * sizeof(unsigned));
  if (!visited || !stack) {
    free(visited);
    free(stack);
    return false;
  }
  int top = 0;
  visited[u] = true;
  stack[top++] = u;
  bool found = false;
  while (top > 0) {
    u = stack[--top];
    if (u == v) {
      found = true;
      break;
    }
    for (Edge *e = g->edges[u]; e; e = e->next)
      if (e->destination < g->size && !visited[e->destination]) {
        visited[e->destination] = true;
        stack[top++] = e->destination;
      }
  }
  free(stack);
  free(visited);
  return found;
}

bool shareNeighbor(const Graph *graph, unsigned u, unsigned v) {
  bool *neighbors = calloc(graph->size, sizeof(bool));
  for (Edge *e = graph->edges[u]; e; e = e->next)
    neighbors[e->destination] = true;
  bool b = false;
  for (Edge *e = graph->edges[v]; e && !b; e = e->next)
    b = b || neighbors[e->destination];
  free(neighbors);
  return b;
}

bool isDirectedBridge(const Graph *graph, unsigned u, unsigned v) {
  Graph *g = copyGraph(graph);
  removeFirstDirectedEdge(g, u, v);
  unsigned n = countComponents(g);
  destroyGraph(g);
  return n > countComponents(graph);
}

bool isUndirectedBridge(const Graph *graph, unsigned u, unsigned v) {
  Graph *g = copyGraph(graph);
  removeFirstUndirectedEdge(g, u, v);
  unsigned n = countComponents(g);
  destroyGraph(g);
  return n > countComponents(graph);
}

bool hasWeightedDirectedEdge(const Graph *graph, unsigned u, unsigned v, double weight) {
  for (Edge *e = graph->edges[u]; e; e = e->next)
    if (e->destination == v && e->weight == weight)
      return true;
  return false;
}

bool hasWeightedUndirectedEdge(const Graph *graph, unsigned u, unsigned v, double weight) {
  return hasWeightedDirectedEdge(graph, u, v, weight) && hasWeightedDirectedEdge(graph, v, u, weight);
}

bool isTriangle(const Graph *graph, unsigned u, unsigned v, unsigned w) {
  return u != v && v != w && w != u && hasDirectedEdge(graph, u, v) && hasDirectedEdge(graph, v, w) && hasDirectedEdge(graph, w, u);
}

bool isClique(const Graph *graph, const bool *subset) {
  for (unsigned u = 0; u < graph->size; u++)
    if (subset[u])
      for (unsigned v = u + 1; v < graph->size; v++)
        if (subset[v])
          if (!hasDirectedEdge(graph, u, v))
            return false;
  return true;
}

bool isIndependentSet(const Graph *graph, const bool *subset) {
  for (unsigned u = 0; u < graph->size; u++)
    if (subset[u])
      for (unsigned v = u + 1; v < graph->size; v++)
        if (subset[v])
          if (hasDirectedEdge(graph, u, v))
            return false;
  return true;
}

bool isVertexCover(const Graph *graph, const bool *subset) {
  for (unsigned u = 0; u < graph->size; u++)
    if (!subset[u])
      for (Edge *e = graph->edges[u]; e != nullptr; e = e->next)
        if (!subset[e->destination])
          return false;
  return true;
}

bool hasDirectedEdges(const Graph *g, unsigned v, const bool *subset) {
  if (!g || !g->edges || v >= g->size || !subset) return false;
  unsigned total = 0;
  for (unsigned u = 0; u < g->size; u++)
    if (subset[u])
      total++;
  bool *visited = calloc(g->size, sizeof(bool));
  if (!visited) return false;
  unsigned found = 0;
  for (Edge *e = g->edges[v]; e && found < total; e = e->next)
    if (e->destination < g->size && subset[e->destination] && !visited[e->destination]) {
      visited[e->destination] = true;
      found++;
    }
  free(visited);
  return found == total;
}

bool hasUndirectedEdges(const Graph *g, unsigned v, const bool *subset) {
  if (!g || !hasDirectedEdges(g, v, subset)) return false;
  for (unsigned u = 0; u < g->size; u++)
    if (subset[u] && !hasDirectedEdge(g, u, v))
      return false;
  return true;
}

bool isTopologicalSort(const Graph *g, const unsigned *sequence) {
  if (!isValid(g) || (g->size > 0 && !sequence)) return false;
  unsigned position[g->size];
  for (unsigned i = 0; i < g->size; i++)
    position[sequence[i]] = i;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (position[v] >= position[e->destination])
        return false;
  return true;
}

bool isWalk(const Graph *graph, const unsigned *sequence, unsigned length) {
  bool b = true;
  for (unsigned i = 1; i < length && b; i++)
    b = b && hasDirectedEdge(graph, sequence[i - 1], sequence[i]);
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

bool isDirectedTrail(const Graph *graph, const unsigned *sequence, unsigned length) {
  Graph *copy = copyGraph(graph);
  bool valid = true;
  for (unsigned i = 1; i < length && valid; i++)
    if (hasDirectedEdge(copy, sequence[i - 1], sequence[i]))
      removeFirstDirectedEdge(copy, sequence[i - 1], sequence[i]);
    else
      valid = false;
  destroyGraph(copy);
  return valid;
}

bool isUndirectedTrail(const Graph *graph, const unsigned *sequence, unsigned length) {
  Graph *copy = copyGraph(graph);
  bool valid = true;
  for (unsigned i = 1; i < length && valid; i++)
    if (hasUndirectedEdge(copy, sequence[i - 1], sequence[i]))
      removeFirstUndirectedEdge(copy, sequence[i - 1], sequence[i]);
    else
      valid = false;
  destroyGraph(copy);
  return valid;
}

bool isDirectedCycle(const Graph *graph, const unsigned *sequence, unsigned length) {
  bool valid = length >= 1;
  for (unsigned i = 0; i < length && valid; i++)
    valid = valid && hasDirectedEdge(graph, sequence[i], sequence[(i + 1) % length]);
  return valid;
}

bool isSimpleCycle(const Graph *graph, const unsigned *sequence, unsigned length) {
  bool valid = isDirectedCycle(graph, sequence, length);
  bool *visited = calloc(graph->size, sizeof(bool));
  for (unsigned i = 0; i < length && valid; i++) {
    valid = valid && !visited[sequence[i]];
    visited[sequence[i]] = true;
  }
  free(visited);
  return valid;
}

bool isHamiltonianCycle(const Graph *graph, const unsigned *sequence, unsigned length) {
  return length == graph->size && isSimpleCycle(graph, sequence, length);
}

bool isDirectedCircuit(const Graph *graph, const unsigned *sequence, unsigned length) {
  bool valid = length >= 1;
  Graph *copy = copyGraph(graph);
  for (unsigned i = 0; i < length && valid; i++)
    if (hasDirectedEdge(copy, sequence[i], sequence[(i + 1) % length]))
      removeFirstDirectedEdge(copy, sequence[i], sequence[(i + 1) % length]);
    else
      valid = false;
  destroyGraph(copy);
  return valid;
}

bool isUndirectedCircuit(const Graph *graph, const unsigned *sequence, unsigned length) {
  bool valid = length >= 1;
  Graph *copy = copyGraph(graph);
  for (unsigned i = 0; i < length && valid; i++)
    if (hasUndirectedEdge(copy, sequence[i], sequence[(i + 1) % length]))
      removeFirstUndirectedEdge(copy, sequence[i], sequence[(i + 1) % length]);
    else
      valid = false;
  destroyGraph(copy);
  return valid;
}

bool isPerfectMatching(const Graph *g, const unsigned (*pairs)[2], unsigned n) {
  if (!g || 2 * n != g->size) return false;
  bool *matched = calloc(g->size, sizeof(bool));
  if (!matched) return false;
  for (unsigned i = 0; i < n; i++) {
    unsigned u = pairs[i][0];
    unsigned v = pairs[i][1];
    if (u >= g->size || v >= g->size || u == v || matched[u] || matched[v] || (!hasDirectedEdge(g, u, v) && !hasDirectedEdge(g, v, u))) {
      free(matched);
      return false;
    }
    matched[u] = true;
    matched[v] = true;
  }
  free(matched);
  return true;
}

bool isSubGraph(const Graph *subgraph, const Graph *graph) {
  bool b = subgraph->size <= graph->size;
  bool *edges = calloc(graph->size, sizeof(bool));
  double *weights = malloc(graph->size * sizeof(double));
  for (unsigned v = 0; v < subgraph->size && b; v++) {
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next) {
      edges[e->destination] = true;
      weights[e->destination] = e->weight;
    }
    for (Edge *e = subgraph->edges[v]; e != nullptr && b; e = e->next)
      b = b && edges[e->destination] && e->weight == weights[e->destination];
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      edges[e->destination] = false;
  }
  free(weights);
  free(edges);
  return b;
}

bool isSpanningUndirectedTree(const Graph *subgraph, const Graph *graph) {
  return subgraph->size == graph->size &&
    subgraph->size > 0 &&
    isSubGraph(subgraph, graph) &&
    countEdges(subgraph) == 2 * (subgraph->size - 1) &&
    isConnectedUndirected(subgraph);
}

bool isSpanningDirectedTree(const Graph *subgraph, const Graph *graph) {
  return subgraph->size == graph->size &&
    subgraph->size > 0 &&
    isSubGraph(subgraph, graph) &&
    countEdges(subgraph) == subgraph->size - 1 &&
    isWeaklyConnected(subgraph);
}



bool *graphCenter(const Graph *graph) {
  double *eccentricity = malloc(graph->size * sizeof(double));
  double radius = INFINITY;
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
  double *eccentricity = malloc(graph->size * sizeof(double));
  double diameter = -INFINITY;
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
  for (Edge *e = graph->edges[vertex]; e != nullptr; e = e->next)
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

bool *findMaximalClique(const Graph *g) {
  if (!g) return nullptr;
  bool *clique = calloc(g->size, sizeof(bool));
  if (!clique) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    if (hasUndirectedEdges(g, v, clique))
      clique[v] = true;
  return clique;
}

static void maximumCliqueHelper(const Graph *g, bool *current, unsigned currentSize, bool *best, unsigned *bestSize, unsigned start) {
  if (currentSize > *bestSize) {
    *bestSize = currentSize;
    for (unsigned v = 0; v < g->size; v++) best[v] = current[v];
  }
  for (unsigned v = start; v < g->size; v++) {
    if (currentSize + (g->size - v) <= *bestSize) return; 
    if (hasUndirectedEdges(g, v, current)) {
      current[v] = true;
      maximumCliqueHelper(g, current, currentSize + 1, best, bestSize, v + 1);
      current[v] = false;
    }
  }
}

bool *findMaximumClique(const Graph *g) {
  if (!g) return nullptr;
  bool *current = calloc(g->size, sizeof(bool));
  bool *best = calloc(g->size, sizeof(bool));
  if (!current || !best) {
    free(current); free(best);
    return nullptr;
  }
  unsigned size = 0;
  maximumCliqueHelper(g, current, 0, best, &size, 0);
  free(current);
  return best;
}



Graph *createGraph(unsigned n) {
  Graph *g = malloc(sizeof(Graph));
  if (!g) return nullptr;
  g->size = n;
  g->edges = calloc(n, sizeof(Edge *));
  if (!g->edges) {
    free(g);
    return nullptr;
  }
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

Graph *createStarGraph(unsigned size) {
  Graph *g = createGraph(size);
  for (unsigned v = 1; v < size; v++) addUndirectedEdge(g, 0, v, 1);
  return g;
}

Graph *createWheelGraph(unsigned size) {
  Graph *g = createGraph(size);
  for (unsigned v = 1; v < size; v++) addUndirectedEdge(g, 0, v, 1);
  for (unsigned v = 2; v < size; v++) addUndirectedEdge(g, v - 1, v, 1);
  addUndirectedEdge(g, size - 1, 1, 1);
  return g;
}

Graph *generateRandomGraph(unsigned n, double p, bool directed, bool weighted) {
  if (p < 0 || p > 1) return nullptr;
  Graph *g = createGraph(n);
  if (!g) return nullptr;
  for (unsigned u = 0; u < n; u++)
    for (unsigned v = directed ? 0 : u + 1; v < n; v++) {
      if (u == v) continue;
      if ((double)rand() / RAND_MAX < p) {
        double weight = weighted ? 1 + ((double)rand() / RAND_MAX) * 9 : 1;
        if (directed) addDirectedEdge(g, u, v, weight); else addUndirectedEdge(g, u, v, weight);
      }
    }
  return g;
}

Graph *copyGraph(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      addDirectedEdge(g, v, e->destination, e->weight);
  return g;
}

Graph *copyTranspose(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      addDirectedEdge(g, e->destination, v, e->weight);
  return g;
}

Graph *copyUnweighted(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      addDirectedEdge(g, v, e->destination, 1);
  return g;
}

Graph *copyUndirected(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      addUndirectedEdge(g, v, e->destination, e->weight);
  return g;
}

Graph *copyComplement(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned u = 0; u < graph->size; u++)
    for (unsigned v = 0; v < graph->size; v++)
      if (u != v && !hasDirectedEdge(graph, u, v))
        addDirectedEdge(g, u, v, 1);
  return g;
}

Graph *lineGraph(const Graph *graph) {
  unsigned n = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      if (v <= e->destination)
        n++;
  Graph *g = createGraph(n);
  unsigned i = 0;
  for (unsigned u = 0; u < graph->size; u++)
    for (Edge *d = graph->edges[u]; d != nullptr; d = d->next)
      if (u <= d->destination) {
        unsigned j = 0;
        for (unsigned v = 0; v < graph->size; v++)
          for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
            if (v <= e->destination) {
              if (i < j && (u == v || u == e->destination || d->destination == v || d->destination == e->destination))
                addUndirectedEdge(g, i, j, 1);
              j++;
            }
        i++;
      }
  return g;
}

Graph *underlyingGraph(const Graph *graph) {
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      if (v != e->destination && !hasUndirectedEdge(g, v, e->destination))
        addUndirectedEdge(g, v, e->destination, 1);
  return g;
}

Graph *kruskal(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *mst = createGraph(g->size);
  unsigned total = countEdges(g);
  bool *used = calloc(total, sizeof(bool));
  if (!mst || (total > 0 && !used)) {
    destroyGraph(mst);
    free(used);
    return nullptr;
  }
  unsigned added = 0;
  unsigned target = g->size - 1;
  while (added < target) {
    double minimumWeight = DBL_MAX;
    unsigned minimumU = 0;
    unsigned minimumV = 0;
    unsigned minimumIndex = UINT_MAX;
    unsigned i = 0;
    for (unsigned v = 0; v < g->size; v++)
      for (Edge *e = g->edges[v]; e; e = e->next) {
        if (!used[i] && e->weight < minimumWeight) {
          minimumWeight = e->weight;
          minimumU = v;
          minimumV = e->destination;
          minimumIndex = i;
        }
        i++;
      }
    if (minimumIndex == UINT_MAX) break;
    used[minimumIndex] = true;
    i = 0;
    for (unsigned v = 0; v < g->size; v++)
      for (Edge *e = g->edges[v]; e; e = e->next) {
        if (!used[i] && v == minimumV && e->destination == minimumU && e->weight == minimumWeight) {
          used[i] = true;
          v = g->size - 1;
          break;
        }
        i++;
      }
    if (!hasPath(mst, minimumU, minimumV)) {
      addUndirectedEdge(mst, minimumU, minimumV, minimumWeight);
      added++;
    }
  }
  free(used);
  return mst;
}

Graph *directedSubdivisionGraph(const Graph *g) {
  if (!g) return nullptr;
  Graph *g2 = createGraph(g->size + countEdges(g));
  if (!g2) return nullptr;
  unsigned u = g->size;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next) {
      addDirectedEdge(g2, v, u, e->weight);
      addDirectedEdge(g2, u, e->destination, e->weight);
      u++;
    }
  return g2;
}

Graph *undirectedSubdivisionGraph(const Graph *g) {
  if (!g) return nullptr;
  Graph *g2 = createGraph(g->size + countEdges(g) / 2);
  if (!g2) return nullptr;
  unsigned u = g->size;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (v < e->destination) {
        addUndirectedEdge(g2, v, u, e->weight);
        addUndirectedEdge(g2, u, e->destination, e->weight);
        u++;
      }
  return g2;
}

Graph *graphPower(const Graph *g, unsigned k) {
  if (g == nullptr) return nullptr;
  Graph *result = createGraph(g->size);
  if (result == nullptr) return nullptr;
  if (k == 0) return result;
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  unsigned *distance = malloc(g->size * sizeof(unsigned));
  if (queue == nullptr || distance == nullptr) {
    free(queue);
    free(distance);
    destroyGraph(result);
    return nullptr;
  }
  for (unsigned source = 0; source < g->size; source++) {
    for (unsigned v = 0; v < g->size; v++) distance[v] = UINT_MAX;
    unsigned head = 0;
    unsigned tail = 0;
    distance[source] = 0;
    queue[tail++] = source;
    while (head < tail) {
      unsigned u = queue[head++];
      if (distance[u] >= k) break;
      for (Edge *e = g->edges[u]; e != nullptr; e = e->next)
        if (distance[e->destination] == UINT_MAX) {
          distance[e->destination] = distance[u] + 1;
          queue[tail++] = e->destination;
        }
    }
    for (unsigned target = 0; target < g->size; target++)
      if (target != source && distance[target] <= k)
        addDirectedEdge(result, source, target, 1);
  }
  free(queue);
  free(distance);
  return result;
}

Graph *removeVertex(const Graph *graph, unsigned vertex) {
  Graph *g = createGraph(graph->size - 1);
  for (unsigned u = 0; u < graph->size; u++)
    if (u != vertex) {
      unsigned v = (u > vertex) ? u - 1 : u;
      for (Edge *e = graph->edges[u]; e != nullptr; e = e->next)
        if (e->destination != vertex) {
          unsigned w = (e->destination > vertex) ? e->destination - 1 : e->destination;
          addDirectedEdge(g, v, w, e->weight);
        }
    }
  return g;
}

Graph *prim(const Graph *g, unsigned v) {
  if (!isValid(g) || g->size == 0 || v >= g->size) return nullptr;
  bool *processed = calloc(g->size, sizeof(bool));
  double *weights = malloc(g->size * sizeof(double));
  unsigned *parents = malloc(g->size * sizeof(unsigned));
  Graph *mst = createGraph(g->size);
  if (!processed || !weights || !parents || !mst) {
    free(processed);
    free(weights);
    free(parents);
    destroyGraph(mst);
    return nullptr;
  }
  for (unsigned u = 0; u < g->size; u++) {
    weights[u] = INFINITY;
    parents[u] = u;
  }
  weights[v] = 0;
  for (;;) {
    processed[v] = true;
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (!processed[e->destination] && e->weight < weights[e->destination]) {
        weights[e->destination] = e->weight;
        parents[e->destination] = v;
      }
    double minimum = INFINITY;
    for (unsigned u = 0; u < g->size; u++)
      if (!processed[u] && weights[u] < minimum) {
        minimum = weights[u];
        v = u;
      }
    if (minimum == INFINITY) break;
    addUndirectedEdge(mst, parents[v], v, minimum);
  }
  free(processed);
  free(weights);
  free(parents);
  return mst;
}

Graph *contractVertices(const Graph *graph, unsigned u, unsigned v) {
  Graph *g = createGraph(graph->size - 1);
  for (unsigned x = 0; x < graph->size; x++) {
    unsigned y = x > v ? x - 1 : x == v ? u : x;
    for (Edge *e = graph->edges[x]; e != nullptr; e = e->next) {
      unsigned z = e->destination > v ? e->destination - 1 : e->destination == v ? u : e->destination;
      addDirectedEdge(g, y, z, e->weight);
    }
  }
  return g;
}

Graph *copySubgraph(const Graph *graph, const bool *subset) {
  Graph *g = createGraph(graph->size);
  for (unsigned v = 0; v < graph->size; v++)
    if (subset[v])
      for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
        if (subset[e->destination])
          addDirectedEdge(g, v, e->destination, e->weight);
  return g;
}

Graph *subgraphInducedByEdges(const Graph *graph, const bool *subset) {
  Graph *g = createGraph(graph->size);
  unsigned n = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next) {
      if (subset[n])
        addDirectedEdge(g, v, e->destination, e->weight);
      n++;
    }
  return g;
}

Graph *graphUnion(const Graph *g1, const Graph *g2) {
  Graph *g3 = createGraph(maximumUnsigned(g1->size, g2->size));
  for (unsigned v = 0; v < g1->size; v++)
    for (Edge *e = g1->edges[v]; e; e = e->next)
      addDirectedEdge(g3, v, e->destination, e->weight);
  for (unsigned v = 0; v < g2->size; v++)
    for (Edge *e = g2->edges[v]; e; e = e->next)
      if (!hasDirectedEdge(g3, v, e->destination))
        addDirectedEdge(g3, v, e->destination, e->weight);
  return g3;
}

Graph *cartesianProduct(const Graph *g1, const Graph *g2) {
  if (!g1 || !g1->edges || !g2 || !g2->edges) return nullptr;
  Graph *g3 = createGraph(g1->size * g2->size);
  for (unsigned u = 0; u < g1->size; u++)
    for (unsigned v = 0; v < g2->size; v++) {
      for (Edge *e = g2->edges[v]; e; e = e->next)
        addDirectedEdge(g3, u * g2->size + v, u * g2->size + e->destination, e->weight);
      for (Edge *e = g1->edges[u]; e; e = e->next)
        addDirectedEdge(g3, u * g2->size + v, e->destination * g2->size + v, e->weight);
    }
  return g3;
}

Graph *disjointUnion(const Graph *g1, const Graph *g2) {
  if (!g1 || !g1->edges || !g2 || !g2->edges) return nullptr;
  Graph *g3 = createGraph(g1->size + g2->size);
  for (unsigned v = 0; v < g1->size; v++)
    for (Edge *e = g1->edges[v]; e; e = e->next)
      addDirectedEdge(g3, v, e->destination, e->weight);
  for (unsigned v = 0; v < g2->size; v++)
    for (Edge *e = g2->edges[v]; e; e = e->next)
      addDirectedEdge(g3, g1->size + v, g1->size + e->destination, e->weight);
  return g3;
}

Graph *tensorProduct(const Graph *g1, const Graph *g2) {
  if (!g1 || !g1->edges || !g2 || !g2->edges) return nullptr;
  Graph *g3 = createGraph(g1->size * g2->size);
  for (unsigned u1 = 0; u1 < g1->size; u1++)
    for (Edge *e1 = g1->edges[u1]; e1; e1 = e1->next)
      for (unsigned u2 = 0; u2 < g2->size; u2++)
        for (Edge *e2 = g2->edges[u2]; e2; e2 = e2->next)
          addDirectedEdge(g3, u1 * g2->size + u2, e1->destination * g2->size + e2->destination, e1->weight + e2->weight);
  return g3;
}



void destroyGraph(Graph *g) {
  if (!g) return;
  if (g->edges)
    for (unsigned v = 0; v < g->size; v++) {
      Edge *e = g->edges[v];
      while (e) {
        Edge *next = e->next;
        free(e);
        e = next;
      }
    }
  free(g->edges);
  free(g);
}

void addVertex(Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return;
  g->edges = realloc(g->edges, (g->size + 1) * sizeof(Edge *));
  g->edges[g->size] = nullptr;
  g->size = g->size + 1;
}

void printGraph(const Graph *graph) {
  printf("{");
  unsigned i = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next) {
      if (i++ > 0)
        printf(", ");
      printf("(%u, %u, %lf)", v, e->destination, e->weight);
    }
  printf("}\n");
}

void removeDirectedEdgeByIndex(Graph *g, unsigned i) {
  if (!g || !g->edges) return;
  unsigned j = 0;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge **e = &g->edges[v]; *e; e = &(*e)->next) {
      if (j == i) {
        Edge *edge = *e;
        *e = edge->next;
        free(edge);
        return;
      }
      j++;
    }
}

void removeFirstDirectedEdge(Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size) return;
  Edge **e = &g->edges[u];
  while (*e) {
    Edge *temporary = *e;
    if (temporary->destination == v) {
      *e = temporary->next;
      free(temporary);
      break;
    } else {
      e = &temporary->next;
    }
  }
}

void removeFirstUndirectedEdge(Graph *graph, unsigned u, unsigned v) {
  assert(isUndirected(graph));
  double weight = edgeWeight(graph, u, v);
  removeFirstWeightedDirectedEdge(graph, u, v, weight);
  removeFirstWeightedDirectedEdge(graph, v, u, weight);
}

void subdivideEdge(Graph *graph, unsigned u, unsigned v) {
  double weight = edgeWeight(graph, u, v);
  removeFirstWeightedDirectedEdge(graph, u, v, weight);
  addVertex(graph);
  addDirectedEdge(graph, u, graph->size - 1, weight / 2);
  addDirectedEdge(graph, graph->size - 1, v, weight / 2);
}

void addDirectedEdge(Graph *g, unsigned u, unsigned v, double x) {
  if (!g || !g->edges || u >= g->size) return;
  Edge *e = malloc(sizeof(Edge));
  if (!e) return;
  e->destination = v;
  e->weight = x;
  e->next = g->edges[u];
  g->edges[u] = e;
}

void addUndirectedEdge(Graph *g, unsigned u, unsigned v, double x) {
  addDirectedEdge(g, u, v, x);
  addDirectedEdge(g, v, u, x);
}

void removeFirstWeightedDirectedEdge(Graph *graph, unsigned u, unsigned v, double weight) {
  assert(u < graph->size);
  assert(v < graph->size);
  assert(hasWeightedDirectedEdge(graph, u, v, weight));
  Edge **e = &graph->edges[u];
  while (*e != nullptr)
    if ((*e)->destination == v && (*e)->weight == weight) {
      Edge *temporary = *e;
      *e = (*e)->next;
      free(temporary);
      break;
    } else {
      e = &(*e)->next;
    }
}

void removeFirstWeightedUndirectedEdge(Graph *graph, unsigned u, unsigned v, double weight) {
  assert(isUndirected(graph));
  removeFirstWeightedDirectedEdge(graph, u, v, weight);
  removeFirstWeightedDirectedEdge(graph, v, u, weight);
}



unsigned countEdges(const Graph *g) {
  if (!g || !g->edges) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      n++;
  return n;
}

unsigned countSelfLoops(const Graph *g) {
  if (!g || !g->edges) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination == v)
        n++;
  return n;
}

unsigned countTriangles(const Graph *graph) {
  unsigned count = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *d = graph->edges[v]; d != nullptr; d = d->next)
      if (d->destination != v)
        for (Edge *e = graph->edges[d->destination]; e != nullptr; e = e->next)
          if (e->destination != v && e->destination != d->destination)
            for (Edge *f = graph->edges[e->destination]; f != nullptr; f = f->next)
              if (f->destination == v)
                count++;
  return count;
}

unsigned minimumInDegree(const Graph *g) {
  if (!g) return UINT_MAX;
  unsigned *inDegree = inDegrees(g);
  if (!inDegree) return UINT_MAX;
  unsigned minimum = UINT_MAX;
  for (unsigned v = 0; v < g->size; v++)
    if (inDegree[v] < minimum)
      minimum = inDegree[v];
  free(inDegree);
  return minimum;
}

unsigned maximumInDegree(const Graph *g) {
  if (!g) return 0;
  unsigned *inDegree = inDegrees(g);
  if (!inDegree) return 0;
  unsigned maximum = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (inDegree[v] > maximum)
      maximum = inDegree[v];
  free(inDegree);
  return maximum;
}

unsigned minimumOutDegree(const Graph *g) {
  if (!g) return UINT_MAX;
  unsigned minimum = UINT_MAX;
  for (unsigned v = 0; v < g->size; v++)
    if (outDegree(g, v) < minimum)
      minimum = outDegree(g, v);
  return minimum;
}

unsigned maximumOutDegree(const Graph *g) {
  if (!g) return 0;
  unsigned maximum = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (outDegree(g, v) > maximum)
      maximum = outDegree(g, v);
  return maximum;
}

unsigned minimumDegree(const Graph *g) {
  return minimumInDegree(g) + minimumOutDegree(g);
}

unsigned maximumDegree(const Graph *g) {
  return maximumInDegree(g) + maximumOutDegree(g);
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
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      if (seen[e->destination])
        count++;
      else
        seen[e->destination] = true;
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
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

unsigned getSize(const Graph *g) {
  if (!g) return 0;
  return g->size;
}

unsigned countComponents(const Graph *g) {
  unsigned n = 0;
  if (g && g->edges) {
    bool *visited = calloc(g->size, sizeof(bool));
    unsigned *stack = malloc(g->size * sizeof(unsigned));
    if (visited && stack)
      for (unsigned u = 0; u < g->size; u++)
        if (!visited[u]) {
          n++;
          unsigned size = 0;
          stack[size++] = u;
          visited[u] = true;
          while (size > 0)
            for (Edge *e = g->edges[stack[--size]]; e; e = e->next)
              if (e->destination < g->size && !visited[e->destination]) {
                visited[e->destination] = true;
                stack[size++] = e->destination;
              }
        }
    free(stack);
    free(visited);
  }
  return n;
}

unsigned firstActiveVertex(const Graph *g) {
  if (!g || !g->edges) return UINT_MAX;
  for (unsigned v = 0; v < g->size; v++)
    if (g->edges[v])
      return v;
  return UINT_MAX;
}

unsigned wienerIndex(const Graph *g) {
  if (!g) return 0;
  unsigned sum = 0;
  for (unsigned u = 0; u < g->size; u++) {
    unsigned *distances = unweightedDijkstra(g, u);
    if (!distances) continue;
    for (unsigned v = u + 1; v < g->size; v++)
      if (distances[v] != UINT_MAX)
        sum += distances[v];
    free(distances);
  }
  return sum;
}

unsigned outDegree(const Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return 0;
  unsigned degree = 0;
  for (Edge *e = g->edges[v]; e; e = e->next)
    degree++;
  return degree;
}

unsigned inDegree(const Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return 0;
  unsigned degree = 0;
  for (unsigned u = 0; u < g->size; u++)
    for (Edge *e = g->edges[u]; e; e = e->next)
      if (e->destination == v)
        degree++;
  return degree;
}

unsigned degree(const Graph *g, unsigned v) {
  return inDegree(g, v) + outDegree(g, v);
}

unsigned getNeighbor(const Graph *g, unsigned v, unsigned i) {
  if (!g || v >= g->size) return UINT_MAX;
  unsigned j = 0;
  for (Edge *e = g->edges[v]; e; e = e->next) {
    if (j == i) return e->destination;
    j++;
  }
  return UINT_MAX;
}

unsigned countCommonNeighbors(const Graph *graph, unsigned u, unsigned v) {
  bool *neighbors = calloc(graph->size, sizeof(bool));
  for (Edge *e = graph->edges[u]; e != nullptr; e = e->next)
    neighbors[e->destination] = true;
  unsigned n = 0;
  for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
    if (neighbors[e->destination]) {
      n++;
      neighbors[e->destination] = false;
    }
  free(neighbors);
  return n;
}

unsigned countShortestPaths(const Graph *graph, unsigned source, unsigned target) {
  if (graph == nullptr || source >= graph->size || target >= graph->size)
    return 0;
  double *distance = malloc(graph->size * sizeof(*distance));
  unsigned *paths = malloc(graph->size * sizeof(*paths));
  bool *visited = malloc(graph->size * sizeof(*visited));
  for (unsigned v = 0; v < graph->size; v++) {
    distance[v] = INFINITY;
    paths[v] = 0;
    visited[v] = false;
  }
  distance[source] = 0;
  paths[source] = 1;
  for (unsigned count = 0; count < graph->size; count++) {
    unsigned u = graph->size;
    double minimum = INFINITY;
    for (unsigned v = 0; v < graph->size; v++) {
      if (!visited[v] && distance[v] < minimum) {
        minimum = distance[v];
        u = v;
      }
    }
    if (u == graph->size || u == target)
      break;
    visited[u] = true;
    for (const Edge *e = graph->edges[u]; e != nullptr; e = e->next)
      if (!visited[e->destination]) {
        if (distance[u] + e->weight < distance[e->destination]) {
          distance[e->destination] = distance[u] + e->weight;
          paths[e->destination] = paths[u];
        } else if (distance[u] + e->weight == distance[e->destination]) {
          paths[e->destination] += paths[u];
        }
      }
  }
  unsigned n = paths[target];
  free(distance);
  free(paths);
  free(visited);
  return n;
}

unsigned distance(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size) return UINT_MAX;
  bool *visited = calloc(g->size, sizeof(bool));
  unsigned *distances = malloc(g->size * sizeof(unsigned));
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  if (!visited || !distances || !queue) {
    free(visited);
    free(distances);
    free(queue);
    return UINT_MAX;
  }
  visited[u] = true;
  for (unsigned v = 0; v < g->size; v++) distances[v] = UINT_MAX;
  distances[u] = 0;
  unsigned head = 0;
  unsigned tail = 0;
  queue[tail++] = u;
  while (head < tail) {
    unsigned current = queue[head++];
    if (current == v) break;
    for (Edge *e = g->edges[current]; e; e = e->next)
      if (e->destination < g->size && !visited[e->destination]) {
        visited[e->destination] = true;
        distances[e->destination] = distances[current] + 1;
        queue[tail++] = e->destination;
      }
  }
  unsigned distance = distances[v];
  free(visited);
  free(distances);
  free(queue);
  return distance;
}

unsigned countMatchingEdges(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size) return 0;
  unsigned n = 0;
  for (Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination == v)
      n++;
  return n;
}

unsigned countMatchingWeightedEdges(const Graph *g, unsigned u, unsigned v, double x) {
  if (!g || !g->edges || u >= g->size) return 0;
  unsigned n = 0;
  for (Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination == v && e->weight == x)
      n++;
  return n;
}



unsigned *inDegrees(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  unsigned *degrees = calloc(g->size, sizeof(unsigned));
  if (!degrees) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size)
        degrees[e->destination]++;
  return degrees;
}

unsigned *outDegrees(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  unsigned *degrees = calloc(g->size, sizeof(unsigned));
  if (!degrees) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    degrees[v] = outDegree(g, v);
  return degrees;
}

unsigned *inDegreeDistribution(const Graph *g) {
  if (!g) return nullptr;
  unsigned *degree = inDegrees(g);
  unsigned *distribution = calloc(g->size, sizeof(unsigned));
  if (!degree || !distribution) {
    free(degree); free(distribution);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++)
    distribution[degree[v] < g->size ? degree[v] : g->size - 1]++;
  free(degree);
  return distribution;
}

unsigned *outDegreeDistribution(const Graph *g) {
  if (!g) return nullptr;
  unsigned *distribution = calloc(g->size, sizeof(unsigned));
  if (!distribution) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    distribution[outDegree(g, v) < g->size ? outDegree(g, v) : g->size - 1]++;
  return distribution;
}

unsigned *undirectedColoring(const Graph *g) {
  if (!isValid(g)) return nullptr;
  unsigned *colors = malloc(g->size  *sizeof(unsigned));
  bool *taken = calloc(g->size, sizeof(bool));
  if (!colors || !taken) {
    free(colors);
    free(taken);
    return nullptr;
  }
  colors[0] = 0;
  for (unsigned v = 1; v < g->size; v++)
    colors[v] = UINT_MAX;
  for (unsigned v = 1; v < g->size; v++) {
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (colors[e->destination] < UINT_MAX)
        taken[colors[e->destination]] = true;
    unsigned color = 0;
    while (taken[color])
      color++;
    colors[v] = color;
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (colors[e->destination] < UINT_MAX)
        taken[colors[e->destination]] = false;
  }
  free(taken);
  return colors;
}

unsigned *stronglyConnectedComponents(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination >= g->size)
        return nullptr;
  unsigned *components = malloc(g->size * sizeof(unsigned));
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  bool *canReach = malloc(g->size * sizeof(bool));
  bool *canBeReachedBy = malloc(g->size * sizeof(bool));
  if (!components || !queue || !canReach || !canBeReachedBy) {
    free(components);
    free(queue);
    free(canReach);
    free(canBeReachedBy);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++) components[v] = UINT_MAX;
  unsigned component = 0;
  for (unsigned u = 0; u < g->size; u++) {
    if (components[u] != UINT_MAX) continue;
    for (unsigned v = 0; v < g->size; v++) canReach[v] = false;
    unsigned head = 0, tail = 0;
    canReach[u] = true;
    queue[tail++] = u;
    while (head < tail)
      for (Edge *e = g->edges[queue[head++]]; e; e = e->next)
        if (!canReach[e->destination]) {
          canReach[e->destination] = true;
          queue[tail++] = e->destination;
        }
    for (unsigned v = 0; v < g->size; v++) canBeReachedBy[v] = false;
    head = 0; tail = 0;
    canBeReachedBy[u] = true;
    queue[tail++] = u;
    while (head < tail) {
      unsigned target = queue[head++];
      for (unsigned v = 0; v < g->size; v++)
        for (Edge *e = g->edges[v]; e; e = e->next)
          if (e->destination == target && !canBeReachedBy[v]) {
            canBeReachedBy[v] = true;
            queue[tail++] = v;
          }
    }
    for (unsigned v = 0; v < g->size; v++)
      if (canReach[v] && canBeReachedBy[v])
        components[v] = component;
    component++;
  }
  free(queue);
  free(canReach);
  free(canBeReachedBy);
  return components;
}

void topologicalSortOfGraphComponent(const Graph *g, unsigned v, unsigned *ordering, unsigned *i, bool *visited) {
  visited[v] = true;
  for (Edge *e = g->edges[v]; e; e = e->next)
    if (!visited[e->destination])
      topologicalSortOfGraphComponent(g, e->destination, ordering, i, visited);
  ordering[--(*i)] = v;
}

unsigned *topologicalSortOfGraph(const Graph *g) {
  if (!isValid(g)) return nullptr;
  unsigned *ordering = malloc(g->size * sizeof(unsigned));
  bool *visited = calloc(g->size, sizeof(bool));
  if (g->size > 0 && (!ordering || !visited)) return nullptr;
  unsigned i = g->size;
  for (unsigned v = 0; v < g->size; v++)
    if (!visited[v])
      topologicalSortOfGraphComponent(g, v, ordering, &i, visited);
  free(visited);
  return ordering;
}

unsigned *findBridges(const Graph *g) {
  if (!isValid(g)) return nullptr;
  unsigned *bridges = malloc((2 * g->size + 1) * sizeof(unsigned));
  unsigned *discovery = calloc(g->size, sizeof(unsigned));
  unsigned *low = malloc(g->size * sizeof(unsigned));
  unsigned *parent = malloc(g->size * sizeof(unsigned));
  bool *skipped = calloc(g->size, sizeof(bool));
  Edge **edge = malloc(g->size * sizeof(Edge*));
  unsigned *stack = malloc(g->size * sizeof(unsigned));
  if (!bridges || (g->size > 0 && (!discovery || !low || !parent || !skipped || !edge || !stack))) {
    free(bridges);
    free(discovery);
    free(low);
    free(parent);
    free(skipped);
    free(edge);
    free(stack);
    return nullptr;
  }
  unsigned timer = 0;
  unsigned count = 0;
  for (unsigned v = 0; v < g->size; v++) {
    if (discovery[v] != 0) continue;
    unsigned top = 0;
    discovery[v] = low[v] = ++timer;
    parent[v] = UINT_MAX;
    edge[v] = g->edges[v];
    stack[top++] = v;
    while (top > 0) {
      unsigned u = stack[top - 1];
      if (edge[u]) {
        Edge *e = edge[u];
        edge[u] = e->next;
        if (e->destination == parent[u] && !skipped[u]) {
          skipped[u] = true;
          continue;
        }
        if (discovery[e->destination] > 0) {
          if (discovery[e->destination] < low[u])
            low[u] = discovery[e->destination];
        } else {
          discovery[e->destination] = low[e->destination] = ++timer;
          parent[e->destination] = u;
          edge[e->destination] = g->edges[e->destination];
          stack[top++] = e->destination;
        }
      } else {
        top--;
        if (top > 0 && parent[u] != UINT_MAX) {
          if (low[u] < low[parent[u]])
            low[parent[u]] = low[u];
          if (low[u] > discovery[parent[u]]) {
            bridges[count++] = parent[u];
            bridges[count++] = u;
          }
        }
      }
    }
  }
  bridges[count++] = UINT_MAX;
  unsigned *trimmed = realloc(bridges, count * sizeof(unsigned));
  if (trimmed) bridges = trimmed;
  free(discovery);
  free(low);
  free(parent);
  free(skipped);
  free(edge);
  free(stack);
  return bridges;
}

unsigned *unweightedDijkstra(const Graph *g, unsigned v) {
  if (!isValid(g) || v >= g->size) return nullptr;
  unsigned *distances = malloc(g->size * sizeof(unsigned));
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  if (!distances || !queue) {
    free(distances);
    free(queue);
    return nullptr;
  }
  for (unsigned u = 0; u < g->size; u++)
    distances[u] = UINT_MAX;
  distances[v] = 0;
  unsigned head = 0, tail = 0;
  queue[tail++] = v;
  while (head < tail) {
    unsigned u = queue[head++];
    for (Edge *e = g->edges[u]; e; e = e->next)
      if (distances[e->destination] == UINT_MAX) {
        distances[e->destination] = distances[u] + 1;
        queue[tail++] = e->destination;
      }
  }
  free(queue);
  return distances;
}

unsigned *getInNeighbors(const Graph *graph, unsigned vertex) {
  unsigned *neighbors = malloc(inDegree(graph, vertex) * sizeof(unsigned));
  unsigned i = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      if (e->destination == vertex)
        neighbors[i++] = v;
  return neighbors;
}

unsigned *getOutNeighbors(const Graph *graph, unsigned vertex) {
  unsigned *neighbors = malloc(outDegree(graph, vertex) * sizeof(unsigned));
  unsigned i = 0;
  for (Edge *e = graph->edges[vertex]; e != nullptr; e = e->next)
    neighbors[i++] = e->destination;
  return neighbors;
}

unsigned *preOrderSort(const Graph *g, unsigned v) {
  if (!isValid(g) || v >= g->size) return nullptr;
  bool *visited = calloc(g->size, sizeof(bool));
  unsigned *result = malloc(g->size * sizeof(unsigned));
  unsigned *vertices = malloc(g->size * sizeof(unsigned));
  Edge **edges = malloc(g->size * sizeof(Edge*));
  if (!visited || !result || !vertices || !edges) {
    free(visited); free(result); free(vertices); free(edges);
    return nullptr;
  }
  unsigned i = 0;
  unsigned top = 0;
  for (unsigned u = 0; u < g->size; u++) {
    unsigned start = u == 0 ? v : u <= v ? u - 1 : u;
    if (visited[start]) continue;
    visited[start] = true;
    result[i++] = start;
    vertices[top] = start;
    edges[top] = g->edges[start];
    top++;
    while (top > 0) {
      Edge *e = edges[top - 1];
      if (e) {
        edges[top - 1] = e->next;
        if (!visited[e->destination]) {
          visited[e->destination] = true;
          result[i++] = e->destination;
          vertices[top] = e->destination;
          edges[top] = g->edges[e->destination];
          top++;
        }
      } else {
        top--;
      }
    }
  }
  free(visited);
  free(vertices);
  free(edges);
  return result;
}

unsigned *postOrderSort(const Graph *g, unsigned v) {
  if (!isValid(g) || v >= g->size) return nullptr;
  bool *visited = calloc(g->size, sizeof(bool));
  unsigned *result = malloc(g->size * sizeof(unsigned));
  unsigned *vertices = malloc(g->size * sizeof(unsigned));
  Edge **edges = malloc(g->size * sizeof(Edge*));
  if (!visited || !result || !vertices || !edges) {
    free(visited); free(result); free(vertices); free(edges);
    return nullptr;
  }
  unsigned i = g->size;
  unsigned top = 0;
  for (unsigned u = 0; u < g->size; u++) {
    unsigned start = u == 0 ? v : u <= v ? u - 1 : u;
    if (visited[start]) continue;
    visited[start] = true;
    vertices[top] = start;
    edges[top] = g->edges[start];
    top++;
    while (top > 0) {
      Edge *e = edges[top - 1];
      if (e) {
        edges[top - 1] = e->next;
        if (!visited[e->destination]) {
          visited[e->destination] = true;
          vertices[top] = e->destination;
          edges[top] = g->edges[e->destination];
          top++;
        }
      } else {
        result[--i] = vertices[--top];
      }
    }
  }
  free(visited);
  free(vertices);
  free(edges);
  return result;
}

unsigned *breadthFirstSort(const Graph *g, unsigned v) {
  if (!isValid(g) || v >= g->size) return nullptr;
  bool *visited = calloc(g->size, sizeof(bool));
  unsigned *result = malloc(g->size * sizeof(unsigned));
  if (!visited || !result) {
    free(visited); free(result);
    return nullptr;
  }
  unsigned i = 0;
  unsigned head = 0;
  unsigned tail = 0;
  for (unsigned u = 0; u < g->size; u++) {
    unsigned start = u == 0 ? v : u <= v ? u - 1 : u;
    if (visited[start]) continue;
    visited[start] = true;
    result[i++] = start;
    while (head < tail) {
      unsigned current = result[head++];
      for (Edge *e = g->edges[current]; e; e = e->next)
        if (!visited[e->destination]) {
          visited[e->destination] = true;
          result[i++] = e->destination;
        }
    }
  }
  free(visited);
  return result;
}

unsigned *shortestPath(const Graph *graph, unsigned source, unsigned target, unsigned *length) {
  *length = 0;
  if (source >= graph->size || target >= graph->size)
    return nullptr;
  double *distance = malloc(graph->size * sizeof(double));
  unsigned *parent = malloc(graph->size * sizeof(unsigned));
  bool *visited = malloc(graph->size * sizeof(bool));
  for (unsigned v = 0; v < graph->size; v++) {
    distance[v] = INFINITY;
    parent[v] = UINT_MAX;
    visited[v] = false;
  }
  distance[source] = 0;
  for (unsigned count = 0; count < graph->size; count++) {
    unsigned u = UINT_MAX;
    double minimum = INFINITY;
    for (unsigned v = 0; v < graph->size; v++)
      if (!visited[v] && distance[v] < minimum) {
        minimum = distance[v];
        u = v;
      }
    if (u == UINT_MAX || u == target)
      break;
    visited[u] = true;
    for (Edge *e = graph->edges[u]; e; e = e->next)
      if (!visited[e->destination] && distance[u] + e->weight < distance[e->destination]) {
        distance[e->destination] = distance[u] + e->weight;
        parent[e->destination] = u;
      }
  }
  unsigned *path = nullptr;
  if (distance[target] != INFINITY) {
    unsigned v = target;
    do {
      (*length)++;
      v = parent[v];
    } while (v != UINT_MAX);
    path = malloc((*length) * sizeof(unsigned));
    v = target;
    unsigned i = *length;
    do {
      path[--i] = v;
      v = parent[v];
    } while (v != UINT_MAX);
  }
  free(distance);
  free(parent);
  free(visited);
  return path;
}



unsigned **allPairsShortestPathsUnweighted(const Graph *graph) {
  if (graph == nullptr || graph->size == 0)
    return nullptr;
  unsigned **distances = malloc(graph->size * sizeof(unsigned *));
  for (unsigned u = 0; u < graph->size; u++) {
    distances[u] = malloc(graph->size * sizeof(unsigned));
    for (unsigned v = 0; v < graph->size; v++)
      distances[u][v] = UINT_MAX;
  }
  unsigned *queue = malloc(graph->size * sizeof(unsigned));
  bool *visited = malloc(graph->size * sizeof(bool));
  for (unsigned start = 0; start < graph->size; start++) {
    for (unsigned v = 0; v < graph->size; v++)
      visited[v] = false;
    unsigned head = 0;
    unsigned tail = 0;
    distances[start][start] = 0;
    visited[start] = true;
    queue[tail++] = start;
    while (head < tail) {
      unsigned v = queue[head++];
      for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
        if (!visited[e->destination]) {
          visited[e->destination] = true;
          distances[start][e->destination] = distances[start][v] + 1;
          queue[tail++] = e->destination;
        }
    }
  }
  free(queue);
  free(visited);
  return distances;
}



double sumWeights(const Graph *graph) {
  double sum = 0;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      sum += e->weight;
  return sum;
}

double graphRadius(const Graph *graph) {
  double radius = INFINITY;
  for (unsigned v = 0; v < graph->size; v++) {
    double eccentricity = graphEccentricity(graph, v);
    if (eccentricity < radius)
      radius = eccentricity;
  }
  return radius;
}

double graphDiameter(const Graph *graph) {
  double diameter = -INFINITY;
  for (unsigned v = 0; v < graph->size; v++) {
    double eccentricity = graphEccentricity(graph, v);
    if (eccentricity > diameter)
      diameter = eccentricity;
  }
  return diameter;
}

double graphDensity(const Graph *graph) {
  if (graph->size < 2)
    return 0;
  return (double)countEdges(graph) / graph->size / (graph->size - 1);
}

double averageClusteringCoefficient(const Graph *graph) {
  if (graph->size == 0)
    return 0;
  double total = 0;
  for (unsigned v = 0; v < graph->size; v++)
    total += localClusteringCoefficient(graph, v);
  return total / graph->size;
}

double graphGirth(const Graph *graph) {
  double girth = INFINITY;
  if (graph == nullptr || graph->size == 0)
    return girth;
  double *distance = malloc(graph->size * sizeof(double));
  unsigned *parent = malloc(graph->size * sizeof(unsigned));
  unsigned *queue = malloc(graph->size * sizeof(unsigned));
  for (unsigned start = 0; start < graph->size; start++) {
    for (unsigned v = 0; v < graph->size; v++) {
      distance[v] = INFINITY;
      parent[v] = graph->size;
    }
    unsigned head = 0;
    unsigned tail = 0;
    distance[start] = 0;
    queue[tail++] = start;
    while (head < tail) {
      unsigned v = queue[head++];
      for (const Edge *e = graph->edges[v]; e != nullptr; e = e->next)
        if (distance[e->destination] == INFINITY) {
          distance[e->destination] = distance[v] + e->weight;
          parent[e->destination] = v;
          queue[tail++] = e->destination;
        } else if (parent[v] != e->destination && parent[e->destination] != v) {
          double length = distance[v] + distance[e->destination] + e->weight;
          if (length < girth)
            girth = length;
        }
    }
  }
  free(distance);
  free(parent);
  free(queue);
  return girth;
}

double minimumEdgeWeight(const Graph *graph) {
  double minimum = INFINITY;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      if (e->weight < minimum)
        minimum = e->weight;
  return minimum;
}

double maximumEdgeWeight(const Graph *graph) {
  double maximum = -INFINITY;
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      if (e->weight > maximum)
        maximum = e->weight;
  return maximum;
}

double graphEccentricity(const Graph *graph, unsigned vertex) {
  double *distance = weightedDijkstra(graph, vertex);
  double eccentricity = -INFINITY;
  for (unsigned v = 0; v < graph->size; v++)
    if (distance[v] > eccentricity)
      eccentricity = distance[v];
  free(distance);
  return eccentricity;
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
  for (Edge *d = graph->edges[vertex]; d != nullptr; d = d->next)
    for (Edge *e = graph->edges[vertex]; e != nullptr; e = e->next)
      if (hasDirectedEdge(graph, d->destination, e->destination))
        edgesBetweenNeighbours++;
  return (double)edgesBetweenNeighbours / outDegree(graph, vertex) / (outDegree(graph, vertex) - 1);
}

double edgeWeight(const Graph *graph, unsigned u, unsigned v) {
  if (u >= graph->size || v >= graph->size)
    return 0;
  for (Edge *e = graph->edges[u]; e != nullptr; e = e->next)
    if (e->destination == v)
      return e->weight;
  return 0;
}

double maxFlowEdmondsKarp(const Graph *g, unsigned source, unsigned sink) {
  if (!isValid(g) || source >= g->size || sink >= g->size) return 0;
  double **residual = malloc(g->size * sizeof(double *));
  if (residual) for (unsigned v = 0; v < g->size; v++) residual[v] = calloc(g->size, sizeof(double));
  unsigned *parent = malloc(g->size * sizeof(unsigned));
  bool *visited = malloc(g->size * sizeof(bool));
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  bool allocated = residual && parent && visited && queue;
  for (unsigned v = 0; v < g->size && allocated; v++) allocated = allocated && residual[v];
  if (!allocated) {
    if (residual) for (unsigned v = 0; v < g->size; v++) free(residual[v]);
    free(residual); free(parent); free(visited); free(queue);
    return 0;
  }
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      residual[v][e->destination] += e->weight;
  double max = 0;
  while (true) {
    for (unsigned v = 0; v < g->size; v++) visited[v] = false;
    unsigned head = 0;
    unsigned tail = 0;
    bool found = false;
    queue[tail++] = source;
    visited[source] = true;
    parent[source] = source;
    while (head < tail && !found) {
      unsigned u = queue[head++];
      for (unsigned v = 0; v < g->size && !found; v++)
        if (!visited[v] && residual[u][v] > 0) {
          queue[tail++] = v;
          parent[v] = u;
          visited[v] = true;
          if (v == sink) found = true;
        }
    }
    if (!found) break;
    double flow = INFINITY;
    unsigned v = sink;
    while (v != source) {
      if (residual[parent[v]][v] < flow) flow = residual[parent[v]][v];
      v = parent[v];
    }
    v = sink;
    while (v != source) {
      residual[parent[v]][v] -= flow;
      residual[v][parent[v]] += flow;
      v = parent[v];
    }
    max += flow;
  }
  for (unsigned v = 0; v < g->size; v++) free(residual[v]);
  free(residual);
  free(parent);
  free(visited);
  free(queue);
  return max;
}

double subgraphDensity(const Graph *graph, const bool *subset) {
  unsigned vertices = 0;
  unsigned edges = 0;
  for (unsigned v = 0; v < graph->size; v++)
    if (subset[v]) {
      vertices++;
      for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
        if (subset[e->destination])
          edges++;
    }
  if (vertices < 2)
    return 0;
  return (double)edges / vertices / (vertices - 1);
}

double pathWeight(const Graph *graph, const unsigned *path, unsigned length) {
  double weight = 0;
  for (unsigned i = 1; i < length; i++)
    weight += edgeWeight(graph, path[i - 1], path[i]);
  return weight;
}



double *closenessCentrality(const Graph *graph) {
  if (graph == nullptr) return nullptr;
  double *centrality = calloc(graph->size, sizeof(double));
  double **distance = floydWarshall(graph);
  for (unsigned u = 0; u < graph->size; u++) {
    double total = 0;
    unsigned count = 0;
    for (unsigned v = 0; v < graph->size; v++)
      if (u != v && distance[u][v] != INFINITY) {
        total += distance[u][v];
        count++;
      }
    if (total > 0 && count > 0)
      centrality[u] = count / total;
    else
      centrality[u] = 0;
  }
  freeMatrix(distance, graph->size);
  return centrality;
}

double *bellmanFord(const Graph *graph, unsigned source) {
  double *distance = malloc(graph->size * sizeof(double));
  for (unsigned v = 0; v < graph->size; v++)
    distance[v] = INFINITY;
  distance[source] = 0;
  for (unsigned i = 1; i < graph->size; i++)
    for (unsigned v = 0; v < graph->size; v++)
      if (distance[v] < INFINITY)
        for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
          if (distance[v] + e->weight < distance[e->destination])
            distance[e->destination] = distance[v] + e->weight;
  for (unsigned v = 0; v < graph->size && distance != nullptr; v++)
    if (distance[v] < INFINITY)
      for (Edge *e = graph->edges[v]; e != nullptr && distance != nullptr; e = e->next)
        if (distance[v] + e->weight < distance[e->destination]) {
          free(distance);
          distance = nullptr;
        }
  return distance;
}

double *weightedDijkstra(const Graph *g, unsigned v) {
  if (!isValid(g) || v >= g->size) return nullptr;
  double *distances = malloc(g->size * sizeof(double));
  bool *visited = malloc(g->size * sizeof(bool));
  if (!distances || !visited) {
    free(distances);
    free(visited);
    return nullptr;
  }
  for (unsigned u = 0; u < g->size; u++) {
    distances[u] = INFINITY;
    visited[u] = false;
  }
  distances[v] = 0;
  for (;;) {
    visited[v] = true;
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (!visited[e->destination] && distances[v] + e->weight < distances[e->destination])
        distances[e->destination] = distances[v] + e->weight;
    double minimum = INFINITY;
    for (unsigned u = 0; u < g->size; u++)
      if (!visited[u] && distances[u] < minimum) {
        minimum = distances[u];
        v = u;
      }
    if (minimum == INFINITY) break;
  }
  free(visited);
  return distances;
}



double **toMatrix(const Graph *graph) {
  double **matrix = malloc(graph->size * sizeof(double *));
  for (unsigned v = 0; v < graph->size; v++)
    matrix[v] = calloc(graph->size, sizeof(double));
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next)
      matrix[v][e->destination] = e->weight;
  return matrix;
}

double **floydWarshall(const Graph *graph) {
  double **distance = malloc(graph->size * sizeof(double *));
  for (unsigned i = 0; i < graph->size; i++) {
    distance[i] = malloc(graph->size * sizeof(double));
    for (unsigned j = 0; j < graph->size; j++)
      if (i == j)
        distance[i][j] = 0;
      else
        distance[i][j] = INFINITY;
  }
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e; e = e->next)
      if (e->weight < distance[v][e->destination])
        distance[v][e->destination] = e->weight;
  for (unsigned k = 0; k < graph->size; k++)
    for (unsigned i = 0; i < graph->size; i++)
      for (unsigned j = 0; j < graph->size; j++)
        if (distance[i][k] < INFINITY && distance[k][j] < INFINITY && distance[i][k] + distance[k][j] < distance[i][j])
          distance[i][j] = distance[i][k] + distance[k][j];
  return distance;
}



Edge **inNeighbors(const Graph *graph) {
  Edge **neighbors = calloc(graph->size, sizeof(Edge *));
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next) {
      Edge *neighbor = malloc(sizeof(Edge));
      neighbor->destination = v;
      neighbor->weight = e->weight;
      neighbor->next = neighbors[e->destination];
      neighbors[e->destination] = neighbor;
    }
  return neighbors;
}

Edge **outNeighbors(const Graph *graph) {
  Edge **neighbors = calloc(graph->size, sizeof(Edge *));
  for (unsigned v = 0; v < graph->size; v++)
    for (Edge *e = graph->edges[v]; e != nullptr; e = e->next) {
      Edge *neighbor = malloc(sizeof(Edge));
      neighbor->destination = e->destination;
      neighbor->weight = e->weight;
      neighbor->next = neighbors[v];
      neighbors[v] = neighbor;
    }
  return neighbors;
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

void testIsWeaklyConnected() {
  Graph *g1 = createGraph(3);
  addDirectedEdge(g1, 0, 1, 1);
  addDirectedEdge(g1, 1, 2, 1);
  assert(isWeaklyConnected(g1) == true);
  printf("Weakly Test 1 passed: Simple chain\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addDirectedEdge(g2, 0, 1, 1);
  addDirectedEdge(g2, 2, 1, 1);
  assert(isWeaklyConnected(g2) == true);
  printf("Weakly Test 2 passed: Source/Sink structure\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(4);
  addDirectedEdge(g3, 0, 1, 1);
  addDirectedEdge(g3, 2, 3, 1);
  assert(isWeaklyConnected(g3) == false);
  printf("Weakly Test 3 passed: Truly disconnected components\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(4);
  addDirectedEdge(g4, 0, 1, 1);
  addDirectedEdge(g4, 0, 2, 1);
  addDirectedEdge(g4, 0, 3, 1);
  assert(isWeaklyConnected(g4) == true);
  printf("Weakly Test 4 passed: Star pattern\n");
  destroyGraph(g4);

  Graph *g5 = createGraph(2);
  addDirectedEdge(g5, 0, 0, 1);
  assert(isWeaklyConnected(g5) == false);
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

void testTopologicalSortOfGraph() {
  Graph *g1 = createGraph(3);
  addDirectedEdge(g1, 0, 1, 1);
  addDirectedEdge(g1, 1, 2, 1);
  unsigned *order1 = topologicalSortOfGraph(g1);
  assert(isTopologicalSort(g1, order1));
  printf("Topo test 1 (linear) passed!\n");
  destroyGraph(g1);
  free(order1);

  Graph *g2 = createGraph(4);
  addDirectedEdge(g2, 0, 1, 1);
  addDirectedEdge(g2, 0, 2, 1);
  addDirectedEdge(g2, 1, 3, 1);
  addDirectedEdge(g2, 2, 3, 1);
  unsigned *order2 = topologicalSortOfGraph(g2);
  assert(isTopologicalSort(g2, order2));
  printf("Topo test 2 (diamond) passed!\n");
  destroyGraph(g2);
  free(order2);

  Graph *g3 = createGraph(4);
  addDirectedEdge(g3, 0, 1, 1);
  addDirectedEdge(g3, 2, 3, 1);
  unsigned *order3 = topologicalSortOfGraph(g3);
  assert(isTopologicalSort(g3, order3));
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
  double *d1 = bellmanFord(g, 0);
  assert(d1 != nullptr);
  assert(d1[3] == 7);
  printf("Bellman-Ford test 1 (positive) passed!\n");
  free(d1);

  addDirectedEdge(g, 3, 1, -10);
  double *d2 = bellmanFord(g, 0);
  assert(d2 == nullptr);
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
    assert(distances[2] == UINT_MAX);

    printf("Passed: Unreachable node (UINT_MAX)\n");
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
  double *dist1 = weightedDijkstra(g1, 0);
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
  double *dist2 = weightedDijkstra(g2, 0);
  assert(dist2[2] == 5);
  printf("Dijkstra test 2 passed: Shortest path selection\n");
  destroyGraph(g2);
  free(dist2);

  Graph *g3 = createGraph(2);
  double *dist3 = weightedDijkstra(g3, 0);
  assert(dist3[0] == 0);
  assert(dist3[1] == INFINITY);
  printf("Dijkstra test 3 passed: Unreachable vertex (INFINITY)\n");
  destroyGraph(g3);
  free(dist3);

  Graph *g4 = createGraph(3);
  addDirectedEdge(g4, 0, 1, 1);
  addDirectedEdge(g4, 1, 2, 1);
  addDirectedEdge(g4, 2, 0, 1);
  double *dist4 = weightedDijkstra(g4, 0);
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

    double **distances = floydWarshall(g);

    assert(distances[0][1] == 10);
    assert(distances[1][2] == 5);
    assert(distances[0][2] == 15);
    assert(distances[2][0] == INFINITY);

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

    double **distances = floydWarshall(g);

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

    double **distances = floydWarshall(g);

    assert(distances[0][1] == 1);
    assert(distances[0][2] == INFINITY);
    assert(distances[3][0] == INFINITY);

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

    double **distances = floydWarshall(g);

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
    double w = sumWeights(mst) / 2;
    assert(countEdges(mst) == 4);
    assert(w == 4);
    printf("Prim test 1 (triangle) passed: weight %lf\n", w);
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
    double w = sumWeights(mst) / 2;
    assert(countEdges(mst) == 8);
    assert(w == 16);
    printf("Prim test 2 (complex) passed: weight %lf\n", w);
    destroyGraph(g);
    destroyGraph(mst);
  }
  {
    Graph *g = createGraph(1);
    Graph *mst = prim(g, 0);
    double w = sumWeights(mst);
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
  double weight = sumWeights(mst) / 2;
  assert(countEdges(mst) == 6);
  assert(weight == 19);
  printf("Kruskal test passed: weight %lf\n", weight);
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

void testGraphDensity() {
  Graph *graph = createGraph(65537);
  for (unsigned v = 0; v < 65536; v++)
    addDirectedEdge(graph, v, v + 1, 1);
  assert(graphDensity(graph) <= 0.99);
  destroyGraph(graph);
}

int main() {
  testIsDirectedCyclicGraph();
  testIsUndirectedCyclicGraph();
  testIsConnectedUndirected();
  testIsWeaklyConnected();
  testIsStronglyConnectedDirected();
  testTopologicalSortOfGraph();
  testBellmanFord();
  testUnweightedDijkstra();
  testWeightedDijkstra();
  testFloydWarshall();
  testPrim();
  testKruskal();
  testFindArticulationPoints();
  testGraphDensity();
  printf("All tests passed!\n");
  return 0;
}
