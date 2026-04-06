typedef struct {
  TreeList *satisfied;
  TreeList *failed;
} PartitionResult;

TreeList *TreeList_map(TreeList *list, void *(*f)(void *));
void TreeList_foreach(TreeList *list, void (*f)(void *));
void TreeList_forEachReverse(TreeList *list, void (*f)(void *));
void *TreeList_fold(TreeList *list, void *acc, void *(*f)(void *acc, void *data));
TreeList *TreeList_scan(TreeList *list, void *acc, void *(*f)(void *acc, void *data));

PartitionResult TreeList_partition(TreeList *list, int (*predicate)(void *));
TreeList *TreeList_filter(TreeList *list, int (*predicate)(void *));
unsigned TreeList_count(TreeList *list, int (*predicate)(void *));
int TreeList_any(TreeList *list, int (*predicate)(void *));
int TreeList_all(TreeList *list, int (*predicate)(void *));
void *TreeList_find(TreeList *list, int (*predicate)(void *));
void *TreeList_findLast(TreeList *list, int (*predicate)(void *));
int TreeList_findIndex(TreeList *list, int (*predicate)(void *));
void TreeList_replaceIf(TreeList *list, int (*predicate)(void *), void *data);


TreeList *TreeList_map(TreeList *list, void *(*f)(void *)) {
  if (!list) return NULL;
  TreeList *map = malloc(sizeof(TreeList));
  if (!map) return NULL;
  map->data = f(list->data);
  map->height = list->height;
  map->size = list->size;
  map->left = TreeList_map(list->left, f);
  map->right = TreeList_map(list->right, f);
  return map;
}

void TreeList_foreach(TreeList *list, void (*f)(void *)) {
  if (!list) return;
  TreeList_foreach(list->left, f);
  f(list->data);
  TreeList_foreach(list->right, f);
}

void TreeList_forEachReverse(TreeList *list, void (*f)(void *)) {
  if (!list) return;
  TreeList_forEachReverse(list->right, f);
  f(list->data);
  TreeList_forEachReverse(list->left, f);
}

void *TreeList_fold(TreeList *list, void *acc, void *(*f)(void *acc, void *data)) {
  if (!list) return acc;
  acc = TreeList_fold(list->left, acc, f);
  acc = f(acc, list->data);
  return TreeList_fold(list->right, acc, f);
}

TreeList *TreeList_scan(TreeList *list, void *acc, void *(*f)(void *acc, void *data)) {
  unsigned n = TreeList_size(list);
  void **results = malloc(sizeof(void *) * (n + 1));
  if (!results) return NULL;
  results[0] = acc;
  unsigned count = 1;
  TreeListIterator it = TreeList_begin(list);
  while (TreeList_hasNext(&it)) {
    void *data = TreeList_next(&it);
    acc = f(acc, data);
    results[count++] = acc;
  }
  TreeList *resultTree = TreeList_fromArray(results, n + 1);
  free(results);
  return resultTree;
}


PartitionResult TreeList_partition(TreeList *list, int (*predicate)(void *)) {
  if (!list) return (PartitionResult){NULL, NULL};
  unsigned n = TreeList_size(list);
  void **allData = TreeList_toArray(list);
  void **satData = malloc(sizeof(void *) * n);
  void **failData = malloc(sizeof(void *) * n);
  unsigned satCount = 0;
  unsigned failCount = 0;
  for (unsigned i = 0; i < n; i++)
    if (predicate(allData[i])) satData[satCount++] = allData[i];
    else failData[failCount++] = allData[i];
  PartitionResult result;
  result.satisfied = TreeList_fromArray(satData, satCount);
  result.failed = TreeList_fromArray(failData, failCount);
  free(allData);
  free(satData);
  free(failData);
  return result;
}

TreeList *TreeList_filter(TreeList *list, int (*predicate)(void *)) {
  PartitionResult result = TreeList_partition(list, predicate);
  TreeList *filtered = result.satisfied;
  TreeList_free(result.failed);
  return filtered;
}

unsigned TreeList_count(TreeList *list, int (*predicate)(void *)) {
  TreeList *filtered = TreeList_filter(list, predicate);
  unsigned count = TreeList_size(filtered);
  TreeList_free(filtered);
  return count;
}

int TreeList_any(TreeList *list, int (*predicate)(void *)) {
  return list && (TreeList_any(list->left, predicate) || predicate(list->data) || TreeList_any(list->right, predicate));
}

int TreeList_all(TreeList *list, int (*predicate)(void *)) {
  return !list || TreeList_all(list->left, predicate) && predicate(list->data) && TreeList_all(list->right, predicate);
}

void *TreeList_find(TreeList *list, int (*predicate)(void *)) {
  if (!list) return NULL;
  void *found = TreeList_find(list->left, predicate);
  if (found) return found;
  if (predicate(list->data)) return list->data;
  return TreeList_find(list->right, predicate);
}

void *TreeList_findLast(TreeList *list, int (*predicate)(void *)) {
  if (!list) return NULL;
  void *found = TreeList_findLast(list->right, predicate);
  if (found) return found;
  if (predicate(list->data)) return list->data;
  return TreeList_findLast(list->left, predicate);
}

int TreeList_findIndex(TreeList *list, int (*predicate)(void *)) {
  if (!list) return -1;
  int index = TreeList_findIndex(list->left, predicate);
  if (index != -1) return index;
  if (predicate(list->data)) return (int)TreeList_size(list->left);
  index = TreeList_findIndex(list->right, predicate);
  if (index != -1) return (int)TreeList_size(list->left) + 1 + index;
  return -1;
}

void TreeList_replaceIf(TreeList *list, int (*predicate)(void *), void *data) {
  if (!list) return NULL;
  if (predicate(list->data)) list->data = data;
  TreeList_replaceIf(list->left, predicate, data);
  TreeList_replaceIf(list->right, predicate, data);
}
