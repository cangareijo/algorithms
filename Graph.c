#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swapUnsigned(unsigned *a, unsigned *b);
unsigned unsignedMinimum(unsigned a, unsigned b);
unsigned unsignedMaximum(unsigned a, unsigned b);

bool **allocateFalseMatrix(unsigned m, unsigned n);
void freeBooleanMatrix(bool **matrix, unsigned m);

double calculateEuclideanNorm(double *a, unsigned n);

typedef struct {
  unsigned rows;
  unsigned columns;
  double **data;
} Matrix;

void destroyMatrix(Matrix *matrix);
bool isValidMatrix(const Matrix *matrix);
double calculateMatrixTrace(const Matrix *matrix);
double calculateMatrixDeterminant(Matrix *matrix);
Matrix *createIdentityMatrix(unsigned n);
Matrix *createZeroMatrix(unsigned rows, unsigned columns);
Matrix *copyMatrix(const Matrix *matrix);
Matrix *transposeMatrix(const Matrix *matrix);
Matrix *powerMatrix(const Matrix *matrix, unsigned k);
Matrix *addMatrices(const Matrix *A, const Matrix *B);
Matrix *multiplyMatrices(const Matrix *A, const Matrix *B);

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
bool isEmpty(const Graph *g);
bool isTrivial(const Graph *g);
bool isEdgeless(const Graph *g);
bool isRegular(const Graph *g);
bool isComplete(const Graph *g);
bool hasSelfLoops(const Graph *g);
bool isBalanced(const Graph *g);
bool isEulerianUndirected(const Graph *g);
bool isEulerianDirected(const Graph *g);
bool isWeaklyConnected(const Graph *g);
bool isStronglyConnected(const Graph *g);
bool isBipartite(const Graph *g);
bool isUndirected(const Graph *g);
bool hasParallelEdges(const Graph *g);
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
bool hasDirectedCycle(const Graph *g);
bool hasUndirectedCycle(const Graph *g);
bool hasNegativeCycle(const Graph *g);
bool isSelfComplementary(const Graph *g);
bool isChordal(const Graph *g);
bool isPerfect(const Graph *g);
bool isPlanar(const Graph *g);
bool isKRegular(const Graph *g, unsigned k);
bool isKConnected(const Graph *g, unsigned k);
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
bool canReachAll(const Graph *g, unsigned v);
bool isArticulationVertex(const Graph *g, unsigned v);
bool isSimplicial(const Graph *g, unsigned v);
bool hasDirectedEdge(const Graph *g, unsigned u, unsigned v);
bool hasUndirectedEdge(const Graph *g, unsigned u, unsigned v);
bool hasPath(const Graph *g, unsigned u, unsigned v);
bool haveCommonNeighbors(const Graph *g, unsigned u, unsigned v);
bool isDirectedBridge(const Graph *g, unsigned u, unsigned v);
bool isUndirectedBridge(const Graph *g, unsigned u, unsigned v);
bool hasWeightedDirectedEdge(const Graph *g, unsigned u, unsigned v, double x);
bool hasWeightedUndirectedEdge(const Graph *g, unsigned u, unsigned v, double x);
bool isTriangle(const Graph *g, unsigned u, unsigned v, unsigned w);
bool isClique(const Graph *g, const bool *set);
bool isIndependentSet(const Graph *g, const bool *set);
bool isVertexCover(const Graph *g, const bool *set);
bool hasDirectedEdges(const Graph *g, unsigned v, const bool *set);
bool hasUndirectedEdges(const Graph *g, unsigned v, const bool *set);
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
bool isSubGraph(const Graph *g1, const Graph *g2);
bool isSpanningDirectedTree(const Graph *g1, const Graph *g2);
bool isSpanningUndirectedTree(const Graph *g1, const Graph *g2);
bool isIsomorphic(const Graph *g1, const Graph *g2);

bool *graphCenter(const Graph *g);
bool *graphPeriphery(const Graph *g);
bool *findArticulationPoints(const Graph *g);
bool *findMaximalClique(const Graph *g);
bool *findMaximumClique(const Graph *g);
bool *getIsolatedVertices(const Graph *g);
bool *getSources(const Graph *g);
bool *getSinks(const Graph *g);
bool *findMaximalVertexCover(const Graph *g);
bool *findMinimumVertexCover(const Graph *g);
bool *getSelfLoops(const Graph *g);
bool *findMaximalIndependentSet(const Graph *g);
bool *findMaximumIndependentSet(const Graph *g);
bool *findFeedbackVertexSet(const Graph *g);
bool *getVertexCut(const Graph *g);
bool *getInNeighbors(const Graph *g, unsigned v);
bool *getOutNeighbors(const Graph *g, unsigned v);
bool *getVerticesReachableFrom(const Graph *g, unsigned v);
bool *getVerticesThatCanReach(const Graph *g, unsigned v);
bool *getCommonNeighbors(const Graph *g, unsigned u, unsigned v);
bool *getLocalVertexCut(const Graph *g, unsigned u, unsigned v);

bool **createAdjacencyMatrix(const Graph *g);
bool **findEdgeCut(const Graph *g);
bool **findLocalEdgeCut(const Graph *g, unsigned u, unsigned v);

Graph *createGraph(unsigned n);
Graph *createPath(unsigned n);
Graph *createCycle(unsigned n);
Graph *createStar(unsigned n);
Graph *createWheel(unsigned n);
Graph *createComplete(unsigned n);
Graph *createRandom(unsigned n, double p, bool directed, bool weighted);
Graph *copyGraph(const Graph *g);
Graph *createTranspose(const Graph *g);
Graph *createUnweighted(const Graph *g);
Graph *createUndirected(const Graph *g);
Graph *createComplement(const Graph *g);
Graph *createDirectedLine(const Graph *g);
Graph *createUndirectedLine(const Graph *g);
Graph *createUnderlying(const Graph *g);
Graph *createKruskal(const Graph *g);
Graph *createPrim(const Graph *g);
Graph *createDirectedSubdivision(const Graph *g);
Graph *createUndirectedSubdivision(const Graph *g);
Graph *createTransitiveClosure(const Graph *g);
Graph *findFeedbackArcSet(const Graph *g);
Graph *createPower(const Graph *g, unsigned k);
Graph *createVertexSubgraph(const Graph *g, const bool *set);
Graph *createEdgeSubgraph(const Graph *g, const bool *set);
Graph *createUnion(const Graph *g1, const Graph *g2);
Graph *createDisjointUnion(const Graph *g1, const Graph *g2);
Graph *createCartesianProduct(const Graph *g1, const Graph *g2);
Graph *createTensorProduct(const Graph *g1, const Graph *g2);
Graph *createLexicographicalProduct(const Graph *g1, const Graph *g2);

void destroyGraph(Graph *g);
void addVertex(Graph *g);
void deleteSelfLoops(Graph *g);
void deleteInvalidEdges(Graph *g);
void printGraph(const Graph *g);
void deleteDirectedEdgeByIndex(Graph *g, unsigned i);
void deleteOutgoingEdges(Graph *g, unsigned v);
void deleteIncomingEdges(Graph *g, unsigned v);
void deleteVertex(Graph *g, unsigned v);
void addDirectedEdge(Graph *g, unsigned u, unsigned v);
void deleteFirstDirectedEdge(Graph *g, unsigned u, unsigned v);
void deleteFirstUndirectedEdge(Graph *g, unsigned u, unsigned v);
void deleteMatchingEdges(Graph *g, unsigned u, unsigned v);
void transferOutgoingEdges(Graph *g, unsigned u, unsigned v);
void transferIncomingEdges(Graph *g, unsigned u, unsigned v);
void contractVertices(Graph *g, unsigned u, unsigned v);
void subdivideEdge(Graph *g, unsigned u, unsigned v);
void addWeightedDirectedEdge(Graph *g, unsigned u, unsigned v, double weight);
void addUndirectedEdge(Graph *g, unsigned u, unsigned v, double weight);
void deleteFirstWeightedDirectedEdge(Graph *g, unsigned u, unsigned v, double weight);
void deleteFirstWeightedUndirectedEdge(Graph *g, unsigned u, unsigned v, double weight);

unsigned getSize(const Graph *g);
unsigned countEdges(const Graph *g);
unsigned countSelfLoops(const Graph *g);
unsigned countTriangles(const Graph *g);
unsigned getMinimumInDegree(const Graph *g);
unsigned getMaximumInDegree(const Graph *g);
unsigned getMinimumOutDegree(const Graph *g);
unsigned getMaximumOutDegree(const Graph *g);
unsigned getMinimumDegree(const Graph *g);
unsigned getMaximumDegree(const Graph *g);
unsigned countSourceLeaves(const Graph *g);
unsigned countSinkLeaves(const Graph *g);
unsigned countUndirectedLeaves(const Graph *g);
unsigned countSources(const Graph *g);
unsigned countSinks(const Graph *g);
unsigned countParallelEdges(const Graph *g);
unsigned countIsolatedVertices(const Graph *g);
unsigned countStronglyConnectedComponents(const Graph *g);
unsigned countComponents(const Graph *g);
unsigned getFirstActiveVertex(const Graph *g);
unsigned calculateWienerIndex(const Graph *g);
unsigned countInvalidEdges(const Graph *g);
unsigned calculateDirectedUnweightedGirth(const Graph *g);
unsigned calculateUndirectedUnweightedGirth(const Graph *g);
unsigned calculateUnweightedRadius(const Graph *g);
unsigned calculateUnweightedDiameter(const Graph *g);
unsigned calculateMinimumVertexCut(const Graph *g);
unsigned countSpanningTrees(const Graph *g);
unsigned calculateChromaticNumber(const Graph *g);
unsigned calculateCliqueNumber(const Graph *g);
unsigned calculateTreewidth(const Graph *g);
unsigned calculateDirectedEdgeConnectivity(const Graph *g);
unsigned calculateDegeneracy(const Graph *g);
unsigned calculateVertexConnectivity(const Graph *g);
unsigned calculateEdgeConnectivity(const Graph *g);
unsigned countSelfLoopsAtVertex(const Graph *g, unsigned v);
unsigned getOutDegree(const Graph *g, unsigned v);
unsigned getInDegree(const Graph *g, unsigned v);
unsigned getDegree(const Graph *g, unsigned v);
unsigned calculateUnweightedEccentricity(const Graph *g, unsigned v);
unsigned getNeighbor(const Graph *g, unsigned v, unsigned i);
unsigned getNeighborhoodSize(const Graph *g, unsigned v, unsigned k);
unsigned countCommonNeighbors(const Graph *g, unsigned u, unsigned v);
unsigned countShortestPaths(const Graph *g, unsigned u, unsigned v);
unsigned calculateUnweightedDistance(const Graph *g, unsigned u, unsigned v);
unsigned countMatchingEdges(const Graph *g, unsigned u, unsigned v);
unsigned calculateLocalVertexConnectivity(const Graph *g, unsigned u, unsigned v);
unsigned calculateLocalEdgeConnectivity(const Graph *g, unsigned u, unsigned v);
unsigned countWalks(const Graph *g, unsigned u, unsigned v);
unsigned countPaths(const Graph *g, unsigned u, unsigned v);
unsigned countDirectedTrails(const Graph *g, unsigned u, unsigned v);
unsigned countUndirectedTrails(const Graph *g, unsigned u, unsigned v);
unsigned countMatchingWeightedEdges(const Graph *g, unsigned u, unsigned v, double weight);
unsigned calculateBandwidth(const Graph *g, const unsigned *ordering);

unsigned *getInDegrees(const Graph *g);
unsigned *getOutDegrees(const Graph *g);
unsigned *getDegrees(const Graph *g);
unsigned *getInDegreeDistribution(const Graph *g);
unsigned *getOutDegreeDistribution(const Graph *g);
unsigned *findGreedyColoring(const Graph *g);
unsigned *findOptimalColoring(const Graph *g);
unsigned *getStronglyConnectedComponents(const Graph *g);
unsigned *getTopologicalSort(const Graph *g);
unsigned *findMaximumBipartiteMatching(const Graph *g);
unsigned *findMaximumUnweightedMatching(const Graph *g);
unsigned *findMaximumWeightedMatching(const Graph *g);
unsigned *findMinimalEdgeCover(const Graph *g);
unsigned *findMinimumEdgeCover(const Graph *g);
unsigned *calculateCoreNumbers(const Graph *g);
unsigned *findCommunities(const Graph *g);
unsigned *findKTrusses(const Graph *g);
unsigned *findDegeneracyOrdering(const Graph *g);
unsigned *calculateUnweightedDistances(const Graph *g, unsigned v);
unsigned *getPreOrderSort(const Graph *g, unsigned v);
unsigned *getPostOrderSort(const Graph *g, unsigned v);
unsigned *getBreadthFirstSort(const Graph *g, unsigned v);
unsigned *getShortestPath(const Graph *g, unsigned u, unsigned v, unsigned *length);

unsigned **getAllPairsUnweightedDistances(const Graph *g);
unsigned **getBridges(const Graph *g);

double getMinimumWeight(const Graph *g);
double getMaximumWeight(const Graph *g);
double sumWeights(const Graph *g);
double calculateWeightedRadius(const Graph *g);
double calculateWeightedDiameter(const Graph *g);
double calculateDensity(const Graph *g);
double calculateAverageClusteringCoefficient(const Graph *g);
double calculateDirectedWeightedGirth(const Graph *g);
double calculateUndirectedWeightedGirth(const Graph *g);
double calculateFordFulkersonMinCut(const Graph *g);
double calculateStoerWagnerMinCut(const Graph *g);
double calculateBruteForceMinCut(const Graph *g);
double calculateMinCut(const Graph *g);
double calculateWeightedEccentricity(const Graph *g, unsigned v);
double getNormalizedInDegree(const Graph *g, unsigned v);
double getNormalizedOutDegree(const Graph *g, unsigned v);
double calculateLocalClusteringCoefficient(const Graph *g, unsigned v);
double getOutWeight(const Graph *g, unsigned v);
double getEdgeWeight(const Graph *g, unsigned u, unsigned v);
double calculateWeightedDistance(const Graph *g, unsigned u, unsigned v);
double calculateMaxFlowEdmondsKarp(const Graph *g, unsigned u, unsigned v);
double calculateSubgraphDensity(const Graph *g, const bool *set);
double calculateModularity(const Graph *g, const unsigned *partition);
double calculatePathWeight(const Graph *g, const unsigned *path, unsigned length);

double *calculateClosenessCentrality(const Graph *g);
double *calculateBetweennessCentrality(const Graph *g);
double *calculateBellmanFord(const Graph *g, unsigned v);
double *calculateWeightedDistances(const Graph *g, unsigned v);
double *calculateEigenvectorCentrality(const Graph *g, unsigned iterations, double tolerance);
double *calculatePageRank(const Graph *g, double damping, unsigned iterations, double tolerance);

Matrix *createLaplacian(const Graph *g);
Matrix *calculateFloydWarshall(const Graph *g);
Matrix *calculateGraphLayout(const Graph *g, unsigned iterations);

void testHasDirectedCycle();
void testHasUndirectedCycle();
void testIsWeaklyConnected();
void testIsStronglyConnected();
void testTopologicalSortOfGraph();
void testBellmanFord();
void testUnweightedDijkstra();
void testWeightedDijkstra();
void testFloydWarshall();
void testPrim();
void testKruskal();
void testFindArticulationPoints();
void testGraphDensity();
void testCalculateBetweennessCentrality();

int main();



void swapUnsigned(unsigned *a, unsigned *b) {
  if (!a || !b) return;
  unsigned t = *a;
  *a = *b;
  *b = t;
}

unsigned unsignedMinimum(unsigned a, unsigned b) {
  return a <= b ? a : b;
}

unsigned unsignedMaximum(unsigned a, unsigned b) {
  return a >= b ? a : b;
}



[[nodiscard]] bool **allocateFalseMatrix(unsigned m, unsigned n) {
  bool **matrix = malloc(m * sizeof(bool *));
  if (!matrix) return nullptr;
  for (unsigned i = 0; i < m; i++) {
    matrix[i] = calloc(n, sizeof(bool));
    if (!matrix[i]) {
      for (unsigned j = 0; j < i; j++) free(matrix[j]);
      free(matrix);
      return nullptr;
    }
  }
  return matrix;
}

void freeBooleanMatrix(bool **matrix, unsigned m) {
  if (!matrix) return;
  for (unsigned i = 0; i < m; i++) free(matrix[i]);
  free(matrix);
}



double calculateEuclideanNorm(double *a, unsigned n) {
  if (!a) return 0;
  double norm = 0;
  for (unsigned i = 0; i < n; i++) norm += a[i] * a[i];
  return sqrt(norm);
}



void destroyMatrix(Matrix *matrix) {
  if (!matrix) return;
  if (matrix->data) {
    for (unsigned i = 0; i < matrix->rows; i++) free(matrix->data[i]);
    free(matrix->data);
  }
  free(matrix);
}

bool isValidMatrix(const Matrix *matrix) {
  if (!matrix || (matrix->rows > 0 && !matrix->data)) return false;
  if (matrix->columns > 0)
    for (unsigned i = 0; i < matrix->rows; i++)
      if (!matrix->data[i])
        return false;
  return true;
}

double calculateMatrixTrace(const Matrix *matrix) {
  if (!isValidMatrix(matrix) || matrix->rows != matrix->columns) return 0;
  double trace = 0;
  for (unsigned i = 0; i < matrix->rows; i++) trace += matrix->data[i][i];
  return trace;
}

double calculateMatrixDeterminant(Matrix *matrix) {
  if (!isValidMatrix(matrix) || matrix->rows != matrix->columns) return 1;
  double determinant = 1;
  for (unsigned i = 0; i < matrix->rows; i++) {
    unsigned pivot = i;
    for (unsigned j = i + 1; j < matrix->rows; j++)
      if (fabs(matrix->data[j][i]) > fabs(matrix->data[pivot][i]))
        pivot = j;
    if (fabs(matrix->data[pivot][i]) < 1e-9) return 0;
    if (pivot != i) {
      double *temporary = matrix->data[i];
      matrix->data[i] = matrix->data[pivot];
      matrix->data[pivot] = temporary;
      determinant *= -1;
    }
    determinant *= matrix->data[i][i];
    for (unsigned j = i + 1; j < matrix->rows; ++j) {
      double factor = matrix->data[j][i] / matrix->data[i][i];
      for (unsigned k = i; k < matrix->rows; ++k) matrix->data[j][k] -= factor * matrix->data[i][k];
    }
  }
  return determinant;
}

[[nodiscard]] Matrix *createIdentityMatrix(unsigned n) {
  Matrix *identity = createZeroMatrix(n, n);
  if (!identity) return nullptr;
  for (unsigned i = 0; i < n; i++) identity->data[i][i] = 1;
  return identity;
}

[[nodiscard]] Matrix *createZeroMatrix(unsigned rows, unsigned columns) {
  Matrix *matrix = malloc(sizeof(Matrix));
  if (!matrix) return nullptr;
  matrix->rows = rows;
  matrix->columns = columns;
  matrix->data = malloc(rows * sizeof(double *));
  if (rows > 0 && !matrix->data) {
    free(matrix);
    return nullptr;
  }
  for (unsigned i = 0; i < rows; ++i) {
    matrix->data[i] = calloc(columns, sizeof(double));
    if (columns > 0 && !matrix->data[i]) {
      for (unsigned j = 0; j < i; ++j) free(matrix->data[j]);
      free(matrix->data);
      free(matrix);
      return nullptr;
    }
  }
  return matrix;
}

[[nodiscard]] Matrix *copyMatrix(const Matrix *matrix) {
  if (!isValidMatrix(matrix)) return nullptr;
  Matrix *copy = createZeroMatrix(matrix->rows, matrix->columns);
  if (!copy) return nullptr;
  for (unsigned i = 0; i < matrix->rows; i++)
    for (unsigned j = 0; j < matrix->columns; j++)
      copy->data[i][j] = matrix->data[i][j];
  return copy;
}

[[nodiscard]] Matrix *transposeMatrix(const Matrix *matrix) {
  if (!isValidMatrix(matrix)) return nullptr;
  Matrix *transpose = createZeroMatrix(matrix->columns, matrix->rows);
  if (!transpose) return nullptr;
  for (unsigned i = 0; i < matrix->rows; i++)
    for (unsigned j = 0; j < matrix->columns; j++)
      transpose->data[j][i] = matrix->data[i][j];
  return transpose;
}

[[nodiscard]] Matrix *powerMatrix(const Matrix *matrix, unsigned k) {
  if (!matrix || matrix->rows != matrix->columns) return nullptr;
  Matrix *power = createIdentityMatrix(matrix->rows);
  Matrix *base = copyMatrix(matrix);
  while (k > 0) {
    if (k % 2 == 1) {
      Matrix *multiplication = multiplyMatrices(power, base);
      destroyMatrix(power);
      power = multiplication;
    }
    Matrix *square = multiplyMatrices(base, base);
    destroyMatrix(base);
    base = square;
    k /= 2;
  }
  destroyMatrix(base);
  return power;
}

[[nodiscard]] Matrix *addMatrices(const Matrix *A, const Matrix *B) {
  if (!isValidMatrix(A) || !isValidMatrix(B) || A->rows != B->rows || A->columns != B->columns) return nullptr;
  Matrix *C = createZeroMatrix(A->rows, A->columns);
  if (!C) return nullptr;
  for (unsigned i = 0; i < A->rows; i++)
    for (unsigned j = 0; j < A->columns; j++)
      C->data[i][j] = A->data[i][j] + B->data[i][j];
  return C;
}

[[nodiscard]] Matrix *multiplyMatrices(const Matrix *A, const Matrix *B) {
  if (!isValidMatrix(A) || !isValidMatrix(B) || A->columns != B->rows) return nullptr;
  Matrix *C = createZeroMatrix(A->rows, B->columns);
  if (!C) return nullptr;
  for (unsigned i = 0; i < A->rows; i++)
    for (unsigned j = 0; j < B->columns; j++)
      for (unsigned k = 0; k < A->columns; k++)
        C->data[i][j] += A->data[i][k] * B->data[k][j];
  return C;
}



bool isValid(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return false;
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination >= g->size)
        return false;
  return true;
}

bool isEmpty(const Graph *g) {
  return g && g->size == 0;
}

bool isTrivial(const Graph *g) {
  return g && g->size == 1 && g->edges && !g->edges[0];
}

bool isEdgeless(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return false;
  for (unsigned v = 0; v < g->size; v++)
    if (g->edges[v])
      return false;
  return true;
}

bool isRegular(const Graph *g) {
  if (!g || !g->edges) return false;
  if (g->size <= 1) return true;
  unsigned target_in = 0;
  unsigned target_out = 0;
  for (Edge *e = g->edges[0]; e; e = e->next)
    target_out++;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination == 0)
        target_in++;
  for (unsigned u = 1; u < g->size; u++) {
    unsigned current_in = 0;
    unsigned current_out = 0;
    for (Edge *e = g->edges[u]; e; e = e->next)
      current_out++;
    for (unsigned v = 0; v < g->size; v++)
      for (Edge *e = g->edges[v]; e; e = e->next)
        if (e->destination == u)
          current_in++;
    if (current_out != target_out || current_in != target_in) return false;
  }
  return true;
}

