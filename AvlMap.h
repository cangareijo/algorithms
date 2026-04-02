#define AVL_MAP_DECLARATION(MAP, KEY, VALUE) \
 \
typedef struct MAP { \
  KEY key; \
  VALUE value; \
  unsigned height; \
  unsigned size; \
  struct MAP *left; \
  struct MAP *right; \
} MAP; \
 \
bool MAP ## _isEmpty(const MAP *map); // O(1) \
bool MAP ## _contains(const MAP *map, const KEY key); // O(log n) \
unsigned MAP ## _size(const MAP *map); // O(1) \
MAP *MAP ## _empty(); // O(1) \
MAP *MAP ## _singleton(KEY key, VALUE value); // O(1) \
MAP *MAP ## _clear(MAP *map); // O(n) \
void MAP ## _destroy(MAP *map); // O(n) \
VALUE *MAP ## _get(const MAP *map, const KEY key); // O(log n) \
MAP *MAP ## _set(MAP *map, KEY key, VALUE value); // O(log n) \
MAP *MAP ## _delete(MAP *map, const KEY key); // O(log n) \
MAP *MAP ## _union(const MAP *map1, const MAP *map2); // O(m + n) \
MAP *MAP ## _intersection(const MAP *map1, const MAP *map2); // O(m + n) \
MAP *MAP ## _difference(const MAP *map1, const MAP *map2); // O(m + n) \
MAP *MAP ## _symmetricDifference(const MAP *map1, const MAP *map2); // O(m + n) \
MAP *MAP ## _copy(const MAP *map); // O(n) \
void MAP ## _print(const MAP *map); // O(n) \
MAP *MAP ## _fromArray(const KEY *keys, const VALUE *values, unsigned m, unsigned n); // O(n log n) \
KEY *MAP ## _keys(MAP *map); // O(n) \
VALUE *MAP ## _values(MAP *map); // O(n) \


#define AVL_SET_DEFINITION(MAP, KEY, VALUE, ALLOCATE, FREE, COMPARE, PRINT_KEY, PRINT_VALUE) \
 \
static unsigned MAP ## _min(unsigned a, unsigned b); // O(1) \
static unsigned MAP ## _max(unsigned a, unsigned b); // O(1) \
static unsigned MAP ## _getHeight(const MAP *map); // O(1) \
static int MAP ## _getBalance(const MAP *map); // O(1) \
static MAP *MAP ## _setLeft(MAP *map, MAP *left); // O(1) \
static MAP *MAP ## _setRight(MAP *map, MAP *right); // O(1) \
static MAP *MAP ## _rotateRight(MAP *map); // O(1) \
static MAP *MAP ## _rotateLeft(MAP *map); // O(1) \
static MAP *MAP ## _rebalance(MAP *map); // O(1) \
static MAP *MAP ## _fromSortedArray(const KEY *keys, const VALUE *values, unsigned m, unsigned n); \
static void MAP ## _printInner(const MAP *map, bool right); \
static unsigned MAP ## _copyKeys(MAP *map, KEY *keys, unsigned i); \
static unsigned MAP ## _copyValues(MAP *map, VALUE *values, unsigned i); \
 \
static unsigned MAP ## _min(unsigned a, unsigned b) { \
  return a <= b ? a : b; \
} \
 \
static unsigned MAP ## _max(unsigned a, unsigned b) { \
  return a >= b ? a : b; \
} \
 \
static unsigned MAP ## _getHeight(const MAP *map) { \
  return map ? map->height : 0; \
} \
 \
static int MAP ## _getBalance(const MAP *map) { \
  return map ? (int)MAP ## _getHeight(map->left) - (int)MAP ## _getHeight(map->right) : 0; \
} \
 \
static MAP *MAP ## _setLeft(MAP *map, MAP *left) { \
  if (!map) return NULL; \
  map->left = left; \
  map->height = 1 + MAP ## _max(MAP ## _getHeight(map->left), MAP ## _getHeight(map->right)); \
  map->size = 1 + MAP ## _size(map->left) + MAP ## _size(map->right); \
  return map; \
} \
 \
static MAP *MAP ## _setRight(MAP *map, MAP *right) { \
  if (!map) return NULL; \
  map->right = right; \
  map->height = 1 + MAP ## _max(MAP ## _getHeight(map->left), MAP ## _getHeight(map->right)); \
  map->size = 1 + MAP ## _size(map->left) + MAP ## _size(map->right); \
  return map; \
} \
 \
