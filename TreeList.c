#include <stdio.h>
#include <stdlib.h>
#include "TreeList.h"

int TreeList_hasNext(TreeListIterator *it) { return it->top >= 0; }

TreeListIterator TreeList_begin(TreeList *list) {
  TreeListIterator it;
  it.top = -1;
  while (list) {
    it.stack[++it.top] = list;
    list = list->left;
  }
  return it;
}

void *TreeList_next(TreeListIterator *it) {
  if (it->top < 0) return NULL;
  TreeList *curr = it->stack[it->top--];
  void *data = curr->data;
  TreeList *node = curr->right;
  while (node) {
    it->stack[++it->top] = node;
    node = node->left;
  }
  return data;
}

TreeListIterator TreeList_reverseBegin(TreeList *list) {
    TreeListIterator it;
    it.top = -1;
    TreeList *curr = list;
    while (curr != NULL) {
        if (it.top < 63) it.stack[++it.top] = curr;
        curr = curr->right;
    }
    return it;
}

void *TreeList_reverseNext(TreeListIterator *it) {
  if (it->top < 0) return NULL;
  TreeList *node = it->stack[it->top--];
  void *data = node->data;
  TreeList *curr = node->left;
  while (curr != NULL) {
      if (it->top < 63) it->stack[++it->top] = curr;
      curr = curr->right;
  }
  return data;
}

static unsigned TreeList_height(TreeList *list) { return list ? list->height : 0; }

static int TreeList_balance(TreeList *list) { return list ? (int)TreeList_height(list->left) - (int)TreeList_height(list->right) : 0; }

static bool TreeList_isValidHeight(TreeList *list) {
  if (!list) return true;
  unsigned left = TreeList_height(list->left);
  unsigned right = TreeList_height(list->right);
  return list->height == 1 + (left >= right ? left : right) && TreeList_isValidHeight(list->left) && TreeList_isValidHeight(list->right);
}

static bool TreeList_isValidBalance(TreeList *list) {
  return !list ||
    -1 <= TreeList_balance(list) && TreeList_balance(list) <= 1 &&
    TreeList_isValidBalance(list->left) && TreeList_isValidBalance(list->right);
}

static bool TreeList_isValidSize(TreeList *list) {
  if (!list) return true;
  if (list->size != 1 + TreeList_size(list->left) + TreeList_size(list->right)) return false;
  return TreeList_isValidSize(list->left) && TreeList_isValidSize(list->right);
}

static void update(TreeList *list) {
  if (!list) return;
  unsigned hl = TreeList_height(list->left);
  unsigned hr = TreeList_height(list->right);
  list->height = 1 + (hl > hr ? hl : hr);
  list->size = 1 + TreeList_size(list->left) + TreeList_size(list->right);
}

static TreeList *setLeft(TreeList *list, TreeList *left) {
  if (!list) return NULL;
  list->left = left;
  update(list);
  return list;
}

static TreeList *setRight(TreeList *list, TreeList *right) {
  if (!list) return NULL;
  list->right = right;
  update(list);
  return list;
}

static TreeList *rotateRight(TreeList *list) {
  TreeList *ll = list->left;
  list->left = ll->right;
  ll->right = list;
  update(list);
  update(ll);
  return ll;
}

static TreeList *rotateLeft(TreeList *list) {
  TreeList *lr = list->right;
  list->right = lr->left;
  lr->left = list;
  update(list);
  update(lr);
  return lr;
}

static TreeList *rebalance(TreeList *list) {
  if (TreeList_balance(list) > 1) {
    if (TreeList_balance(list->left) < 0) list->left = rotateLeft(list->left);
    return rotateRight(list);
  }
  if (TreeList_balance(list) < -1) {
    if (TreeList_balance(list->right) > 0) list->right = rotateRight(list->right);
    return rotateLeft(list);
  }
  return list;
}

bool TreeList_isValid(TreeList *list) {
  return TreeList_isValidHeight(list) && TreeList_isValidBalance(list) && TreeList_isValidSize(list);
}

int TreeList_isEmpty(TreeList *list) {
  return !list;
}

unsigned TreeList_size(TreeList *list) { return list ? list->size : 0; }

TreeList *TreeList_empty() { return NULL; }

TreeList *TreeList_singleton(void *data) {
  TreeList *list = malloc(sizeof(TreeList));
  if (!list) return NULL;
  list->data = data;
  list->left = list->right = NULL;
  list->height = list->size = 1;
  return list;
}

void TreeList_free(TreeList *list) {
  if (!list) return;
  TreeList_free(list->left);
  TreeList_free(list->right);
  free(list);
}

TreeList *TreeList_clear(TreeList *list) {
  TreeList_free(list);
  return NULL;
}