bool isComplete(const Graph *g) {
  return !g || (!hasSelfLoops(g) && !hasParallelEdges(g) && countEdges(g) == g->size * (g->size - 1));
}

bool hasSelfLoops(const Graph *g) {
  if (!g) return false;
  for (unsigned v = 0; v < g->size; v++)
    if (hasSelfLoopsAtVertex(g, v))
      return true;
  return false;
}

bool isBalanced(const Graph *g) {
  unsigned *in = getInDegrees(g);
  bool b = g && (g->size == 0 || in);
  for (unsigned v = 0; b && v < g->size; v++) b = b && in[v] == getOutDegree(g, v);
  free(in);
  return b;
}

bool isEulerianUndirected(const Graph *g) {
  if (!g || isEdgeless(g)) return true;
  bool *reachable = getVerticesReachableFrom(g, getFirstActiveVertex(g));
  bool b = reachable;
  for (unsigned v = 0; v < g->size && b; v++) b = b && getOutDegree(g, v) % 2 == 0 && (reachable[v] || !g->edges[v]);
  free(reachable);
  return b;
}

bool isEulerianDirected(const Graph *g) {
  if (!g || isEdgeless(g)) return true;
  if (!isBalanced(g)) return false;
  bool *reachable = getVerticesReachableFrom(g, getFirstActiveVertex(g));
  bool *isolated = getIsolatedVertices(g);
  bool b = reachable && isolated;
  for (unsigned v = 0; v < g->size && b; v++) b = b && (reachable[v] || isolated[v]);
  free(isolated);
  free(reachable);
  return b;
}

bool isWeaklyConnected(const Graph *g) {
  if (getSize(g) < 2) return true;
  Graph *g2 = createUndirected(g);
  bool reachable = canReachAll(g2, 0);
  destroyGraph(g2);
  return reachable;
}

bool isStronglyConnected(const Graph *g) {
  if (getSize(g) < 2) return true;
  Graph *g2 = createTranspose(g);
  bool reachable = canReachAll(g, 0) && canReachAll(g2, 0);
  destroyGraph(g2);
  return reachable;
}

bool isBipartite(const Graph *g) {
  if (!g || !g->edges) return false;
  unsigned *colors = calloc(g->size, sizeof(unsigned));
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  bool bipartite = true;
  if (colors && queue)
    for (unsigned u = 0; u < g->size && bipartite; u++)
      if (colors[u] == 0) {
        colors[u] = 1;
        unsigned head = 0, tail = 0;
        queue[tail++] = u;
        while (head < tail && bipartite) {
          unsigned v = queue[head++];
          for (Edge *e = g->edges[v]; e && bipartite; e = e->next)
            if (e->destination < g->size) {
              if (colors[e->destination] == 0) {
                colors[e->destination] = (colors[v] == 1) ? 2 : 1;
                queue[tail++] = e->destination;
              } else if (colors[e->destination] == colors[v]) {
                bipartite = false;
              }
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
      if (e->destination < g->size)
        if (countMatchingWeightedEdges(g, u, e->destination, e->weight) != countMatchingWeightedEdges(g, e->destination, u, e->weight))
          return false;
  return true;
}

bool hasParallelEdges(const Graph *g) {
  if (!g || !g->edges) return false;
  bool *seen = malloc(g->size * sizeof(bool));
  bool b = false;
  if (seen)
    for (unsigned v = 0; v < g->size && !b; v++) {
      for (Edge *e = g->edges[v]; e; e = e->next)
        if (e->destination < g->size)
          seen[e->destination] = false;
      for (Edge *e = g->edges[v]; e && !b; e = e->next)
        if (e->destination < g->size) {
          if (seen[e->destination]) b = true; else seen[e->destination] = true;
        }
    }
  free(seen);
  return b;
}

bool isDirectedForest(const Graph *g) {
  unsigned *degree = getInDegrees(g);
  bool b = g && !hasDirectedCycle(g) && (g->size == 0 || degree);
  for (unsigned v = 0; b && v < g->size; v++) b = b && degree[v] < 2;
  free(degree);
  return b;
}

bool isUndirectedForest(const Graph *g) {
  return isUndirected(g) && !hasUndirectedCycle(g);
}

bool isDirectedTree(const Graph *g) {
  if (!g) return false;
  unsigned *degree = getInDegrees(g);
  if (!degree) return false;
  unsigned roots = 0;
  bool b = !hasDirectedCycle(g);
  for (unsigned v = 0; v < g->size && b; v++) {
    if (degree[v] == 0) roots++;
    b = b && roots < 2 && degree[v] < 2;
  }
  free(degree);
  return b && roots == 1;
}

bool isUndirectedTree(const Graph *g) {
  return isUndirected(g) && !hasUndirectedCycle(g) && isWeaklyConnected(g);
}

bool isPathGraph(const Graph *g) {
  if (getSize(g) == 0) return false;
  if (getSize(g) == 1) return isEdgeless(g);
  if (hasSelfLoops(g)) return false;
  if (hasParallelEdges(g)) return false;
  if (!isWeaklyConnected(g)) return false;
  unsigned endpoints = 0;
  unsigned internal = 0;
  for (unsigned v = 0; v < getSize(g); v++)
    if (getInDegree(g, v) == 1 && getOutDegree(g, v) == 1)
      endpoints++;
    else if (getInDegree(g, v) == 2 && getOutDegree(g, v) == 2)
      internal++;
    else
      return false;
  return endpoints == 2 && internal == getSize(g) - 2;
}

bool isCycleGraph(const Graph *g) {
  return getSize(g) >= 3 && !hasSelfLoops(g) && !hasParallelEdges(g) && isUndirected(g) && isWeaklyConnected(g) && isKRegular(g, 2);
}

bool isStarGraph(const Graph *g) {
  if (!g || g->size < 2 || !isUndirected(g)) return false;
  unsigned count = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (getOutDegree(g, v) == g->size - 1)
      count++;
    else if (getOutDegree(g, v) != 1)
      return false;
  return (g->size == 2 && count == 2) || (g->size > 2 && count == 1);
}

bool isWheelGraph(const Graph *g) {
  if (!isValid(g) || g->size < 4 || !isUndirected(g)) return false;
  unsigned hub;
  unsigned hubCount = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (getOutDegree(g, v) == g->size - 1) {
      hub = v;
      hubCount++;
    } else if (getOutDegree(g, v) != 3) {
      return false;
    }
  if ((g->size == 4 && hubCount != 4) || (g->size > 4 && hubCount != 1)) return false;
  unsigned visitedCount = 0;
  unsigned previous = hub;
  unsigned current = hub;
  do {
    visitedCount++;
    unsigned next = UINT_MAX;
    for (Edge *e = g->edges[current]; e && next == UINT_MAX; e = e->next)
      if (e->destination != hub && e->destination != previous)
        next = e->destination;
    previous = current;
    current = next;
  } while (current != UINT_MAX);
  return visitedCount == g->size;
}

bool hasIsolatedVertices(const Graph *g) {
  bool *isolated = getIsolatedVertices(g);
  if (!g || !isolated) return false;
  bool b = false;
  for (unsigned v = 0; v < g->size && !b; v++) b = b || isolated[v];
  free(isolated);
  return b;
}

bool isTournament(const Graph *g) {
  if (!isValid(g)) return false;
  unsigned *m = malloc(g->size * g->size * sizeof(unsigned *));
  if (!m) return false;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      m[v * g->size + e->destination] += 1;
  bool b = true;
  for (unsigned v = 0; v < g->size && b; v++)
    b = b && m[v * g->size + v] == 0;
  for (unsigned u = 0; u < g->size && b; u++)
    for (unsigned v = u + 1; v < g->size && b; v++)
      b = b && m[u * g->size + v] + m[v * g->size + u] == 1;
  free(m);
  return b;
}

bool hasNegativeWeights(const Graph *g) {
  if (!g || !g->edges) return false;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->weight < 0)
        return true;
  return false;
}

bool isCubic(const Graph *g) {
  return isKRegular(g, 3);
}

static bool hasDirectedCycleDfs(const Graph *g, unsigned v, char visited[g->size]) {
  visited[v] = 1;
  for (Edge *e = g->edges[v]; e; e = e->next)
    if (visited[e->destination] == 1 || (visited[e->destination] == 0 && hasDirectedCycleDfs(g, e->destination, visited)))
      return true;
  visited[v] = 2;
  return false;
}

bool hasDirectedCycle(const Graph *g) {
  if (!isValid(g)) return false;
  char visited[g->size] = {};
  for (unsigned v = 0; v < g->size; v++)
    if (visited[v] == 0 && hasDirectedCycleDfs(g, v, visited))
      return true;
  return false;
}

static bool hasUndirectedCycleDfs(const Graph *g, unsigned v, unsigned parent, bool visited[g->size]) {
  visited[v] = true;
  for (Edge *e = g->edges[v]; e; e = e->next)
    if (visited[e->destination]) {
      if (e->destination != parent)
        return true;
    } else if (hasUndirectedCycleDfs(g, e->destination, v, visited)) {
      return true;
    }
  return false;
}

bool hasUndirectedCycle(const Graph *g) {
  if (!isValid(g)) return false;
  bool visited[g->size] = {};
  for (unsigned v = 0; v < g->size; v++)
    if (!visited[v] && hasUndirectedCycleDfs(g, v, UINT_MAX, visited))
      return true;
  return false;
}

bool hasNegativeCycle(const Graph *g) {
  if (!isValid(g)) return false;
  double distances[g->size] = {};
  for (unsigned i = 1; i < g->size; i++)
    for (unsigned v = 0; v < g->size; v++)
      for (const Edge *e = g->edges[v]; e; e = e->next)
        if (distances[v] + e->weight < distances[e->destination])
          distances[e->destination] = distances[v] + e->weight;
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (distances[v] + e->weight < distances[e->destination])
        return true;
  return false;
}

static bool isSelfComplementaryIsomorphism(const Graph *g, unsigned *p, unsigned i) {
  if (i == g->size) {
    for (unsigned u = 0; u < g->size; u++)
      for (unsigned v = 0; v < g->size; v++)
        if (u != v && hasDirectedEdge(g, u, v) == hasDirectedEdge(g, p[u], p[v]))
          return false;
    return true;
  }
  for (unsigned j = i; j < g->size; j++) {
    swapUnsigned(&p[i], &p[j]);
    if (isSelfComplementaryIsomorphism(g, p, i + 1)) return true;
    swapUnsigned(&p[i], &p[j]);
  }
  return false;
}

bool isSelfComplementary(const Graph *g) {
  if (!g) return true;
  unsigned p[g->size];
  for (unsigned i = 0; i < g->size; i++) p[i] = i;
  return isSelfComplementaryIsomorphism(g, p, 0);
}

bool isChordal(const Graph *g) {
  if (!g) return false;
  bool removed[g->size] = {};
  for (unsigned step = 0; step < g->size; step++) {
    unsigned simplicial = UINT_MAX;
    for (unsigned u = 0; u < g->size && simplicial == UINT_MAX; u++) {
      if (removed[u]) continue;
      bool connected = true;
      for (Edge *e1 = g->edges[u]; e1 && connected; e1 = e1->next) {
        unsigned v = e1->destination;
        if (u == v || removed[v]) continue;
        for (Edge *e2 = e1->next; e2 && connected; e2 = e2->next) {
          unsigned w = e2->destination;
          if (u == w || v == w || removed[w]) continue;
          if (!hasDirectedEdge(g, v, w) && !hasDirectedEdge(g, w, v)) connected = false;
        }
      }
      if (connected) simplicial = u;
    }
    if (simplicial == UINT_MAX) return false;
    removed[simplicial] = true;
  }
  return true;
}

static void computeCliqueNumberRecursive(
  const Graph *g, const bool *active, unsigned v, bool *current, unsigned currentSize, unsigned *maximumSize)
{
  if (v == g->size) {
    if (currentSize > *maximumSize) *maximumSize = currentSize;
    return;
  }
  if (active[v]) {
    bool addable = true;
    for (unsigned u = 0; u < v; u++)
      if (current[u] && (!hasDirectedEdge(g, u, v) || !hasDirectedEdge(g, v, u)))
        addable = false;
    if (addable) {
      current[v] = true;
      computeCliqueNumberRecursive(g, active, v + 1, current, currentSize + 1, maximumSize);
      current[v] = false;
    }
  }
  computeCliqueNumberRecursive(g, active, v + 1, current, currentSize, maximumSize);
}

static unsigned computeCliqueNumber(const Graph *g, const bool *active) {
  bool *current = calloc(g->size, sizeof(bool));
  if (!current) return 0;
  unsigned maximum = 0;
  computeCliqueNumberRecursive(g, active, 0, current, 0, &maximum);
  free(current);
  return maximum;
}

static bool canColor(const Graph *g, const bool *active, unsigned v, unsigned *colors, unsigned numberColors) {
  if (v == g->size) return true;
  if (!active[v]) return canColor(g, active, v + 1, colors, numberColors);
  for (unsigned c = 0; c < numberColors; c++) {
    bool conflict = false;
    for (unsigned u = 0; u < v; u++)
      if (active[u] && colors[u] == c && (hasDirectedEdge(g, u, v) || hasDirectedEdge(g, v, u)))
        conflict = true;
    if (!conflict) {
      colors[v] = c;
      if (canColor(g, active, v + 1, colors, numberColors)) return true;
    }
  }
  return false;
}

static unsigned computeChromaticNumber(const Graph *g, const bool *active) {
  unsigned *colors = malloc(g->size * sizeof(unsigned));
  if (!colors) return 0;
  for (unsigned numberColors = 0; numberColors <= g->size; numberColors++)
    if (canColor(g, active, 0, colors, numberColors)) {
      free(colors);
      return numberColors;
    }
  free(colors);
  return 0;
}

static bool verifyAllInducedSubgraphs(const Graph *g, unsigned v, bool *active, unsigned activeCount) {
  if (v == g->size) {
    unsigned omega = computeCliqueNumber(g, active);
    unsigned chi = computeChromaticNumber(g, active);
    return omega == chi;
  }
  active[v] = false;
  if (!verifyAllInducedSubgraphs(g, v + 1, active, activeCount)) return false;
  active[v] = true;
  if (!verifyAllInducedSubgraphs(g, v + 1, active, activeCount + 1)) return false;
  return true;
}

bool isPerfect(const Graph *g) {
  if (!g || g->size == 0) return true;
  bool *active = calloc(g->size, sizeof(bool));
  if (!active) return false;
  bool result = verifyAllInducedSubgraphs(g, 0, active, 0);
  free(active);
  return result;
}

static bool holdsForbidden(unsigned n, bool adjacent[n][n], bool active[n]) {
  for (unsigned a = 0; a < n; a++) if (active[a])
  for (unsigned b = a + 1; b < n; b++) if (active[b] && adjacent[a][b])
  for (unsigned c = b + 1; c < n; c++) if (active[c] && adjacent[a][c] && adjacent[b][c])
  for (unsigned d = c + 1; d < n; d++) if (active[d] && adjacent[a][d] && adjacent[b][d] && adjacent[c][d])
  for (unsigned e = d + 1; e < n; e++) if (active[e] && adjacent[a][e] && adjacent[b][e] && adjacent[c][e] && adjacent[d][e])
    return true;
  for (unsigned a = 0; a < n; a++) if (active[a])
  for (unsigned b = 0; b < n; b++) if (active[b] && adjacent[a][b])
  for (unsigned c = a + 1; c < n; c++) if (active[c] && adjacent[b][c])
  for (unsigned d = b + 1; d < n; d++) if (active[d] && adjacent[a][d] && adjacent[c][d])
  for (unsigned e = c + 1; e < n; e++) if (active[e] && adjacent[b][e] && adjacent[d][e])
  for (unsigned f = d + 1; f < n; f++) if (active[f] && adjacent[a][f] && adjacent[c][f] && adjacent[e][f])
    return true;
  return false;
}

static bool checkMinors(unsigned n, bool adjacent[n][n], bool active[n]) {
  if (holdsForbidden(n, adjacent, active)) return false;
  for (unsigned u = 0; u < n; u++) if (active[u])
    for (unsigned v = u + 1; v < n; v++) if (active[v] && adjacent[u][v]) {
      adjacent[u][v] = adjacent[v][u] = false;
      if (!checkMinors(n, adjacent, active)) return false;
      adjacent[u][v] = adjacent[v][u] = true;

      bool backup[n];
      for (unsigned w = 0; w < n; w++) backup[w] = adjacent[u][w];
      for (unsigned w = 0; w < n; w++) if (adjacent[v][w]) adjacent[u][w] = adjacent[w][u] = true;
      adjacent[u][u] = false;
      active[v] = false;
      if (!checkMinors(n, adjacent, active)) return false;
      active[v] = true;
      for (unsigned w = 0; w < n; w++) adjacent[u][w] = adjacent[w][u] = backup[w];
    }
  return true;
}

bool isPlanar(const Graph *g) {
  if (!g || !g->edges || g->size < 5) return true;
  bool adjacent[g->size][g->size] = {};
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (v != e->destination && e->destination < g->size)
        adjacent[v][e->destination] = adjacent[e->destination][v] = true;
  bool active[g->size];
  for (unsigned v = 0; v < g->size; v++) active[v] = true;
  return checkMinors(g->size, adjacent, active);
}

bool isKRegular(const Graph *g, unsigned k) {
  if (!g) return true;
  for (unsigned v = 0; v < g->size; v++)
    if (getInDegree(g, v) != k || getOutDegree(g, v) != k)
      return false;
  return true;
}

bool isKConnected(const Graph *g, unsigned k) {
  return calculateVertexConnectivity(g) >= k;
}

bool isProperColoring(const Graph *g, const unsigned *coloring) {
  if (!g || !g->edges) return false;
  if (g->size == 0) return true;
  if (!coloring) return false;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size && coloring[v] == coloring[e->destination])
        return false;
  return true;
}

bool hasConstantWeights(const Graph *g, double weight) {
  if (!g || !g->edges) return true;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->weight != weight)
        return false;
  return true;
}

bool isDense(const Graph *g, double threshold) {
  return !g || countEdges(g) >= threshold * g->size * (g->size - 1);
}

bool isIsolated(const Graph *g, unsigned v) {
  return getOutDegree(g, v) == 0 && getInDegree(g, v) == 0;
}

bool isSource(const Graph *g, unsigned v) {
  return getInDegree(g, v) == 0 && getOutDegree(g, v) > 0;
}

bool isSink(const Graph *g, unsigned v) {
  return getOutDegree(g, v) == 0 && getInDegree(g, v) > 0;
}

bool isUniversalSource(const Graph *g, unsigned v) {
  return g && getInDegree(g, v) == 0 && getOutDegree(g, v) == g->size - 1;
}

bool isUniversalSink(const Graph *g, unsigned v) {
  return g && getOutDegree(g, v) == 0 && getInDegree(g, v) == g->size - 1;
}

bool isDirectedLeaf(const Graph *g, unsigned v) {
  return getOutDegree(g, v) + getInDegree(g, v) == 1;
}

bool isUndirectedLeaf(const Graph *g, unsigned v) {
  return getOutDegree(g, v) == 1;
}

bool hasSelfLoopsAtVertex(const Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return false;
  for (Edge *e = g->edges[v]; e; e = e->next)
    if (e->destination == v)
      return true;
  return false;
}

bool canReachAll(const Graph *g, unsigned v) {
  bool *reachable = getVerticesReachableFrom(g, v);
  if (!g || !reachable) return false;
  bool b = true;
  for (unsigned v = 0; v < g->size && b; v++) b = b && reachable[v];
  free(reachable);
  return b;
}

bool isArticulationVertex(const Graph *g, unsigned v) {
  Graph *g2 = copyGraph(g);
  deleteVertex(g2, v);
  unsigned n = countComponents(g2);
  destroyGraph(g2);
  return n > countComponents(g);
}

bool isSimplicial(const Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return false;
  for (const Edge *e1 = g->edges[v]; e1; e1 = e1->next)
    for (const Edge *e2 = e1->next; e2; e2 = e2->next) {
      unsigned v2 = e1->destination, v3 = e2->destination;
      if (v != v2 && v != v3 && v2 != v3 && !hasDirectedEdge(g, v2, v3) && !hasDirectedEdge(g, v3, v2)) return false;
    }
  return true;
}

bool hasDirectedEdge(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size) return false;
  for (const Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination == v)
      return true;
  return false;
}

bool hasUndirectedEdge(const Graph *g, unsigned u, unsigned v) {
  return hasWeightedDirectedEdge(g, u, v, getEdgeWeight(g, u, v)) && hasWeightedDirectedEdge(g, v, u, getEdgeWeight(g, u, v));
}

bool hasPath(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size) return false;
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

bool haveCommonNeighbors(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size) return false;
  bool *neighbors = calloc(g->size, sizeof(bool));
  if (!neighbors) return false;
  for (Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination < g->size)
      neighbors[e->destination] = true;
  bool b = false;
  for (Edge *e = g->edges[v]; e && !b; e = e->next)
    if (e->destination < g->size)
      b = b || neighbors[e->destination];
  free(neighbors);
  return b;
}

bool isDirectedBridge(const Graph *g, unsigned u, unsigned v) {
  Graph *g2 = copyGraph(g);
  deleteFirstDirectedEdge(g2, u, v);
  unsigned n = countComponents(g2);
  destroyGraph(g2);
  return n > countComponents(g);
}

bool isUndirectedBridge(const Graph *g, unsigned u, unsigned v) {
  Graph *g2 = copyGraph(g);
  deleteFirstUndirectedEdge(g2, u, v);
  unsigned n = countComponents(g2);
  destroyGraph(g2);
  return n > countComponents(g);
}

bool hasWeightedDirectedEdge(const Graph *g, unsigned u, unsigned v, double weight) {
  if (!g || !g->edges || u >= g->size) return false;
  for (Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination == v && e->weight == weight)
      return true;
  return false;
}

bool hasWeightedUndirectedEdge(const Graph *g, unsigned u, unsigned v, double weight) {
  return hasWeightedDirectedEdge(g, u, v, weight) && hasWeightedDirectedEdge(g, v, u, weight);
}

bool isTriangle(const Graph *g, unsigned u, unsigned v, unsigned w) {
  return u != v && v != w && w != u && hasDirectedEdge(g, u, v) && hasDirectedEdge(g, v, w) && hasDirectedEdge(g, w, u);
}

bool isClique(const Graph *g, const bool *set) {
  if (!set) return true;
  if (!g) return false;
  for (unsigned u = 0; u < g->size; u++)
    if (set[u])
      for (unsigned v = u + 1; v < g->size; v++)
        if (set[v])
          if (!hasDirectedEdge(g, u, v) || !hasDirectedEdge(g, v, u))
            return false;
  return true;
}

bool isIndependentSet(const Graph *g, const bool *set) {
  if (!g || !g->edges || !set) return true;
  for (unsigned u = 0; u < g->size; u++)
    if (set[u])
      for (Edge *e = g->edges[u]; e; e = e->next)
        if (e->destination < g->size && set[e->destination])
          return false;
  return true;
}

bool isVertexCover(const Graph *g, const bool *set) {
  if (!g || !g->edges) return true;
  for (unsigned u = 0; u < g->size; u++)
    if (!set || !set[u])
      for (Edge *e = g->edges[u]; e; e = e->next)
        if (!set || e->destination >= g->size || !set[e->destination])
          return false;
  return true;
}

