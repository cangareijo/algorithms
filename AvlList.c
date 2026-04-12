#include <stdlib.h>
#include "AvlList.h"



static unsigned unsigned_min(unsigned m, unsigned n);
static unsigned unsigned_max(unsigned m, unsigned n);

static void Array_swap(void **x, void **y);
static void Array_shuffle(void **array, unsigned n);
static unsigned Array_filter(void **array, unsigned n, bool (*predicate)(void *));
static unsigned Array_filterFail(void **array, unsigned n, bool (*predicate)(void *));

static unsigned AvlList_height(const AvlList *list);
static int AvlList_balance(const AvlList *list);
static bool AvlList_isValidHeight(const AvlList *list);
static bool AvlList_isValidBalance(const AvlList *list);
static bool AvlList_isValidSize(const AvlList *list);
static void AvlList_update(AvlList *list);
static AvlList *AvlList_rotateRight(AvlList *list);
static AvlList *AvlList_rotateLeft(AvlList *list);
static AvlList *AvlList_rebalance(AvlList *list);



static unsigned unsigned_min(unsigned m, unsigned n) { return m <= n ? m : n; }

static unsigned unsigned_max(unsigned m, unsigned n) { return m >= n ? m : n; }



static void Array_swap(void **x, void **y) {
  void *u = *x;
  void *v = *y;
  *x = u;
  *y = v;
}

static void Array_shuffle(void **array, unsigned n) { for (unsigned i = 1; i < n; i++) Array_swap(array + i, array + rand() % (i + 1)); }

static unsigned Array_filter(void **array, unsigned n, bool (*predicate)(void *)) {
  unsigned count = 0;
  for (unsigned i = 0; i < n; i++) if (predicate(array[i])) array[count++] = array[i];
  return count;
}

static unsigned Array_filterFail(void **array, unsigned n, bool (*predicate)(void *)) {
  unsigned count = 0;
  for (unsigned i = 0; i < n; i++) if (!predicate(array[i])) array[count++] = array[i];
  return count;
}



static unsigned AvlList_height(const AvlList *list) { return list ? list->height : 0; }

static int AvlList_balance(const AvlList *list) { return list ? (int)AvlList_height(list->left) - (int)AvlList_height(list->right) : 0; }

static bool AvlList_isValidHeight(const AvlList *list) {
  return !list || list->height == 1 + unsigned_max(AvlList_height(list->left), AvlList_height(list->right)) &&
    AvlList_isValidHeight(list->left) && AvlList_isValidHeight(list->right);
}

static bool AvlList_isValidBalance(const AvlList *list) {
  return !list || -1 <= AvlList_balance(list) && AvlList_balance(list) <= 1 &&
    AvlList_isValidBalance(list->left) && AvlList_isValidBalance(list->right);
}

static bool AvlList_isValidSize(const AvlList *list) {
  return !list || list->size == 1 + AvlList_size(list->left) + AvlList_size(list->right) &&
    AvlList_isValidSize(list->left) && AvlList_isValidSize(list->right);
}

static void AvlList_update(AvlList *list) {
  list->height = 1 + unsigned_max(AvlList_height(list->left), AvlList_height(list->right));
  list->size = 1 + AvlList_size(list->left) + AvlList_size(list->right);
}

static AvlList *AvlList_rotateRight(AvlList *list) {
  AvlList *left = list->left;
  list->left = left->right;
  left->right = list;
  AvlList_update(list);
  AvlList_update(left);
  return left;
}

static AvlList *AvlList_rotateLeft(AvlList *list) {
  AvlList *right = list->right;
  list->right = right->left;
  right->left = list;
  AvlList_update(list);
  AvlList_update(right);
  return right;
}

