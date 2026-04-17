#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>



struct QNode {
  unsigned vertex;
  struct QNode *next;
};

struct Queue {
  struct QNode *front, *rear;
};

struct Queue *createQueue() { struct Queue *queue = malloc(sizeof(struct Queue)); queue->front = queue->rear = NULL; return queue; }

void enqueue(struct Queue *queue, unsigned vertex) {
  struct QNode *node = malloc(sizeof(struct QNode));
  node->vertex = vertex;
  node->next = NULL;
  if (queue->rear) queue->rear->next = node; else queue->front = node;
  queue->rear = node;
}

unsigned dequeue(struct Queue *queue) {
  assert(queue->front);
  struct QNode *node = queue->front;
  unsigned vertex = node->vertex;
  queue->front = queue->front->next;
  if (!queue->front) queue->rear = NULL;
  free(node);
  return vertex;
}

bool isEmpty(const struct Queue *queue) { return !queue->front; }

void freeQueue(struct Queue *queue) { while (!isEmpty(queue)) dequeue(queue); free(queue); }



struct HeapNode {
  unsigned vertex;
  unsigned distance;
};

void swapInHeap(struct HeapNode **a, struct HeapNode **b) { struct HeapNode *x = *a; struct HeapNode *y = *b; *a = y; *b = x; }

struct Heap {
  unsigned size;
  unsigned capacity;
  struct HeapNode **array;
};

struct Heap *createHeap() {
  struct Heap *heap = malloc(sizeof(struct Heap));
  heap->size = 0;
  heap->capacity = 1;
  heap->array = malloc(heap->capacity * sizeof(struct HeapNode *));
  return heap;
}

void freeHeap(struct Heap *heap) { for (unsigned i = 0; i < heap->size; i++) free(heap->array[i]); free(heap->array); free(heap); }

void insertInHeap(struct Heap *heap, unsigned vertex, unsigned distance) {
  if (heap->size == heap->capacity) { heap->capacity *= 2; heap->array = realloc(heap->array, heap->capacity * sizeof(struct HeapNode *)); }
  struct HeapNode *node = malloc(sizeof(struct HeapNode));
  node->vertex = vertex;
  node->distance = distance;
  unsigned i = heap->size;
  heap->array[i] = node;
  while (i > 0 && heap->array[i]->distance < heap->array[(i - 1) / 2]->distance) {
    swapInHeap(&heap->array[i], &heap->array[(i - 1) / 2]);
    i = (i - 1) / 2;
  }
  heap->size++;
}

unsigned getHeapVertex(const struct Heap *heap) { assert(heap->size > 0); return heap->array[0]->vertex; }

unsigned getHeapDistance(const struct Heap *heap) { assert(heap->size > 0); return heap->array[0]->distance; }

void extractFromHeap(struct Heap *heap) {
  assert(heap->size > 0);
  free(heap->array[0]);
  heap->array[0] = heap->array[--heap->size];
  unsigned i = 0;
  while ((2 * i + 1 < heap->size && heap->array[2 * i + 1]->distance < heap->array[i]->distance) ||
    (2 * i + 2 < heap->size && heap->array[2 * i + 2]->distance < heap->array[i]->distance))
      if (2 * i + 2 >= heap->size || heap->array[2 * i + 1]->distance <= heap->array[2 * i + 2]->distance) {
        swapInHeap(&heap->array[i], &heap->array[2 * i + 1]);
        i = 2 * i + 1;
      } else {
        swapInHeap(&heap->array[i], &heap->array[2 * i + 2]);
        i = 2 * i + 2;
      }
}



struct Node {
  unsigned vertex;
  int weight;
  struct Node *next;
};

bool areValidNeighbors(const struct Node *node, unsigned size) {
  return !node || (node->vertex < size && areValidNeighbors(node->next, size));
}

void freeNeighbors(struct Node *node) { if (!node) return; freeNeighbors(node->next); free(node); }