bool hasDirectedEdges(const Graph *g, unsigned v, const bool *set) {
  if (!g || !g->edges || v >= g->size || !set) return false;
  unsigned total = 0;
  for (unsigned u = 0; u < g->size; u++)
    if (set[u])
      total++;
  bool *visited = calloc(g->size, sizeof(bool));
  if (!visited) return false;
  unsigned found = 0;
  for (Edge *e = g->edges[v]; e && found < total; e = e->next)
    if (e->destination < g->size && set[e->destination] && !visited[e->destination]) {
      visited[e->destination] = true;
      found++;
    }
  free(visited);
  return found == total;
}

bool hasUndirectedEdges(const Graph *g, unsigned v, const bool *set) {
  if (!g || !set || !hasDirectedEdges(g, v, set)) return false;
  for (unsigned u = 0; u < g->size; u++)
    if (set[u] && !hasDirectedEdge(g, u, v))
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

bool isWalk(const Graph *g, const unsigned *sequence, unsigned length) {
  bool b = sequence || length < 2;
  for (unsigned i = 1; i < length && b; i++)
    b = b && hasDirectedEdge(g, sequence[i - 1], sequence[i]);
  return b;
}

bool isPath(const Graph *g, const unsigned *sequence, unsigned length) {
  bool b = (sequence || length == 0) && isWalk(g, sequence, length);
  bool *visited = calloc(g->size, sizeof(bool));
  for (unsigned i = 0; i < length && b; i++) {
    b = b && !visited[sequence[i]];
    visited[sequence[i]] = true;
  }
  free(visited);
  return b;
}

bool isHamiltonianPath(const Graph *g, const unsigned *sequence, unsigned length) {
  return g && length == g->size && isPath(g, sequence, length);
}

bool isDirectedTrail(const Graph *g, const unsigned *sequence, unsigned length) {
  Graph *g2 = copyGraph(g);
  bool valid = sequence || length < 2;
  for (unsigned i = 1; i < length && valid; i++)
    if (hasDirectedEdge(g2, sequence[i - 1], sequence[i]))
      deleteFirstDirectedEdge(g2, sequence[i - 1], sequence[i]);
    else
      valid = false;
  destroyGraph(g2);
  return valid;
}

bool isUndirectedTrail(const Graph *g, const unsigned *sequence, unsigned length) {
  Graph *copy = copyGraph(g);
  bool valid = sequence || length < 2;
  for (unsigned i = 1; i < length && valid; i++)
    if (hasUndirectedEdge(copy, sequence[i - 1], sequence[i]))
      deleteFirstUndirectedEdge(copy, sequence[i - 1], sequence[i]);
    else
      valid = false;
  destroyGraph(copy);
  return valid;
}

bool isDirectedCycle(const Graph *g, const unsigned *sequence, unsigned length) {
  if (!g || g->size == 0 || !sequence || length == 0) return false;
  bool valid = true;
  for (unsigned i = 0; i < length && valid; i++)
    valid = valid && hasDirectedEdge(g, sequence[i], sequence[(i + 1) % length]);
  return valid;
}

bool isSimpleCycle(const Graph *g, const unsigned *sequence, unsigned length) {
  if (!g || g->size == 0 || !sequence || length == 0 || !isDirectedCycle(g, sequence, length)) return false;
  bool *visited = calloc(g->size, sizeof(bool));
  if (!visited) return false;
  bool valid = true;
  for (unsigned i = 0; i < length && valid; i++) {
    valid = valid && !visited[sequence[i]];
    visited[sequence[i]] = true;
  }
  free(visited);
  return valid;
}

bool isHamiltonianCycle(const Graph *g, const unsigned *sequence, unsigned length) {
  return g && length == g->size && isSimpleCycle(g, sequence, length);
}

bool isDirectedCircuit(const Graph *g, const unsigned *sequence, unsigned length) {
  if (!sequence || length == 0) return false;
  bool valid = true;
  Graph *copy = copyGraph(g);
  for (unsigned i = 0; i < length && valid; i++)
    if (hasDirectedEdge(copy, sequence[i], sequence[(i + 1) % length]))
      deleteFirstDirectedEdge(copy, sequence[i], sequence[(i + 1) % length]);
    else
      valid = false;
  destroyGraph(copy);
  return valid;
}

bool isUndirectedCircuit(const Graph *g, const unsigned *sequence, unsigned length) {
  if (!sequence || length == 0) return false;
  bool valid = true;
  Graph *copy = copyGraph(g);
  for (unsigned i = 0; i < length && valid; i++)
    if (hasUndirectedEdge(copy, sequence[i], sequence[(i + 1) % length]))
      deleteFirstUndirectedEdge(copy, sequence[i], sequence[(i + 1) % length]);
    else
      valid = false;
  destroyGraph(copy);
  return valid;
}

bool isSubGraph(const Graph *g1, const Graph *g2) {
  if (!isValid(g1) || !isValid(g2) || g1->size > g2->size) return false;
  for (unsigned v = 0; v < g1->size; v++)
    for (Edge *e = g1->edges[v]; e; e = e->next)
      if (countMatchingWeightedEdges(g1, v, e->destination, e->weight) > countMatchingWeightedEdges(g2, v, e->destination, e->weight))
        return false;
  return true;
}

bool isSpanningDirectedTree(const Graph *g1, const Graph *g2) {
  return g1 && g2 && g1->size == g2->size && g1->size > 0 && countEdges(g1) == g1->size - 1 &&
    isWeaklyConnected(g1) && isSubGraph(g1, g2);
}

bool isSpanningUndirectedTree(const Graph *g1, const Graph *g2) {
  return g1 && g2 && g1->size == g2->size && g1->size > 0 && countEdges(g1) == 2 * (g1->size - 1) &&
    !hasSelfLoops(g1) && !hasParallelEdges(g1) && isWeaklyConnected(g1) && isSubGraph(g1, g2);
}

static bool isIsomorphicRecursive(const Graph *g1, const Graph *g2, unsigned v1, unsigned *mapping, bool *used) {
  for (unsigned u = 0; u < v1; u++)
    for (const Edge *e = g1->edges[u]; e; e = e->next)
      if (e->destination < v1 && (u == v1 - 1 || e->destination == v1 - 1)) {
        unsigned count1 = countMatchingWeightedEdges(g1, u, e->destination, e->weight);
        unsigned count2 = countMatchingWeightedEdges(g2, mapping[u], mapping[e->destination], e->weight);
        if (count1 != count2) return false;
      }
  if (v1 == g1->size) return true;
  for (unsigned v2 = 0; v2 < g2->size; v2++)
    if (!used[v2]) {
      mapping[v1] = v2;
      used[v2] = true;
      if (isIsomorphicRecursive(g1, g2, v1 + 1, mapping, used)) return true;
      used[v2] = false;
    }
  return false;
}

bool isIsomorphic(const Graph *g1, const Graph *g2) {
  if (!g1 || !g1->edges || !g2 || !g2->edges) return false;
  if (g1->size != g2->size) return false;
  if (g1->size == 0) return true;
  if (countEdges(g1) != countEdges(g2)) return false;
  unsigned mapping[g1->size] = {};
  bool used[g1->size] = {};
  return isIsomorphicRecursive(g1, g2, 0, mapping, used);
}



[[nodiscard]] bool *graphCenter(const Graph *g) {
  if (!g || g->size == 0) return nullptr;
  double *eccentricity = malloc(g->size * sizeof(double));
  bool *center = calloc(g->size, sizeof(bool));
  if (!eccentricity || !center) {
    free(eccentricity);
    free(center);
    return nullptr;
  }
  double radius = INFINITY;
  for (unsigned v = 0; v < g->size; v++) {
    eccentricity[v] = calculateWeightedEccentricity(g, v);
    if (eccentricity[v] < radius) radius = eccentricity[v];
  }
  if (radius == INFINITY) {
    free(eccentricity);
    free(center);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++)
    if (eccentricity[v] == radius)
      center[v] = true;
  free(eccentricity);
  return center;
}

[[nodiscard]] bool *graphPeriphery(const Graph *g) {
  if (!g || g->size == 0) return nullptr;
  double *eccentricity = malloc(g->size * sizeof(double));
  bool *periphery = calloc(g->size, sizeof(bool));
  if (!eccentricity || !periphery) {
    free(eccentricity);
    free(periphery);
    return nullptr;
  }
  double diameter = -INFINITY;
  for (unsigned v = 0; v < g->size; v++) {
    eccentricity[v] = calculateWeightedEccentricity(g, v);
    if (eccentricity[v] != INFINITY && eccentricity[v] > diameter) diameter = eccentricity[v];
  }
  if (diameter == -INFINITY) {
    free(eccentricity);
    free(periphery);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++)
    if (eccentricity[v] == diameter)
      periphery[v] = true;
  free(eccentricity);
  return periphery;
}

static void findArticulationPointsDfs(
  const Graph *g, unsigned u, unsigned parent, unsigned *timer, unsigned *discovery, unsigned *low, bool *articulations)
{
  discovery[u] = low[u] = ++(*timer);
  unsigned children = 0;
  for (Edge *e = g->edges[u]; e; e = e->next) {
    if (e->destination >= g->size || e->destination == parent) continue;
    if (discovery[e->destination] > 0) {
      low[u] = unsignedMinimum(low[u], discovery[e->destination]);
    } else {
      children++;
      findArticulationPointsDfs(g, e->destination, u, timer, discovery, low, articulations);
      low[u] = unsignedMinimum(low[u], low[e->destination]);
      if (parent != u && low[e->destination] >= discovery[u]) articulations[u] = true;
    }
  }
  if (parent == u && children > 1) articulations[u] = true;
}

[[nodiscard]] bool *findArticulationPoints(const Graph *g) {
  if (!isValid(g) || g->size == 0) return nullptr;
  bool *articulations = calloc(g->size, sizeof(bool));
  if (!articulations) return nullptr;
  unsigned discovery[g->size] = {};
  unsigned low[g->size] = {};
  unsigned timer = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (discovery[v] == 0)
      findArticulationPointsDfs(g, v, v, &timer, discovery, low, articulations);
  return articulations;
}

[[nodiscard]] bool *findMaximalClique(const Graph *g) {
  if (!g) return nullptr;
  bool *clique = calloc(g->size, sizeof(bool));
  if (!clique) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    if (hasUndirectedEdges(g, v, clique))
      clique[v] = true;
  return clique;
}

static void findMaximumCliqueRecursive(
  const Graph *g, unsigned v, bool *current, unsigned currentSize, bool *maximum, unsigned *maximumSize)
{
  if (v == g->size) {
    if (currentSize > *maximumSize) {
      *maximumSize = currentSize;
      for (unsigned u = 0; u < g->size; u++) maximum[u] = current[u];
    }
    return;
  }
  bool addable = true;
  for (unsigned u = 0; u < v; u++)
    if (current[u])
      if (!hasDirectedEdge(g, v, u) || !hasDirectedEdge(g, u, v))
        addable = false;
  if (addable) {
    current[v] = true;
    findMaximumCliqueRecursive(g, v + 1, current, currentSize + 1, maximum, maximumSize);
  }
  current[v] = false;
  findMaximumCliqueRecursive(g, v + 1, current, currentSize, maximum, maximumSize);
}

[[nodiscard]] bool *findMaximumClique(const Graph *g) {
  if (!g) return nullptr;
  bool *current = calloc(g->size, sizeof(bool));
  bool *maximum = calloc(g->size, sizeof(bool));
  if (!current || !maximum) {
    free(current);
    free(maximum);
    return nullptr;
  }
  unsigned size = 0;
  findMaximumCliqueRecursive(g, 0, current, 0, maximum, &size);
  free(current);
  return maximum;
}

[[nodiscard]] bool *getIsolatedVertices(const Graph *g) {
  if (!g) return nullptr;
  unsigned *in = getInDegrees(g);
  bool *isolated = calloc(g->size, sizeof(bool));
  if (!in || !isolated) {
    free(in);
    free(isolated);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++) isolated[v] = in[v] == 0 && getOutDegree(g, v) == 0;
  free(in);
  return isolated;
}

[[nodiscard]] bool *getSources(const Graph *g) {
  if (!g) return nullptr;
  unsigned *in = getInDegrees(g);
  bool *sources = calloc(g->size, sizeof(bool));
  if (!in || !sources) {
    free(in);
    free(sources);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++) sources[v] = in[v] == 0 && getOutDegree(g, v) > 0;
  free(in);
  return sources;
}

[[nodiscard]] bool *getSinks(const Graph *g) {
  if (!g) return nullptr;
  unsigned *in = getInDegrees(g);
  bool *sinks = calloc(g->size, sizeof(bool));
  if (!in || !sinks) {
    free(in);
    free(sinks);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++) sinks[v] = in[v] > 0 && getOutDegree(g, v) == 0;
  free(in);
  return sinks;
}

[[nodiscard]] bool *findMaximalVertexCover(const Graph *g) {
  if (!isValid(g)) return nullptr;
  bool *cover = calloc(g->size, sizeof(bool));
  if (!cover) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    if (!cover[v])
      for (Edge *e = g->edges[v]; e; e = e->next)
        if (!cover[e->destination]) {
          cover[v] = true;
          cover[e->destination] = true;
          break;
        }
  return cover;
}

[[nodiscard]] bool *findMinimumVertexCover(const Graph *g) {
  if (!isValid(g) || g->size == 0) return nullptr;
  bool *best = malloc(g->size * sizeof(bool));
  if (!best) return nullptr;
  bool current[g->size] = {};
  for (unsigned v = 0; v < g->size; v++) best[v] = true;
  unsigned bestCount = g->size;
  while (true) {
    unsigned currentCount = 0;
    for (unsigned v = 0; v < g->size; v++)
      if (current[v])
        currentCount++;
    if (currentCount < bestCount) {
      bool isCover = true;
      for (unsigned u = 0; u < g->size && isCover; u++)
        for (Edge *e = g->edges[u]; e && isCover; e = e->next)
          if (!current[u] && !current[e->destination])
            isCover = false;
      if (isCover) {
        bestCount = currentCount;
        for (unsigned v = 0; v < g->size; v++) best[v] = current[v];
      }
    }
    unsigned v = 0;
    while (v < g->size && current[v]) {
      current[v] = false;
      v++;
    }
    if (v < g->size) current[v] = true; else break;
  }
  return best;
}

[[nodiscard]] bool *getSelfLoops(const Graph *g) {
  if (!g) return nullptr;
  bool *loops = calloc(g->size, sizeof(bool));
  if (!loops) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination == v)
        loops[v] = true;
  return loops;
}

[[nodiscard]] bool *findMaximalIndependentSet(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  unsigned *degrees = malloc(g->size * sizeof(unsigned));
  bool *removed = getSelfLoops(g);
  bool *independent = calloc(g->size, sizeof(bool));
  if (!degrees || !removed || !independent) {
    free(degrees); free(removed); free(independent);
    return nullptr;
  }
  while (true) {
    for (unsigned v = 0; v < g->size; v++)
      degrees[v] = 0;
    for (unsigned v = 0; v < g->size; v++)
      if (!removed[v])
        for (Edge *e = g->edges[v]; e; e = e->next)
          if (e->destination < g->size && !removed[e->destination]) {
            degrees[v]++;
            degrees[e->destination]++;
          }
    unsigned minimumDegree = UINT_MAX;
    unsigned minimumVertex = UINT_MAX;
    for (unsigned v = 0; v < g->size; v++)
      if (!removed[v] && degrees[v] < minimumDegree) {
        minimumDegree = degrees[v];
        minimumVertex = v;
      }
    if (minimumVertex == UINT_MAX) break;
    independent[minimumVertex] = true;
    removed[minimumVertex] = true;
    for (Edge *e = g->edges[minimumVertex]; e; e = e->next)
      if (e->destination < g->size)
        removed[e->destination] = true;
    for (unsigned v = 0; v < g->size; v++)
      for (Edge *e = g->edges[v]; e; e = e->next)
        if (e->destination == minimumVertex)
          removed[v] = true;
  }
  free(degrees);
  free(removed);
  return independent;
}

static void searchForMaximumIndependentSet(
  const Graph *g, unsigned v, bool *current, unsigned currentCount, bool *maximum, unsigned *maximumCount)
{
  if (v >= g->size) {
    if (currentCount > *maximumCount) {
      *maximumCount = currentCount;
      for (unsigned u = 0; u < g->size; u++) maximum[u] = current[u];
    }
    return;
  }
  bool includable = true;
  for (Edge *e = g->edges[v]; e; e = e->next)
    if ((e->destination < g->size && current[e->destination]) || e->destination == v)
      includable = false;
  for (unsigned u = 0; u < v; u++)
    if (current[u])
      for (Edge *e = g->edges[u]; e; e = e->next)
        if (e->destination == v)
          includable = false;
  if (includable) {
    current[v] = true;
    searchForMaximumIndependentSet(g, v + 1, current, currentCount + 1, maximum, maximumCount);
    current[v] = false;
  }
  searchForMaximumIndependentSet(g, v + 1, current, currentCount, maximum, maximumCount);
}

[[nodiscard]] bool *findMaximumIndependentSet(const Graph *g) {
  if (!g || !g->edges || g->size == 0) return nullptr;
  bool *current = malloc(g->size * sizeof(bool));
  bool *maximum = malloc(g->size * sizeof(bool));
  if (!current || !maximum) {
    free(current);
    free(maximum);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++) current[v] = maximum[v] = false;
  unsigned maximumCount = 0;
  searchForMaximumIndependentSet(g, 0, current, 0, maximum, &maximumCount);
  free(current);
  return maximum;
}

static bool findFeedbackVertexSet_hasCycleDfs(const Graph *g, const bool *removed, unsigned v, char *visited) {
  visited[v] = 1;
  for (Edge *e = g->edges[v]; e; e = e->next) {
    if (e->destination >= g->size || removed[e->destination]) continue;
    if (visited[e->destination] == 1) return true;
    if (visited[e->destination] == 0 && findFeedbackVertexSet_hasCycleDfs(g, removed, e->destination, visited)) return true;
  }
  visited[v] = 2;
  return false;
}

static bool findFeedbackVertexSet_hasCycle(const Graph *g, const bool *removed) {
  char *visited = calloc(g->size, sizeof(char));
  if (!visited) return false;
  for (unsigned v = 0; v < g->size; v++)
    if (visited[v] == 0 && !removed[v] && findFeedbackVertexSet_hasCycleDfs(g, removed, v, visited)) {
      free(visited);
      return true;
    }
  free(visited);
  return false;
}

static void findFeedbackVertexSetBacktracking(
  const Graph *g, unsigned v, bool *current, unsigned currentSize, bool *best, unsigned *bestSize)
{
  if (currentSize >= *bestSize) return;
  if (v == g->size) {
    if (!findFeedbackVertexSet_hasCycle(g, current)) {
      for (unsigned u = 0; u < g->size; u++) best[u] = current[u];
      *bestSize = currentSize;
    }
    return;
  }
  current[v] = false;
  findFeedbackVertexSetBacktracking(g, v + 1, current, currentSize, best, bestSize);
  current[v] = true;
  findFeedbackVertexSetBacktracking(g, v + 1, current, currentSize + 1, best, bestSize);
}

[[nodiscard]] bool *findFeedbackVertexSet(const Graph *g) {
  if (!g || g->size == 0) return nullptr;
  bool *current = calloc(g->size, sizeof(bool));
  bool *best = calloc(g->size, sizeof(bool));
  if (!current || !best) {
    free(current);
    free(best);
    return nullptr;
  }
  unsigned bestSize = UINT_MAX;
  findFeedbackVertexSetBacktracking(g, 0, current, 0, best, &bestSize);
  free(current);
  return best;
}

[[nodiscard]] bool *getVertexCut(const Graph *g) {
  if (!g || g->size <= 1) return nullptr;
  unsigned global = calculateVertexConnectivity(g);
  if (global >= g->size - 1) return nullptr;
  for (unsigned u = 0; u < g->size; u++)
    for (unsigned v = 0; v < g->size; v++) {
      if (u == v || hasDirectedEdge(g, u, v)) continue;
      unsigned local = calculateLocalVertexConnectivity(g, u, v);
      if (local == global) return getLocalVertexCut(g, u, v);
    }
  return nullptr;
}

[[nodiscard]] bool *getInNeighbors(const Graph *g, unsigned v) {
  if (!g || !g->edges) return nullptr;
  bool *neighbors = calloc(g->size, sizeof(bool));
  if (!neighbors) return nullptr;
  for (unsigned u = 0; u < g->size; u++)
    for (Edge *e = g->edges[u]; e; e = e->next)
      if (e->destination == v)
        neighbors[u] = true;
  return neighbors;
}

[[nodiscard]] bool *getOutNeighbors(const Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return nullptr;
  bool *neighbors = calloc(g->size, sizeof(bool));
  if (!neighbors) return nullptr;
  for (Edge *e = g->edges[v]; e; e = e->next)
    if (e->destination < g->size)
      neighbors[e->destination] = true;
  return neighbors;
}

[[nodiscard]] bool *getVerticesReachableFrom(const Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return nullptr;
  bool *reach = calloc(g->size, sizeof(bool));
  if (!reach) return nullptr;
  unsigned stack[g->size];
  unsigned top = 0;
  stack[top++] = v;
  reach[v] = true;
  while (top > 0)
    for (Edge *e = g->edges[stack[--top]]; e; e = e->next)
      if (e->destination < g->size && !reach[e->destination]) {
        reach[e->destination] = true;
        stack[top++] = e->destination;
      }
  return reach;
}

[[nodiscard]] bool *getVerticesThatCanReach(const Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return nullptr;
  bool *reach = calloc(g->size, sizeof(bool));
  if (!reach) return nullptr;
  unsigned stack[g->size];
  unsigned top = 0;
  stack[top++] = v;
  reach[v] = true;
  while (top > 0) {
    unsigned u = stack[--top];
    for (unsigned w = 0; w < g->size; w++)
      if (!reach[w])
        for (Edge *e = g->edges[w]; e; e = e->next)
          if (e->destination == u) {
            reach[w] = true;
            stack[top++] = w;
            break;
          }
  }
  return reach;
}

[[nodiscard]] bool *getCommonNeighbors(const Graph *g, unsigned u, unsigned v) {
  bool *s1 = getOutNeighbors(g, u);
  bool *s2 = getOutNeighbors(g, v);
  if (!g || !s1 || !s2) {
    free(s1);
    free(s2);
    return nullptr;
  }
  for (unsigned w = 0; w < g->size; w++) s1[w] = s1[w] && s2[w];
  free(s2);
  return s1;
}

static bool findAugmentingPath(unsigned u, unsigned v, unsigned n, unsigned capacity[n][n], bool visited[n]) {
  if (u == v) return true;
  visited[u] = true;
  for (unsigned w = 0; w < n; w++)
    if (!visited[w] && capacity[u][w] > 0 && findAugmentingPath(w, v, n, capacity, visited)) {
      capacity[u][w]--;
      capacity[w][u]++;
      return true;
    }
  return false;
}

static void findReachableVertices(unsigned u, unsigned n, unsigned capacity[n][n], bool visited[n]) {
  visited[u] = true;
  for (unsigned v = 0; v < n; v++)
    if (!visited[v] && capacity[u][v] > 0)
      findReachableVertices(v, n, capacity, visited);
}

