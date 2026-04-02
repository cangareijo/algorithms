#define AVL_SET_DECLARATION(SET, TYPE) \
 \
typedef struct SET { \
  TYPE value; \
  size_t height; \
  size_t size; \
  struct SET *left; \
  struct SET *right; \
} SET; \
 \
size_t SET ## _size(const SET *set); // O(1) \
bool SET ## _contains(const SET *set, TYPE value); \
bool SET ## _isSubset(const SET *first, const SET *second); \
bool SET ## _areDisjoint(const SET *first, const SET *second); \
bool SET ## _areEqual(const SET *first, const SET *second); \
SET *SET ## _empty(); // O(1) \
SET *SET ## _singleton(TYPE value); // O(1) \
void SET ## _destroy(SET *set); \
SET *SET ## _insert(SET *set, TYPE value); \
SET *SET ## _delete(SET *set, TYPE value); \
SET *SET ## _fromArray(const TYPE *array, size_t m, size_t n); // O(n log n) \
size_t SET ## _copyToArray(const SET *set, TYPE *array, size_t i); // O(n) \
TYPE *SET ## _toArray(const SET *set); // O(n) \
SET *SET ## _insertSet(SET *destination, const SET *source); \
SET *SET ## _copy(const SET *set); // O(n) \
SET *SET ## _union(const SET *first, const SET *second); // O(m + n) \
SET *SET ## _insertIntersection(SET *destination, const SET *source, const SET *filter); \
SET *SET ## _intersection(const SET *first, const SET *second); // O(m + n) \
SET *SET ## _insertDifference(SET *destination, const SET *source, const SET *filter); \
SET *SET ## _difference(const SET *first, const SET *second); // O(m + n) \
SET *SET ## _symmetricDifference(const SET *first, const SET *second); // O(m + n) \
void SET ## _print(const SET *set); // O(n) \


#define AVL_SET_DEFINITION(SET, TYPE, ALLOCATE, FREE, COMPARE, PRINT) \
 \
static size_t SET ## _getHeight(const SET *set); \
static size_t SET ## _getSize(const SET *set); \
static int SET ## _getBalance(const SET *set); \
static SET *SET ## _setLeft(SET *set, SET *left); \
static SET *SET ## _setRight(SET *set, SET *right); \
static bool SET ## _isSortedBetween(const SET *set, TYPE x, TYPE y); \
static bool SET ## _isSortedBelow(const SET *set, TYPE x); \
static bool SET ## _isSortedAbove(const SET *set, TYPE x); \
static bool SET ## _isSorted(const SET *set); \
static bool SET ## _isHeighted(const SET *set); \
static bool SET ## _isSized(const SET *set); \
static bool SET ## _isBalanced(const SET *set); \
static bool SET ## _isValid(const SET *set); \
static SET *SET ## _rotateRight(SET *set); \
static SET *SET ## _rotateLeft(SET *set); \
static SET *SET ## _rebalance(SET *set); \
static SET *SET ## _merge(SET *left, SET *right); \
static SET *SET ## _fromSortedArray(const VALUE *values, unsigned m, unsigned n); // O(n) \
static void SET ## _printInner(const SET *set, bool right); // O(n) \
 \
static size_t SET ## _getHeight(const SET *set) { \
  return set ? set->height : 0; \
} \
 \
static size_t SET ## _getSize(const SET *set) { \
  return set ? set->size : 0; \
} \
 \
static int SET ## _getBalance(const SET *set) { \
  return set ? (int)SET ## _getHeight(set->left) - (int)SET ## _getHeight(set->right) : 0; \
} \
 \
static SET *SET ## _setLeft(SET *set, SET *left) { \
  if (!set) return NULL; \
  set->left = left; \
  set->height = 1 + max(SET ## _getHeight(set->left), SET ## _getHeight(set->right)); \
  set->size = 1 + SET ## _getSize(set->left) + SET ## _getSize(set->right); \
  return set; \
} \
 \
static SET *SET ## _setRight(SET *set, SET *right) { \
  if (!set) return NULL; \
  set->right = right; \
  set->height = 1 + max(SET ## _getHeight(set->left), SET ## _getHeight(set->right)); \
  set->size = 1 + SET ## _getSize(set->left) + SET ## _getSize(set->right); \
  return set; \
} \
 \
static bool SET ## _isSortedBetween(const SET *set, TYPE x, TYPE y) { \
  if (set) \
    return \
      COMPARE(x, set->value) < 0 && \
      COMPARE(set->value, y) < 0 && \
      SET ## _isSortedBetween(set->left, x, set->value) && \
      SET ## _isSortedBetween(set->right, set->value, y); \
  return true; \
} \
 \
