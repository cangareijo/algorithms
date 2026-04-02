#define ARRAY_LIST_DECLARATION(LIST, KEY) \
 \
typedef struct { \
  unsigned start; \
  unsigned length; \
  unsigned capacity; \
  KEY *array; \
} LIST; \
 \
bool LIST ## _isValid(const LIST *list); // O(1) \
LIST *LIST ## _create(unsigned length); // O(1) \
LIST *LIST ## _empty(); // O(1) \
void LIST ## _destroy(LIST *list); // O(1) \
LIST *LIST ## _clear(LIST *list); // O(1) \
unsigned LIST ## _length(const LIST *list); // O(1) \
KEY LIST ## _get(const LIST *list, unsigned i); // O(1) \
LIST *LIST ## _set(LIST *list, unsigned i, KEY x); // O(1) \
int LIST ## _indexOf(const LIST *list, KEY x); // O(n) \
LIST *LIST ## _push(LIST *list, KEY x); // O(1) \
KEY LIST ## _pop(LIST *list); // O(1) \
LIST *LIST ## _pushLeft(LIST *list, KEY x); // O(1) \
KEY LIST ## _popLeft(LIST *list); // O(1) \
LIST *LIST ## _pushSublist(LIST *destination, LIST *source, unsigned m, unsigned n); // O(n) \
LIST *LIST ## _pushList(LIST *destination, LIST *source); // O(n) \
LIST *LIST ## _slice(LIST *list, unsigned m, unsigned n); // O(n) \
LIST *LIST ## _copy(LIST *list); // O(n) \
LIST *LIST ## _filter(LIST *list, bool (*predicate)(KEY)); // O(n) \
LIST *LIST ## _reverse(LIST *list); // O(n) \
LIST *LIST ## _concatenate(LIST *left, LIST *right); // O(m + n) \
LIST *LIST ## _insert(LIST *list, unsigned i, KEY x); // O(n) \
LIST *LIST ## _delete(LIST *list, unsigned i); // O(n) \
bool LIST ## _contains(const LIST *list, KEY x); // O(n) \
bool LIST ## _equals(LIST *list1, LIST *list2); // O(n) \
unsigned LIST ## _count(LIST *list, KEY x); // O(n) \
LIST *LIST ## _repeat(LIST *list, unsigned n); // O(m * n) \
LIST *LIST ## _swap(LIST *list, unsigned i, unsigned j); // O(1) \
LIST *LIST ## _shuffle(LIST *list); // O(n) \
bool LIST ## _isSorted(const LIST *list); // O(n) \
LIST *LIST ## _merge(LIST *first, LIST *second); // O(m + n) \
LIST *LIST ## _sort(LIST *list); // O(n log n) \
LIST *LIST ## _unique(const LIST *list); // O(n log n) \
LIST *LIST ## _intersection(LIST *list1, LIST *list2); // O(n log n) \
LIST *LIST ## _difference(LIST *list1, LIST *list2); // O(n log n) \
LIST *LIST ## _rotate(LIST *list, int n); // O(n) \


#define ARRAY_LIST_DEFINITION(LIST, KEY, ALLOCATE, FREE, COMPARE) \
 \
static LIST *LIST ## _changeCapacity(LIST *list, unsigned capacity); // O(n) \
 \
bool LIST ## _isValid(const LIST *list) { \
  return list && list->start < list->capacity && list->length <= list->capacity && list->array; \
} \
 \
LIST *LIST ## _create(unsigned length) { \
  unsigned capacity = max(1, length); \
  LIST *list = ALLOCATE(sizeof(LIST)); \
  list->start = 0; \
  list->length = length; \
  list->capacity = capacity; \
  list->array = ALLOCATE(capacity  *sizeof(KEY)); \
  return list; \
} \
 \
void LIST ## _destroy(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  FREE(list->array); \
  FREE(list); \
} \
 \
unsigned LIST ## _length(const LIST *list) { \
  assert(LIST ## _isValid(list)); \
  return list->length; \
} \
 \
KEY LIST ## _get(const LIST *list, unsigned i) { \
  assert(LIST ## _isValid(list)); \
  assert(i < list->length); \
  return list->array[(list->start + i) % list->capacity]; \
} \
 \