[[nodiscard]] bool *getLocalVertexCut(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size || u == v || hasDirectedEdge(g, u, v)) return nullptr;
  unsigned capacity[2 * g->size][2 * g->size] = {};
  bool visited[2 * g->size] = {};
  for (unsigned w = 0; w < g->size; w++) capacity[2 * w][2 * w + 1] = 1;
  for (unsigned w = 0; w < g->size; w++)
    for (Edge *e = g->edges[w]; e; e = e->next)
      if (e->destination < g->size)
        capacity[2 * w + 1][2 * e->destination] = g->size + 1;
  while (true) {
    for (unsigned i = 0; i < 2 * g->size; i++) visited[i] = false;
    if (!findAugmentingPath(2 * u + 1, 2 * v, 2 * g->size, capacity, visited)) break;
  }
  for (unsigned i = 0; i < 2 * g->size; i++) visited[i] = false;
  findReachableVertices(2 * u + 1, 2 * g->size, capacity, visited);
  bool *cut = calloc(g->size, sizeof(bool));
  if (!cut) return nullptr;
  for (unsigned w = 0; w < g->size; w++)
    if (w != u && w != v && visited[2 * w] && !visited[2 * w + 1])
      cut[w] = true;
  return cut;
}



[[nodiscard]] bool **createAdjacencyMatrix(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  bool **adjacency = allocateFalseMatrix(g->size, g->size);
  if (!adjacency) return nullptr;
  for (unsigned u = 0; u < g->size; u++)
    for (const Edge *e = g->edges[u]; e; e = e->next)
      if (e->destination < g->size)
        adjacency[u][e->destination] = true;
  return adjacency;
}

[[nodiscard]] bool **findEdgeCut(const Graph *g) {
  if (!g || !g->edges || g->size <= 1) return nullptr;
  unsigned minimum = UINT_MAX;
  unsigned source = 0;
  unsigned sink = 1;
  for (unsigned v = 1; v < g->size; v++) {
    unsigned flow = calculateLocalEdgeConnectivity(g, 0, v);
    if (flow < minimum) {
      minimum = flow;
      source = 0;
      sink = v;
    }
    flow = calculateLocalEdgeConnectivity(g, v, 0);
    if (flow < minimum) {
      minimum = flow;
      source = v;
      sink = 0;
    }
  }
  return findLocalEdgeCut(g, source, sink);
}

static void initializeResidualMatrix(const Graph *g, unsigned residual[g->size][g->size]) {
  for (unsigned u = 0; u < g->size; u++)
    for (const Edge *e = g->edges[u]; e; e = e->next)
      if (e->destination < g->size)
        residual[u][e->destination]++;
}

[[nodiscard]] bool **findLocalEdgeCut(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size || u == v) return nullptr;
  bool **cut = allocateFalseMatrix(g->size, g->size);
  if (!cut) return nullptr;
  unsigned residual[g->size][g->size] = {};
  initializeResidualMatrix(g, residual);
  while (true) {
    bool visited[g->size] = {};
    if (!findAugmentingPath(u, v, g->size, residual, visited)) break;
  }
  bool reachable[g->size] = {};
  findReachableVertices(u, g->size, residual, reachable);
  for (unsigned w = 0; w < g->size; w++)
    if (reachable[w])
      for (const Edge *e = g->edges[w]; e; e = e->next)
        if (e->destination < g->size && !reachable[e->destination])
          cut[w][e->destination] = true;
  return cut;
}



[[nodiscard]] Graph *createGraph(unsigned n) {
  Graph *g = malloc(sizeof(Graph));
  Edge **edges = calloc(n, sizeof(Edge *));
  if (!g || (n > 0 && !edges)) {
    free(g);
    free(edges);
    return nullptr;
  }
  g->size = n;
  g->edges = edges;
  return g;
}

[[nodiscard]] Graph *createPath(unsigned n) {
  Graph *g = createGraph(n);
  for (unsigned v = 1; v < n; v++) addUndirectedEdge(g, v - 1, v, 1);
  return g;
}

[[nodiscard]] Graph *createCycle(unsigned n) {
  Graph *g = createGraph(n);
  for (unsigned v = 0; v < n; v++) addUndirectedEdge(g, v, (v + 1) % n, 1);
  return g;
}

[[nodiscard]] Graph *createStar(unsigned n) {
  Graph *g = createGraph(n);
  for (unsigned v = 1; v < n; v++) addUndirectedEdge(g, 0, v, 1);
  return g;
}

[[nodiscard]] Graph *createWheel(unsigned n) {
  Graph *g = createGraph(n);
  for (unsigned v = 1; v < n; v++) addUndirectedEdge(g, 0, v, 1);
  for (unsigned v = 2; v < n; v++) addUndirectedEdge(g, v - 1, v, 1);
  addUndirectedEdge(g, n - 1, 1, 1);
  return g;
}

[[nodiscard]] Graph *createComplete(unsigned n) {
  Graph *g = createGraph(n);
  for (unsigned u = 0; u < n; u++)
    for (unsigned v = 0; v < n; v++)
      if (u != v)
        addDirectedEdge(g, u, v);
  return g;
}

[[nodiscard]] Graph *createRandom(unsigned n, double p, bool directed, bool weighted) {
  if (p < 0 || p > 1) return nullptr;
  Graph *g = createGraph(n);
  for (unsigned u = 0; u < n; u++)
    for (unsigned v = directed ? 0 : u + 1; v < n; v++)
      if (u != v)
        if (rand() / ((double)RAND_MAX + 1) < p) {
          double weight = weighted ? 1 + (rand() / ((double)RAND_MAX + 1)) * 9 : 1;
          if (directed) addWeightedDirectedEdge(g, u, v, weight); else addUndirectedEdge(g, u, v, weight);
        }
  return g;
}

[[nodiscard]] Graph *copyGraph(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *g2 = createGraph(g->size);
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      addWeightedDirectedEdge(g2, v, e->destination, e->weight);
  return g2;
}

[[nodiscard]] Graph *createTranspose(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *transpose = createGraph(g->size);
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      addWeightedDirectedEdge(transpose, e->destination, v, e->weight);
  return transpose;
}

[[nodiscard]] Graph *createUnweighted(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *g2 = createGraph(g->size);
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      addDirectedEdge(g2, v, e->destination);
  return g2;
}

[[nodiscard]] Graph *createUndirected(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *g2 = createGraph(g->size);
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      addUndirectedEdge(g2, v, e->destination, e->weight);
  return g2;
}

[[nodiscard]] Graph *createComplement(const Graph *g) {
  if (!g) return nullptr;
  Graph *g2 = createGraph(g->size);
  for (unsigned u = 0; u < g->size; u++)
    for (unsigned v = 0; v < g->size; v++)
      if (u != v && !hasDirectedEdge(g, u, v))
        addDirectedEdge(g2, u, v);
  return g2;
}

[[nodiscard]] Graph *createDirectedLine(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *g2 = createGraph(countEdges(g));
  unsigned i = 0;
  for (unsigned u = 0; u < g->size; u++)
    for (Edge *d = g->edges[u]; d; d = d->next) {
      unsigned j = 0;
      for (unsigned v = 0; v < g->size; v++)
        for (Edge *e = g->edges[v]; e; e = e->next) {
          if (d->destination == v)
            addDirectedEdge(g2, i, j);
          j++;
        }
      i++;
    }
  return g2;
}

[[nodiscard]] Graph *createUndirectedLine(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *g2 = createGraph(countEdges(g) / 2);
  unsigned i = 0;
  for (unsigned u = 0; u < g->size; u++) {
    unsigned uSelf = 0;
    for (Edge *d = g->edges[u]; d; d = d->next) {
      if (u < d->destination || (u == d->destination && uSelf % 2 == 0)) {
        unsigned j = 0;
        for (unsigned v = 0; v < g->size; v++) {
          unsigned vSelf = 0;
          for (Edge *e = g->edges[v]; e; e = e->next) {
            if (v < e->destination || (v == e->destination && vSelf % 2 == 0)) {
              if (i < j && (u == v || u == e->destination || d->destination == v || d->destination == e->destination))
                addUndirectedEdge(g2, i, j, 1);
              if (i == j && (u == e->destination || d->destination == v))
                addUndirectedEdge(g2, i, j, 1);
              j++;
            }
            if (v == e->destination) vSelf++;
          }
        }
        i++;
      }
      if (u == d->destination) uSelf++;
    }
  }
  return g2;
}

[[nodiscard]] Graph *createUnderlying(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *g2 = createGraph(g->size);
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (v != e->destination && !hasUndirectedEdge(g2, v, e->destination))
        addUndirectedEdge(g2, v, e->destination, 1);
  return g2;
}

[[nodiscard]] Graph *createKruskal(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *mst = createGraph(g->size);
  while (true) {
    unsigned u, v;
    double weight = INFINITY;
    for (unsigned w = 0; w < g->size; w++)
      for (Edge *e = g->edges[w]; e; e = e->next)
        if (e->weight < weight && !hasPath(mst, w, e->destination)) {
          u = w;
          v = e->destination;
          weight = e->weight;
        }
    if (weight == INFINITY) break;
    addUndirectedEdge(mst, u, v, weight);
  }
  return mst;
}

[[nodiscard]] Graph *createPrim(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *mst = createGraph(g->size);
  if (!mst || g->size < 2) return mst;
  bool *added = calloc(g->size, sizeof(bool));
  double *weights = malloc(g->size * sizeof(double));
  unsigned *parents = malloc(g->size * sizeof(unsigned));
  if (!added || !weights || !parents) {
    free(added);
    free(weights);
    free(parents);
    destroyGraph(mst);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++) {
    weights[v] = INFINITY;
    parents[v] = UINT_MAX;
  }
  unsigned u = 0;
  weights[0] = 0;
  do {
    added[u] = true;
    for (Edge *e = g->edges[u]; e; e = e->next)
      if (e->destination < g->size && !added[e->destination] && e->weight < weights[e->destination]) {
        weights[e->destination] = e->weight;
        parents[e->destination] = u;
      }
    double weight = INFINITY;
    u = UINT_MAX;
    for (unsigned v = 0; v < g->size; v++)
      if (!added[v] && weights[v] < weight) {
        weight = weights[v];
        u = v;
      }
  } while (u != UINT_MAX);
  for (unsigned v = 1; v < g->size; ++v)
    if (parents[v] != UINT_MAX)
      addUndirectedEdge(mst, parents[v], v, weights[v]);
  free(added);
  free(weights);
  free(parents);
  return mst;
}

[[nodiscard]] Graph *createDirectedSubdivision(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *g2 = createGraph(g->size + countEdges(g));
  unsigned u = g->size;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next) {
      addWeightedDirectedEdge(g2, v, u, e->weight);
      addWeightedDirectedEdge(g2, u, e->destination, e->weight);
      u++;
    }
  return g2;
}

[[nodiscard]] Graph *createUndirectedSubdivision(const Graph *g) {
  if (!g || (g->size > 0 && !g->edges)) return nullptr;
  Graph *g2 = createGraph(g->size + countEdges(g) / 2);
  unsigned u = g->size;
  for (unsigned v = 0; v < g->size; v++) {
    unsigned self = 0;
    for (Edge *e = g->edges[v]; e; e = e->next) {
      if (v < e->destination || (v == e->destination && self % 2 == 0)) {
        addUndirectedEdge(g2, v, u, e->weight);
        addUndirectedEdge(g2, u, e->destination, e->weight);
        u++;
      }
      if (v == e->destination) self++;
    }
  }
  return g2;
}

[[nodiscard]] Graph *createTransitiveClosure(const Graph *g) {
  if (!g) return nullptr;
  Graph *closure = createGraph(g->size);
  for (unsigned u = 0; u < g->size; u++)
    for (unsigned v = 0; v < g->size; v++)
      if (u != v && hasPath(g, u, v))
        addDirectedEdge(closure, u, v);
  return closure;
}

[[nodiscard]] Graph *findFeedbackArcSet(const Graph *g) {
  if (!g) return nullptr;
  double *weights = calloc(g->size, sizeof(double));
  unsigned *permutation = malloc(g->size * sizeof(unsigned));
  unsigned *position = malloc(g->size * sizeof(unsigned));
  Graph *result = createGraph(g->size);
  if ((g->size > 0 && (!weights || !permutation || !position)) || !result) {
    free(weights);
    free(permutation);
    free(position);
    destroyGraph(result);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size) {
        weights[v] += e->weight;
        weights[e->destination] -= e->weight;
      }
  for (unsigned i = 0; i < g->size; i++) permutation[i] = i;
  for (unsigned i = 1; i < g->size; i++) {
    unsigned v = permutation[i], j;
    for (j = i; j > 0 && weights[permutation[j - 1]] < weights[v]; j--) permutation[j] = permutation[j - 1];
    permutation[j] = v;
  }
  for (unsigned i = 0; i < g->size; i++) position[permutation[i]] = i;
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size && position[v] >= position[e->destination])
        addWeightedDirectedEdge(result, v, e->destination, e->weight);
  free(weights);
  free(permutation);
  free(position);
  return result;
}

[[nodiscard]] Graph *createPower(const Graph *g, unsigned k) {
  if (!g) return nullptr;
  Graph *power = createGraph(g->size);
  for (unsigned u = 0; u < g->size; u++)
    for (unsigned v = 0; v < g->size; v++)
      if (u != v && calculateUnweightedDistance(g, u, v) <= k)
        addDirectedEdge(power, u, v);
  return power;
}

[[nodiscard]] Graph *createVertexSubgraph(const Graph *g, const bool *set) {
  if (!isValid(g) || !set) return nullptr;
  Graph *g2 = createGraph(g->size);
  for (unsigned v = 0; v < g->size; v++)
    if (set[v])
      for (Edge *e = g->edges[v]; e; e = e->next)
        if (set[e->destination])
          addWeightedDirectedEdge(g2, v, e->destination, e->weight);
  return g2;
}

[[nodiscard]] Graph *createEdgeSubgraph(const Graph *g, const bool *set) {
  if (!g || (g->size > 0 && !g->edges) || !set) return nullptr;
  Graph *g2 = createGraph(g->size);
  unsigned i = 0;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next) {
      if (set[i])
        addWeightedDirectedEdge(g2, v, e->destination, e->weight);
      i++;
    }
  return g2;
}

[[nodiscard]] Graph *createUnion(const Graph *g1, const Graph *g2) {
  if (!g1 || (g1->size > 0 && !g1->edges) || !g2 || (g2->size > 0 && !g2->edges)) return nullptr;
  Graph *g3 = createGraph(unsignedMaximum(g1->size, g2->size));
  for (unsigned v = 0; v < g1->size; v++)
    for (Edge *e = g1->edges[v]; e; e = e->next)
      if (!hasDirectedEdge(g3, v, e->destination))
        addWeightedDirectedEdge(g3, v, e->destination, e->weight);
  for (unsigned v = 0; v < g2->size; v++)
    for (Edge *e = g2->edges[v]; e; e = e->next)
      if (!hasDirectedEdge(g3, v, e->destination))
        addWeightedDirectedEdge(g3, v, e->destination, e->weight);
  return g3;
}

[[nodiscard]] Graph *createDisjointUnion(const Graph *g1, const Graph *g2) {
  if (!g1 || (g1->size > 0 && !g1->edges) || !g2 || (g2->size > 0 && !g2->edges)) return nullptr;
  Graph *g3 = createGraph(g1->size + g2->size);
  for (unsigned v = 0; v < g1->size; v++)
    for (Edge *e = g1->edges[v]; e; e = e->next)
      addWeightedDirectedEdge(g3, v, e->destination, e->weight);
  for (unsigned v = 0; v < g2->size; v++)
    for (Edge *e = g2->edges[v]; e; e = e->next)
      addWeightedDirectedEdge(g3, g1->size + v, g1->size + e->destination, e->weight);
  return g3;
}

[[nodiscard]] Graph *createCartesianProduct(const Graph *g1, const Graph *g2) {
  if (!g1 || (g1->size > 0 && !g1->edges) || !g2 || (g2->size > 0 && !g2->edges)) return nullptr;
  Graph *g3 = createGraph(g1->size * g2->size);
  for (unsigned u = 0; u < g1->size; u++)
    for (unsigned v = 0; v < g2->size; v++) {
      for (Edge *e = g2->edges[v]; e; e = e->next)
        addWeightedDirectedEdge(g3, u * g2->size + v, u * g2->size + e->destination, e->weight);
      for (Edge *e = g1->edges[u]; e; e = e->next)
        addWeightedDirectedEdge(g3, u * g2->size + v, e->destination * g2->size + v, e->weight);
    }
  return g3;
}

[[nodiscard]] Graph *createTensorProduct(const Graph *g1, const Graph *g2) {
  if (!g1 || (g1->size > 0 && !g1->edges) || !g2 || (g2->size > 0 && !g2->edges)) return nullptr;
  Graph *g3 = createGraph(g1->size * g2->size);
  for (unsigned u = 0; u < g1->size; u++)
    for (unsigned v = 0; v < g2->size; v++)
      for (Edge *e1 = g1->edges[u]; e1; e1 = e1->next)
        for (Edge *e2 = g2->edges[v]; e2; e2 = e2->next)
          addWeightedDirectedEdge(g3, u * g2->size + v, e1->destination * g2->size + e2->destination, e1->weight * e2->weight);
  return g3;
}

[[nodiscard]] Graph *createLexicographicalProduct(const Graph *g1, const Graph *g2) {
  if (!g1 || (g1->size > 0 && !g1->edges) || !g2 || (g2->size > 0 && !g2->edges)) return nullptr;
  Graph *g3 = createGraph(g1->size * g2->size);
  for (unsigned u = 0; u < g1->size; u++)
    for (unsigned v = 0; v < g2->size; v++)
      for (Edge *e = g2->edges[v]; e; e = e->next)
        addWeightedDirectedEdge(g3, u * g2->size + v, u * g2->size + e->destination, e->weight);
  for (unsigned u = 0; u < g1->size; u++)
    for (Edge *e = g1->edges[u]; e; e = e->next)
      for (unsigned v = 0; v < g2->size; v++)
        for (unsigned w = 0; w < g2->size; w++)
          addWeightedDirectedEdge(g3, u * g2->size + v, e->destination * g2->size + w, e->weight);
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
  Edge **edges = realloc(g->edges, (g->size + 1) * sizeof(Edge *));
  if (!edges) return;
  g->edges = edges;
  g->edges[g->size] = nullptr;
  g->size = g->size + 1;
}

void deleteSelfLoops(Graph *g) {
  if (!g) return;
  for (unsigned v = 0; v < g->size; v++)
    deleteMatchingEdges(g, v, v);
}

void deleteInvalidEdges(Graph *g) {
  if (!g || !g->edges) return;
  for (unsigned v = 0; v < g->size; v++) {
    Edge **e = &g->edges[v];
    while (*e)
      if ((*e)->destination >= g->size) {
        Edge *next = (*e)->next;
        free(*e);
        *e = next;
      } else {
        e = &(*e)->next;
      }
  }
}

void printGraph(const Graph *g) {
  printf("{");
  unsigned i = 0;
  if (g && g->edges)
    for (unsigned v = 0; v < g->size; v++)
      for (Edge *e = g->edges[v]; e; e = e->next) {
        if (i++ > 0) printf(", ");
        printf("(%u, %u, %lg)", v, e->destination, e->weight);
      }
  printf("}\n");
}

void deleteDirectedEdgeByIndex(Graph *g, unsigned i) {
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

void deleteOutgoingEdges(Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return;
  Edge *e = g->edges[v];
  while (e) {
    Edge *next = e->next;
    free(e);
    e = next;
  }
  g->edges[v] = nullptr;
}

void deleteIncomingEdges(Graph *g, unsigned v) {
  if (!g) return;
  for (unsigned u = 0; u < g->size; u++)
    deleteMatchingEdges(g, u, v);
}

void deleteVertex(Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return;
  deleteIncomingEdges(g, v);
  deleteOutgoingEdges(g, v);
  for (unsigned u = 0; u < g->size; u++)
    for (Edge *e = g->edges[u]; e; e = e->next)
      if (e->destination > v)
        e->destination--;
  for (unsigned u = v + 1; u < g->size; u++)
    g->edges[u - 1] = g->edges[u];
  g->size--;
}

void addDirectedEdge(Graph *g, unsigned u, unsigned v) {
  addWeightedDirectedEdge(g, u, v, 1);
}

void deleteFirstDirectedEdge(Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size) return;
  Edge **e = &g->edges[u];
  while (*e) {
    Edge *current = *e;
    if (current->destination == v) {
      *e = current->next;
      free(current);
      return;
    }
    e = &current->next;
  }
}

void deleteFirstUndirectedEdge(Graph *g, unsigned u, unsigned v) {
  double weight = getEdgeWeight(g, u, v);
  deleteFirstWeightedDirectedEdge(g, u, v, weight);
  deleteFirstWeightedDirectedEdge(g, v, u, weight);
}

void deleteMatchingEdges(Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size) return;
  Edge **e = &g->edges[u];
  while (*e) {
    Edge *temporary = *e;
    if (temporary->destination == v) {
      *e = temporary->next;
      free(temporary);
    } else {
      e = &temporary->next;
    }
  }
}

void transferOutgoingEdges(Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size || u == v) return;
  while (g->edges[u]) {
    Edge *e = g->edges[u];
    g->edges[u] = e->next;
    e->next = g->edges[v];
    g->edges[v] = e;
  }
}

void transferIncomingEdges(Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size || u == v) return;
  for (unsigned w = 0; w < g->size; w++)
    for (Edge *e = g->edges[w]; e; e = e->next)
      if (e->destination == u)
        e->destination = v;
}

void contractVertices(Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size || u == v) return;
  deleteMatchingEdges(g, u, v);
  deleteMatchingEdges(g, v, u);
  transferOutgoingEdges(g, u, v);
  transferIncomingEdges(g, u, v);
  for (unsigned w = 0; w < g->size; w++)
    for (Edge *e = g->edges[w]; e; e = e->next)
      if (e->destination > u)
        e->destination--;
  for (unsigned w = u + 1; w < g->size; w++)
    g->edges[w - 1] = g->edges[w];
  g->size--;
}

void subdivideEdge(Graph *g, unsigned u, unsigned v) {
  if (!g) return;
  double weight = getEdgeWeight(g, u, v);
  deleteFirstWeightedDirectedEdge(g, u, v, weight);
  addVertex(g);
  addWeightedDirectedEdge(g, u, g->size - 1, weight / 2);
  addWeightedDirectedEdge(g, g->size - 1, v, weight / 2);
}

void addWeightedDirectedEdge(Graph *g, unsigned u, unsigned v, double weight) {
  if (!g || !g->edges || u >= g->size) return;
  Edge *e = malloc(sizeof(Edge));
  if (!e) return;
  e->destination = v;
  e->weight = weight;
  e->next = g->edges[u];
  g->edges[u] = e;
}

void addUndirectedEdge(Graph *g, unsigned u, unsigned v, double x) {
  addWeightedDirectedEdge(g, u, v, x);
  addWeightedDirectedEdge(g, v, u, x);
}

void deleteFirstWeightedDirectedEdge(Graph *g, unsigned u, unsigned v, double weight) {
  if (!g || !g->edges || u >= g->size) return;
  Edge **e = &g->edges[u];
  while (*e)
    if ((*e)->destination == v && (*e)->weight == weight) {
      Edge *temporary = *e;
      *e = (*e)->next;
      free(temporary);
      break;
    } else {
      e = &(*e)->next;
    }
}