static AvlList *AvlList_rebalance(AvlList *list) {
  AvlList_update(list);
  if (AvlList_balance(list) > 1) {
    if (AvlList_balance(list->left) < 0) list->left = AvlList_rotateLeft(list->left);
    return AvlList_rotateRight(list);
  }
  if (AvlList_balance(list) < -1) {
    if (AvlList_balance(list->right) > 0) list->right = AvlList_rotateRight(list->right);
    return AvlList_rotateLeft(list);
  }
  return list;
}



AvlListIterator AvlList_begin(AvlList *list) {
  AvlListIterator iterator;
  iterator.top = -1;
  while (list) {
    iterator.stack[++iterator.top] = list;
    list = list->left;
  }
  return iterator;
}

AvlListIterator AvlList_reverseBegin(AvlList *list) {
  AvlListIterator iterator;
  iterator.top = -1;
  while (list) {
    iterator.stack[++iterator.top] = list;
    list = list->right;
  }
  return iterator;
}

int AvlListIterator_hasNext(AvlListIterator *iterator) { return iterator->top >= 0; }

void *AvlListIterator_get(AvlListIterator *iterator) {
  if (iterator->top < 0) return NULL;
  return iterator->stack[iterator->top]->data;
}

void AvlListIterator_next(AvlListIterator *iterator) {
  if (iterator->top < 0) return;
  AvlList *list = iterator->stack[iterator->top--]->right;
  while (list) {
    iterator->stack[++iterator->top] = list;
    list = list->left;
  }
}

void AvlListIterator_reverseNext(AvlListIterator *iterator) {
  if (iterator->top < 0) return;
  AvlList *list = iterator->stack[iterator->top--]->left;
  while (list) {
    iterator->stack[++iterator->top] = list;
    list = list->right;
  }
}



bool AvlList_isValid(const AvlList *list) { return AvlList_isValidHeight(list) && AvlList_isValidBalance(list) && AvlList_isValidSize(list); }

bool AvlList_isEmpty(const AvlList *list) { return !list; }

unsigned AvlList_size(const AvlList *list) { return list ? list->size : 0; }

AvlList *AvlList_empty() { return NULL; }

AvlList *AvlList_single(void *data) {
  AvlList *list = malloc(sizeof(AvlList));
  list->data = data;
  list->left = list->right = NULL;
  list->height = list->size = 1;
  return list;
}

AvlList *AvlList_fromArray(void **array, unsigned n) {
  if (n == 0) return NULL;
  AvlList *list = malloc(sizeof(AvlList));
  list->data = array[n / 2];
  list->left = AvlList_fromArray(array, n / 2);
  list->right = AvlList_fromArray(array + n / 2 + 1, n - n / 2 - 1);
  AvlList_update(list);
  return list;
}

void **AvlList_toArray(AvlList *list) {
  if (!list) return NULL;
  void **array = malloc(sizeof(void *) * list->size);
  AvlListIterator iterator = AvlList_begin(list);
  unsigned i = 0;
  while (AvlListIterator_hasNext(&iterator)) {
    array[i++] = AvlListIterator_get(&iterator);
    AvlListIterator_next(&iterator);
  }
  return array;
}

AvlList *AvlList_copy(AvlList *list) {
  if (!list) return NULL;
  AvlList *copy = malloc(sizeof(AvlList));
  copy->data = list->data;
  copy->height = list->height;
  copy->size = list->size;
  copy->left = AvlList_copy(list->left);
  copy->right = AvlList_copy(list->right);
  return copy;
}

void AvlList_free(AvlList *list) {
  if (!list) return;
  AvlList_free(list->left);
  AvlList_free(list->right);
  free(list);
}

void AvlList_reverse(AvlList *list) {
  if (!list) return;
  AvlList_reverse(list->left);
  AvlList_reverse(list->right);
  AvlList *left = list->left;
  AvlList *right = list->right;
  list->left = right;
  list->right = left;
}

void *AvlList_get(AvlList *list, unsigned i) {
  if (!list) return NULL;
  if (i < AvlList_size(list->left)) return AvlList_get(list->left, i);
  if (i > AvlList_size(list->left)) return AvlList_get(list->right, i - AvlList_size(list->left) - 1);
  return list->data;
}