static bool SET ## _isSortedBelow(const SET *set, TYPE x) { \
  if (set) \
    return \
      COMPARE(set->value, x) < 0 && \
      SET ## _isSortedBelow(set->left, set->value) && \
      SET ## _isSortedBetween(set->right, set->value, x); \
  return true; \
} \
 \
static bool SET ## _isSortedAbove(const SET *set, TYPE x) { \
  if (set) \
    return \
      COMPARE(x, set->value) < 0 && \
      SET ## _isSortedBetween(set->left, x, set->value) && \
      SET ## _isSortedAbove(set->right, set->value); \
  return true; \
} \
 \
static bool SET ## _isSorted(const SET *set) { \
  if (set) return SET ## _isSortedBelow(set->left, set->value) && SET ## _isSortedAbove(set->right, set->value); \
  return true; \
} \
 \
static bool SET ## _isHeighted(const SET *set) { \
  if (set) \
    return \
      set->height == 1 + max(SET ## _getHeight(set->left), SET ## _getHeight(set->right)) && \
      SET ## _isHeighted(set->left) && \
      SET ## _isHeighted(set->right); \
  return true; \
} \
 \
static bool SET ## _isSized(const SET *set) { \
  if (set) \
    return \
      set->size == 1 + SET ## _getSize(set->left) + SET ## _getSize(set->right) && \
      SET ## _isSized(set->left) && \
      SET ## _isSized(set->right); \
  return true; \
} \
 \
static bool SET ## _isBalanced(const SET *set) { \
  if (set) \
    return \
      -1 <= SET ## _getBalance(set) && \
      SET ## _getBalance(set) <= 1 && \
      SET ## _isBalanced(set->left) && \
      SET ## _isBalanced(set->right); \
  return true; \
} \
 \
static bool SET ## _isValid(const SET *set) { \
  return SET ## _isSized(set) && SET ## _isHeighted(set) && SET ## _isBalanced(set) && SET ## _isSorted(set); \
} \
 \
static SET *SET ## _rotateRight(SET *set) { \
  if (set == NULL || set->left == NULL) return set; \
  SET *left = set->left; \
  SET *right = left->right; \
  set = SET ## _setLeft(set, right); \
  left = SET ## _setRight(left, set); \
  return left; \
} \
 \
static SET *SET ## _rotateLeft(SET *set) { \
  if (set == NULL || set->right == NULL) return set; \
  SET *right = set->right; \
  SET *left = right->left; \
  set = SET ## _setRight(set, left); \
  right = SET ## _setLeft(right, set); \
  return right; \
} \
 \
static SET *SET ## _rebalance(SET *set) { \
  if (SET ## _getBalance(set) > 1) { \
    if (SET ## _getBalance(set->left) >= 0) { \
      return SET ## _rotateRight(set); \
    } else { \
      set->left = SET ## _rotateLeft(set->left); \
      return SET ## _rotateRight(set); \
    } \
  } else if (SET ## _getBalance(set) < -1) { \
    if (SET ## _getBalance(set->right) <= 0) { \
      return SET ## _rotateLeft(set); \
    } else { \
      set->right = SET ## _rotateRight(set->right); \
      return SET ## _rotateLeft(set); \
    } \
  } \
  return set; \
} \
 \
static SET *SET ## _merge(SET *left, SET *right) { \
  if (!left) \
    return right; \
  else if (!right) \
    return left; \
  else if (SET ## _getHeight(left) <= SET ## _getHeight(right)) \
    return SET ## _rebalance(SET ## _setLeft(right, SET ## _merge(left, right->left))); \
  else \
    return SET ## _rebalance(SET ## _setRight(left, SET ## _merge(left->right, right))); \
} \
 \
static SET *SET ## _fromSortedArray(const VALUE *values, unsigned m, unsigned n) { \
  if (n <= m) return NULL; \
  unsigned mid = (m + n) / 2; \
  SET *set = ALLOCATE(sizeof(SET)); \
  set->value = values[mid]; \
  set->left = SET ## _fromSortedArray(values, m, mid); \
  set->right = SET ## _fromSortedArray(values, mid, n); \
  set->height = 1 + SET ## _max(SET ## _getHeight(set->left), SET ## _getHeight(set->right)); \
  set->size = 1 + SET ## _size(set->left) + SET ## _size(set->right); \
  return set; \
} \
 \
size_t SET ## _size(const SET *set) {return SET ## _getSize(set);} \
 \
bool SET ## _contains(const SET *set, const TYPE value) { \
  if (!set) return false; \
  int comparison = COMPARE(value, set->value); \
  if (comparison < 0) return SET ## _contains(set->left, value); \
  if (comparison > 0) return SET ## _contains(set->right, value); \
  return true; \
} \
 \
