#include <stdlib.h>
#include "TreeList.h"



static unsigned max(unsigned m, unsigned n);

static unsigned TreeList_height(TreeList *list);
static int TreeList_balance(TreeList *list);
static bool TreeList_isValidHeight(TreeList *list);
static bool TreeList_isValidBalance(TreeList *list);
static bool TreeList_isValidSize(TreeList *list);
static void TreeList_update(TreeList *list);
static TreeList *TreeList_rotateRight(TreeList *list);
static TreeList *TreeList_rotateLeft(TreeList *list);
static TreeList *TreeList_rebalance(TreeList *list);



static unsigned max(unsigned m, unsigned n) { return m >= n ? m : n; }

static void Array_swap(void **x, void **y) {
  void *u = *x;
  void *v = *y;
  *x = *u;
  *y = *v;
}

static void Array_shuffle(void **array, unsigned n) { for (unsigned i = 1; i < n; i++) Array_swap(array + i, array + rand() % (i + 1)); }

static unsigned TreeList_height(TreeList *list) { return list ? list->height : 0; }

static int TreeList_balance(TreeList *list) { return list ? (int)TreeList_height(list->left) - (int)TreeList_height(list->right) : 0; }

static bool TreeList_isValidHeight(TreeList *list) {
  return !list || list->height == 1 + max(TreeList_height(list->left), TreeList_height(list->right)) &&
    TreeList_isValidHeight(list->left) && TreeList_isValidHeight(list->right);
}

static bool TreeList_isValidBalance(TreeList *list) {
  return !list || -1 <= TreeList_balance(list) && TreeList_balance(list) <= 1 &&
    TreeList_isValidBalance(list->left) && TreeList_isValidBalance(list->right);
}

static bool TreeList_isValidSize(TreeList *list) {
  return !list || list->size == 1 + TreeList_size(list->left) + TreeList_size(list->right) &&
    TreeList_isValidSize(list->left) && TreeList_isValidSize(list->right);
}

static void TreeList_update(TreeList *list) {
  list->height = 1 + max(TreeList_height(list->left), TreeList_height(list->right));
  list->size = 1 + TreeList_size(list->left) + TreeList_size(list->right);
}

static TreeList *TreeList_rotateRight(TreeList *list) {
  TreeList *left = list->left;
  list->left = left->right;
  left->right = list;
  TreeList_update(list);
  TreeList_update(left);
  return left;
}

static TreeList *TreeList_rotateLeft(TreeList *list) {
  TreeList *right = list->right;
  list->right = right->left;
  right->left = list;
  TreeList_update(list);
  TreeList_update(right);
  return right;
}

static TreeList *TreeList_rebalance(TreeList *list) {
  if (TreeList_balance(list) > 1) {
    if (TreeList_balance(list->left) < 0) list->left = TreeList_rotateLeft(list->left);
    return TreeList_rotateRight(list);
  }
  if (TreeList_balance(list) < -1) {
    if (TreeList_balance(list->right) > 0) list->right = TreeList_rotateRight(list->right);
    return TreeList_rotateLeft(list);
  }
  return list;
}



TreeListIterator TreeList_begin(TreeList *list) {
  TreeListIterator iterator;
  iterator.top = -1;
  while (list) {
    iterator.stack[++iterator.top] = list;
    list = list->left;
  }
  return iterator;
}

TreeListIterator TreeList_reverseBegin(TreeList *list) {
  TreeListIterator iterator;
  iterator.top = -1;
  while (list) {
    iterator.stack[++iterator.top] = list;
    list = list->right;
  }
  return iterator;
}

int TreeListIterator_hasNext(TreeListIterator *iterator) { return iterator->top >= 0; }

void *TreeListIterator_get(TreeListIterator *iterator) {
  if (iterator->top < 0) return NULL;
  return iterator->stack[iterator->top]->data;
}

void TreeListIterator_next(TreeListIterator *iterator) {
  if (iterator->top < 0) return;
  TreeList *list = iterator->stack[iterator->top--]->right;
  while (list) {
    iterator->stack[++iterator->top] = list;
    list = list->left;
  }
}

void TreeListIterator_reverseNext(TreeListIterator *iterator) {
  if (iterator->top < 0) return;
  TreeList *list = iterator->stack[iterator->top--]->left;
  while (list) {
    iterator->stack[++iterator->top] = list;
    list = list->right;
  }
}



bool TreeList_isValid(TreeList *list) { return TreeList_isValidHeight(list) && TreeList_isValidBalance(list) && TreeList_isValidSize(list); }

bool TreeList_isEmpty(TreeList *list) { return !list; }

unsigned TreeList_size(TreeList *list) { return list ? list->size : 0; }

TreeList *TreeList_empty() { return NULL; }

TreeList *TreeList_single(void *data) {
  TreeList *list = malloc(sizeof(TreeList));
  list->data = data;
  list->left = list->right = NULL;
  list->height = list->size = 1;
  return list;
}

TreeList *fromArray(void **array, unsigned n) {
  if (n == 0) return NULL;
  TreeList *list = malloc(sizeof(TreeList));
  list->data = array[n / 2];
  list->left = fromArray(array, n / 2);
  list->right = fromArray(array + n / 2 + 1, n - n / 2 - 1);
  TreeList_update(list);
  return list;
}