void deleteFirstWeightedUndirectedEdge(Graph *g, unsigned u, unsigned v, double weight) {
  deleteFirstWeightedDirectedEdge(g, u, v, weight);
  deleteFirstWeightedDirectedEdge(g, v, u, weight);
}



unsigned getSize(const Graph *g) {
  if (!g) return 0;
  return g->size;
}

unsigned countEdges(const Graph *g) {
  if (!g || !g->edges) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      n++;
  return n;
}

unsigned countSelfLoops(const Graph *g) {
  if (!g) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++) n += countSelfLoopsAtVertex(g, v);
  return n;
}

unsigned countTriangles(const Graph *g) {
  if (!isValid(g)) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *d = g->edges[v]; d; d = d->next)
      if (d->destination != v)
        for (Edge *e = g->edges[d->destination]; e; e = e->next)
          if (e->destination != v && e->destination != d->destination)
            for (Edge *f = g->edges[e->destination]; f; f = f->next)
              if (f->destination == v)
                n++;
  return n;
}

unsigned getMinimumInDegree(const Graph *g) {
  if (!g) return UINT_MAX;
  unsigned *degree = getInDegrees(g);
  if (!degree) return UINT_MAX;
  unsigned minimum = UINT_MAX;
  for (unsigned v = 0; v < g->size; v++)
    if (degree[v] < minimum)
      minimum = degree[v];
  free(degree);
  return minimum;
}

unsigned getMaximumInDegree(const Graph *g) {
  if (!g) return 0;
  unsigned *degree = getInDegrees(g);
  if (!degree) return 0;
  unsigned maximum = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (degree[v] > maximum)
      maximum = degree[v];
  free(degree);
  return maximum;
}

unsigned getMinimumOutDegree(const Graph *g) {
  if (!g) return UINT_MAX;
  unsigned minimum = UINT_MAX;
  for (unsigned v = 0; v < g->size; v++)
    if (getOutDegree(g, v) < minimum)
      minimum = getOutDegree(g, v);
  return minimum;
}

unsigned getMaximumOutDegree(const Graph *g) {
  if (!g) return 0;
  unsigned maximum = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (getOutDegree(g, v) > maximum)
      maximum = getOutDegree(g, v);
  return maximum;
}

unsigned getMinimumDegree(const Graph *g) {
  if (!g) return UINT_MAX;
  unsigned *degrees = getDegrees(g);
  if (!degrees) return UINT_MAX;
  unsigned minimum = UINT_MAX;
  for (unsigned v = 0; v < g->size; v++)
    if (degrees[v] < minimum)
      minimum = degrees[v];
  free(degrees);
  return minimum;
}

unsigned getMaximumDegree(const Graph *g) {
  if (!g) return 0;
  unsigned *degrees = getDegrees(g);
  if (!degrees) return 0;
  unsigned maximum = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (degrees[v] > maximum)
      maximum = degrees[v];
  free(degrees);
  return maximum;
}

unsigned countSourceLeaves(const Graph *g) {
  if (!g) return 0;
  unsigned *in = getInDegrees(g);
  if (!in) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (in[v] == 0 && getOutDegree(g, v) == 1)
      n++;
  free(in);
  return n;
}

unsigned countSinkLeaves(const Graph *g) {
  if (!g) return 0;
  unsigned *in = getInDegrees(g);
  if (!in) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (in[v] == 1 && getOutDegree(g, v) == 0)
      n++;
  free(in);
  return n;
}

unsigned countUndirectedLeaves(const Graph *g) {
  if (!g) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (getOutDegree(g, v) == 1)
      n++;
  return n;
}

unsigned countIsolatedVertices(const Graph *g) {
  if (!g) return 0;
  bool *isolated = getIsolatedVertices(g);
  if (!isolated) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (isolated[v])
      n++;
  free(isolated);
  return n;
}

unsigned countStronglyConnectedComponents(const Graph *g) {
  if (!g || g->size == 0) return 0;
  unsigned *components = getStronglyConnectedComponents(g);
  if (!components) return 0;
  unsigned count = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (components[v] == v)
      count++;
  free(components);
  return count;
}

unsigned countSources(const Graph *g) {
  if (!g) return 0;
  bool *sources = getSources(g);
  if (!sources) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (sources[v])
      n++;
  free(sources);
  return n;
}

unsigned countSinks(const Graph *g) {
  if (!g) return 0;
  bool *sinks = getSinks(g);
  if (!sinks) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (sinks[v])
      n++;
  free(sinks);
  return n;
}

unsigned countParallelEdges(const Graph *g) {
  if (!isValid(g)) return 0;
  bool *seen = calloc(g->size, sizeof(bool));
  if (!seen) return 0;
  unsigned count = 0;
  for (unsigned v = 0; v < g->size; v++) {
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (seen[e->destination])
        count++;
      else
        seen[e->destination] = true;
    for (Edge *e = g->edges[v]; e; e = e->next)
      seen[e->destination] = false;
  }
  free(seen);
  return count;
}

unsigned countComponents(const Graph *g) {
  if (!isValid(g)) return 0;
  Graph *g2 = createUndirected(g);
  if (!g2) return 0;
  deleteInvalidEdges(g2);
  bool *visited = calloc(g2->size, sizeof(bool));
  unsigned *stack = malloc(g2->size * sizeof(unsigned));
  unsigned n = 0;
  if (visited && stack)
    for (unsigned u = 0; u < g2->size; u++)
      if (!visited[u]) {
        n++;
        unsigned size = 0;
        stack[size++] = u;
        visited[u] = true;
        while (size > 0)
          for (Edge *e = g2->edges[stack[--size]]; e; e = e->next)
            if (!visited[e->destination]) {
              visited[e->destination] = true;
              stack[size++] = e->destination;
            }
      }
  destroyGraph(g2);
  free(visited);
  free(stack);
  return n;
}

unsigned getFirstActiveVertex(const Graph *g) {
  if (g && g->edges)
    for (unsigned v = 0; v < g->size; v++)
      if (g->edges[v])
        return v;
  return UINT_MAX;
}

unsigned calculateWienerIndex(const Graph *g) {
  if (!g) return 0;
  unsigned sum = 0;
  for (unsigned u = 0; u < g->size; u++) {
    unsigned *distances = calculateUnweightedDistances(g, u);
    if (!distances) return 0;
    for (unsigned v = 0; v < g->size; v++)
      if (distances[v] < UINT_MAX) {
        sum += distances[v];
      } else {
        free(distances);
        return UINT_MAX;
      }
    free(distances);
  }
  return sum;
}

unsigned countInvalidEdges(const Graph *g) {
  if (!g || !g->edges) return 0;
  unsigned n = 0;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination >= g->size)
        n++;
  return n;
}

unsigned calculateDirectedUnweightedGirth(const Graph *g) {
  if (!g || !g->edges) return UINT_MAX;
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  unsigned *distances = malloc(g->size * sizeof(unsigned));
  if (!queue || !distances) {
    free(queue);
    free(distances);
    return UINT_MAX;
  }
  unsigned minimum = UINT_MAX;
  for (unsigned start = 0; start < g->size; start++) {
    for (unsigned v = 0; v < g->size; v++) distances[v] = UINT_MAX;
    distances[start] = 0;
    unsigned head = 0, tail = 0;
    queue[tail++] = start;
    while (head < tail) {
      unsigned v = queue[head++];
      for (const Edge *e = g->edges[v]; e; e = e->next)
        if (e->destination == start) {
          if (distances[v] + 1 < minimum) minimum = distances[v] + 1;
        } else if (e->destination < g->size && distances[e->destination] == UINT_MAX) {
          distances[e->destination] = distances[v] + 1;
          queue[tail++] = e->destination;
        }
    }
  }
  free(queue);
  free(distances);
  return minimum;
}

unsigned calculateUndirectedUnweightedGirth(const Graph *g) {
  if (!g || !g->edges) return UINT_MAX;
  unsigned *distances = malloc(g->size * sizeof(unsigned));
  unsigned *parent = malloc(g->size * sizeof(unsigned));
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  if (!distances || !parent || !queue) {
    free(distances);
    free(parent);
    free(queue);
    return UINT_MAX;
  }
  unsigned minimum = UINT_MAX;
  for (unsigned start = 0; start < g->size; start++) {
    for (unsigned v = 0; v < g->size; v++) distances[v] = parent[v] = UINT_MAX;
    distances[start] = 0;
    unsigned head = 0, tail = 0;
    queue[tail++] = start;
    while (head < tail) {
      unsigned v = queue[head++];
      for (const Edge *e = g->edges[v]; e; e = e->next)
        if (e->destination < g->size) {
          if (distances[e->destination] == UINT_MAX) {
            distances[e->destination] = distances[v] + 1;
            parent[e->destination] = v;
            queue[tail++] = e->destination;
          } else if (e->destination != parent[v]) {
            unsigned length = distances[v] + distances[e->destination] + 1;
            if (length < minimum) minimum = length;
          }
        }
    }
  }
  free(distances);
  free(parent);
  free(queue);
  return minimum;
}

unsigned calculateUnweightedRadius(const Graph *g) {
  if (!g) return UINT_MAX;
  unsigned radius = UINT_MAX;
  for (unsigned v = 0; v < g->size; v++) {
    unsigned eccentricity = calculateUnweightedEccentricity(g, v);
    if (eccentricity < radius)
      radius = eccentricity;
  }
  return radius;
}

unsigned calculateUnweightedDiameter(const Graph *g) {
  if (!g) return 0;
  unsigned diameter = 0;
  for (unsigned v = 0; v < g->size; v++) {
    unsigned eccentricity = calculateUnweightedEccentricity(g, v);
    if (eccentricity > diameter)
      diameter = eccentricity;
  }
  return diameter;
}

unsigned calculateMinimumVertexCut(const Graph *g) {
  if (!g || g->size < 2) return 0;
  bool **adjacent = createAdjacencyMatrix(g);
  Graph *net = createGraph(2 * g->size);
  if (!adjacent || !net) {
    freeBooleanMatrix(adjacent, g->size);
    destroyGraph(net);
    return 0;
  }
  for (unsigned v = 0; v < g->size; v++) addWeightedDirectedEdge(net, v, v + g->size, 1);
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (v != e->destination)
        addWeightedDirectedEdge(net, v + g->size, e->destination, INFINITY);
  double minimum = g->size - 1;
  for (unsigned u = 0; u < g->size; u++)
    for (unsigned v = 0; v < g->size; v++) {
      if (u == v || adjacent[u][v]) continue;
      double flow = calculateMaxFlowEdmondsKarp(net, u + g->size, v);
      if (flow < minimum) minimum = flow;
    }
  freeBooleanMatrix(adjacent, g->size);
  destroyGraph(net);
  return round(minimum);
}

unsigned countSpanningTrees(const Graph *g) {
  if (!g || g->size == 0) return 0;
  Matrix *laplacian = createZeroMatrix(g->size - 1, g->size - 1);
  if (!laplacian) return 0;
  for (unsigned v = 0; v < g->size - 1; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size - 1) {
        laplacian->data[e->destination][e->destination] += 1;
        laplacian->data[v][e->destination] -= 1;
      }
  double determinant = calculateMatrixDeterminant(laplacian);
  destroyMatrix(laplacian);
  return determinant > 0 ? round(determinant) : 0;
}

unsigned calculateChromaticNumber(const Graph *g) {
  if (!g) return 0;
  unsigned *colors = findOptimalColoring(g);
  if (!colors) return 0;
  unsigned chromatic = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (colors[v] >= chromatic)
      chromatic = colors[v] + 1;
  free(colors);
  return chromatic;
}

unsigned calculateCliqueNumber(const Graph *g) {
  if (!g) return 0;
  bool *clique = findMaximumClique(g);
  if (!clique) return 0;
  unsigned size = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (clique[v])
      size++;
  free(clique);
  return size;
}

static unsigned calculateTreewidthRecursive(const Graph *g, bool adjacent[g->size][g->size], bool eliminated[g->size], unsigned remaining) {
  if (remaining == 0) return 0;
  unsigned minimum = UINT_MAX;
  bool backup[g->size][g->size];
  for (unsigned u = 0; u < g->size; u++)
    for (unsigned v = 0; v < g->size; v++)
      backup[u][v] = adjacent[u][v];
  for (unsigned u = 0; u < g->size; u++) {
    if (eliminated[u]) continue;
    unsigned degree = 0;
    for (unsigned v = 0; v < g->size; v++)
      if (u != v && !eliminated[v] && adjacent[u][v])
        degree++;
    if (degree >= minimum) continue;
    for (unsigned v = 0; v < g->size; v++)
      if (u != v && !eliminated[v] && adjacent[u][v])
        for (unsigned w = 0; w < g->size; w++)
          if (u != w && v != w && !eliminated[w] && adjacent[u][w]) {
            adjacent[v][w] = true;
            adjacent[w][v] = true;
          }
    eliminated[u] = true;
    unsigned maximum = calculateTreewidthRecursive(g, adjacent, eliminated, remaining - 1);
    eliminated[u] = false;
    if (degree > maximum) maximum = degree;
    if (maximum < minimum) minimum = maximum;
    for (unsigned v = 0; v < g->size; v++)
      for (unsigned w = 0; w < g->size; w++)
        adjacent[v][w] = backup[v][w];
  }
  return minimum;
}

unsigned calculateTreewidth(const Graph *g) {
  if (!g || !g->edges || g->size <= 1) return 0;
  bool adjacent[g->size][g->size] = {};
  bool eliminated[g->size] = {};
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size) {
        adjacent[v][e->destination] = true;
        adjacent[e->destination][v] = true;
      }
  return calculateTreewidthRecursive(g, adjacent, eliminated, g->size);
}

static void calculateDirectedEdgeConnectivityRecursive(
  const Graph *g, Graph *copy, unsigned v, const Edge *e, unsigned current, unsigned *minimum)
{
  if (current >= *minimum) return;
  if (countStronglyConnectedComponents(copy) >= 2) {
    *minimum = current;
    return;
  }
  if (!e) {
    if (v + 1 < g->size) calculateDirectedEdgeConnectivityRecursive(g, copy, v + 1, g->edges[v + 1], current, minimum);
    return;
  }
  calculateDirectedEdgeConnectivityRecursive(g, copy, v, e->next, current, minimum);
  deleteFirstDirectedEdge(copy, v, e->destination);
  calculateDirectedEdgeConnectivityRecursive(g, copy, v, e->next, current + 1, minimum);
  addDirectedEdge(copy, v, e->destination);
}

unsigned calculateDirectedEdgeConnectivity(const Graph *g) {
  if (!g || g->size <= 1) return 0;
  Graph *copy = copyGraph(g);
  if (!copy) return UINT_MAX;
  unsigned minimum = UINT_MAX;
  calculateDirectedEdgeConnectivityRecursive(g, copy, 0, g->edges[0], 0, &minimum);
  destroyGraph(copy);
  return minimum;
}

unsigned calculateDegeneracy(const Graph *g) {
  if (!g || g->size == 0) return 0;
  unsigned *core = calculateCoreNumbers(g);
  if (!core) return 0;
  unsigned degeneracy = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (core[v] > degeneracy)
      degeneracy = core[v];
  free(core);
  return degeneracy;
}

unsigned calculateVertexConnectivity(const Graph *g) {
  if (!g || g->size <= 1) return 0;
  unsigned minimum = g->size - 1;
  for (unsigned u = 0; u < g->size; u++)
    for (unsigned v = 0; v < g->size; v++) {
      if (u == v) continue;
      unsigned local = calculateLocalVertexConnectivity(g, u, v);
      if (local < minimum) minimum = local;
    }
  return minimum;
}

unsigned calculateEdgeConnectivity(const Graph *g) {
  if (!g || g->size <= 1) return 0;
  unsigned minimum = UINT_MAX;
  for (unsigned v = 1; v < g->size; v++) {
    unsigned flow = calculateLocalEdgeConnectivity(g, 0, v);
    if (flow < minimum) minimum = flow;
    flow = calculateLocalEdgeConnectivity(g, v, 0);
    if (flow < minimum) minimum = flow;
    if (minimum == 0) return 0;
  }
  return minimum;
}

unsigned countSelfLoopsAtVertex(const Graph *g, unsigned v) {
  return countMatchingEdges(g, v, v);
}

unsigned getOutDegree(const Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return 0;
  unsigned n = 0;
  for (Edge *e = g->edges[v]; e; e = e->next) n++;
  return n;
}

unsigned getInDegree(const Graph *g, unsigned v) {
  if (!g) return 0;
  unsigned n = 0;
  for (unsigned u = 0; u < g->size; u++) n += countMatchingEdges(g, u, v);
  return n;
}

unsigned getDegree(const Graph *g, unsigned v) {
  return getInDegree(g, v) + getOutDegree(g, v) - countSelfLoopsAtVertex(g, v);
}

unsigned calculateUnweightedEccentricity(const Graph *g, unsigned v) {
  if (!g || v >= g->size) return 0;
  unsigned *distance = calculateUnweightedDistances(g, v);
  if (!distance) return 0;
  unsigned eccentricity = 0;
  for (unsigned u = 0; u < g->size; u++)
    if (distance[u] > eccentricity)
      eccentricity = distance[u];
  free(distance);
  return eccentricity;
}

unsigned getNeighbor(const Graph *g, unsigned v, unsigned i) {
  if (!g || !g->edges || v >= g->size) return UINT_MAX;
  unsigned j = 0;
  for (Edge *e = g->edges[v]; e; e = e->next) {
    if (j == i) return e->destination;
    j++;
  }
  return UINT_MAX;
}

unsigned getNeighborhoodSize(const Graph *g, unsigned v, unsigned k) {
  if (!g || !g->edges || v >= g->size) return 0;
  unsigned *distance = malloc(g->size * sizeof(unsigned));
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  if (!distance || !queue) {
    free(distance); free(queue);
    return 0;
  }
  for (unsigned i = 0; i < g->size; i++) distance[i] = UINT_MAX;
  distance[v] = 0;
  unsigned front = 0, rear = 0;
  queue[rear++] = v;
  unsigned n = 1;
  while (front < rear) {
    v = queue[front++];
    if (distance[v] >= k) break;
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size && distance[e->destination] == UINT_MAX) {
        distance[e->destination] = distance[v] + 1;
        queue[rear++] = e->destination;
        n++;
      }
  }
  free(distance); free(queue);
  return n;
}

unsigned countCommonNeighbors(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size) return 0;
  bool *neighbors = calloc(g->size, sizeof(bool));
  if (!neighbors) return 0;
  for (Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination < g->size)
      neighbors[e->destination] = true;
  unsigned n = 0;
  for (Edge *e = g->edges[v]; e; e = e->next)
    if (e->destination < g->size && neighbors[e->destination]) {
      n++;
      neighbors[e->destination] = false;
    }
  free(neighbors);
  return n;
}

unsigned countShortestPaths(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size) return 0;
  double *distance = malloc(g->size * sizeof(*distance));
  unsigned *paths = calloc(g->size, sizeof(*paths));
  bool *visited = calloc(g->size, sizeof(*visited));
  if (!distance || !paths || !visited) {
    free(distance);
    free(paths);
    free(visited);
    return 0;
  }
  for (unsigned x = 0; x < g->size; x++) distance[x] = INFINITY;
  distance[u] = 0;
  paths[u] = 1;
  while (true) {
    unsigned x = UINT_MAX;
    double minimum = INFINITY;
    for (unsigned y = 0; y < g->size; y++)
      if (!visited[y] && distance[y] < minimum) {
        minimum = distance[y];
        x = y;
      }
    if (x == UINT_MAX || x == v) break;
    visited[x] = true;
    for (Edge *e = g->edges[x]; e; e = e->next)
      if (e->destination < g->size && !visited[e->destination]) {
        if (distance[x] + e->weight < distance[e->destination]) {
          distance[e->destination] = distance[x] + e->weight;
          paths[e->destination] = paths[x];
        } else if (distance[x] + e->weight == distance[e->destination]) {
          if (paths[e->destination] > UINT_MAX - paths[x])
            paths[e->destination] = UINT_MAX;
          else
            paths[e->destination] += paths[x];
        }
      }
  }
  unsigned n = paths[v];
  free(distance);
  free(paths);
  free(visited);
  return n;
}

unsigned calculateUnweightedDistance(const Graph *g, unsigned u, unsigned v) {
  if (!g || u >= g->size || v >= g->size) return UINT_MAX;
  unsigned *distances = calculateUnweightedDistances(g, u);
  if (!distances) return UINT_MAX;
  unsigned distance = distances[v];
  free(distances);
  return distance;
}

unsigned countMatchingEdges(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size) return 0;
  unsigned n = 0;
  for (const Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination == v)
      n++;
  return n;
}

unsigned calculateLocalVertexConnectivity(const Graph *g, unsigned u, unsigned v) {
  if (!g || u >= g->size || v >= g->size || u == v) return 0;
  if (hasDirectedEdge(g, u, v)) return g->size - 1;
  bool *cut = getLocalVertexCut(g, u, v);
  if (!cut) return 0;
  unsigned connectivity = 0;
  for (unsigned i = 0; i < g->size; i++)
    if (cut[i])
      connectivity++;
  free(cut);
  return connectivity;
}

static bool calculateLocalEdgeConnectivityHelper(unsigned u, unsigned v, unsigned n, unsigned residual[n][n], bool visited[n]) {
  visited[u] = true;
  if (u == v) return true;
  for (unsigned w = 0; w < n; w++)
    if (!visited[w] && residual[u][w] > 0 && calculateLocalEdgeConnectivityHelper(w, v, n, residual, visited)) {
      residual[u][w]--;
      residual[w][u]++;
      return true;
    }
  return false;
}

unsigned calculateLocalEdgeConnectivity(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size || u == v) return 0;
  unsigned residual[g->size][g->size] = {};
  for (unsigned w = 0; w < g->size; w++)
    for (const Edge *e = g->edges[w]; e; e = e->next)
      if (e->destination < g->size)
        residual[w][e->destination]++;
  unsigned flow = 0;
  while (true) {
    bool visited[g->size] = {};
    if (!calculateLocalEdgeConnectivityHelper(u, v, g->size, residual, visited)) break;
    flow++;
  }
  return flow;
}

static void canReachTargetHelper(const Graph *g, unsigned v, bool *reach) {
  reach[v] = true;
  for (unsigned u = 0; u < g->size; u++)
    for (const Edge *e = g->edges[u]; e; e = e->next)
      if (e->destination == v && !reach[u])
        canReachTargetHelper(g, u, reach);
}

static bool hasInfiniteWalkHelper(const Graph *g, unsigned v, const bool *reach, bool *visiting) {
  if (v >= g->size) return false;
  if (visiting[v]) return reach[v];
  visiting[v] = true;
  for (const Edge *e = g->edges[v]; e; e = e->next)
    if (hasInfiniteWalkHelper(g, e->destination, reach, visiting))
      return true;
  visiting[v] = false;
  return false;
}

static unsigned countAcyclicWalksHelper(const Graph *g, unsigned u, unsigned v, const bool *reach) {
  if (u >= g->size || !reach[u]) return 0;
  if (u == v) return 1;
  unsigned total = 0;
  for (const Edge *e = g->edges[u]; e; e = e->next) {
    unsigned result = countAcyclicWalksHelper(g, e->destination, v, reach);
    if (total > UINT_MAX - result)
      return UINT_MAX;
    else
      total += result;
  }
  return total;
}