void AvlList_set(AvlList *list, unsigned i, void *data) {
  if (!list) return;
  if (i < AvlList_size(list->left)) AvlList_set(list->left, i, data);
  else if (i > AvlList_size(list->left)) AvlList_set(list->right, i - AvlList_size(list->left) - 1, data);
  else list->data = data;
}

AvlList *AvlList_concat(AvlList *left, AvlList *right) {
  if (!left) return right;
  if (!right) return left;
  if (AvlList_height(left) > AvlList_height(right)) {
    left->right = AvlList_concat(left->right, right);
    return AvlList_rebalance(left);
  } else {
    right->left = AvlList_concat(left, right->left);
    return AvlList_rebalance(right);
  }
}

void AvlList_split(AvlList *list, unsigned i, AvlList **left, AvlList **right) {
  if (!list) *left = *right = NULL;
  if (i <= AvlList_size(list->left)) {
    AvlList_split(list->left, i, left, right);
    list->left = *right;
    *right = AvlList_rebalance(list);
  } else {
    AvlList_split(list->right, i - AvlList_size(list->left) - 1, left, right);
    list->right = *left;
    *left = AvlList_rebalance(list);
  }
}



AvlList *AvlList_clear(AvlList *list) {
  AvlList_free(list);
  return AvlList_empty();
}

AvlList *AvlList_replicate(AvlList *list, unsigned n) {
  if (n == 0) return AvlList_empty();
  AvlList *half = AvlList_replicate(list, n / 2);
  AvlList *combined = AvlList_concat(half, AvlList_copy(half));
  if (n % 2 == 1) combined = AvlList_concat(combined, AvlList_copy(list));
  return combined;
}

AvlList *AvlList_insertList(AvlList *list, unsigned i, AvlList *other) {
  AvlList *left, *right;
  AvlList_split(list, i, &left, &right);
  left = AvlList_concat(left, other);
  return AvlList_concat(left, right);
}

AvlList *AvlList_removeRange(AvlList *list, unsigned i, unsigned length) {
  AvlList *left, *middle, *right;
  AvlList_split(list, i, &left, &right);
  AvlList_split(right, length, &middle, &right);
  AvlList_free(middle);
  return AvlList_concat(left, right);
}

AvlList *AvlList_slice(AvlList *list, unsigned i, unsigned length) {
  AvlList *left, *middle, *right;
  AvlList_split(list, i, &left, &right);
  AvlList_split(right, length, &middle, &right);
  AvlList_free(left);
  AvlList_free(right);
  return middle;
}

AvlList *AvlList_rotate(AvlList *list, int i) {
  unsigned n = AvlList_size(list);
  if (n == 0) return list;
  AvlList *left, *right;
  AvlList_split(list, ((i % n) + n) % n, &left, &right);
  return AvlList_concat(right, left);
}

AvlList *AvlList_compact(AvlList *list) {
  unsigned n = AvlList_size(list);
  void **array = AvlList_toArray(list);
  AvlList *compact = AvlList_fromArray(array, n);
  free(array);
  return compact;
}

AvlList *AvlList_shuffle(AvlList *list) {
  unsigned n = AvlList_size(list);
  void **array = AvlList_toArray(list);
  Array_shuffle(array, n);
  AvlList *shuffle = AvlList_fromArray(array, n);
  free(array);
  return shuffle;
}

AvlList *AvlList_zip(AvlList *list1, AvlList *list2) {
  unsigned n = unsigned_min(AvlList_size(list1), AvlList_size(list2));
  void **array1 = AvlList_toArray(list1);
  void **array2 = AvlList_toArray(list2);
  void ***array3 = malloc(sizeof(void **) * n);
  for (unsigned i = 0; i < n; i++) {
      array3[i] = malloc(sizeof(void *) * 2);
      array3[i][0] = array1[i];
      array3[i][1] = array2[i];
  }
  AvlList *list3 = AvlList_fromArray((void **)array3, n);
  free(array1);
  free(array2);
  free(array3);
  return list3;
}