void **TreeList_toArray(TreeList *list) {
  if (!list) return NULL;
  void **array = malloc(sizeof(void *) * list->size);
  TreeListIterator iterator = TreeList_begin(list);
  unsigned i = 0;
  while (TreeListIterator_hasNext(&iterator)) {
    array[i++] = TreeListIterator_get(&iterator);
    TreeListIterator_next(&iterator);
  }
  return array;
}

TreeList *TreeList_copy(TreeList *list) {
  if (!list) return NULL;
  TreeList *copy = malloc(sizeof(TreeList));
  copy->data = list->data;
  copy->height = list->height;
  copy->size = list->size;
  copy->left = TreeList_copy(list->left);
  copy->right = TreeList_copy(list->right);
  return copy;
}

void TreeList_free(TreeList *list) {
  if (!list) return;
  TreeList_free(list->left);
  TreeList_free(list->right);
  free(list);
}

void TreeList_reverse(TreeList *list) {
  if (!list) return;
  TreeList_reverse(list->left);
  TreeList_reverse(list->right);
  TreeList *left = list->left;
  TreeList *right = list->right;
  list->left = right;
  list->right = left;
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
  if (TreeList_height(left) > TreeList_height(right)) {
    left->right = TreeList_concat(left->right, right);
    TreeList_update(left);
    return TreeList_rebalance(left);
  } else {
    right->left = TreeList_concat(left, right->left);
    TreeList_update(right);
    return TreeList_rebalance(right);
  }
}

void TreeList_split(TreeList *list, unsigned i, TreeList **left, TreeList **right) {
  if (!list) *left = *right = NULL;
  if (i <= TreeList_size(list->left)) {
    TreeList_split(list->left, i, left, right);
    list->left = *right;
    TreeList_update(list);
    *right = TreeList_rebalance(list);
  } else {
    TreeList_split(list->right, i - TreeList_size(list->left) - 1, left, right);
    list->right = *left;
    TreeList_update(list);
    *left = TreeList_rebalance(list);
  }
}



TreeList *TreeList_clear(TreeList *list) {
  TreeList_free(list);
  return TreeList_empty();
}

TreeList *TreeList_replicate(TreeList *list, unsigned n) {
  if (n == 0) return TreeList_empty();
  TreeList *half = TreeList_replicate(list, n / 2);
  TreeList *combined = TreeList_concat(half, TreeList_copy(half));
  if (n % 2 == 1) combined = TreeList_concat(combined, TreeList_copy(list));
  return combined;
}

TreeList *TreeList_insertList(TreeList *list, unsigned i, TreeList *other) {
  TreeList *left, *right;
  TreeList_split(list, i, &left, &right);
  left = TreeList_concat(left, other);
  return TreeList_concat(left, right);
}

TreeList *TreeList_removeRange(TreeList *list, unsigned i, unsigned length) {
  TreeList *left, *middle, *right;
  TreeList_split(list, i, &left, &right);
  TreeList_split(right, length, &middle, &right);
  TreeList_free(middle);
  return TreeList_concat(left, right);
}

TreeList *TreeList_slice(TreeList *list, unsigned i, unsigned length) {
  TreeList *left, *middle, *right;
  TreeList_split(list, i, &left, &right);
  TreeList_split(right, length, &middle, &right);
  TreeList_free(left);
  TreeList_free(right);
  return middle;
}

TreeList *TreeList_rotate(TreeList *list, int i) {
  unsigned n = TreeList_size(list);
  if (n == 0) return list;
  TreeList *left, *right;
  TreeList_split(list, ((i % n) + n) % n, &left, &right);
  return TreeList_concat(right, left);
}

TreeList *TreeList_compact(TreeList *list) {
  unsigned n = TreeList_size(list);
  void **array = TreeList_toArray(list);
  TreeList *compact = TreeList_fromArray(array, n);
  free(array);
  return compact;
}

TreeList *TreeList_shuffle(TreeList *list) {
  unsigned n = TreeList_size(list);
  void **array = TreeList_toArray(list);
  Array_shuffle(array, n);
  TreeList *shuffle = TreeList_fromArray(array, n);
  free(array);
  return shuffle;
}



TreeList *TreeList_repeat(void *data, unsigned n) {
  TreeList *single = TreeList_single(data);
  TreeList *replicate = TreeList_replicate(single, n);
  TreeList_free(single);
  return replicate;
}

TreeList *TreeList_insert(TreeList *list, unsigned i, void *data) { return TreeList_insertList(list, i, TreeList_single(data)); }

TreeList *TreeList_remove(TreeList *list, unsigned i) { return TreeList_removeRange(list, i, 1); }

TreeList *TreeList_push(TreeList *list, void *data) { return TreeList_concat(list, TreeList_single(data)); }

void *TreeList_peek(TreeList *list) { return TreeList_get(list, TreeList_size(list) - 1); }

TreeList *TreeList_pop(TreeList *list) { return TreeList_removeRange(list, TreeList_size(list) - 1, 1); }