unsigned countWalks(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size) return 0;
  bool reach[g->size] = {};
  bool visiting[g->size] = {};
  canReachTargetHelper(g, v, reach);
  if (hasInfiniteWalkHelper(g, u, reach, visiting)) return UINT_MAX;
  return countAcyclicWalksHelper(g, u, v, reach);
}

static unsigned countPathsDfs(const Graph *g, unsigned u, unsigned v, bool visited[g->size]) {
  if (u == v) return 1;
  if (visited[u]) return 0;
  visited[u] = true;
  unsigned count = 0;
  for (const Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination < g->size)
      count += countPathsDfs(g, e->destination, v, visited);
  visited[u] = false;
  return count;
}

unsigned countPaths(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size) return 0;
  bool visited[g->size] = {};
  return countPathsDfs(g, u, v, visited);
}

static unsigned countDirectedTrailsDfs(const Graph *g, unsigned u, unsigned v, unsigned capacity[g->size][g->size]) {
  unsigned trails = u == v ? 1 : 0;
  for (Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination < g->size && capacity[u][e->destination] > 0) {
      capacity[u][e->destination]--;
      trails += countDirectedTrailsDfs(g, e->destination, v, capacity);
      capacity[u][e->destination]++;
    }
  return trails;
}

unsigned countDirectedTrails(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size) return 0;
  unsigned capacity[g->size][g->size] = {};
  for (unsigned w = 0; w < g->size; w++)
    for (Edge *e = g->edges[w]; e; e = e->next)
      if (e->destination < g->size)
        capacity[w][e->destination]++;
  return countDirectedTrailsDfs(g, u, v, capacity);
}

static unsigned countUndirectedTrailsDfs(const Graph *g, unsigned u, unsigned v, unsigned available[g->size][g->size]) {
  unsigned trails = u == v;
  for (const Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination < g->size && available[u][e->destination] > 0 && available[e->destination][u] > 0) {
      available[u][e->destination]--;
      if (u != e->destination) available[e->destination][u]--; 
      trails += countUndirectedTrailsDfs(g, e->destination, v, available);
      available[u][e->destination]++;
      if (u != e->destination) available[e->destination][u]++;
    }
  return trails;
}

unsigned countUndirectedTrails(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size || v >= g->size) return 0;
  unsigned available[g->size][g->size] = {};
  for (unsigned w = 0; w < g->size; w++)
    for (const Edge *e = g->edges[w]; e; e = e->next)
      if (e->destination < g->size)
        available[w][e->destination]++;
  return countUndirectedTrailsDfs(g, u, v, available);
}

unsigned countMatchingWeightedEdges(const Graph *g, unsigned u, unsigned v, double weight) {
  if (!g || !g->edges || u >= g->size) return 0;
  unsigned n = 0;
  for (const Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination == v && e->weight == weight)
      n++;
  return n;
}

unsigned calculateBandwidth(const Graph *g, const unsigned *ordering) {
  if (!g || !ordering) return 0;
  unsigned *position = malloc(g->size * sizeof(unsigned));
  if (!position) return 0;
  for (unsigned i = 0; i < g->size; i++)
    if (ordering[i] < g->size)
      position[ordering[i]] = i;
  unsigned bandwidth = 0;
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size) {
        unsigned i = position[v];
        unsigned j = position[e->destination];
        unsigned distance = i >= j ? i - j : j - i;
        if (distance > bandwidth) bandwidth = distance;
      }
  free(position);
  return bandwidth;
}



[[nodiscard]] unsigned *getInDegrees(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  unsigned *degrees = calloc(g->size, sizeof(unsigned));
  if (!degrees) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size)
        degrees[e->destination]++;
  return degrees;
}

[[nodiscard]] unsigned *getOutDegrees(const Graph *g) {
  if (!g) return nullptr;
  unsigned *degrees = calloc(g->size, sizeof(unsigned));
  if (!degrees) return nullptr;
  for (unsigned v = 0; v < g->size; v++) degrees[v] = getOutDegree(g, v);
  return degrees;
}

[[nodiscard]] unsigned *getDegrees(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  unsigned *degrees = calloc(g->size, sizeof(unsigned));
  if (!degrees) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size) {
        degrees[v]++;
        degrees[e->destination]++;
      }
  return degrees;
}

[[nodiscard]] unsigned *getInDegreeDistribution(const Graph *g) {
  if (!g) return nullptr;
  unsigned *degree = getInDegrees(g);
  unsigned *distribution = calloc(g->size, sizeof(unsigned));
  if (!degree || !distribution) {
    free(degree);
    free(distribution);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++)
    if (degree[v] < g->size)
      distribution[degree[v]]++;
  free(degree);
  return distribution;
}

[[nodiscard]] unsigned *getOutDegreeDistribution(const Graph *g) {
  if (!g) return nullptr;
  unsigned *distribution = calloc(g->size, sizeof(unsigned));
  if (!distribution) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    if (getOutDegree(g, v) < g->size)
      distribution[getOutDegree(g, v)]++;
  return distribution;
}

[[nodiscard]] unsigned *findGreedyColoring(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  unsigned *colors = malloc(g->size * sizeof(unsigned));
  if (!colors) return nullptr;
  for (unsigned v = 0; v < g->size; v++) colors[v] = UINT_MAX;
  bool available[g->size];
  for (unsigned u = 0; u < g->size; u++) {
    for (unsigned c = 0; c < g->size; c++) available[c] = true;
    for (Edge *e = g->edges[u]; e; e = e->next)
      if (e->destination < g->size && colors[e->destination] != UINT_MAX)
        available[colors[e->destination]] = false;
    for (unsigned v = 0; v < g->size; v++)
      for (Edge *e = g->edges[v]; e; e = e->next)
        if (e->destination == u && colors[v] != UINT_MAX)
          available[colors[v]] = false;
    unsigned c = 0;
    while (!available[c]) c++;
    colors[u] = c;
  }
  return colors;
}

static bool canBeColored(const Graph *g, unsigned v, unsigned maximum, unsigned *colors) {
  if (v == g->size) return true;
  for (unsigned c = 0; c < maximum; c++) {
    bool colorable = true;
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size && colors[e->destination] == c)
        colorable = false;
    for (unsigned u = 0; u < g->size; u++)
      for (const Edge *e = g->edges[u]; e; e = e->next)
        if (e->destination == v && colors[u] == c)
          colorable = false;
    if (colorable) {
      colors[v] = c;
      if (canBeColored(g, v + 1, maximum, colors)) return true;
      colors[v] = UINT_MAX;
    }
  }
  return false;
}

[[nodiscard]] unsigned *findOptimalColoring(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  unsigned *colors = malloc(g->size * sizeof(unsigned));
  if (!colors) return nullptr;
  for (unsigned v = 0; v < g->size; v++) colors[v] = UINT_MAX;
  for (unsigned maximum = 0; maximum <= g->size; maximum++)
    if (canBeColored(g, 0, maximum, colors))
      return colors;
  free(colors);
  return nullptr;
}

[[nodiscard]] unsigned *getStronglyConnectedComponents(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  bool *visited = malloc(g->size * sizeof(bool));
  Graph *transpose = createTranspose(g);
  unsigned *components = malloc(g->size * sizeof(unsigned));
  if (!queue || !visited || !transpose || !components) {
    free(queue);
    free(visited);
    destroyGraph(transpose);
    free(components);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++) components[v] = UINT_MAX;
  for (unsigned u = 0; u < g->size; u++) {
    if (components[u] < UINT_MAX) continue;
    for (unsigned v = 0; v < g->size; v++) visited[v] = false;
    unsigned head = 0, tail = 0;
    visited[u] = true;
    queue[tail++] = u;
    while (head < tail)
      for (Edge *e = g->edges[queue[head++]]; e; e = e->next)
        if (e->destination < g->size && !visited[e->destination]) {
          visited[e->destination] = true;
          queue[tail++] = e->destination;
        }
    components[u] = u;
    head = tail = 0;
    queue[tail++] = u;
    while (head < tail)
      for (Edge *e = transpose->edges[queue[head++]]; e; e = e->next)
        if (e->destination < transpose->size && components[e->destination] == UINT_MAX && visited[e->destination]) {
          components[e->destination] = u;
          queue[tail++] = e->destination;
        }
  }
  free(queue);
  free(visited);
  destroyGraph(transpose);
  return components;
}

static void getTopologicalSortDfs(const Graph *g, unsigned v, unsigned *ordering, unsigned *i, bool *visited) {
  visited[v] = true;
  for (Edge *e = g->edges[v]; e; e = e->next)
    if (!visited[e->destination])
      getTopologicalSortDfs(g, e->destination, ordering, i, visited);
  ordering[--(*i)] = v;
}

[[nodiscard]] unsigned *getTopologicalSort(const Graph *g) {
  if (!isValid(g) || hasDirectedCycle(g)) return nullptr;
  unsigned *ordering = malloc(g->size * sizeof(unsigned));
  bool *visited = calloc(g->size, sizeof(bool));
  if (!ordering || !visited) {
    free(ordering);
    free(visited);
    return nullptr;
  }
  unsigned i = g->size;
  for (unsigned v = 0; v < g->size; v++)
    if (!visited[v])
      getTopologicalSortDfs(g, v, ordering, &i, visited);
  free(visited);
  return ordering;
}

static bool searchForMaximumBipartiteMatching(const Graph *g, unsigned u, bool *visited, unsigned *match) {
  for (Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination < g->size && !visited[e->destination]) {
      visited[e->destination] = true;
      if (match[e->destination] == UINT_MAX || searchForMaximumBipartiteMatching(g, match[e->destination], visited, match)) {
        match[e->destination] = u;
        return true;
      }
    }
  return false;
}

[[nodiscard]] unsigned *findMaximumBipartiteMatching(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  bool *visited = malloc(g->size * sizeof(bool));
  unsigned *match = malloc(g->size * sizeof(unsigned));
  if (!visited || !match) {
    free(visited);
    free(match);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++) match[v] = UINT_MAX;
  for (unsigned u = 0; u < g->size; u++) {
    for (unsigned v = 0; v < g->size; v++) visited[v] = false;
    searchForMaximumBipartiteMatching(g, u, visited, match);
  }
  free(visited);
  return match;
}

static void searchMaximumUnweightedMatching(const Graph *g, unsigned u, Edge *e, unsigned *c, unsigned cc, unsigned *b, unsigned *bc) {
  if (!e) {
    if (u + 1 >= g->size) {
      if (cc > *bc) {
        *bc = cc;
        for (unsigned v = 0; v < g->size; v++) b[v] = c[v];
      }
      return;
    }
    searchMaximumUnweightedMatching(g, u + 1, g->edges[u + 1], c, cc, b, bc);
    return;
  }
  searchMaximumUnweightedMatching(g, u, e->next, c, cc, b, bc);
  unsigned v = e->destination;
  if (c[u] == UINT_MAX && c[v] == UINT_MAX && u != v) {
    c[u] = v;
    c[v] = u;
    searchMaximumUnweightedMatching(g, u, e->next, c, cc + 1, b, bc);
    c[u] = UINT_MAX;
    c[v] = UINT_MAX;
  }
}

[[nodiscard]] unsigned *findMaximumUnweightedMatching(const Graph *g) {
  if (!isValid(g) || g->size == 0) return nullptr;
  unsigned *best = malloc(g->size * sizeof(unsigned));
  if (!best) return nullptr;
  unsigned current[g->size];
  for (unsigned i = 0; i < g->size; i++) best[i] = current[i] = UINT_MAX;
  unsigned bestCount = 0;
  searchMaximumUnweightedMatching(g, 0, g->edges[0], current, 0, best, &bestCount);
  return best;
}

static void searchMaximumWeightedMatching(const Graph *g, unsigned u, Edge *e, unsigned *c, double cw, unsigned *b, double *bw) {
  if (!e) {
    if (u + 1 >= g->size) {
      if (cw > *bw) {
        *bw = cw;
        for (unsigned i = 0; i < g->size; i++) b[i] = c[i];
      }
      return;
    }
    searchMaximumWeightedMatching(g, u + 1, g->edges[u + 1], c, cw, b, bw);
    return;
  }
  searchMaximumWeightedMatching(g, u, e->next, c, cw, b, bw);
  unsigned v = e->destination;
  if (c[u] == UINT_MAX && c[v] == UINT_MAX && u != v) {
    c[u] = v;
    c[v] = u;
    searchMaximumWeightedMatching(g, u, e->next, c, cw + e->weight, b, bw);
    c[u] = UINT_MAX;
    c[v] = UINT_MAX;
  }
}

[[nodiscard]] unsigned *findMaximumWeightedMatching(const Graph *g) {
  if (!isValid(g) || g->size == 0) return nullptr;
  unsigned *best = malloc(g->size * sizeof(unsigned));
  if (!best) return nullptr;
  unsigned current[g->size];
  for (unsigned i = 0; i < g->size; i++) best[i] = current[i] = UINT_MAX;
  double bestWeight = -INFINITY;
  searchMaximumWeightedMatching(g, 0, g->edges[0], current, 0, best, &bestWeight);
  return best;
}

[[nodiscard]] unsigned *findMinimalEdgeCover(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  unsigned *cover = malloc(g->size * sizeof(unsigned));
  if (!cover) return nullptr;
  for (unsigned v = 0; v < g->size; v++)
    cover[v] = UINT_MAX;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (cover[v] == UINT_MAX && e->destination < g->size && cover[e->destination] == UINT_MAX && v != e->destination) {
        cover[v] = e->destination;
        cover[e->destination] = v;
      }
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (cover[v] == UINT_MAX || (e->destination < g->size && cover[e->destination] == UINT_MAX)) {
        cover[v] = e->destination;
        cover[e->destination] = v;
      }
  return cover;
}

static void searchForMinimumEdgeCover(
  const Graph *g, unsigned v, const Edge *e, unsigned *current, unsigned currentCount, unsigned *minimum, unsigned *minimumCount)
{
  if (currentCount >= *minimumCount) return;
  if (!e) {
    if (v + 1 < g->size) {
      searchForMinimumEdgeCover(g, v + 1, g->edges[v + 1], current, currentCount, minimum, minimumCount);
      return;
    }
    bool covered = true;
    for (unsigned v = 0; v < g->size; v++)
      if (current[v] == UINT_MAX)
        covered = false;
    if (covered && currentCount < *minimumCount) {
      for (unsigned v = 0; v < g->size; v++) minimum[v] = current[v];
      *minimumCount = currentCount;
    }
    return;
  }
  searchForMinimumEdgeCover(g, v, e->next, current, currentCount, minimum, minimumCount);
  if (e->destination < g->size) {
    unsigned source = current[v];
    unsigned target = current[e->destination];
    current[v] = e->destination;
    current[e->destination] = v;
    searchForMinimumEdgeCover(g, v, e->next, current, currentCount + 1, minimum, minimumCount);
    current[v] = source;
    current[e->destination] = target;
  }
}

[[nodiscard]] unsigned *findMinimumEdgeCover(const Graph *g) {
  if (!g || !g->edges || g->size == 0) return nullptr;
  unsigned *current = malloc(g->size * sizeof(unsigned));
  unsigned *minimum = malloc(g->size * sizeof(unsigned));
  if (!minimum || !current) {
    free(current);
    free(minimum);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++) current[v] = minimum[v] = UINT_MAX;
  unsigned minimumCount = UINT_MAX;
  searchForMinimumEdgeCover(g, 0, g->edges[0], current, 0, minimum, &minimumCount);
  free(current);
  return minimum;
}

[[nodiscard]] unsigned *calculateCoreNumbers(const Graph *g) {
  if (!g || g->size == 0) return nullptr;
  bool *processed = calloc(g->size, sizeof(bool));
  unsigned *degrees = calloc(g->size, sizeof(unsigned));
  unsigned *core = malloc(g->size * sizeof(unsigned));
  if (!processed || !degrees || !core) {
    free(processed);
    free(degrees);
    free(core);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination != v && e->destination < g->size)
        degrees[v]++;
  while (true) {
    unsigned degree = UINT_MAX;
    unsigned v = UINT_MAX;
    for (unsigned u = 0; u < g->size; u++)
      if (!processed[u] && degrees[u] < degree) {
        degree = degrees[u];
        v = u;
      }
    if (v == UINT_MAX) break;
    core[v] = degree;
    processed[v] = true;
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size && !processed[e->destination] && degrees[e->destination] > degree)
        degrees[e->destination]--;
  }
  free(processed);
  free(degrees);
  return core;
}

[[nodiscard]] unsigned *findCommunities(const Graph *g) {
  if (!g || g->size == 0) return nullptr;
  unsigned *partition = malloc(g->size * sizeof(unsigned));
  if (!partition) return nullptr;
  for (unsigned v = 0; v < g->size; v++) partition[v] = v;
  double current_modularity = calculateModularity(g, partition);
  bool active = true;
  while (active) {
    active = false;
    for (unsigned u = 0; u < g->size; u++) {
      unsigned original_community = partition[u];
      unsigned best_community = original_community;
      double maximum_modularity = current_modularity;
      for (const Edge *e = g->edges[u]; e; e = e->next) {
        if (e->destination >= g->size || partition[e->destination] == best_community) continue;
        partition[u] = partition[e->destination];
        double next_modularity = calculateModularity(g, partition);
        if (next_modularity > maximum_modularity) {
          maximum_modularity = next_modularity;
          best_community = partition[u];
        }
      }
      partition[u] = best_community;
      if (best_community != original_community) {
        current_modularity = maximum_modularity;
        active = true;
      }
    }
  }
  return partition;
}

[[nodiscard]] unsigned *findKTrusses(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  unsigned *truss = calloc(g->size, sizeof(unsigned));
  if (!truss) return nullptr;
  bool adjacent[g->size][g->size] = {};
  for (unsigned u = 0; u < g->size; u++)
    for (Edge *e = g->edges[u]; e; e = e->next)
      if (u != e->destination && e->destination < g->size) {
        adjacent[u][e->destination] = true;
        adjacent[e->destination][u] = true;
      }
  for (unsigned k = 2; k <= g->size; k++) {
    bool changed = true;
    while (changed) {
      changed = false;
      for (unsigned u = 0; u < g->size; u++)
        for (unsigned v = 0; v < g->size; v++)
          if (adjacent[u][v]) {
            unsigned triangles = 0;
            for (unsigned w = 0; w < g->size; w++)
              if (adjacent[u][w] && adjacent[v][w])
                triangles++;
            if (triangles < k - 2) {
              adjacent[u][v] = false;
              adjacent[v][u] = false;
              changed = true;
            }
          }
    }
    for (unsigned u = 0; u < g->size; u++)
      for (unsigned v = 0; v < g->size; v++)
        if (adjacent[u][v])
          truss[u] = k;
  }
  return truss;
}

[[nodiscard]] unsigned *findDegeneracyOrdering(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  if (g->size == 0) return malloc(sizeof(unsigned));
  unsigned *ordering = malloc(g->size * sizeof(unsigned));
  if (!ordering) return nullptr;
  bool removed[g->size] = {};
  for (unsigned step = 0; step < g->size; step++) {
    unsigned minimum = -1;
    unsigned chosen = 0;
    for (unsigned u = 0; u < g->size; u++)
      if (!removed[u]) {
        unsigned degree = 0;
        for (unsigned v = 0; v < g->size; v++)
          if (!removed[v])
            for (Edge *e = g->edges[v]; e; e = e->next)
              if (v != e->destination && e->destination < g->size && !removed[e->destination] && (u == v || u == e->destination))
                degree++;
        if (degree < minimum) {
          minimum = degree;
          chosen = u;
        }
      }
    ordering[step] = chosen;
    removed[chosen] = true;
  }
  return ordering;
}