LIST *LIST ## _set(LIST *list, unsigned i, KEY x) { \
  assert(LIST ## _isValid(list)); \
  assert(i < list->length); \
  list->array[(list->start + i) % list->capacity] = x; \
  return list; \
} \
 \
int LIST ## _indexOf(const LIST *list, KEY x) { \
  assert(LIST ## _isValid(list)); \
  for (unsigned i = 0; i < LIST ## _length(list); i++) if (COMPARE(LIST ## _get(list, i), x) == 0) return (int)i; \
  return -1; \
} \
 \
static LIST *LIST ## _changeCapacity(LIST *list, unsigned capacity) { \
  assert(LIST ## _isValid(list)); \
  assert(list->length <= capacity); \
  capacity = max(1, capacity); \
  KEY *array = ALLOCATE(capacity * sizeof(KEY)); \
  for (unsigned i = 0; i < list->length; i++) array[i] = list->array[(list->start + i) % list->capacity]; \
  FREE(list->array); \
  list->start = 0; \
  list->capacity = capacity; \
  list->array = array; \
  return list; \
} \
 \
LIST *LIST ## _empty() { \
  return LIST ## _create(0); \
} \
 \
LIST *LIST ## _push(LIST *list, KEY x) { \
  assert(LIST ## _isValid(list)); \
  if (list->length >= list->capacity) list = LIST ## _changeCapacity(list, list->capacity * 2); \
  list->array[(list->start + list->length) % list->capacity] = x; \
  list->length++; \
  return list; \
} \
 \
KEY LIST ## _pop(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  assert(list->length > 0); \
  list->length--; \
  return list->array[(list->start + list->length) % list->capacity]; \
} \
 \
LIST *LIST ## _pushLeft(LIST *list, KEY x) { \
  assert(LIST ## _isValid(list)); \
  if (list->length >= list->capacity) list = LIST ## _changeCapacity(list, list->capacity * 2); \
  list->start = (list->start + list->capacity - 1) % list->capacity; \
  list->length++; \
  list->array[list->start] = x; \
  return list; \
} \
 \
KEY LIST ## _popLeft(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  assert(list->length > 0); \
  KEY x = list->array[list->start]; \
  list->start = (list->start + 1) % list->capacity; \
  list->length--; \
  return x; \
} \
 \
