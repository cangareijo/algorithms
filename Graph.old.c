
bool have_common_neighbors(const Graph *g, unsigned u, unsigned v) {
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