AvlList *AvlList_unzipLeft(AvlList *list) {
  if (!list) return NULL;
  void **pair = list->data;
  AvlList *unzip = malloc(sizeof(AvlList));
  unzip->data = pair[0];
  unzip->left = AvlList_unzipLeft(list->left);
  unzip->right = AvlList_unzipLeft(list->right);
  unzip->height = list->height;
  unzip->size = list->size;
  return unzip;
}

AvlList *AvlList_unzipRight(AvlList *list) {
  if (!list) return NULL;
  void **pair = list->data;
  AvlList *unzip = malloc(sizeof(AvlList));
  unzip->data = pair[1];
  unzip->left = AvlList_unzipRight(list->left);
  unzip->right = AvlList_unzipRight(list->right);
  unzip->height = list->height;
  unzip->size = list->size;
  return unzip;
}



AvlList *AvlList_repeat(void *data, unsigned n) {
  AvlList *single = AvlList_single(data);
  AvlList *replicate = AvlList_replicate(single, n);
  AvlList_free(single);
  return replicate;
}

AvlList *AvlList_insert(AvlList *list, unsigned i, void *data) { return AvlList_insertList(list, i, AvlList_single(data)); }

AvlList *AvlList_remove(AvlList *list, unsigned i) { return AvlList_removeRange(list, i, 1); }

AvlList *AvlList_push(AvlList *list, void *data) { return AvlList_concat(list, AvlList_single(data)); }

void *AvlList_peek(AvlList *list) { return AvlList_get(list, AvlList_size(list) - 1); }

AvlList *AvlList_pop(AvlList *list) { return AvlList_removeRange(list, AvlList_size(list) - 1, 1); }

AvlList *AvlList_pushLeft(AvlList *list, void *data) { return AvlList_concat(AvlList_single(data), list); }

void *AvlList_peekLeft(AvlList *list) { return AvlList_get(list, 0); }

AvlList *AvlList_popLeft(AvlList *list) { return AvlList_removeRange(list, 0, 1); }



double AvlList_sum(const AvlList *list) {
  if (!list) return 0;
  return *(double *)(list->data) + AvlList_sum(list->left) + AvlList_sum(list->right);
}

double AvlList_product(const AvlList *list) {
  if (!list) return 1;
  return *(double *)(list->data) * AvlList_product(list->left) * AvlList_product(list->right);
}

double AvlList_average(const AvlList *list) {
  if (AvlList_isEmpty(list)) return 0;
  return AvlList_sum(list) / AvlList_size(list);
}



int AvlList_indexOf(AvlList *list, void *target, int (*compare)(const void *, const void *)) {
  if (!list) return -1;
  int i = AvlList_indexOf(list->left, target, compare);
  if (i != -1) return i;
  if (compare(list->data, target) == 0) return AvlList_size(list->left);
  i = AvlList_indexOf(list->right, target, compare);
  if (i != -1) return AvlList_size(list->left) + 1 + i;
  return -1;
}

int AvlList_lastIndexOf(AvlList *list, void *target, int (*compare)(const void *, const void *)) {
  if (!list) return -1;
  unsigned left_size = list->left ? list->left->size : 0;
  int i = AvlList_lastIndexOf(list->right, target, compare);
  if (i != -1) return AvlList_size(list->left) + 1 + i;
  if (compare(list->data, target) == 0) return AvlList_size(list->left);
  return AvlList_lastIndexOf(list->left, target, compare);
}

void AvlList_replace(AvlList *list, int (*compare)(const void *, const void *), void *target, void *replacement) {
  if (!list) return;
  if (compare(list->data, target) == 0) list->data = replacement;
  AvlList_replace(list->left, compare, target, replacement);
  AvlList_replace(list->right, compare, target, replacement);
}