void *TreeList_get(TreeList *list, unsigned i) {
  if (!list) return NULL;
  if (i < TreeList_size(list->left)) return TreeList_get(list->left, i);
  if (i > TreeList_size(list->left)) return TreeList_get(list->right, i - TreeList_size(list->left) - 1);
  return list->data;
}

void TreeList_set(TreeList *list, unsigned i, void *data) {
  if (!list) return;
  if (i < TreeList_size(list->left)) TreeList_set(list->left, i, data);
  else if (i > TreeList_size(list->left)) TreeList_set(list->right, i - TreeList_size(list->left) - 1, data);
  else list->data = data;
}

TreeList *TreeList_concat(TreeList *left, TreeList *right) {
  if (!left) return right;
  if (!right) return left;
  if (TreeList_height(left) > TreeList_height(right)) return rebalance(setRight(left, TreeList_concat(left->right, right)));
  return rebalance(setLeft(right, TreeList_concat(left, right->left)));
}

SplitResult TreeList_split(TreeList *list, unsigned i) {
  if (!list) return (SplitResult){NULL, NULL};
  if (i <= TreeList_size(list->left)) {
    SplitResult res = TreeList_split(list->left, i);
    list->left = res.right;
    update(list);
    res.right = rebalance(list);
    return res;
  } else {
    SplitResult res = TreeList_split(list->right, i - TreeList_size(list->left) - 1);
    list->right = res.left;
    update(list);
    res.left = rebalance(list);
    return res;
  }
}

TreeList *TreeList_insert(TreeList *list, unsigned i, void *data) {
  TreeList *single = TreeList_singleton(data);
  if (!single) return list;
  SplitResult parts = TreeList_split(list, i);
  TreeList *temp = TreeList_concat(parts.left, single);
  return TreeList_concat(temp, parts.right);
}

TreeList *TreeList_remove(TreeList *list, unsigned i) {
  if (!list || i >= TreeList_size(list)) return list;
  SplitResult parts1 = TreeList_split(list, i);
  SplitResult parts2 = TreeList_split(parts1.right, 1);
  free(parts2.left);
  return TreeList_concat(parts1.left, parts2.right);
}

TreeList *TreeList_insertList(TreeList *list, unsigned i, TreeList *other) {
  if (!other) return list;
  if (!list) return other;
  SplitResult parts = TreeList_split(list, i);
  TreeList *temp = TreeList_concat(parts.left, other);
  return TreeList_concat(temp, parts.right);
}

TreeList *TreeList_removeRange(TreeList *list, unsigned i, unsigned length) {
  if (!list || length == 0) return list;
  SplitResult parts1 = TreeList_split(list, i);
  SplitResult parts2 = TreeList_split(parts1.right, length);
  TreeList_free(parts2.left);
  return TreeList_concat(parts1.left, parts2.right);
}

TreeList *TreeList_push(TreeList *list, void *data) {
  return TreeList_concat(list, TreeList_singleton(data));
}

void *TreeList_peek(TreeList *list) {
  return TreeList_get(list, TreeList_size(list) - 1);
}

TreeList *TreeList_pop(TreeList *list) {
  return TreeList_remove(list, TreeList_size(list) - 1);
}

TreeList *TreeList_pushLeft(TreeList *list, void *data) {
  return TreeList_concat(TreeList_singleton(data), list);
}

void *TreeList_peekLeft(TreeList *list) {
  return TreeList_get(list, 0);
}

TreeList *TreeList_popLeft(TreeList *list) {
  return TreeList_remove(list, 0);
}

TreeList *TreeList_slice(TreeList *list, unsigned i, unsigned length) {
  if (!list || length == 0) {
    TreeList_free(list);
    return NULL;
  }
  SplitResult parts1 = TreeList_split(list, i);
  SplitResult parts2 = TreeList_split(parts1.right, length);
  TreeList_free(parts1.left);
  TreeList_free(parts2.right);
  return parts2.left;
}

TreeList *TreeList_copy(TreeList *list) {
  if (!list) return NULL;
  TreeList *copy = malloc(sizeof(TreeList));
  if (!copy) return NULL;
  copy->data = list->data;
  copy->height = list->height;
  copy->size = list->size;
  copy->left = TreeList_copy(list->left);
  copy->right = TreeList_copy(list->right);
  return copy;
}

TreeList *TreeList_reverse(TreeList *list) {
  if (!list) return NULL;
  TreeList *reverse = malloc(sizeof(TreeList));
  if (!reverse) return NULL;
  reverse->data = list->data;
  reverse->height = list->height;
  reverse->size = list->size;
  reverse->left = TreeList_reverse(list->right);
  reverse->right = TreeList_reverse(list->left);
  return reverse;
}