[[nodiscard]] unsigned *calculateUnweightedDistances(const Graph *g, unsigned v) {
  if (!isValid(g) || v >= g->size) return nullptr;
  unsigned *distances = malloc(g->size * sizeof(unsigned));
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  if (!distances || !queue) {
    free(distances);
    free(queue);
    return nullptr;
  }
  for (unsigned u = 0; u < g->size; u++) distances[u] = UINT_MAX;
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

static void getPreOrderSortDfs(const Graph *g, unsigned u, bool *visited, unsigned *order, unsigned *index) {
  visited[u] = true;
  order[(*index)++] = u;
  for (const Edge *e = g->edges[u]; e; e = e->next)
    if (!visited[e->destination])
      getPreOrderSortDfs(g, e->destination, visited, order, index);
}

[[nodiscard]] unsigned *getPreOrderSort(const Graph *g, unsigned v) {
  if (!isValid(g) || v >= g->size) return nullptr;
  unsigned *order = malloc(g->size * sizeof(unsigned));
  bool *visited = calloc(g->size, sizeof(bool));
  if (!order || !visited) {
    free(order);
    free(visited);
    return nullptr;
  }
  unsigned index = 0;
  getPreOrderSortDfs(g, v, visited, order, &index);
  for (unsigned u = 0; u < g->size; u++)
    if (!visited[u])
      getPreOrderSortDfs(g, u, visited, order, &index);
  free(visited);
  return order;
}

static void getPostOrderSortDfs(const Graph *g, unsigned u, bool *visited, unsigned *order, unsigned *index) {
  visited[u] = true;
  for (const Edge *e = g->edges[u]; e; e = e->next)
    if (!visited[e->destination])
      getPostOrderSortDfs(g, e->destination, visited, order, index);
  order[(*index)++] = u;
}

[[nodiscard]] unsigned *getPostOrderSort(const Graph *g, unsigned v) {
  if (!isValid(g) || v >= g->size) return nullptr;
  unsigned *order = malloc(g->size * sizeof(unsigned));
  bool *visited = calloc(g->size, sizeof(bool));
  if (!order || !visited) {
    free(order);
    free(visited);
    return nullptr;
  }
  unsigned index = 0;
  getPostOrderSortDfs(g, v, visited, order, &index);
  for (unsigned u = 0; u < g->size; u++)
    if (!visited[u])
      getPostOrderSortDfs(g, u, visited, order, &index);
  free(visited);
  return order;
}

[[nodiscard]] unsigned *getBreadthFirstSort(const Graph *g, unsigned v) {
  if (!isValid(g) || v >= g->size) return nullptr;
  bool *visited = calloc(g->size, sizeof(bool));
  unsigned *result = malloc(g->size * sizeof(unsigned));
  if (!visited || !result) {
    free(visited);
    free(result);
    return nullptr;
  }
  unsigned head = 0;
  unsigned tail = 0;
  for (unsigned u = 0; u < g->size; u++) {
    const unsigned start = u == 0 ? v : u <= v ? u - 1 : u;
    if (visited[start]) continue;
    visited[start] = true;
    result[tail++] = start;
    while (head < tail)
      for (const Edge *e = g->edges[result[head++]]; e; e = e->next)
        if (!visited[e->destination]) {
          visited[e->destination] = true;
          result[tail++] = e->destination;
        }
  }
  free(visited);
  return result;
}

[[nodiscard]] unsigned *getShortestPath(const Graph *g, unsigned u, unsigned v, unsigned *length) {
  if (length) *length = 0; else return nullptr;
  if (!isValid(g) || u >= g->size || v >= g->size) return nullptr;
  double *distances = malloc(g->size * sizeof(double));
  unsigned *parent = malloc(g->size * sizeof(unsigned));
  bool *visited = malloc(g->size * sizeof(bool));
  if (!distances || !parent || !visited) {
    free(distances);
    free(parent);
    free(visited);
    return nullptr;
  }
  for (unsigned x = 0; x < g->size; x++) {
    distances[x] = INFINITY;
    parent[x] = UINT_MAX;
    visited[x] = false;
  }
  distances[u] = 0;
  while (true) {
    unsigned x = UINT_MAX;
    double distance = INFINITY;
    for (unsigned y = 0; y < g->size; y++)
      if (!visited[y] && distances[y] < distance) {
        distance = distances[y];
        x = y;
      }
    if (x == UINT_MAX || x == v) break;
    visited[x] = true;
    for (const Edge *e = g->edges[x]; e; e = e->next)
      if (!visited[e->destination] && distances[x] + e->weight < distances[e->destination]) {
        distances[e->destination] = distances[x] + e->weight;
        parent[e->destination] = x;
      }
  }
  unsigned *path = nullptr;
  if (distances[v] < INFINITY) {
    unsigned x = v;
    do {
      (*length)++;
      x = parent[x];
    } while (x < UINT_MAX);
    path = malloc((*length) * sizeof(unsigned));
    if (path) {
      x = v;
      unsigned i = *length;
      do {
        path[--i] = x;
        x = parent[x];
      } while (x < UINT_MAX);
    }
  }
  free(distances);
  free(parent);
  free(visited);
  return path;
}



[[nodiscard]] unsigned **getAllPairsUnweightedDistances(const Graph *g) {
  if (!g) return nullptr;
  unsigned **distances = malloc(g->size * sizeof(unsigned *));
  if (!distances) return nullptr;
  for (unsigned v = 0; v < g->size; v++) distances[v] = calculateUnweightedDistances(g, v);
  for (unsigned u = 0; u < g->size; u++)
    if (!distances[u]) {
      for (unsigned v = 0; v < g->size; v++) free(distances[v]);
      free(distances);
      return nullptr;
    }
  return distances;
}

static void getBridgesDfs(
  const Graph *g, unsigned v, unsigned parent, unsigned *timer, unsigned *discovery, unsigned *low, unsigned **bridges, unsigned *count)
{
  discovery[v] = low[v] = ++(*timer);
  bool skipped = false;
  for (Edge *e = g->edges[v]; e; e = e->next) {
    if (e->destination == parent && !skipped) {
      skipped = true;
      continue;
    }
    if (discovery[e->destination] > 0) {
      if (discovery[e->destination] < low[v]) low[v] = discovery[e->destination];
    } else {
      getBridgesDfs(g, e->destination, v, timer, discovery, low, bridges, count);
      if (low[e->destination] < low[v]) low[v] = low[e->destination];
      if (low[e->destination] > discovery[v]) {
        unsigned *bridge = malloc(2 * sizeof(unsigned));
        if (bridge) {
          bridge[0] = v;
          bridge[1] = e->destination;
          bridges[*count] = bridge;
          (*count)++;
        }
      }
    }
  }
}

[[nodiscard]] unsigned **getBridges(const Graph *g) {
  if (!isValid(g)) return nullptr;
  unsigned *discovery = calloc(g->size, sizeof(unsigned));
  unsigned *low = calloc(g->size, sizeof(unsigned));
  unsigned **bridges = calloc(g->size + 1, sizeof(unsigned *));
  if ((g->size > 0 && (!discovery || !low)) || !bridges) {
    free(discovery);
    free(low);
    free(bridges);
    return nullptr;
  }
  unsigned timer = 0;
  unsigned count = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (discovery[v] == 0)
      getBridgesDfs(g, v, UINT_MAX, &timer, discovery, low, bridges, &count);
  free(discovery);
  free(low);
  return bridges;
}



double getMinimumWeight(const Graph *g) {
  if (!g || !g->edges) return INFINITY;
  double minimum = INFINITY;
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (e->weight < minimum)
        minimum = e->weight;
  return minimum;
}

double getMaximumWeight(const Graph *g) {
  if (!g || !g->edges) return -INFINITY;
  double maximum = -INFINITY;
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (e->weight > maximum)
        maximum = e->weight;
  return maximum;
}

double sumWeights(const Graph *g) {
  if (!g || !g->edges) return 0;
  double sum = 0;
  double low = 0;
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next) {
      double term = e->weight + low;
      double high = sum + term;
      low = term - (high - sum);
      sum = high;
    }
  return sum;
}

double calculateWeightedRadius(const Graph *g) {
  if (!g) return INFINITY;
  double radius = INFINITY;
  for (unsigned v = 0; v < g->size; v++) {
    double eccentricity = calculateWeightedEccentricity(g, v);
    if (eccentricity < radius)
      radius = eccentricity;
  }
  return radius;
}

double calculateWeightedDiameter(const Graph *g) {
  if (!g) return -INFINITY;
  double diameter = -INFINITY;
  for (unsigned v = 0; v < g->size; v++) {
    double eccentricity = calculateWeightedEccentricity(g, v);
    if (eccentricity > diameter)
      diameter = eccentricity;
  }
  return diameter;
}

double calculateDensity(const Graph *g) {
  if (!g || g->size < 2) return 0;
  return (double)countEdges(g) / g->size / (g->size - 1);
}

double calculateAverageClusteringCoefficient(const Graph *g) {
  if (!g || g->size == 0) return 0;
  double sum = 0;
  double low = 0;
  for (unsigned v = 0; v < g->size; v++) {
    double term = calculateLocalClusteringCoefficient(g, v) + low;
    double high = sum + term;
    low = term - (high - sum);
    sum = high;
  }
  return sum / g->size;
}

double calculateDirectedWeightedGirth(const Graph *g) {
  if (!g || !g->edges) return INFINITY;
  double minimum = INFINITY;
  double *distances = malloc(g->size * sizeof(double));
  bool *visited = malloc(g->size * sizeof(bool));
  if (!distances || !visited) {
    free(distances);
    free(visited);
    return INFINITY;
  }
  for (unsigned start = 0; start < g->size; start++) {
    for (unsigned v = 0; v < g->size; v++) {
      distances[v] = INFINITY;
      visited[v] = false;
    }
    distances[start] = 0;
    while (true) {
      unsigned u = UINT_MAX;
      double distance = INFINITY;
      for (unsigned v = 0; v < g->size; v++)
        if (!visited[v] && distances[v] < distance) {
          distance = distances[v];
          u = v;
        }
      if (u == UINT_MAX) break;
      visited[u] = true;
      for (const Edge *e = g->edges[u]; e; e = e->next)
        if (e->destination == start) {
          if (distances[u] + e->weight < minimum) minimum = distances[u] + e->weight;
        } else if (e->destination < g->size && distances[u] + e->weight < distances[e->destination]) {
          distances[e->destination] = distances[u] + e->weight;
        }
    }
  }
  free(distances);
  free(visited);
  return minimum;
}

double calculateUndirectedWeightedGirth(const Graph *g) {
  if (!g || !g->edges) return INFINITY;
  double *distances = malloc(g->size * sizeof(double));
  unsigned *parent = malloc(g->size * sizeof(unsigned));
  bool *visited = malloc(g->size * sizeof(bool));
  if (!distances || !parent || !visited) {
    free(distances); free(parent); free(visited);
    return INFINITY;
  }
  double minimum = INFINITY;
  for (unsigned start = 0; start < g->size; start++) {
    for (unsigned v = 0; v < g->size; v++) {
      distances[v] = INFINITY;
      parent[v] = UINT_MAX;
      visited[v] = false;
    }
    distances[start] = 0;
    while (true) {
      unsigned u = UINT_MAX;
      double distance = INFINITY;
      for (unsigned v = 0; v < g->size; v++)
        if (!visited[v] && distances[v] < distance) {
          distance = distances[v];
          u = v;
        }
      if (u == UINT_MAX) break;
      visited[u] = true;
      for (const Edge *e = g->edges[u]; e; e = e->next)
        if (e->destination < g->size && e->destination != parent[u]) {
          if (visited[e->destination]) {
            double cycle = distances[u] + distances[e->destination] + e->weight;
            if (cycle < minimum) minimum = cycle;
          } else if (distances[u] + e->weight < distances[e->destination]) {
            distances[e->destination] = distances[u] + e->weight;
            parent[e->destination] = u;
          }
        }
    }
  }
  free(distances); free(parent); free(visited);
  return minimum;
}

static double searchFordFulkersonMinCut(unsigned u, unsigned t, double flow, unsigned size, double cap[size][size], bool visited[size]) {
  if (u == t) return flow;
  visited[u] = true;
  for (unsigned v = 0; v < size; v++)
    if (!visited[v] && cap[u][v] > 1e-9) {
      double currentCap = flow < cap[u][v] ? flow : cap[u][v];
      double pushed = searchFordFulkersonMinCut(v, t, currentCap, size, cap, visited);
      if (pushed > 0) {
        cap[u][v] -= pushed;
        cap[v][u] += pushed;
        return pushed;
      }
    }
  return 0;
}

double calculateFordFulkersonMinCut(const Graph *g) {
  if (!isValid(g) || g->size < 2) return 0;

  double minCut = INFINITY;

  for (unsigned t = 1; t < g->size; t++) {
    double cap[g->size][g->size] = {};

    for (unsigned u = 0; u < g->size; u++)
      for (Edge *e = g->edges[u]; e; e = e->next)
        cap[u][e->destination] += e->weight;

    double totalFlow = 0, pushed;

    do {
      bool visited[g->size] = {};
      pushed = searchFordFulkersonMinCut(0, t, INFINITY, g->size, cap, visited);
      totalFlow += pushed;
    } while (pushed > 0);

    if (totalFlow < minCut) minCut = totalFlow;
  }

  return minCut;
}

double calculateStoerWagnerMinCut(const Graph *g) {
  if (!isValid(g) || g->size < 2) return 0;

  unsigned n = g->size;
  double w[n][n] = {};
  for (unsigned u = 0; u < n; u++)
    for (Edge *e = g->edges[u]; e; e = e->next)
      w[u][e->destination] += e->weight;

  double minCut = INFINITY;

  while (n > 1) {
    double distance[n] = {};
    bool visited[n] = {};
    unsigned previous = 0, current = 0;

    for (unsigned step = 0; step < n; step++) {
      unsigned bestV = UINT_MAX;

      for (unsigned v = 0; v < n; v++)
        if (!visited[v] && (bestV == UINT_MAX || distance[v] > distance[bestV]))
          bestV = v;

      visited[bestV] = true;
      previous = current;
      current = bestV;

      for (unsigned v = 0; v < n; v++)
        if (!visited[v])
          distance[v] += w[current][v];
    }

    if (distance[current] < minCut) minCut = distance[current];

    for (unsigned v = 0; v < n; v++)
      if (v != previous && v != current) {
        w[previous][v] += w[current][v];
        w[v][previous] += w[v][current];
      }

    unsigned last = n - 1;

    if (current != last) {
      for (unsigned v = 0; v < n; v++) {
        w[current][v] = w[last][v];
        w[v][current] = w[v][last];
      }

      if (previous == last) previous = current;
    }

    n--;
  }

  return minCut;
}

static double searchBruteForceMinCut(const Graph *g, bool side[g->size], unsigned v) {
  if (v == g->size) {
    bool hasSideA = false;
    bool hasSideB = false;

    for (unsigned u = 0; u < g->size; u++)
      if (side[u])
        hasSideA = true;
      else
        hasSideB = true;

    if (!hasSideA || !hasSideB) return INFINITY;

    double cut = 0;

    for (unsigned u = 0; u < g->size; u++)
      for (Edge *e = g->edges[u]; e; e = e->next)
        if (side[u] && !side[e->destination])
          cut += e->weight;

    return cut;
  }

  side[v] = true;
  double cutA = searchBruteForceMinCut(g, side, v + 1);

  side[v] = false;
  double cutB = searchBruteForceMinCut(g, side, v + 1);

  return cutA < cutB ? cutA : cutB;
}

double calculateBruteForceMinCut(const Graph *g) {
  if (!isValid(g) || g->size < 2) return 0;
  bool side[g->size] = {};
  return searchBruteForceMinCut(g, side, 0);
}

double calculateMinCut(const Graph *g) {
  return calculateFordFulkersonMinCut(g);
}

double calculateWeightedEccentricity(const Graph *g, unsigned v) {
  if (!g || v >= g->size) return -INFINITY;
  double *distance = calculateWeightedDistances(g, v);
  if (!distance) return -INFINITY;
  double eccentricity = -INFINITY;
  for (unsigned u = 0; u < g->size; u++)
    if (distance[u] > eccentricity)
      eccentricity = distance[u];
  free(distance);
  return eccentricity;
}

double getNormalizedInDegree(const Graph *g, unsigned v) {
  if (!g || g->size < 2) return 0;
  return (double)getInDegree(g, v) / (g->size - 1);
}

double getNormalizedOutDegree(const Graph *g, unsigned v) {
  if (!g || g->size < 2) return 0;
  return (double)getOutDegree(g, v) / (g->size - 1);
}

double calculateLocalClusteringCoefficient(const Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size || getOutDegree(g, v) < 2) return 0;
  unsigned edges = 0;
  for (const Edge *e1 = g->edges[v]; e1; e1 = e1->next)
    for (const Edge *e2 = g->edges[v]; e2; e2 = e2->next)
      if (hasDirectedEdge(g, e1->destination, e2->destination))
        edges++;
  return (double)edges / getOutDegree(g, v) / (getOutDegree(g, v) - 1);
}

double getOutWeight(const Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return 0;
  double total = 0;
  for (const Edge *e = g->edges[v]; e; e = e->next)
    total += e->weight;
  return total;
}

double getEdgeWeight(const Graph *g, unsigned u, unsigned v) {
  if (!g || !g->edges || u >= g->size) return INFINITY;
  for (const Edge *e = g->edges[u]; e; e = e->next)
    if (e->destination == v)
      return e->weight;
  return INFINITY;
}

double calculateWeightedDistance(const Graph *g, unsigned u, unsigned v) {
  if (!g || v >= g->size) return INFINITY;
  double *distances = calculateWeightedDistances(g, u);
  if (!distances) return INFINITY;
  double distance = distances[v];
  free(distances);
  return distance;
}

double calculateMaxFlowEdmondsKarp(const Graph *g, unsigned source, unsigned sink) {
  if (!isValid(g) || source >= g->size || sink >= g->size) return 0;
  Matrix *residual = createZeroMatrix(g->size, g->size);
  unsigned *parent = malloc(g->size * sizeof(unsigned));
  bool *visited = malloc(g->size * sizeof(bool));
  unsigned *queue = malloc(g->size * sizeof(unsigned));
  if (!residual || !parent || !visited || !queue) {
    destroyMatrix(residual);
    free(parent); free(visited); free(queue);
    return 0;
  }
  for (unsigned u = 0; u < g->size; u++)
    for (unsigned v = 0; v < g->size; v++)
      residual->data[u][v] = 0;
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      residual->data[v][e->destination] += e->weight;
  double max = 0;
  while (true) {
    for (unsigned v = 0; v < g->size; v++) visited[v] = false;
    visited[source] = true;
    unsigned head = 0, tail = 0;
    queue[tail++] = source;
    bool found = false;
    parent[source] = source;
    while (head < tail && !found) {
      unsigned u = queue[head++];
      for (unsigned v = 0; v < g->size && !found; v++)
        if (!visited[v] && residual->data[u][v] > 1e-9) {
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
      if (residual->data[parent[v]][v] < flow) flow = residual->data[parent[v]][v];
      v = parent[v];
    }
    v = sink;
    while (v != source) {
      residual->data[parent[v]][v] -= flow;
      residual->data[v][parent[v]] += flow;
      v = parent[v];
    }
    max += flow;
  }
  destroyMatrix(residual);
  free(parent); free(visited); free(queue);
  return max;
}

double calculateSubgraphDensity(const Graph *g, const bool *set) {
  if (!g || !g->edges || !set) return 0;
  unsigned vertices = 0;
  unsigned edges = 0;
  for (unsigned v = 0; v < g->size; v++)
    if (set[v]) {
      vertices++;
      for (const Edge *e = g->edges[v]; e; e = e->next)
        if (e->destination < g->size && set[e->destination])
          edges++;
    }
  if (vertices < 2) return 0;
  return (double)edges / vertices / (vertices - 1);
}

double calculateModularity(const Graph *g, const unsigned *partition) {
  if (!g || !partition || g->size == 0) return 0;
  double *in = calloc(g->size, sizeof(double));
  double *out = calloc(g->size, sizeof(double));
  if (!in || !out) {
    free(in);
    free(out);
    return 0;
  }
  double total = 0;
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size) {
        out[v] += e->weight;
        in[e->destination] += e->weight;
        total += e->weight;
      }
  if (total <= DBL_EPSILON) {
    free(in);
    free(out);
    return 0;
  }
  double actual = 0;
  double expected = 0;
  for (unsigned u = 0; u < g->size; u++) {
    for (const Edge *e = g->edges[u]; e; e = e->next)
      if (e->destination < g->size && partition[e->destination] == partition[u])
        actual += e->weight;
    for (unsigned v = 0; v < g->size; v++)
      if (partition[v] == partition[u])
        expected += out[u] * in[v] / total;
  }
  free(in);
  free(out);
  return (actual - expected) / total;
}

double calculatePathWeight(const Graph *g, const unsigned *path, unsigned length) {
  if (!path) return 0;
  double weight = 0;
  for (unsigned i = 1; i < length; i++)
    weight += getEdgeWeight(g, path[i - 1], path[i]);
  return weight;
}



[[nodiscard]] double *calculateClosenessCentrality(const Graph *g) {
  if (!g) return nullptr;
  double *centrality = calloc(g->size, sizeof(double));
  Matrix *distance = calculateFloydWarshall(g);
  if (!centrality || !distance) {
    free(centrality);
    destroyMatrix(distance);
    return nullptr;
  }
  for (unsigned u = 0; u < g->size; u++) {
    double total = 0;
    unsigned count = 0;
    for (unsigned v = 0; v < g->size; v++)
      if (u != v && distance->data[u][v] != INFINITY) {
        total += distance->data[u][v];
        count++;
      }
    if (total > 0)
      centrality[u] = (double) count * count / total / (g->size - 1);
    else
      centrality[u] = 0;
  }
  destroyMatrix(distance);
  return centrality;
}

[[nodiscard]] double *calculateBetweennessCentrality(const Graph *g) {
  if (!isValid(g) || hasNegativeWeights(g)) return nullptr;
  unsigned n = g->size;
  double *c = calloc(n, sizeof(double));
  if (!c || !n) return c;
  double d[n][n], p[n][n];
  for (unsigned u = 0; u < n; u++)
    for (unsigned v = 0; v < n; v++) {
      d[u][v] = u == v ? 0 : INFINITY;
      p[u][v] = u == v;
    }
  for (unsigned u = 0; u < n; u++)
    for (const Edge *e = g->edges[u]; e; e = e->next)
      if (e->weight < d[u][e->destination]) {
        d[u][e->destination] = e->weight;
        p[u][e->destination] = 1;
      }
  for (unsigned w = 0; w < n; w++)
    for (unsigned u = 0; u < n; u++)
      for (unsigned v = 0; v < n; v++) {
        double t = d[u][w] + d[w][v];
        if (t < d[u][v] - 1e-9) {
          d[u][v] = t;
          p[u][v] = p[u][w] * p[w][v];
        } else if (t == d[u][v] && w != u && w != v && d[u][v] < INFINITY) {
          p[u][v] += p[u][w] * p[w][v];
        }
      }
  for (unsigned u = 0; u < n; u++)
    for (unsigned v = 0; v < n; v++)
      if (u != v && p[u][v] > 0)
        for (unsigned w = 0; w < n; w++)
          if (w != u && w != v && fabs(d[u][w] + d[w][v] - d[u][v]) < 1e-9)
            c[w] += p[u][w] * p[w][v] / p[u][v];
  return c;
}

[[nodiscard]] double *calculateBellmanFord(const Graph *g, unsigned v) {
  if (!g || !g->edges || v >= g->size) return nullptr;
  double *distance = malloc(g->size * sizeof(double));
  if (!distance) return nullptr;
  for (unsigned u = 0; u < g->size; u++) distance[u] = INFINITY;
  distance[v] = 0;
  for (unsigned i = 1; i < g->size; i++)
    for (unsigned u = 0; u < g->size; u++)
      for (const Edge *e = g->edges[u]; e; e = e->next)
        if (e->destination < g->size && distance[u] + e->weight < distance[e->destination])
          distance[e->destination] = distance[u] + e->weight;
  for (unsigned i = 1; i < g->size; i++)
    for (unsigned u = 0; u < g->size; u++)
      for (const Edge *e = g->edges[u]; e; e = e->next)
        if (e->destination < g->size && distance[u] + e->weight < distance[e->destination])
          distance[e->destination] = -INFINITY;
  return distance;
}