bool AvlList_isSorted(AvlList *list, int (*compare)(const void *, const void *)) {
  if (AvlList_size(list) <= 1) return true;
  AvlListIterator iterator = AvlList_begin(list);
  void *previous = AvlListIterator_get(&iterator);
  AvlListIterator_next(&iterator);
  while (AvlListIterator_hasNext(&iterator)) {
    void *current = AvlListIterator_get(&iterator);
    AvlListIterator_next(&iterator);
    if (compare(previous, current) > 0) return false;
    previous = current;
  }
  return true;
}

AvlList *AvlList_sort(AvlList *list, int (*compare)(const void *, const void *)) {
  void **array = AvlList_toArray(list);
  qsort(array, AvlList_size(list), sizeof(void *), compare);
  AvlList *sorted = AvlList_fromArray(array, AvlList_size(list));
  free(array);
  return sorted;
}

static void AvlList_indicesArray(
  AvlList *list,
  void *target,
  int (*compare)(const void *, const void *),
  unsigned offset,
  unsigned **indices,
  unsigned *count)
{
  if (!list) return;
  AvlList_indicesArray(list->left, target, compare, offset, indices, count);
  if (compare(list->data, target) == 0) {
    indices[*count] = malloc(sizeof(unsigned));
    *indices[(*count)++] = offset + AvlList_size(list->left);
  }
  AvlList_indicesArray(list->right, target, compare, offset + AvlList_size(list->left) + 1, indices, count);
}

AvlList *AvlList_indices(AvlList *list, void *target, int (*compare)(const void *, const void *)) {
  unsigned **array = malloc(sizeof(unsigned *) * AvlList_size(list));
  unsigned count = 0;
  AvlList_indicesArray(list, target, compare, 0, array, &count);
  AvlList *indices = AvlList_fromArray((void **)array, count);
  free(array);
  return indices;
}

void *AvlList_min(AvlList *list, int (*compare)(const void *, const void *)) {
  if (!list) return NULL;
  void *minimum = list->data;
  void *left = AvlList_min(list->left, compare);
  if (left && compare(left, minimum) < 0) minimum = left;
  void *right = AvlList_min(list->right, compare);
  if (right && compare(right, minimum) < 0) minimum = right;
  return minimum;
}

void *AvlList_max(AvlList *list, int (*compare)(const void *, const void *)) {
  if (!list) return NULL;
  void *maximum = list->data;
  void *left = AvlList_max(list->left, compare);
  if (left && compare(left, maximum) > 0) maximum = left;
  void *right = AvlList_max(list->right, compare);
  if (right && compare(right, maximum) > 0) maximum = right;
  return maximum;
}

AvlList *AvlList_unique(AvlList *list, int (*compare)(const void *, const void *)) {
  if (AvlList_size(list) <= 1) return AvlList_copy(list);
  AvlList *sorted = AvlList_sort(list, compare);
  AvlList *unique = AvlList_empty();
  AvlListIterator iterator = AvlList_begin(sorted);
  void *previous = AvlListIterator_get(&iterator);
  AvlListIterator_next(&iterator);
  unique = AvlList_push(unique, previous);
  while (AvlListIterator_hasNext(&iterator)) {
    void *current = AvlListIterator_get(&iterator);
    AvlListIterator_next(&iterator);
    if (compare(previous, current) != 0) {
      unique = AvlList_push(unique, current);
      previous = current;
    }
  }
  AvlList_free(sorted);
  return unique;
}


void AvlList_foreach(AvlList *list, void (*f)(void *)) {
  if (!list) return;
  AvlList_foreach(list->left, f);
  f(list->data);
  AvlList_foreach(list->right, f);
}

void AvlList_forEachReverse(AvlList *list, void (*f)(void *)) {
  if (!list) return;
  AvlList_forEachReverse(list->right, f);
  f(list->data);
  AvlList_forEachReverse(list->left, f);
}

