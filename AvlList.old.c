typedef struct {
  AvlList *satisfied;
  AvlList *failed;
} PartitionResult;

PartitionResult AvlList_partition(AvlList *list, int (*predicate)(void *));
AvlList *AvlList_filter(AvlList *list, int (*predicate)(void *));
unsigned AvlList_count(AvlList *list, int (*predicate)(void *));
int AvlList_any(AvlList *list, int (*predicate)(void *));
int AvlList_all(AvlList *list, int (*predicate)(void *));
void *AvlList_find(AvlList *list, int (*predicate)(void *));
void *AvlList_findLast(AvlList *list, int (*predicate)(void *));
int AvlList_findIndex(AvlList *list, int (*predicate)(void *));
void AvlList_replaceIf(AvlList *list, int (*predicate)(void *), void *data);



PartitionResult AvlList_partition(AvlList *list, int (*predicate)(void *)) {
  if (!list) return (PartitionResult){NULL, NULL};
  unsigned n = AvlList_size(list);
  void **allData = AvlList_toArray(list);
  void **satData = malloc(sizeof(void *) * n);
  void **failData = malloc(sizeof(void *) * n);
  unsigned satCount = 0;
  unsigned failCount = 0;
  for (unsigned i = 0; i < n; i++)
    if (predicate(allData[i])) satData[satCount++] = allData[i];
    else failData[failCount++] = allData[i];
  PartitionResult result;
  result.satisfied = AvlList_fromArray(satData, satCount);
  result.failed = AvlList_fromArray(failData, failCount);
  free(allData);
  free(satData);
  free(failData);
  return result;
}

AvlList *AvlList_filter(AvlList *list, int (*predicate)(void *)) {
  PartitionResult result = AvlList_partition(list, predicate);
  AvlList *filtered = result.satisfied;
  AvlList_free(result.failed);
  return filtered;
}

unsigned AvlList_count(AvlList *list, int (*predicate)(void *)) {
  AvlList *filtered = AvlList_filter(list, predicate);
  unsigned count = AvlList_size(filtered);
  AvlList_free(filtered);
  return count;
}

int AvlList_any(AvlList *list, int (*predicate)(void *)) {
  return list && (AvlList_any(list->left, predicate) || predicate(list->data) || AvlList_any(list->right, predicate));
}

int AvlList_all(AvlList *list, int (*predicate)(void *)) {
  return !list || AvlList_all(list->left, predicate) && predicate(list->data) && AvlList_all(list->right, predicate);
}

void *AvlList_find(AvlList *list, int (*predicate)(void *)) {
  if (!list) return NULL;
  void *found = AvlList_find(list->left, predicate);
  if (found) return found;
  if (predicate(list->data)) return list->data;
  return AvlList_find(list->right, predicate);
}

void *AvlList_findLast(AvlList *list, int (*predicate)(void *)) {
  if (!list) return NULL;
  void *found = AvlList_findLast(list->right, predicate);
  if (found) return found;
  if (predicate(list->data)) return list->data;
  return AvlList_findLast(list->left, predicate);
}

int AvlList_findIndex(AvlList *list, int (*predicate)(void *)) {
  if (!list) return -1;
  int index = AvlList_findIndex(list->left, predicate);
  if (index != -1) return index;
  if (predicate(list->data)) return (int)AvlList_size(list->left);
  index = AvlList_findIndex(list->right, predicate);
  if (index != -1) return (int)AvlList_size(list->left) + 1 + index;
  return -1;
}

void AvlList_replaceIf(AvlList *list, int (*predicate)(void *), void *data) {
  if (!list) return NULL;
  if (predicate(list->data)) list->data = data;
  AvlList_replaceIf(list->left, predicate, data);
  AvlList_replaceIf(list->right, predicate, data);
}