struct Graph {
  unsigned size;
  struct Node **neighbors;
};

bool isValidGraph(struct Graph *graph) {
  if (!graph) return false;
  bool valid = true;
  for (unsigned i = 0; i < graph->size; i++) valid &= areValidNeighbors(graph->neighbors[i], graph->size);
  return valid;
}

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
  assert(isValidGraph(graph));
  assert(source < graph->size);
  assert(destination < graph->size);
  struct Node *node = malloc(sizeof(struct Node));
  node->vertex = destination;
  node->next = graph->neighbors[source];
  graph->neighbors[source] = node;
}

void addUndirectedEdge(struct Graph *graph, unsigned source, unsigned destination) {
  assert(isValidGraph(graph));
  assert(source < graph->size);
  assert(destination < graph->size);
  addDirectedEdge(graph, source, destination);
  if (source != destination) addDirectedEdge(graph, destination, source);
}

void addWeightedEdge(struct Graph *graph, unsigned source, unsigned destination, int weight) {
  assert(isValidGraph(graph));
  assert(source < graph->size);
  assert(destination < graph->size);
  struct Node *node = malloc(sizeof(struct Node));
  node->vertex = destination;
  node->weight = weight;
  node->next = graph->neighbors[source];
  graph->neighbors[source] = node;
}

static void depthFirstSearchRecursive(const struct Graph *graph, bool *visited, unsigned vertex) {
  assert(vertex < graph->size);
  visited[vertex] = true;
  printf(" %u", vertex);
  for (const struct Node *neighbors = graph->neighbors[vertex]; neighbors; neighbors = neighbors->next)
    if (!visited[neighbors->vertex]) depthFirstSearchRecursive(graph, visited, neighbors->vertex);
}

void depthFirstSearch(const struct Graph *graph, unsigned vertex) {
  bool *visited = calloc(graph->size, sizeof(*visited));
  printf("Depth-first order:");
  depthFirstSearchRecursive(graph, visited, vertex);
  printf("\n");
  free(visited);
}

