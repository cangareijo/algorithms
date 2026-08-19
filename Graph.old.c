
bool **findFeedbackArcSet(const Graph *g);

static bool findFeedbackArcSet_hasCycleDfs(const Graph *g, bool **removed, unsigned v, char *visited) {
  visited[v] = 1;
  for (Edge *e = g->edges[v]; e; e = e->next) {
    if (e->destination >= g->size || removed[v][e->destination]) continue;
    if (visited[e->destination] == 1) return true;
    if (visited[e->destination] == 0 && findFeedbackArcSet_hasCycleDfs(g, removed, e->destination, visited)) return true;
  }
  visited[v] = 2;
  return false;
}

static bool findFeedbackArcSet_hasCycle(const Graph *g, bool **removed) {
  char *visited = calloc(g->size, sizeof(char));
  if (!visited) return false;
  for (unsigned v = 0; v < g->size; v++)
    if (visited[v] == 0 && findFeedbackArcSet_hasCycleDfs(g, removed, v, visited)) {
      free(visited);
      return true;
    }
  free(visited);
  return false;
}

static void searchFeedbackArcSet(
  const Graph *g, unsigned u, Edge *e, bool **current, unsigned currentSize, bool **best, unsigned *bestSize)
{
  if (currentSize >= *bestSize) return;
  if (!e) {
    if (u + 1 >= g->size) {
      if (!findFeedbackArcSet_hasCycle(g, current)) {
        *bestSize = currentSize;
        for (unsigned v = 0; v < g->size; v++)
          for (unsigned w = 0; w < g->size; w++)
            best[v][w] = current[v][w];
      }
      return;
    }
    searchFeedbackArcSet(g, u + 1, g->edges[u + 1], current, currentSize, best, bestSize);
    return;
  }
  if (e->destination >= g->size || !current[u][e->destination])
    searchFeedbackArcSet(g, u, e->next, current, currentSize, best, bestSize);
  if (e->destination < g->size) {
    bool previous = current[u][e->destination];
    current[u][e->destination] = true;
    searchFeedbackArcSet(g, u, e->next, current, currentSize + 1, best, bestSize);
    current[u][e->destination] = previous;
  }
}

[[nodiscard]] bool **findFeedbackArcSet(const Graph *g) {
  if (!g || !g->edges || g->size == 0) return nullptr;
  bool **current = allocateFalseMatrix(g->size, g->size);
  bool **best = allocateFalseMatrix(g->size, g->size);
  if (!current || !best) {
    freeBooleanMatrix(current, g->size);
    freeBooleanMatrix(best, g->size);
    return nullptr;
  }
  unsigned bestSize = UINT_MAX;
  searchFeedbackArcSet(g, 0, g->edges[0], current, 0, best, &bestSize);
  freeBooleanMatrix(current, g->size);
  return best;
}

bool hasInvalidEdges(const Graph *g) {
  if (!g || !g->edges) return false;
  for (unsigned v = 0; v < g->size; v++)
    for (const Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination >= g->size)
        return true;
  return false;
}

static unsigned countDirectedTrailsFrom(unsigned v, unsigned n, unsigned adjacency[n][n]) {
  unsigned trails = 0;
  for (unsigned w = 0; w < n; w++)
    if (adjacency[v][w] > 0) {
      adjacency[v][w]--;
      trails += 1 + countDirectedTrailsFrom(w, n, adjacency);
      adjacency[v][w]++;
    }
  return trails;
}

unsigned countDirectedTrails(const Graph *g) {
  if (!g || !g->edges || g->size == 0) return 0;
  unsigned adjacency[g->size][g->size] = {};
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size)
        adjacency[v][e->destination]++;
  unsigned trails = 0;
  for (unsigned v = 0; v < g->size; v++) trails += countDirectedTrailsFrom(v, g->size, adjacency);
  return trails;
}

static unsigned countUndirectedTrailsFrom(unsigned v, unsigned n, unsigned adjacency[n][n]) {
  unsigned trails = 0;
  for (unsigned w = 0; w < n; w++)
    if (adjacency[v][w] > 0 && adjacency[w][v] > 0) {
      adjacency[v][w]--;
      if (v != w) adjacency[w][v]--;
      trails += 1 + countUndirectedTrailsFrom(w, n, adjacency);
      adjacency[v][w]++;
      if (v != w) adjacency[w][v]++;
    }
  return trails;
}

unsigned countUndirectedTrails(const Graph *g) {
  if (!g || !g->edges || g->size == 0) return 0;
  unsigned adjacency[g->size][g->size] = {};
  for (unsigned v = 0; v < g->size; v++)
    for (Edge *e = g->edges[v]; e; e = e->next)
      if (e->destination < g->size)
        adjacency[v][e->destination]++;
  unsigned trails = 0;
  for (unsigned v = 0; v < g->size; v++) trails += countUndirectedTrailsFrom(v, g->size, adjacency);
  return trails;
}

bool isRegular(const Graph *g) {
  unsigned *in = getInDegrees(g);
  unsigned *out = getOutDegrees(g);
  bool b = g && (g->size == 0 || (in && out && in[0] == out[0]));
  for (unsigned v = 1; b && v < g->size; v++)
    b = b && in[v - 1] == in[v] && out[v - 1] == out[v];
  free(in);
  free(out);
  return b;
}