void *AvlList_fold(AvlList *list, void *acc, void *(*f)(void *acc, void *data)) {
  if (!list) return acc;
  acc = AvlList_fold(list->left, acc, f);
  acc = f(acc, list->data);
  return AvlList_fold(list->right, acc, f);
}

AvlList *AvlList_map(AvlList *list, void *(*f)(void *)) {
  if (!list) return NULL;
  AvlList *map = malloc(sizeof(AvlList));
  map->height = list->height;
  map->size = list->size;
  map->left = AvlList_map(list->left, f);
  map->data = f(list->data);
  map->right = AvlList_map(list->right, f);
  return map;
}

AvlList *AvlList_scan(AvlList *list, void *accumulator, void *(*f)(void *accumulator, void *data)) {
  unsigned n = AvlList_size(list);
  void **results = malloc(sizeof(void *) * (n + 1));
  results[0] = accumulator;
  unsigned count = 1;
  AvlListIterator iterator = AvlList_begin(list);
  while (AvlListIterator_hasNext(&iterator)) {
    void *data = AvlListIterator_get(&iterator);
    accumulator = f(accumulator, data);
    results[count++] = accumulator;
    AvlListIterator_next(&iterator);
  }
  AvlList *scan = AvlList_fromArray(results, n + 1);
  free(results);
  return scan;
}


AvlList *AvlList_filter(AvlList *list, bool (*predicate)(void *)) {
  void **array = AvlList_toArray(list);
  unsigned n = Array_filter(array, AvlList_size(list), predicate);
  AvlList *filter = AvlList_fromArray(array, n);
  free(array);
  return filter;
}

AvlList *AvlList_filterFail(AvlList *list, bool (*predicate)(void *)) {
  void **array = AvlList_toArray(list);
  unsigned n = Array_filterFail(array, AvlList_size(list), predicate);
  AvlList *filter = AvlList_fromArray(array, n);
  free(array);
  return filter;
}

unsigned AvlList_count(AvlList *list, bool (*predicate)(void *)) {
  if (!list) return 0;
  return AvlList_count(list->left, predicate) + (predicate(list->data) ? 1 : 0) + AvlList_count(list->right, predicate);
}

unsigned AvlList_countFail(AvlList *list, bool (*predicate)(void *)) {
  if (!list) return 0;
  return AvlList_countFail(list->left, predicate) + (predicate(list->data) ? 0 : 1) + AvlList_countFail(list->right, predicate);
}

bool AvlList_any(AvlList *list, bool (*predicate)(void *)) {
  return list && (AvlList_any(list->left, predicate) || predicate(list->data) || AvlList_any(list->right, predicate));
}

bool AvlList_all(AvlList *list, bool (*predicate)(void *)) {
  return !list || AvlList_all(list->left, predicate) && predicate(list->data) && AvlList_all(list->right, predicate);
}

void *AvlList_find(AvlList *list, bool (*predicate)(void *)) {
  if (!list) return NULL;
  void *found = AvlList_find(list->left, predicate);
  if (found) return found;
  if (predicate(list->data)) return list->data;
  return AvlList_find(list->right, predicate);
}

void *AvlList_findLast(AvlList *list, bool (*predicate)(void *)) {
  if (!list) return NULL;
  void *found = AvlList_findLast(list->right, predicate);
  if (found) return found;
  if (predicate(list->data)) return list->data;
  return AvlList_findLast(list->left, predicate);
}

int AvlList_findIndex(AvlList *list, bool (*predicate)(void *)) {
  if (!list) return -1;
  int index = AvlList_findIndex(list->left, predicate);
  if (index != -1) return index;
  if (predicate(list->data)) return AvlList_size(list->left);
  index = AvlList_findIndex(list->right, predicate);
  if (index != -1) return AvlList_size(list->left) + 1 + index;
  return -1;
}