static MAP *MAP ## _rotateRight(MAP *map) { \
  if (!map || !map->left) return map; \
  MAP *left = map->left; \
  MAP *right = left->right; \
  map = MAP ## _setLeft(map, right); \
  left = MAP ## _setRight(left, map); \
  return left; \
} \
 \
static MAP *MAP ## _rotateLeft(MAP *map) { \
  if (!map || !map->right) return map; \
  MAP *right = map->right; \
  MAP *left = right->left; \
  map = MAP ## _setRight(map, left); \
  right = MAP ## _setLeft(right, map); \
  return right; \
} \
 \
static MAP *MAP ## _rebalance(MAP *map) { \
  if (MAP ## _getBalance(map) > 1) { \
    if (MAP ## _getBalance(map->left) >= 0) { \
      return MAP ## _rotateRight(map); \
    } else { \
      map->left = MAP ## _rotateLeft(map->left); \
      return MAP ## _rotateRight(map); \
    } \
  } else if (MAP ## _getBalance(map) < -1) { \
    if (MAP ## _getBalance(map->right) <= 0) { \
      return MAP ## _rotateLeft(map); \
    } else { \
      map->right = MAP ## _rotateRight(map->right); \
      return MAP ## _rotateLeft(map); \
    } \
  } \
  return map; \
} \
 \
static MAP *MAP ## _fromSortedArray(const KEY *keys, const VALUE *values, unsigned m, unsigned n) { \
  if (n <= m) return NULL; \
  unsigned mid = (m + n) / 2; \
  MAP *map = ALLOCATE(sizeof(MAP)); \
  map->key = keys[mid]; \
  map->value = values[mid]; \
  map->left = MAP ## _fromSortedArray(keys, values, m, mid); \
  map->right = MAP ## _fromSortedArray(keys, values, mid, n); \
  map->height = 1 + MAP ## _max(MAP ## _getHeight(map->left), MAP ## _getHeight(map->right)); \
  map->size = 1 + MAP ## _size(map->left) + MAP ## _size(map->right); \
  return map; \
} \
 \
static void MAP ## _printInner(const MAP *map, bool right) { \
  if (!map) return; \
  MAP ## _printInner(map->left, false); \
  PRINT_KEY(map->key); \
  printf(": "); \
  PRINT_VALUE(map->value); \
  if (map->right || !right) printf(", "); \
  MAP ## _printInner(map->right, right); \
} \
 \
bool MAP ## _isEmpty(const MAP *map) { \
  return !map; \
} \
 \
bool MAP ## _contains(const MAP *map, const KEY key) { \
  if (!map) return false; \
  int comparison = COMPARE(key, map->key); \
  if (comparison < 0) return MAP ## _contains(map->left, key); \
  if (comparison > 0) return MAP ## _contains(map->right, key); \
  return true; \
} \
 \
unsigned MAP ## _size(const MAP *map) { \
  return map ? map->size : 0; \
} \
 \
MAP *MAP ## _empty() { \
  return NULL; \
} \
 \
MAP *MAP ## _singleton(KEY key, VALUE value) { \
  MAP *map = ALLOCATE(sizeof(MAP)); \
  map->key = key; \
  map->value = value; \
  map->height = map->size = 1; \
  map->left = map->right = NULL; \
  return map; \
} \
 \
MAP *MAP ## _clear(MAP *map) { \
  MAP ## _destroy(map); \
  return MAP ## _empty(); \
} \
 \
void MAP ## _destroy(MAP *map) { \
  if (!map) return; \
  MAP ## _destroy(map->left); \
  MAP ## _destroy(map->right); \
  FREE(map); \
} \
 \
VALUE *MAP ## _get(const MAP *map, const KEY key) { \
  if (!map) return NULL; \
  int comparison = COMPARE(key, map->key); \
  if (comparison < 0) return MAP ## _get(map->left, key); \
  if (comparison > 0) return MAP ## _get(map->right, key); \
  return &map->value; \
} \
 \
MAP *MAP ## _set(MAP *map, KEY key, VALUE value) { \
  if (!map) return MAP ## _singleton(key, value); \
  int comparison = COMPARE(key, map->key); \
  if (comparison < 0) return MAP ## _rebalance(MAP ## _setLeft(map, MAP ## _set(map->left, key, value))); \
  if (comparison > 0) return MAP ## _rebalance(MAP ## _setRight(map, MAP ## _set(map->right, key, value))); \
  map->key = key; \
  map->value = value; \
  return map; \
} \
 \