LIST *LIST ## _pushSublist(LIST *destination, const LIST *source, unsigned m, unsigned n) { \
  assert(LIST ## _isValid(destination)); \
  assert(LIST ## _isValid(source)); \
  assert(n <= LIST ## _length(source)); \
  for (unsigned i = m; i < n; i++) destination = LIST ## _push(destination, LIST ## _get(source, i)); \
  return destination; \
} \
 \
LIST *LIST ## _pushList(LIST *destination, const LIST *source) { \
  assert(LIST ## _isValid(destination)); \
  assert(LIST ## _isValid(source)); \
  return LIST ## _pushSublist(destination, source, 0, LIST ## _length(source)); \
} \
 \
LIST *LIST ## _clear(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  LIST ## _destroy(list); \
  return LIST ## _create(0); \
} \
 \
LIST *LIST ## _slice(LIST *list, unsigned m, unsigned n) { \
  assert(LIST ## _isValid(list)); \
  assert(m <= n); \
  assert(n <= LIST ## _length(list)); \
  LIST *slice = LIST ## _create(n - m); \
  for (unsigned i = m; i < n; i++) LIST ## _set(slice, i - m, LIST ## _get(list, i)); \
  return slice; \
} \
 \
LIST *LIST ## _copy(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  return LIST ## _slice(list, 0, LIST ## _length(list)); \
} \
 \
LIST *LIST ## _filter(LIST *list, bool (*predicate)(KEY)) { \
  assert(LIST ## _isValid(list)); \
  LIST *filtered = LIST ## _empty(); \
  for (unsigned i = 0; i < LIST ## _length(list); i++) if (predicate(LIST ## _get(list, i))) LIST ## _push(filtered, LIST ## _get(list, i)); \
  return filtered; \
}
 \
LIST *LIST ## _reverse(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  LIST *reverse = LIST ## _create(0); \
  for (unsigned i = LIST ## _length(list); i > 0; i--) reverse = LIST ## _push(reverse, LIST ## _get(list, i - 1)); \
  return reverse; \
} \
 \
LIST *LIST ## _concatenate(LIST *left, LIST *right) { \
  assert(LIST ## _isValid(left)); \
  assert(LIST ## _isValid(right)); \
  LIST *concatenation = LIST ## _create(0); \
  concatenation = LIST ## _pushList(concatenation, left); \
  concatenation = LIST ## _pushList(concatenation, right); \
  return concatenation; \
} \
 \
LIST *LIST ## _insert(LIST *list, unsigned i, KEY x) { \
  assert(LIST ## _isValid(list)); \
  assert(i < LIST ## _length(list)); \
  LIST *inserted = LIST ## _create(0); \
  inserted = LIST ## _pushSublist(inserted, list, 0, i); \
  inserted = LIST ## _push(inserted, x); \
  inserted = LIST ## _pushSublist(inserted, list, i, LIST ## _length(list)); \
  return inserted; \
} \
 \
LIST *LIST ## _delete(LIST *list, unsigned i) { \
  assert(LIST ## _isValid(list)); \
  assert(i < LIST ## _length(list)); \
  LIST *deleted = LIST ## _create(0); \
  deleted = LIST ## _pushSublist(deleted, list, 0, i); \
  deleted = LIST ## _pushSublist(deleted, list, i + 1, LIST ## _length(list)); \
  return deleted; \
} \
 \
bool LIST ## _contains(const LIST *list, KEY x) { \
  return LIST ## _indexOf(list, x) != -1; \
} \
 \
bool LIST ## _equals(LIST *list1, LIST *list2) { \
  assert(LIST ## _isValid(list1)); \
  assert(LIST ## _isValid(list2)); \
  if (LIST ## _length(list1) != LIST ## _length(list2)) return false; \
  for (unsigned i = 0; i < LIST ## _length(list1); i++) if (COMPARE(LIST ## _get(list1, i), LIST ## _get(list2, i)) != 0) return false; \
  return true; \
} \
 \
unsigned LIST ## _count(LIST *list, KEY x) { \
  assert(LIST ## _isValid(list)); \
  unsigned count = 0; \
  for (unsigned i = 0; i < LIST ## _length(list); i++) if (COMPARE(LIST ## _get(list, i), x) == 0) count++; \
  return count; \
} \
 \
LIST *LIST ## _repeat(LIST *list, unsigned n) { \
  assert(LIST ## _isValid(list)); \
  LIST *repeated = LIST ## _create(0); \
  for (unsigned i = 0; i < n; i++) repeated = LIST ## _pushList(repeated, list); \
  return repeated; \
} \
 \
LIST *LIST ## _swap(LIST *list, unsigned i, unsigned j) { \
  assert(LIST ## _isValid(list)); \
  assert(i < LIST ## _length(list)); \
  assert(j < LIST ## _length(list)); \
  KEY x = LIST ## _get(list, i); \
  KEY y = LIST ## _get(list, j); \
  list = LIST ## _set(list, i, y); \
  list = LIST ## _set(list, j, x); \
  return list; \
} \
 \
LIST *LIST ## _shuffle(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  LIST *shuffle = LIST ## _copy(list); \
  for (unsigned i = 0; i < LIST ## _length(list); i++) LIST ## _swap(shuffle, i, randomBetween(i, LIST ## _length(list))); \
  return shuffle; \
} \
 \
bool LIST ## _isSorted(const LIST *list) { \
  assert(LIST ## _isValid(list)); \
  for (unsigned i = 1; i < LIST ## _length(list); i++) if (COMPARE(LIST ## _get(list, i - 1), LIST ## _get(list, i)) > 0) return false; \
  return true; \
}
 \
LIST *LIST ## _merge(LIST *first, LIST *second) { \
  assert(LIST ## _isValid(first)); \
  assert(LIST ## _isValid(second)); \
  LIST *merged = LIST ## _create(0); \
  unsigned i = 0, j = 0; \
  while (i < LIST ## _length(first) && j < LIST ## _length(second)) \
    if (COMPARE(LIST ## _get(first, i), LIST ## _get(second, j)) <= 0) \
      merged = LIST ## _push(merged, LIST ## _get(first, i++)); \
    else \
      merged = LIST ## _push(merged, LIST ## _get(second, j++)); \
  merged = LIST ## _pushSublist(merged, first, i, LIST ## _length(first)); \
  merged = LIST ## _pushSublist(merged, second, j, LIST ## _length(second)); \
  return merged; \
} \
 \
LIST *LIST ## _sort(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  if (LIST ## _length(list) < 2) return LIST ## _copy(list); \
  LIST *first = LIST ## _slice(list, 0, LIST ## _length(list) / 2); \
  LIST *second = LIST ## _slice(list, LIST ## _length(list) / 2, LIST ## _length(list)); \
  LIST *sortedFirst = LIST ## _sort(first); \
  LIST *sortedSecond = LIST ## _sort(second); \
  LIST *merged = LIST ## _merge(sortedFirst, sortedSecond); \
  LIST ## _destroy(first); \
  LIST ## _destroy(second); \
  LIST ## _destroy(sortedFirst); \
  LIST ## _destroy(sortedSecond); \
  return merged; \
} \
 \
LIST *LIST ## _unique(const LIST *list) { \
  assert(LIST ## _isValid(list)); \
  LIST *list2 = LIST ## _sort(list); \
  LIST *list3 = LIST ## _empty(); \
  if (LIST ## _length(list2) > 0) LIST ## _push(list3, LIST ## _get(list2, 0)); \
  for (unsigned i = 1; i < LIST ## _length(list2); i++) \
    if (COMPARE(LIST ## _get(list2, i - 1), LIST ## _get(list2, i)) != 0) LIST ## _push(list3, LIST ## _get(list2, i)); \
  LIST ## _destroy(list2); \
  return list3; \
} \
 \
LIST *LIST ## _intersection(const LIST *list1, const LIST *list2) { \
  assert(LIST ## _isValid(list1)); \
  assert(LIST ## _isValid(list2)); \
  LIST *list3 = LIST ## _sort(list1); \
  LIST *list4 = LIST ## _sort(list2); \
  LIST *list5 = LIST ## _empty(); \
  unsigned i = 0, j = 0; \
  while (i < LIST ## _length(list3) && j < LIST ## _length(list4)) { \
    int comparison = COMPARE(LIST ## _get(list3, i), LIST ## _get(list4, j)); \
    if (comparison == 0) LIST ## _push(list5, LIST ## _get(list3, i)); \
    if (comparison <= 0) i++; \
    if (comparison >= 0) j++; \
  } \
  LIST ## _destroy(list3); \
  LIST ## _destroy(list4); \
  return list5; \
} \
 \
LIST *LIST ## _difference(const LIST *list1, const LIST *list2) { \
  assert(LIST ## _isValid(list1)); \
  assert(LIST ## _isValid(list2)); \
  LIST *list3 = LIST ## _sort(list1); \
  LIST *list4 = LIST ## _sort(list2); \
  LIST *list5 = LIST ## _empty(); \
  unsigned i = 0, j = 0; \
  while (i < LIST ## _length(list3) && j < LIST ## _length(list4)) { \
    int comparison = COMPARE(LIST ## _get(list3, i), LIST ## _get(list4, j)); \
    if (comparison < 0) LIST ## _push(list5, LIST ## _get(list3, i)); \
    if (comparison <= 0) i++; \
    if (comparison >= 0) j++; \
  } \
  while (i < LIST ## _length(list3)) LIST ## _push(list5, LIST ## _get(list3, i)); \
  LIST ## _destroy(list3); \
  LIST ## _destroy(list4); \
  return list5; \
} \
 \
LIST *LIST ## _rotate(const LIST *list, int n) { \
  assert(LIST ## _isValid(list)); \
  n = (n % LIST ## _length(list) + LIST ## _length(list)) % LIST ## _length(list);
  LIST *list2 = LIST ## _empty(); \
  LIST ## _pushSublist(list2, list, n, LIST ## _length(list)); \
  LIST ## _pushSublist(list2, list, 0, n); \
  return list2; \
} \