bool SET ## _isSubset(const SET *set1, const SET *set2) { \
  TYPE *array1 = SET ## _toArray(set1); \
  TYPE *array2 = SET ## _toArray(set2); \
  size_t i = 0, j = 0; \
  while (i < SET ## _size(set1) && j < SET ## _size(set2)) { \
    int comparison = COMPARE(array1[i], array2[j]); \
    if (comparison == 0) { \
      i++; \
      j++; \
    } else if (comparison > 0) { \
      j++; \
    } else { \
      break; \
    } \
  } \
  FREE(array1); \
  FREE(array2); \
  return i >= SET ## _size(set1); \
} \
 \
bool SET ## _areDisjoint(const SET *set1, const SET *set2) { \
  TYPE *array1 = SET ## _toArray(set1); \
  TYPE *array2 = SET ## _toArray(set2); \
  size_t i = 0, j = 0; \
  while (i < SET ## _size(set1) && j < SET ## _size(set2)) { \
    int comparison = COMPARE(array1[i], array2[j]); \
    if (comparison == 0) break; \
    else if (comparison < 0) i++; \
    else j++; \
  } \
  FREE(array1); \
  FREE(array2); \
  return i >= SET ## _size(set1) || j >= SET ## _size(set2); \
} \
 \
bool SET ## _areEqual(const SET *first, const SET *second) { \
  return SET ## _size(first) == SET ## _size(second) && SET ## _isSubset(first, second); \
} \
 \
SET *SET ## _empty() {return NULL;} \
 \
SET *SET ## _singleton(TYPE value) { \
  SET *set = ALLOCATE(sizeof(SET)); \
  set->value = value; \
  set->height = set->size = 1; \
  set->left = set->right = NULL; \
  return set; \
} \
 \
void SET ## _destroy(SET *set) { \
  if (!set) return; \
  SET ## _destroy(set->left); \
  SET ## _destroy(set->right); \
  FREE(set); \
} \
 \
SET *SET ## _insert(SET *set, TYPE value) { \
  if (!set) return SET ## _singleton(value); \
  int comparison = COMPARE(value, set->value); \
  if (comparison < 0) return SET ## _rebalance(SET ## _setLeft(set, SET ## _insert(set->left, value))); \
  if (comparison > 0) return SET ## _rebalance(SET ## _setRight(set, SET ## _insert(set->right, value))); \
  return set; \
} \
 \
SET *SET ## _delete(SET *set, TYPE value) { \
  if (!set) return set; \
  int comparison = COMPARE(value, set->value); \
  if (comparison < 0) return SET ## _rebalance(SET ## _setLeft(set, SET ## _delete(set->left, value))); \
  if (comparison > 0) return SET ## _rebalance(SET ## _setRight(set, SET ## _delete(set->right, value))); \
  SET *merged = SET ## _merge(set->left, set->right); \
  FREE(set); \
  return merged; \
} \
 \
SET *SET ## _fromArray(const TYPE *array, size_t m, size_t n) { \
  SET *set = SET ## _empty(); \
  for (int i = m; i < n; i++) set = SET ## _insert(set, array[i]); \
  return set; \
} \
 \
size_t SET ## _copyToArray(const SET *set, TYPE *array, size_t i) { \
  if (!set) return i; \
  i = SET ## _copyToArray(set->left, array, i); \
  array[i++] = set->value; \
  i = SET ## _copyToArray(set->right, array, i); \
  return i; \
} \
 \
TYPE *SET ## _toArray(const SET *set) { \
  size_t size = SET ## _size(set); \
  if (size == 0) return NULL; \
  TYPE *array = ALLOCATE(size * sizeof(TYPE)); \
  SET ## _copyToArray(set, array, 0); \
  return array; \
} \
 \
SET *SET ## _insertSet(SET *destination, const SET *source) { \
  if (source) { \
    destination = SET ## _insertSet(destination, source->left); \
    destination = SET ## _insert(destination, source->value); \
    destination = SET ## _insertSet(destination, source->right); \
  } \
  return destination; \
} \
 \
SET *SET ## _copy(const SET *set) { \
  if (set == NULL) return NULL; \
  SET *copy = ALLOCATE(sizeof(SET)); \
  copy->value = set->value; \
  copy->height = set->height; \
  copy->size = set->size; \
  copy->left = SET ## _copy(set->left); \
  copy->right = SET ## _copy(set->right); \
  return copy; \
} \
 \