MAP *MAP ## _delete(MAP *map, const KEY key) { \
  if (!map) return NULL; \
  int comparison = COMPARE(key, map->key); \
  if (comparison < 0) return MAP ## _rebalance(MAP ## _setLeft(map, MAP ## _delete(map->left, key))); \
  if (comparison > 0) return MAP ## _rebalance(MAP ## _setRight(map, MAP ## _delete(map->right, key))); \
  MAP *merged = MAP ## _merge(map->left, map->right); \
  FREE(map); \
  return merged; \
} \
 \
MAP *MAP ## _insertMap(MAP *destination, const MAP *source) { \
  if (!source) return destination; \
  destination = MAP ## _insertMap(destination, source->left); \
  destination = MAP ## _set(destination, source->key, source->value); \
  destination = MAP ## _insertMap(destination, source->right); \
  return destination; \
} \
 \
MAP *MAP ## _union(const MAP *map1, const MAP *map2) { \
  KEY *keys1 = MAP ## _keys(map1); \
  KEY *keys2 = MAP ## _keys(map2); \
  KEY *keys3 = ALLOCATE((MAP ## _size(map1) + MAP ## _size(map2)) * sizeof(KEY)); \
  VALUE *values1 = MAP ## _values(map1); \
  VALUE *values2 = MAP ## _values(map2); \
  VALUE *values3 = ALLOCATE((MAP ## _size(map1) + MAP ## _size(map2)) * sizeof(VALUE)); \
  unsigned i = 0, j = 0, k = 0; \
  while (i < MAP ## _size(map1) && j < MAP ## _size(map2)) { \
    int comparison = COMPARE(keys1[i], keys2[j]); \
    if (comparison < 0) { \
      keys3[k] = keys1[i]; \
      values3[k++] = values1[i]; \
    } else { \
      keys3[k] = keys2[j]; \
      values3[k++] = values2[j]; \
    } \
    if (comparison <= 0) i++; \
    if (comparison >= 0) j++; \
  } \
  while (i < MAP ## _size(map1)) { \
    keys3[k] = keys1[i]; \
    values3[k++] = values1[i++]; \
  } \
  while (j < MAP ## _size(map2)) { \
    keys3[k] = keys2[j]; \
    values3[k++] = values2[j++]; \
  } \
  MAP *map3 = MAP ## _fromSortedArray(keys3, values3, 0, k); \
  FREE(keys1); \
  FREE(keys2); \
  FREE(keys3); \
  FREE(values1); \
  FREE(values2); \
  FREE(values3); \
  return map3; \
} \
 \
MAP *MAP ## _intersection(const MAP *map1, const MAP *map2) { \
  KEY *keys1 = MAP ## _keys(map1); \
  KEY *keys2 = MAP ## _keys(map2); \
  KEY *keys3 = ALLOCATE(MAP ## _min(MAP ## _size(map1), MAP ## _size(map2)) * sizeof(KEY)); \
  VALUE *values2 = MAP ## _values(map2); \
  VALUE *values3 = ALLOCATE(MAP ## _min(MAP ## _size(map1), MAP ## _size(map2)) * sizeof(VALUE)); \
  unsigned i = 0, j = 0, k = 0; \
  while (i < MAP ## _size(map1) && j < MAP ## _size(map2)) { \
    int comparison = COMPARE(keys1[i], keys2[j]); \
    if (comparison == 0) { \
      keys3[k] = keys2[j]; \
      values3[k++] = values2[j]; \
    } \
    if (comparison <= 0) i++; \
    if (comparison >= 0) j++; \
  } \
  MAP *map3 = MAP ## _fromSortedArray(keys3, values3, 0, k); \
  FREE(keys1); \
  FREE(keys2); \
  FREE(keys3); \
  FREE(values2); \
  FREE(values3); \
  return map3; \
} \
 \
MAP *MAP ## _difference(const MAP *map1, const MAP *map2) { \
  KEY *keys1 = MAP ## _keys(map1); \
  KEY *keys2 = MAP ## _keys(map2); \
  KEY *keys3 = ALLOCATE(MAP ## _size(map1) * sizeof(KEY)); \
  VALUE *values1 = MAP ## _values(map1); \
  VALUE *values3 = ALLOCATE(MAP ## _size(map1) * sizeof(VALUE)); \
  unsigned i = 0, j = 0, k = 0; \
  while (i < MAP ## _size(map1) && j < MAP ## _size(map2)) { \
    int comparison = COMPARE(keys1[i], keys2[j]); \
    if (comparison < 0) { \
      keys3[k] = keys1[i]; \
      values3[k++] = values1[i]; \
    } \
    if (comparison <= 0) i++; \
    if (comparison >= 0) j++; \
  } \
  while (i < MAP ## _size(map1)) { \
    keys3[k] = keys1[i]; \
    values3[k++] = values1[i++]; \
  } \
  MAP *map3 = MAP ## _fromSortedArray(keys3, values3, 0, k); \
  FREE(keys1); \
  FREE(keys2); \
  FREE(keys3); \
  FREE(values1); \
  FREE(values3); \
  return map3; \
} \
 \
MAP *MAP ## _symmetricDifference(const MAP *map1, const MAP *map2) { \
  KEY *keys1 = MAP ## _keys(map1); \
  KEY *keys2 = MAP ## _keys(map2); \
  KEY *keys3 = ALLOCATE((MAP ## _size(map1) + MAP ## _size(map2)) * sizeof(KEY)); \
  VALUE *values1 = MAP ## _values(map1); \
  VALUE *values2 = MAP ## _values(map2); \
  VALUE *values3 = ALLOCATE((MAP ## _size(map1) + MAP ## _size(map2)) * sizeof(VALUE)); \
  unsigned i = 0, j = 0, k = 0; \
  while (i < MAP ## _size(map1) && j < MAP ## _size(map2)) { \
    int comparison = COMPARE(keys1[i], keys2[j]); \
    if (comparison < 0) { \
      keys3[k] = keys1[i]; \
      values3[k++] = values1[i]; \
    } \
    if (comparison > 0) { \
      keys3[k] = keys2[j]; \
      values3[k++] = values2[j]; \
    } \
    if (comparison <= 0) i++; \
    if (comparison >= 0) j++; \
  } \
  while (i < MAP ## _size(map1)) { \
    keys3[k] = keys1[i]; \
    values3[k++] = values1[i++]; \
  } \
  while (j < MAP ## _size(map2)) { \
    keys3[k] = keys2[j]; \
    values3[k++] = values2[j++]; \
  } \
  MAP *map3 = MAP ## _fromSortedArray(keys3, values3, 0, k); \
  FREE(keys1); \
  FREE(keys2); \
  FREE(keys3); \
  FREE(values1); \
  FREE(values2); \
  FREE(values3); \
  return map3; \
} \
 \
MAP *MAP ## _copy(const MAP *map) { \
  if (!map) return NULL; \
  MAP *copy = ALLOCATE(sizeof(MAP)); \
  copy->key = map->key; \
  copy->value = map->value; \
  copy->height = map->height; \
  copy->size = map->size; \
  copy->left = MAP ## _copy(map->left); \
  copy->right = MAP ## _copy(map->right); \
  return copy; \
} \
 \
void MAP ## _print(const MAP *map) { \
  printf("{"); \
  MAP ## _printRight(map, true); \
  printf("}"); \
} \
 \
MAP *MAP ## _fromArray(const KEY *keys, const VALUE *values, unsigned m, unsigned n) { \
  MAP *map = MAP ## _empty(); \
  for (unsigned i = m; i < n; i++) map = MAP ## _set(map, keys[i], values[i]); \
  return map; \
} \
 \
unsigned MAP ## _copyKeys(MAP *map, KEY *keys, unsigned i) { \
  if (!map) return i; \
  i = MAP ## _copyKeys(map->left, keys, i); \
  keys[i++] = map->key; \
  i = MAP ## _copyKeys(map->right, keys, i); \
  return i; \
} \
 \
KEY *MAP ## _keys(MAP *map) { \
  unsigned size = MAP ## _size(map); \
  KEY *keys = ALLOCATE(size * sizeof(KEY)); \
  MAP ## _copyKeys(map, keys, 0); \
  return keys; \
} \
 \
unsigned MAP ## _copyValues(MAP *map, VALUE *values, unsigned i) { \
  if (!map) return i; \
  i = MAP ## _copyValues(map->left, values, i); \
  values[i++] = map->value; \
  i = MAP ## _copyValues(map->right, values, i); \
  return i; \
} \
 \
VALUE *MAP ## _values(MAP *map) { \
  unsigned size = MAP ## _size(map); \
  VALUE *values = ALLOCATE(size * sizeof(VALUE)); \
  MAP ## _copyValues(map, values, 0); \
  return values; \
} \