static TreeList *fromArrayRecursive(void **array, int start, int end) {
  if (start > end) return NULL;
  int mid = start + (end - start) / 2;
  TreeList *list = TreeList_singleton(array[mid]);
  list->left = fromArrayRecursive(array, start, mid - 1);
  list->right = fromArrayRecursive(array, mid + 1, end);
  update(list);
  return list;
}

TreeList *TreeList_fromArray(void **array, unsigned n) {
  return fromArrayRecursive(array, 0, (int)n - 1);
}

void **TreeList_toArray(TreeList *list) {
  if (!list) return NULL;
  unsigned size = TreeList_size(list);
  void **array = malloc(sizeof(void *) * size);
  if (!array) return NULL;
  TreeListIterator iterator = TreeList_begin(list);
  unsigned i = 0;
  while (TreeList_hasNext(&iterator)) array[i++] = TreeList_next(&iterator);
  return array;
}

TreeList *TreeList_rotate(TreeList *list, int k) {
  unsigned n = TreeList_size(list);
  if (n == 0) return list;
  unsigned splitIndex = ((k % n) + n) % n;
  if (splitIndex == 0) return list;
  SplitResult parts = TreeList_split(list, splitIndex);
  return TreeList_concat(parts.right, parts.left);
}

TreeList *TreeList_shuffle(TreeList *list) {
  unsigned n = TreeList_size(list);
  if (n <= 1) return TreeList_copy(list);
  void **array = TreeList_toArray(list);
  for (unsigned i = n - 1; i > 0; i--) {
    unsigned j = rand() % (i + 1);
    void *temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
  TreeList *shuffled = TreeList_fromArray(array, n);
  free(array);
  return shuffled;
}

TreeList *TreeList_compact(TreeList *list) {
  if (!list) return NULL;
  unsigned n = TreeList_size(list);
  void **array = TreeList_toArray(list);
  TreeList *compacted = TreeList_fromArray(array, n);
  free(array);
  return compacted;
}

TreeList *TreeList_zip(TreeList *list1, TreeList *list2) {
  TreeListIterator iterator1 = TreeList_begin(list1);
  TreeListIterator iterator2 = TreeList_begin(list2);
  TreeList *list3 = TreeList_empty();
  while (TreeList_hasNext(&iterator1) && TreeList_hasNext(&iterator2)) {
    Pair *pair = malloc(sizeof(Pair));
    if (!pair) {
      TreeList_free(list3);
      return NULL; 
    }
    pair->first = TreeList_next(&iterator1);
    pair->second = TreeList_next(&iterator2);
    TreeList *list4 = TreeList_push(list3, pair);
    if (!list4) {
      free(pair);
      TreeList_free(list3);
      return NULL;
    }
    list3 = list4;
  }
  return list3;
}

TreeList *TreeList_unzipLeft(TreeList *list) {
  if (!list) return NULL;
  TreeList *list2 = TreeList_unzipLeft(list->left);
  TreeList *list3 = malloc(sizeof(TreeList));
  TreeList *list4 = TreeList_unzipLeft(list->right);
  if (list->left && !list2 || !list3 || list->right && !list4) {
    TreeList_free(list2);
    free(list3);
    TreeList_free(list4);
    return NULL;
  }
  Pair *pair = list->data;
  list3->data = pair->first;
  list3->left = list2;
  list3->right = list4;
  list3->height = list->height;
  list3->size = list->size;
  return list3;
}

TreeList *TreeList_unzipRight(TreeList *list) {
  if (!list) return NULL;
  TreeList *list2 = TreeList_unzipLeft(list->left);
  TreeList *list3 = malloc(sizeof(TreeList));
  TreeList *list4 = TreeList_unzipLeft(list->right);
  if (list->left && !list2 || !list3 || list->right && !list4) {
    TreeList_free(list2);
    free(list3);
    TreeList_free(list4);
    return NULL;
  }
  Pair *pair = list->data;
  list3->data = pair->second;
  list3->left = list2;
  list3->right = list4;
  list3->height = list->height;
  list3->size = list->size;
  return list3;
}

static void *sum_step(void *acc, void *data) {
  double *a = (double *)acc;
  double *d = (double *)data;
  *a += *d;
  return acc;
}

double TreeList_sum(TreeList *list) {
  double total = 0;
  TreeList_fold(list, &total, sum_step);
  return total;
}

static void *prod_step(void *acc, void *data) {
  double *a = (double *)acc;
  double *d = (double *)data;
  *a *= *d;
  return acc;
}

double TreeList_product(TreeList *list) {
  double total = 1;
  TreeList_fold(list, &total, prod_step);
  return total;
}

double TreeList_average(TreeList *list) {
  unsigned n = TreeList_size(list);
  if (n == 0) return 0;
  return TreeList_sum(list) / n;
}


int TreeList_indexOf(TreeList *list, void *target, int (*compare)(const void *, const void *)) {
  TreeListIterator it = TreeList_begin(list);
  int index = 0;
  while (TreeList_hasNext(&it)) {
    void *current = TreeList_next(&it);
    if (compare(current, target) == 0) return index;
    index++;
  }
  return -1;
}

int TreeList_lastIndexOf(TreeList *list, void *target, int (*compare)(const void *, const void *)) {
  TreeListIterator it = TreeList_reverseBegin(list);
  int index = (int)list->size - 1;
  while (TreeList_hasNext(&it)) {
    void *data = TreeList_reverseNext(&it);
    if (compare(data, target) == 0) return index;
    index--;
  }
  return -1;
}

void TreeList_replace(TreeList *list, int (*compare)(const void *, const void *), void *target, void *replacement) {
  if (!list) return NULL;
  if (compare(list->data, target) == 0) list->data = replacement;
  TreeList *left = TreeList_replace(list->left, target, replacement, compare);
  TreeList *right = TreeList_replace(list->right, target, replacement, compare);
}

bool TreeList_isSorted(TreeList *list, int (*compare)(const void *, const void *)) {
  if (!list || list->size <= 1) return true;
  TreeListIterator it = TreeList_begin(list);
  void *prev = TreeList_next(&it);
  while (TreeList_hasNext(&it)) {
    void *curr = TreeList_next(&it);
    if (compare(prev, curr) > 0) return false;
    prev = curr;
  }
  return true;
}

TreeList *TreeList_sort(TreeList *list, int (*compare)(const void *, const void *)) {
  if (!list || list->size <= 1) return TreeList_copy(list);
  unsigned n = TreeList_size(list);
  void **array = TreeList_toArray(list);
  if (!array) return NULL;
  qsort(array, n, sizeof(void *), compare);
  TreeList *sortedTree = TreeList_fromArray(array, n);
  free(array);
  return sortedTree;
}

static void collectToVoidPtrArray(
  TreeList *node,
  void *target,
  int (*compare)(const void *, const void *),
  unsigned offset,
  unsigned *buffer,
  unsigned *count)
{
  if (!node) return;
  collectToVoidPtrArray(node->left, target, compare, offset, buffer, count);
  unsigned currentIdx = offset + TreeList_size(node->left);
  if (compare(node->data, target) == 0) buffer[(*count)++] = currentIdx;
  collectToVoidPtrArray(node->right, target, compare, currentIdx + 1, buffer, count);
}

TreeList *TreeList_elemIndices(TreeList *list, void *target, int (*compare)(const void *, const void *)) {
  unsigned n = TreeList_size(list);
  if (n == 0) return NULL;
  void **results = malloc(sizeof(void *) * n);
  unsigned count = 0;
  collectToVoidPtrArray(list, target, compare, 0, results, &count);
  TreeList *resultTree = TreeList_fromArray(results, count);
  free(results);
  return resultTree;
}

void *TreeList_min(TreeList *list, int (*compare)(void *, void *)) {
  if (!list) return NULL;
  void *minVal = list->data;
  void *leftMin = TreeList_min(list->left, compare);
  if (leftMin && compare(leftMin, minVal) < 0) minVal = leftMin;
  void *rightMin = TreeList_min(list->right, compare);
  if (rightMin && compare(rightMin, minVal) < 0) minVal = rightMin;
  return minVal;
}

void *TreeList_max(TreeList *list, int (*compare)(void *, void *)) {
  if (!list) return NULL;
  void *maxVal = list->data;
  void *leftMax = TreeList_max(list->left, compare);
  if (leftMax && compare(leftMax, maxVal) > 0) maxVal = leftMax;
  void *rightMax = TreeList_max(list->right, compare);
  if (rightMax && compare(rightMax, maxVal) > 0) maxVal = rightMax;
  return maxVal;
}

TreeList *TreeList_uniq(TreeList *list, int (*compare)(const void *, const void *)) {
  if (!list) return NULL;
  if (list->size <= 1) return TreeList_copy(list);
  TreeList *result = NULL;
  TreeListIterator it = TreeList_begin(list);
  void *prev = TreeList_next(&it);
  result = TreeList_concat(result, TreeList_singleton(prev));
  while (TreeList_hasNext(&it)) {
    void *curr = TreeList_next(&it);
    if (compare(prev, curr) != 0) {
      result = TreeList_concat(result, TreeList_singleton(curr));
      prev = curr;
    }
  }
  return result;
}


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