SET *SET ## _union(const SET *set1, const SET *set2) { \
  VALUE *values1 = SET ## _values(set1); \
  VALUE *values2 = SET ## _values(set2); \
  VALUE *values3 = ALLOCATE((SET ## _size(set1) + SET ## _size(set2)) * sizeof(VALUE)); \
  unsigned i = 0, j = 0, k = 0; \
  while (i < SET ## _size(set1) && j < SET ## _size(set2)) { \
    int comparison = COMPARE(values1[i], values2[j]); \
    if (comparison < 0) values3[k++] = values1[i]; \
    if (comparison >= 0) values3[k++] = values2[j]; \
    if (comparison <= 0) i++; \
    if (comparison >= 0) j++; \
  } \
  while (i < SET ## _size(set1)) values3[k++] = values1[i++]; \
  while (j < SET ## _size(set2)) values3[k++] = values2[j++]; \
  SET *set3 = SET ## _fromSortedArray(values3, 0, k); \
  FREE(values1); \
  FREE(values2); \
  FREE(values3); \
  return set3; \
} \
 \
SET *SET ## _insertIntersection(SET *destination, const SET *source, const SET *filter) { \
  if (source) { \
    destination = SET ## _insertIntersection(destination, source->left, filter); \
    if (SET ## _contains(filter, source->value)) destination = SET ## _insert(destination, source->value); \
    destination = SET ## _insertIntersection(destination, source->right, filter); \
  } \
  return destination; \
} \
 \
SET *SET ## _intersection(const SET *set1, const SET *set2) { \
  VALUE *values1 = SET ## _values(set1); \
  VALUE *values2 = SET ## _values(set2); \
  VALUE *values3 = ALLOCATE(SET ## _min(SET ## _size(set1), SET ## _size(set2)) * sizeof(VALUE)); \
  unsigned i = 0, j = 0, k = 0; \
  while (i < SET ## _size(set1) && j < SET ## _size(set2)) { \
    int comparison = COMPARE(values1[i], values2[j]); \
    if (comparison == 0) values3[k++] = values2[j]; \
    if (comparison <= 0) i++; \
    if (comparison >= 0) j++; \
  } \
  SET *intersection = SET ## _fromSortedArray(values3, 0, k); \
  FREE(values1); \
  FREE(values2); \
  FREE(values3); \
  return intersection; \
} \
 \
SET *SET ## _insertDifference(SET *destination, const SET *source, const SET *filter) { \
  if (source) { \
    destination = SET ## _insertDifference(destination, source->left, filter); \
    if (!SET ## _contains(filter, source->value)) destination = SET ## _insert(destination, source->value); \
    destination = SET ## _insertDifference(destination, source->right, filter); \
  } \
  return destination; \
} \
 \
SET *SET ## _difference(const SET *set1, const SET *set2) { \
  VALUE *values1 = SET ## _values(set1); \
  VALUE *values2 = SET ## _values(set2); \
  VALUE *values3 = ALLOCATE(SET ## _size(set1) * sizeof(VALUE)); \
  unsigned i = 0, j = 0, k = 0; \
  while (i < SET ## _size(set1) && j < SET ## _size(set2)) { \
    int comparison = COMPARE(values1[i], values2[j]); \
    if (comparison < 0) values3[k++] = values1[i]; \
    if (comparison <= 0) i++; \
    if (comparison >= 0) j++; \
  } \
  while (i < SET ## _size(set1)) values3[k++] = values1[i++]; \
  SET *set3 = SET ## _fromSortedArray(values3, 0, k); \
  FREE(values1); \
  FREE(values2); \
  FREE(values3); \
  return set3; \
} \
 \
SET *SET ## _symmetricDifference(const SET *set1, const SET *set2) { \
  VALUE *values1 = SET ## _values(set1); \
  VALUE *values2 = SET ## _values(set2); \
  VALUE *values3 = ALLOCATE((SET ## _size(set1) + SET ## _size(set2)) * sizeof(VALUE)); \
  unsigned i = 0, j = 0, k = 0; \
  while (i < SET ## _size(set1) && j < SET ## _size(set2)) { \
    int comparison = COMPARE(values1[i], values2[j]); \
    if (comparison < 0) values3[k++] = values1[i]; \
    if (comparison > 0) values3[k++] = values2[j]; \
    if (comparison <= 0) i++; \
    if (comparison >= 0) j++; \
  } \
  while (i < SET ## _size(set1)) values3[k++] = values1[i++]; \
  while (j < SET ## _size(set2)) values3[k++] = values2[j++]; \
  SET *set3 = SET ## _fromSortedArray(values3, 0, k); \
  FREE(values1); \
  FREE(values2); \
  FREE(values3); \
  return set3; \
} \
 \
static void SET ## _printInner(const SET *set, bool right) { \
  if (!set) return; \
  SET ## _printInner(set->left, false); \
  PRINT(set->value); \
  if (set->right || !right) printf(", "); \
  SET ## _printInner(set->right, right); \
} \
 \
void SET ## _print(const SET *set) { \
  printf("{"); \
  SET ## _printInner(set, true); \
  printf("}"); \
} \