[[nodiscard]] double *calculateWeightedDistances(const Graph *g, unsigned v) {
  if (!isValid(g) || hasNegativeWeights(g) || v >= g->size) return nullptr;
  bool *visited = calloc(g->size, sizeof(bool));
  double *distances = malloc(g->size * sizeof(double));
  if (!visited || !distances) {
    free(visited);
    free(distances);
    return nullptr;
  }
  for (unsigned u = 0; u < g->size; u++) distances[u] = INFINITY;
  distances[v] = 0;
  while (true) {
    visited[v] = true;
    for (const Edge *e = g->edges[v]; e; e = e->next)
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

[[nodiscard]] double *calculateEigenvectorCentrality(const Graph *g, unsigned iterations, double tolerance) {
  if (!g || !g->edges || g->size == 0) return nullptr;
  double *scores = malloc(g->size * sizeof(double));
  double *next = malloc(g->size * sizeof(double));
  if (!scores || !next) {
    free(scores);
    free(next);
    return nullptr;
  }
  for (unsigned v = 0; v < g->size; v++) scores[v] = 1 / sqrt(g->size);
  bool converged = false;
  for (unsigned i = 0; i < iterations; i++) {
    for (unsigned v = 0; v < g->size; v++) next[v] = 0;
    for (unsigned v = 0; v < g->size; v++)
      for (Edge *e = g->edges[v]; e; e = e->next)
        if (e->destination < g->size)
          next[e->destination] += e->weight * scores[v];
    double norm = calculateEuclideanNorm(next, g->size);
    if (norm < DBL_EPSILON) break;
    double max = 0;
    for (unsigned v = 0; v < g->size; v++) {
      next[v] /= norm;
      double delta = fabs(next[v] - scores[v]);
      if (delta > max) max = delta;
    }
    double *swap = scores;
    scores = next;
    next = swap;
    if (max < tolerance) {
      converged = true;
      break;
    }
  }
  free(next);
  if (!converged) {
    free(scores);
    return nullptr;
  }
  return scores;
}

[[nodiscard]] double *calculatePageRank(const Graph *g, double damping, unsigned iterations, double tolerance) {
  if (!g || g->size == 0) return nullptr;

  double *ranks = malloc(g->size * sizeof(double));
  double *nextRanks = malloc(g->size * sizeof(double));

  if (!ranks || !nextRanks) {
    free(ranks);
    free(nextRanks);
    return nullptr;
  }

  for (unsigned v = 0; v < g->size; v++)
    ranks[v] = 1.0 / g->size;

  bool converged = false;

  for (unsigned i = 0; i < iterations; i++) {
    double sinkMass = 0;
    for (unsigned v = 0; v < g->size; v++)
      if (getOutDegree(g, v) == 0)
        sinkMass += ranks[v];

    for (unsigned v = 0; v < g->size; v++)
      nextRanks[v] = (1 - damping) / g->size;

    if (sinkMass > 0)
      for (unsigned v = 0; v < g->size; v++)
        nextRanks[v] += damping * sinkMass / g->size;

    for (unsigned v = 0; v < g->size; v++)
      if (getOutDegree(g, v) > 0)
        for (Edge *e = g->edges[v]; e; e = e->next)
          if (e->destination < g->size)
            nextRanks[e->destination] += damping * ranks[v] / getOutDegree(g, v);

    double maxDelta = 0;
    for (unsigned v = 0; v < g->size; v++) {
      double delta = fabs(nextRanks[v] - ranks[v]);
      if (delta > maxDelta) maxDelta = delta;
    }

    double *swap = ranks;
    ranks = nextRanks;
    nextRanks = swap;

    if (maxDelta < tolerance) {
      converged = true;
      break;
    }
  }

  free(nextRanks);

  if (!converged) {
    free(ranks);
    return nullptr;
  }

  return ranks;
}



[[nodiscard]] Matrix *createLaplacian(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  Matrix *L = createZeroMatrix(g->size, g->size);
  if (!L) return nullptr;
  for (unsigned u = 0; u < g->size; u++)
    for (Edge *e = g->edges[u]; e; e = e->next)
      if (e->destination < g->size) {
        L->data[u][u] += e->weight;
        L->data[u][e->destination] -= e->weight;
      }
  return L;
}

[[nodiscard]] Matrix *calculateFloydWarshall(const Graph *g) {
  if (!g || !g->edges) return nullptr;
  Matrix *distance = createZeroMatrix(g->size, g->size);
  if (!distance) return nullptr;
  for (unsigned u = 0; u < g->size; u++)
    for (unsigned v = 0; v < g->size; v++)
      if (u == v)
        distance->data[u][v] = 0;
      else
        distance->data[u][v] = INFINITY;
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size && e->weight < distance->data[v][e->destination])
        distance->data[v][e->destination] = e->weight;
  for (unsigned w = 0; w < g->size; w++)
    for (unsigned u = 0; u < g->size; u++)
      for (unsigned v = 0; v < g->size; v++)
        if (distance->data[u][w] + distance->data[w][v] < distance->data[u][v])
          distance->data[u][v] = distance->data[u][w] + distance->data[w][v];
  for (unsigned w = 0; w < g->size; w++)
    for (unsigned u = 0; u < g->size; u++)
      for (unsigned v = 0; v < g->size; v++)
        if (distance->data[u][w] + distance->data[w][v] < distance->data[u][v])
          distance->data[u][v] = -INFINITY;
  return distance;
}

[[nodiscard]] Matrix *calculateGraphLayout(const Graph *g, unsigned iterations) {
  if (!isValid(g)) return nullptr;
  Matrix *position = createZeroMatrix(g->size, 2);
  Matrix *displacement = createZeroMatrix(g->size, 2);
  if (!position || !displacement) {
    destroyMatrix(position);
    destroyMatrix(displacement);
    return nullptr;
  }
  const double width = 1000;
  const double height = 1000;
  const double area = width * height;
  const double k = 0.75 * sqrt(area / g->size);
  for (unsigned v = 0; v < g->size; v++) {
    position->data[v][0] = width / 4 + rand() / ((double)RAND_MAX + 1) * width / 2;
    position->data[v][1] = height / 4 + rand() / ((double)RAND_MAX + 1) * height / 2;
  }
  double temperature = width / 10;
  const double cooling = temperature / iterations;
  for (unsigned i = 0; i < iterations; i++) {
    for (unsigned v = 0; v < g->size; v++) {
      displacement->data[v][0] = 0;
      displacement->data[v][1] = 0;
    }
    for (unsigned u = 0; u < g->size; u++)
      for (unsigned v = 0; v < g->size; v++) {
        if (u == v) continue;
        double dx = position->data[u][0] - position->data[v][0];
        double dy = position->data[u][1] - position->data[v][1];
        if (fabs(dx) < 1e-4 && fabs(dy) < 1e-4) {
          dx = 0.1 * (rand() % 2 ? 1 : -1);
          dy = 0.1 * (rand() % 2 ? 1 : -1);
        }
        const double distance = sqrt(dx * dx + dy * dy);
        const double repulsion = k * k / distance / distance;
        displacement->data[u][0] += dx * repulsion;
        displacement->data[u][1] += dy * repulsion;
      }
    for (unsigned u = 0; u < g->size; u++)
      for (const Edge *e = g->edges[u]; e; e = e->next) {
        const unsigned v = e->destination;
        if (u == v) continue;
        const double dx = position->data[u][0] - position->data[v][0];
        const double dy = position->data[u][1] - position->data[v][1];
        const double distance = sqrt(dx * dx + dy * dy);
        const double attraction = distance / k;
        displacement->data[u][0] -= dx * attraction;
        displacement->data[u][1] -= dy * attraction;
      }
    for (unsigned v = 0; v < g->size; v++) {
      const double dx = displacement->data[v][0];
      const double dy = displacement->data[v][1];
      const double distance = sqrt(dx * dx + dy * dy);
      if (distance == 0) continue;
      const double capped = distance < temperature ? distance : temperature;
      position->data[v][0] += dx / distance * capped;
      position->data[v][1] += dy / distance * capped;
      if (position->data[v][0] < 0) position->data[v][0] = 0;
      if (position->data[v][0] > width) position->data[v][0] = width;
      if (position->data[v][1] < 0) position->data[v][1] = 0;
      if (position->data[v][1] > height) position->data[v][1] = height;
    }
    temperature -= cooling;
    if (temperature < 0) temperature = 0;
  }
  destroyMatrix(displacement);
  return position;
}



void testHasDirectedCycle() {
  Graph *g1 = createGraph(3);
  addDirectedEdge(g1, 0, 1);
  addDirectedEdge(g1, 1, 2);
  addDirectedEdge(g1, 2, 0);
  assert(hasDirectedCycle(g1) == true);
  printf("Directed cyclic test 1 passed: Simple cycle found.\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addDirectedEdge(g2, 0, 1);
  addDirectedEdge(g2, 1, 2);
  addDirectedEdge(g2, 0, 2);
  assert(hasDirectedCycle(g2) == false);
  printf("Directed cyclic test 2 passed: DAG correctly identified as acyclic.\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(1);
  addDirectedEdge(g3, 0, 0);
  assert(hasDirectedCycle(g3) == true);
  printf("Directed cyclic test 3 passed: Self-loop detected.\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(4);
  addDirectedEdge(g4, 0, 1);
  addDirectedEdge(g4, 2, 3);
  addDirectedEdge(g4, 3, 2);
  assert(hasDirectedCycle(g4) == true);
  printf("Directed cyclic test 4 passed: Cycle in disconnected component found.\n");
  destroyGraph(g4);

  Graph *g5 = createGraph(0);
  assert(hasDirectedCycle(g5) == false);
  printf("Directed cyclic test 5 passed: Empty graph is acyclic.\n");
  destroyGraph(g5);
}

void testHasUndirectedCycle() {
  Graph *g1 = createGraph(3);
  addUndirectedEdge(g1, 0, 1, 1);
  addUndirectedEdge(g1, 1, 2, 1);
  if (!hasUndirectedCycle(g1))
    printf("Undirected Test 1 passed: Tree is acyclic.\n");
  else
    printf("Undirected Test 1 failed: False positive in tree.\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addUndirectedEdge(g2, 0, 1, 1);
  addUndirectedEdge(g2, 1, 2, 1);
  addUndirectedEdge(g2, 2, 0, 1);
  if (hasUndirectedCycle(g2))
    printf("Undirected Test 2 passed: Triangle cycle detected.\n");
  else
    printf("Undirected Test 2 failed: Triangle cycle missed.\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(5);
  addUndirectedEdge(g3, 0, 1, 1);
  addUndirectedEdge(g3, 2, 3, 1);
  addUndirectedEdge(g3, 3, 4, 1);
  addUndirectedEdge(g3, 4, 2, 1);
  if (hasUndirectedCycle(g3))
    printf("Undirected Test 3 passed: Cycle in disconnected component detected.\n");
  else
    printf("Undirected Test 3 failed: Missed cycle in disconnected component.\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(2);
  addUndirectedEdge(g4, 0, 1, 1);
  if (!hasUndirectedCycle(g4))
    printf("Undirected Test 4 passed: Simple edge is acyclic.\n");
  else
    printf("Undirected Test 4 failed: Parent incorrectly triggered cycle.\n");
  destroyGraph(g4);
}

void testIsWeaklyConnected() {
  Graph *g1 = createGraph(3);
  addDirectedEdge(g1, 0, 1);
  addDirectedEdge(g1, 1, 2);
  assert(isWeaklyConnected(g1) == true);
  printf("Weakly Test 1 passed: Simple chain\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addDirectedEdge(g2, 0, 1);
  addDirectedEdge(g2, 2, 1);
  assert(isWeaklyConnected(g2) == true);
  printf("Weakly Test 2 passed: Source/Sink structure\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(4);
  addDirectedEdge(g3, 0, 1);
  addDirectedEdge(g3, 2, 3);
  assert(isWeaklyConnected(g3) == false);
  printf("Weakly Test 3 passed: Truly disconnected components\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(4);
  addDirectedEdge(g4, 0, 1);
  addDirectedEdge(g4, 0, 2);
  addDirectedEdge(g4, 0, 3);
  assert(isWeaklyConnected(g4) == true);
  printf("Weakly Test 4 passed: Star pattern\n");
  destroyGraph(g4);

  Graph *g5 = createGraph(2);
  addDirectedEdge(g5, 0, 0);
  assert(isWeaklyConnected(g5) == false);
  printf("Weakly Test 5 passed: Isolated vertex with self-loop\n");
  destroyGraph(g5);
}

void testIsStronglyConnected() {
  Graph *g1 = createGraph(3);
  addDirectedEdge(g1, 0, 1);
  addDirectedEdge(g1, 1, 2);
  addDirectedEdge(g1, 2, 0);
  assert(isStronglyConnected(g1) == true);
  printf("Strongly Test 1 passed: Simple cycle\n");
  destroyGraph(g1);

  Graph *g2 = createGraph(3);
  addDirectedEdge(g2, 0, 1);
  addDirectedEdge(g2, 1, 2);
  assert(isStronglyConnected(g2) == false);
  printf("Strongly Test 2 passed: Linear chain (not strong)\n");
  destroyGraph(g2);

  Graph *g3 = createGraph(4);
  addDirectedEdge(g3, 0, 1);
  addDirectedEdge(g3, 1, 0);
  addDirectedEdge(g3, 2, 3);
  addDirectedEdge(g3, 3, 2);
  assert(isStronglyConnected(g3) == false);
  printf("Strongly Test 3 passed: Disconnected cycles\n");
  destroyGraph(g3);

  Graph *g4 = createGraph(3);
  addDirectedEdge(g4, 0, 1);
  addDirectedEdge(g4, 1, 0);
  addDirectedEdge(g4, 1, 2);
  addDirectedEdge(g4, 2, 1);
  assert(isStronglyConnected(g4) == true);
  printf("Strongly Test 4 passed: Bidirectional chain\n");
  destroyGraph(g4);

  Graph *g5 = createGraph(1);
  assert(isStronglyConnected(g5) == true);
  printf("Strongly Test 5 passed: Single vertex\n");
  destroyGraph(g5);
}

void testTopologicalSortOfGraph() {
  Graph *g1 = createGraph(3);
  addDirectedEdge(g1, 0, 1);
  addDirectedEdge(g1, 1, 2);
  unsigned *order1 = getTopologicalSort(g1);
  assert(isTopologicalSort(g1, order1));
  printf("Topo test 1 (linear) passed!\n");
  destroyGraph(g1);
  free(order1);

  Graph *g2 = createGraph(4);
  addDirectedEdge(g2, 0, 1);
  addDirectedEdge(g2, 0, 2);
  addDirectedEdge(g2, 1, 3);
  addDirectedEdge(g2, 2, 3);
  unsigned *order2 = getTopologicalSort(g2);
  assert(isTopologicalSort(g2, order2));
  printf("Topo test 2 (diamond) passed!\n");
  destroyGraph(g2);
  free(order2);

  Graph *g3 = createGraph(4);
  addDirectedEdge(g3, 0, 1);
  addDirectedEdge(g3, 2, 3);
  unsigned *order3 = getTopologicalSort(g3);
  assert(isTopologicalSort(g3, order3));
  printf("Topo test 3 (disconnected) passed!\n");
  destroyGraph(g3);
  free(order3);

  Graph *g4 = createGraph(1);
  unsigned *order4 = getTopologicalSort(g4);
  assert(order4[0] == 0);
  printf("Topo test 4 (single vertex) passed!\n");
  destroyGraph(g4);
  free(order4);
}

void testBellmanFord() {
  Graph *g = createGraph(4);
  addWeightedDirectedEdge(g, 0, 1, 5);
  addWeightedDirectedEdge(g, 1, 2, 1);
  addWeightedDirectedEdge(g, 0, 2, 10);
  addWeightedDirectedEdge(g, 2, 3, 1);
  double *d1 = calculateBellmanFord(g, 0);
  assert(d1 != nullptr);
  assert(d1[3] == 7);
  printf("Bellman-Ford test 1 (positive) passed!\n");
  free(d1);

  addWeightedDirectedEdge(g, 3, 1, -10);
  double *d2 = calculateBellmanFord(g, 0);
  bool b = false;
  for (unsigned i = 0; i < 4; i++) b = b || d2[i] == -INFINITY;
  assert(b);
  printf("Bellman-Ford test 2 (negative cycle) passed!\n");
  destroyGraph(g);
}

void testUnweightedDijkstra() {
  printf("Running calculateUnweightedDistances tests...\n");

  {
    Graph *g = createGraph(3);
    addWeightedDirectedEdge(g, 0, 1, 10);
    addWeightedDirectedEdge(g, 1, 2, 20);

    unsigned *distances = calculateUnweightedDistances(g, 0);
    assert(distances[0] == 0);
    assert(distances[1] == 1);
    assert(distances[2] == 2);

    printf("Passed: Simple path (weights ignored)\n");
    free(distances);
    destroyGraph(g);
  }

  {
    Graph *g = createGraph(3);
    addWeightedDirectedEdge(g, 0, 2, 100);
    addWeightedDirectedEdge(g, 0, 1, 1);
    addWeightedDirectedEdge(g, 1, 2, 1);

    unsigned *distances = calculateUnweightedDistances(g, 0);
    assert(distances[2] == 1);

    printf("Passed: Shortest hops selection\n");
    free(distances);
    destroyGraph(g);
  }

  {
    Graph *g = createGraph(3);
    addDirectedEdge(g, 0, 1);

    unsigned *distances = calculateUnweightedDistances(g, 0);
    assert(distances[0] == 0);
    assert(distances[1] == 1);
    assert(distances[2] == UINT_MAX);

    printf("Passed: Unreachable node (UINT_MAX)\n");
    free(distances);
    destroyGraph(g);
  }

  {
    Graph *g = createGraph(3);
    addDirectedEdge(g, 0, 1);
    addDirectedEdge(g, 1, 2);
    addDirectedEdge(g, 2, 0);

    unsigned *distances = calculateUnweightedDistances(g, 0);
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
  addWeightedDirectedEdge(g1, 0, 1, 5);
  addWeightedDirectedEdge(g1, 1, 2, 10);
  double *dist1 = calculateWeightedDistances(g1, 0);
  assert(dist1[0] == 0);
  assert(dist1[1] == 5);
  assert(dist1[2] == 15);
  printf("Dijkstra test 1 passed: Simple path\n");
  destroyGraph(g1);
  free(dist1);

  Graph *g2 = createGraph(3);
  addWeightedDirectedEdge(g2, 0, 2, 10);
  addWeightedDirectedEdge(g2, 0, 1, 2);
  addWeightedDirectedEdge(g2, 1, 2, 3);
  double *dist2 = calculateWeightedDistances(g2, 0);
  assert(dist2[2] == 5);
  printf("Dijkstra test 2 passed: Shortest path selection\n");
  destroyGraph(g2);
  free(dist2);

  Graph *g3 = createGraph(2);
  double *dist3 = calculateWeightedDistances(g3, 0);
  assert(dist3[0] == 0);
  assert(dist3[1] == INFINITY);
  printf("Dijkstra test 3 passed: Unreachable vertex (INFINITY)\n");
  destroyGraph(g3);
  free(dist3);

  Graph *g4 = createGraph(3);
  addWeightedDirectedEdge(g4, 0, 1, 1);
  addWeightedDirectedEdge(g4, 1, 2, 1);
  addWeightedDirectedEdge(g4, 2, 0, 1);
  double *dist4 = calculateWeightedDistances(g4, 0);
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
    addWeightedDirectedEdge(g, 0, 1, 10);
    addWeightedDirectedEdge(g, 1, 2, 5);
    addWeightedDirectedEdge(g, 0, 2, 20);

    Matrix *distances = calculateFloydWarshall(g);

    assert(distances->data[0][1] == 10);
    assert(distances->data[1][2] == 5);
    assert(distances->data[0][2] == 15);
    assert(distances->data[2][0] == INFINITY);

    destroyMatrix(distances);
    destroyGraph(g);
    printf("Passed!\n");
  }
  {
    printf("Floyd-Warshall: Testing negative weight (no cycle)... ");
    Graph *g = createGraph(3);
    addWeightedDirectedEdge(g, 0, 1, 4);
    addWeightedDirectedEdge(g, 0, 2, 5);
    addWeightedDirectedEdge(g, 1, 2, -2);

    Matrix *distances = calculateFloydWarshall(g);

    assert(distances->data[0][2] == 2);
    assert(distances->data[1][2] == -2);

    destroyMatrix(distances);
    destroyGraph(g);
    printf("Passed!\n");
  }
  {
    printf("Floyd-Warshall: Testing disconnected components... ");
    Graph *g = createGraph(4);
    addWeightedDirectedEdge(g, 0, 1, 1);
    addWeightedDirectedEdge(g, 2, 3, 1);

    Matrix *distances = calculateFloydWarshall(g);

    assert(distances->data[0][1] == 1);
    assert(distances->data[0][2] == INFINITY);
    assert(distances->data[3][0] == INFINITY);

    destroyMatrix(distances);
    destroyGraph(g);
    printf("Passed!\n");
  }
  {
    printf("Floyd-Warshall: Testing negative cycle detection... ");
    unsigned n = 3;
    Graph *g = createGraph(n);

    addWeightedDirectedEdge(g, 0, 1, 1);
    addWeightedDirectedEdge(g, 1, 2, 1);
    addWeightedDirectedEdge(g, 2, 0, -5);

    Matrix *distances = calculateFloydWarshall(g);

    bool hasNegativeCycle = false;
    for (unsigned i = 0; i < n; i++)
      if (distances->data[i][i] < 0)
        hasNegativeCycle = true;

    assert(hasNegativeCycle == true);
    assert(distances->data[0][0] == -INFINITY);

    destroyMatrix(distances);
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
    Graph *mst = createPrim(g);
    double w = sumWeights(mst) / 2;
    assert(countEdges(mst) == 4);
    assert(w == 4);
    printf("Prim test 1 (triangle) passed: weight %lg\n", w);
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
    Graph *mst = createPrim(g);
    double w = sumWeights(mst) / 2;
    assert(countEdges(mst) == 8);
    assert(w == 16);
    printf("Prim test 2 (complex) passed: weight %lg\n", w);
    destroyGraph(g);
    destroyGraph(mst);
  }
  {
    Graph *g = createGraph(1);
    Graph *mst = createPrim(g);
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
  Graph *mst = createKruskal(g);
  double weight = sumWeights(mst) / 2;
  assert(countEdges(mst) == 6);
  assert(weight == 19);
  printf("Kruskal test passed: weight %lg\n", weight);
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
  for (unsigned v = 0; v < 65536; v++) addDirectedEdge(graph, v, v + 1);
  assert(calculateDensity(graph) <= 0.99);
  destroyGraph(graph);
}

void testCalculateBetweennessCentrality() {
  printf("\n");

  {
    printf("[FloatingPointPrecision] Running scenario...\n");
    Graph *g = createGraph(4);
    assert(g);

    addWeightedDirectedEdge(g, 0, 1, 0.1);
    addWeightedDirectedEdge(g, 1, 2, 0.2);

    addWeightedDirectedEdge(g, 0, 3, 0.15);
    addWeightedDirectedEdge(g, 3, 2, 0.15);

    double *centrality = calculateBetweennessCentrality(g);
    assert(centrality);

    printf("[FloatingPointPrecision] Centrality Node 1: %lf, Node 3: %lf\n", centrality[1], centrality[3]);

    bool pass = (fabs(centrality[1] - centrality[3]) < 1e-6);
    free(centrality);
    destroyGraph(g);

    if (!pass) {
      printf("[FAIL] Strict floating-point equality missed a valid path!\n\n");
      assert(false);
    }
    printf("[SUCCESS] Floating point precision tolerance verified.\n\n");
  }

  {
    printf("[ParallelEdgeOverwrite] Running scenario...\n");
    Graph *g = createGraph(3);
    assert(g);

    addDirectedEdge(g, 0, 1);
    addDirectedEdge(g, 0, 1);
    addDirectedEdge(g, 1, 2);

    double *centrality = calculateBetweennessCentrality(g);
    assert(centrality);

    printf("[ParallelEdgeOverwrite] Centrality Node 1: %lf\n", centrality[1]);

    bool pass = (centrality[1] > 0.0);
    free(centrality);
    destroyGraph(g);

    if (!pass) {
      printf("[FAIL] Parallel edges corrupted internal path counting arrays!\n\n");
      assert(false);
    }
    printf("[SUCCESS] Parallel edge structural multi-paths verified.\n\n");
  }

  {
    printf("[IdentityPathInterference] Running scenario...\n");
    Graph *g = createGraph(3);
    assert(g);

    addDirectedEdge(g, 0, 1);
    addDirectedEdge(g, 1, 2);

    double *centrality = calculateBetweennessCentrality(g);
    assert(centrality);

    printf("[IdentityPathInterference] Centrality Node 1 (Expected 1.0): %lf\n", centrality[1]);

    bool pass = fabs(centrality[1] - 1) < 1e-6;
    free(centrality);
    destroyGraph(g);

    if (!pass) {
      printf("[FAIL] Identity initialization (u == v) leaked into path products!\n\n");
      assert(false);
    }
    printf("[SUCCESS] Identity path protection verified.\n\n");
  }
}

int main() {
  testHasDirectedCycle();
  testHasUndirectedCycle();
  testIsWeaklyConnected();
  testIsStronglyConnected();
  testTopologicalSortOfGraph();
  testBellmanFord();
  testUnweightedDijkstra();
  testWeightedDijkstra();
  testFloydWarshall();
  testPrim();
  testKruskal();
  testFindArticulationPoints();
  testGraphDensity();
  testCalculateBetweennessCentrality();
  printf("All tests passed!\n");
  return 0;
}