TreeList *TreeList_pushLeft(TreeList *list, void *data) { return TreeList_concat(TreeList_single(data), list); }

void *TreeList_peekLeft(TreeList *list) { return TreeList_get(list, 0); }

TreeList *TreeList_popLeft(TreeList *list) { return TreeList_removeRange(list, 0, 1); }



TreeList *TreeList_zip(TreeList *list1, TreeList *list2) {
  TreeListIterator iterator1 = TreeList_begin(list1);
  TreeListIterator iterator2 = TreeList_begin(list2);
  TreeList *list3 = TreeList_empty();
  while (TreeListIterator_hasNext(&iterator1) && TreeListIterator_hasNext(&iterator2)) {
    Pair *pair = malloc(sizeof(Pair));
    if (!pair) {
      TreeList_free(list3);
      return NULL;
    }
    pair->first = TreeListIterator_get(&iterator1);
    pair->second = TreeListIterator_get(&iterator2);
    TreeListIterator_next(&iterator1);
    TreeListIterator_next(&iterator2);
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
  TreeList *list2 = TreeList_unzipRight(list->left);
  TreeList *list3 = malloc(sizeof(TreeList));
  TreeList *list4 = TreeList_unzipRight(list->right);
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

double TreeList_sum(TreeList *list) {
  if (!list) return 0;
  return *(double *)(list->data) + TreeList_sum(list->left) + TreeList_sum(list->right);
}

double TreeList_product(TreeList *list) {
  if (!list) return 1;
  return *(double *)(list->data) * TreeList_product(list->left) * TreeList_product(list->right);
}

double TreeList_average(TreeList *list) {
  if (TreeList_isEmpty(list)) return 0;
  return TreeList_sum(list) / TreeList_size(list);
}

int TreeList_indexOf(TreeList *list, void *target, int (*compare)(const void *, const void *)) {
  TreeListIterator iterator = TreeList_begin(list);
  int index = 0;
  while (TreeListIterator_hasNext(&iterator)) {
    void *current = TreeListIterator_get(&iterator);
    TreeListIterator_next(&iterator);
    if (compare(current, target) == 0) return index;
    index++;
  }
  return -1;
}

int TreeList_lastIndexOf(TreeList *list, void *target, int (*compare)(const void *, const void *)) {
  TreeListIterator iterator = TreeList_reverseBegin(list);
  int index = (int)list->size - 1;
  while (TreeListIterator_hasNext(&iterator)) {
    void *data = TreeListIterator_get(&iterator);
    TreeListIterator_reverseNext(&iterator);
    if (compare(data, target) == 0) return index;
    index--;
  }
  return -1;
}

void TreeList_replace(TreeList *list, int (*compare)(const void *, const void *), void *target, void *replacement) {
  if (!list) return;
  if (compare(list->data, target) == 0) list->data = replacement;
  TreeList_replace(list->left, compare, target, replacement);
  TreeList_replace(list->right, compare, target, replacement);
}

bool TreeList_isSorted(TreeList *list, int (*compare)(const void *, const void *)) {
  if (!list || list->size <= 1) return true;
  TreeListIterator iterator = TreeList_begin(list);
  void *prev = TreeListIterator_get(&iterator);
  TreeListIterator_next(&iterator);
  while (TreeListIterator_hasNext(&iterator)) {
    void *curr = TreeListIterator_get(&iterator);
    TreeListIterator_next(&iterator);
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

void *TreeList_min(TreeList *list, int (*compare)(const void *, const void *)) {
  if (!list) return NULL;
  void *minVal = list->data;
  void *leftMin = TreeList_min(list->left, compare);
  if (leftMin && compare(leftMin, minVal) < 0) minVal = leftMin;
  void *rightMin = TreeList_min(list->right, compare);
  if (rightMin && compare(rightMin, minVal) < 0) minVal = rightMin;
  return minVal;
}

void *TreeList_max(TreeList *list, int (*compare)(const void *, const void *)) {
  if (!list) return NULL;
  void *maxVal = list->data;
  void *leftMax = TreeList_max(list->left, compare);
  if (leftMax && compare(leftMax, maxVal) > 0) maxVal = leftMax;
  void *rightMax = TreeList_max(list->right, compare);
  if (rightMax && compare(rightMax, maxVal) > 0) maxVal = rightMax;
  return maxVal;
}

TreeList *TreeList_unique(TreeList *list, int (*compare)(const void *, const void *)) {
  if (TreeList_size(list) <= 1) return TreeList_copy(list);
  TreeList *sorted = TreeList_sort(list, compare);
  TreeList *unique = TreeList_empty();
  TreeListIterator iterator = TreeList_begin(sorted);
  void *previous = TreeListIterator_get(&iterator);
  TreeListIterator_next(&iterator);
  unique = TreeList_push(unique, previous);
  while (TreeListIterator_hasNext(&iterator)) {
    void *current = TreeListIterator_get(&iterator);
    TreeListIterator_next(&iterator);
    if (compare(previous, current) != 0) {
      unique = TreeList_push(unique, current);
      previous = current;
    }
  }
  TreeList_free(sorted);
  return unique;
}