void breadthFirstSearch(const struct Graph *graph, unsigned start) {
  bool *visited = calloc(graph->size, sizeof(bool));
  struct Queue *queue = createQueue();
  enqueue(queue, start);
  printf("Breadth-first order:");
  while (!isEmpty(queue)) {
    unsigned current = dequeue(queue);
    if (!visited[current]) {
      printf(" %u", current);
      visited[current] = true;
      for (struct Node *neighbor = graph->neighbors[current]; neighbor; neighbor = neighbor->next) enqueue(queue, neighbor->vertex);
    }
  }
  printf("\n");
  free(visited);
  freeQueue(queue);
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

unsigned *shortestDistanceAlternative1(const struct Graph *graph, unsigned start) {
  unsigned *distance = malloc(graph->size * sizeof(unsigned));
  for (unsigned i = 0; i < graph->size; i++) distance[i] = UINT_MAX;
  distance[start] = 0;
  for (unsigned count = 0; count < graph->size; count++)
    for (unsigned v = 0; v < graph->size; v++)
      if (distance[v] != UINT_MAX)
        for (struct Node *neighbor = graph->neighbors[v]; neighbor; neighbor = neighbor->next)
          if (distance[v] + neighbor->weight < distance[neighbor->vertex])
            distance[neighbor->vertex] = distance[v] + neighbor->weight;
  return distance;
}

unsigned *shortestDistanceAlternative2(const struct Graph *graph, unsigned start) {
  unsigned *distance = malloc(graph->size * sizeof(unsigned));
  bool *visited = calloc(graph->size, sizeof(bool));
  for (unsigned i = 0; i < graph->size; i++) distance[i] = UINT_MAX;
  distance[start] = 0;
  for (unsigned count = 0; count < graph->size; count++) {
    unsigned u = 0;
    while (visited[u]) u++;
    for (unsigned v = 0; v < graph->size; v++) if (!visited[v] && distance[v] < distance[u]) u = v;
    visited[u] = true;
    if (distance[u] != UINT_MAX)
      for (struct Node *neighbor = graph->neighbors[u]; neighbor; neighbor = neighbor->next)
        if (distance[u] + neighbor->weight < distance[neighbor->vertex])
          distance[neighbor->vertex] = distance[u] + neighbor->weight;
  }
  free(visited);
  return distance;
}

unsigned *shortestDistance(const struct Graph *graph, unsigned start) {
  unsigned *distance = malloc(graph->size * sizeof(unsigned));
  for (unsigned i = 0; i < graph->size; i++) distance[i] = UINT_MAX;
  distance[start] = 0;
  struct Heap *heap = createHeap();
  insertInHeap(heap, start, 0);
  while (heap->size > 0) {
    unsigned u = getHeapVertex(heap);
    unsigned d = getHeapDistance(heap);
    extractFromHeap(heap);
    if (d > distance[u]) continue;
    for (struct Node *edge = graph->neighbors[u]; edge; edge = edge->next)
      if (distance[u] + edge->weight < distance[edge->vertex]) {
        distance[edge->vertex] = distance[u] + edge->weight;
        insertInHeap(heap, edge->vertex, distance[edge->vertex]);
      }
  }
  freeHeap(heap);
  return distance;
}

int main() {
  struct Graph *graph1 = createGraph(4);
  addUndirectedEdge(graph1, 0, 1);
  addUndirectedEdge(graph1, 0, 2);
  addUndirectedEdge(graph1, 1, 2);
  addUndirectedEdge(graph1, 2, 3);
  depthFirstSearch(graph1, 2);
  freeGraph(graph1);

  struct Graph *graph2 = createGraph(4);
  addDirectedEdge(graph2, 0, 1);
  addDirectedEdge(graph2, 1, 2);
  addDirectedEdge(graph2, 2, 0);
  addDirectedEdge(graph2, 2, 3);
  if (isCyclicDirected(graph2)) printf("Graph 2 contains a cycle.\n"); else printf("Graph 2 does not contain a cycle.\n");
  freeGraph(graph2);

  struct Graph *graph3 = createGraph(4);
  addUndirectedEdge(graph3, 0, 1);
  addUndirectedEdge(graph3, 1, 2);
  addUndirectedEdge(graph3, 2, 3);
  breadthFirstSearch(graph3, 0);
  if (isCyclicUndirected(graph3)) printf("Graph 3 is cyclic.\n"); else printf("Graph 3 is acyclic.\n");
  freeGraph(graph3);

  struct Graph *graph4 = createGraph(3);
  addUndirectedEdge(graph4, 0, 1);
  addUndirectedEdge(graph4, 1, 2);
  addUndirectedEdge(graph4, 2, 0);
  if (isCyclicUndirected(graph4)) printf("Graph 4 contains an undirected cycle.\n"); else printf("Graph 4 cycle not detected.\n");
  freeGraph(graph4);

  struct Graph *graph5 = createGraph(5);
  addWeightedEdge(graph5, 0, 1, 10);
  addWeightedEdge(graph5, 0, 4, 5);
  addWeightedEdge(graph5, 1, 2, 1);
  addWeightedEdge(graph5, 1, 4, 2);
  addWeightedEdge(graph5, 2, 3, 4);
  addWeightedEdge(graph5, 4, 1, 3);
  addWeightedEdge(graph5, 4, 2, 9);
  addWeightedEdge(graph5, 4, 3, 2);
  int start = 0;
  unsigned *distance = shortestDistance(graph5, start);
  printf("Shortest distances from vertex %u: ", start);
  for (unsigned i = 0; i < graph5->size; i++) { if (i > 0) printf(", "); printf("%u: %u", i, distance[i]); }
  printf("\n");
  freeGraph(graph5);
  free(distance);

  return 0;
}
