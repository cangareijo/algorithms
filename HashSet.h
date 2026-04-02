#define HASH_SET_DECLARATION(SET, TYPE) \
 \
typedef struct SET ## Node { \
  TYPE value; \
  struct SET ## Node *next; \
} SET ## Node; \
 \
typedef struct { \
  SET ## Node **buckets; \
  size_t size; \
  size_t capacity; \
} SET; \
 \
bool SET ## _isEmpty(const SET *set); \
bool SET ## _contains(const SET *set, TYPE value); \
bool SET ## _isSubset(const SET *a, const SET *b); \
bool SET ## _areEqual(const SET *a, const SET *b); \
bool SET ## _areDisjoint(const SET *a, const SET *b); \
size_t SET ## _size(const SET *set); \
SET *SET ## _empty(); \
void SET ## _clear(SET *set); \
void SET ## _destroy(SET *set); \
void SET ## _insert(SET *set, TYPE value); \
void SET ## _delete(SET *set, TYPE value); \
SET *SET ## _singleton(TYPE value); \
void SET ## _insertSet(SET *destination, const SET *source); \
void SET ## _insertIntersection(SET *destination, const SET *a, const SET *b); \
void SET ## _insertDifference(SET *destination, const SET *a, const SET *b); \
SET *SET ## _copy(const SET *set); \
SET *SET ## _union(const SET *a, const SET *b); \
SET *SET ## _intersection(const SET *a, const SET *b); \
SET *SET ## _difference(const SET *a, const SET *b); \
SET *SET ## _symmetricDifference(const SET *a, const SET *b); \
void SET ## _print(const SET* set); \
SET *SET ## _fromArray(const TYPE *array, size_t size); \
TYPE *SET ## _toArray(const SET *set); \


#define HASH_SET_DEFINITION(SET, TYPE, ALLOCATE, FREE, HASH, COMPARE, PRINT) \
 \
static void SET ## _resize(SET *set); \
 \
bool SET ## _isEmpty(const SET *set) { \
  return SET ## _size(set) == 0; \
} \
 \
bool SET ## _contains(const SET *set, TYPE value) { \
  size_t index = HASH(value) % set->capacity; \
  SET ## Node *current = set->buckets[index]; \
  while (current) { \
    if (COMPARE(current->value, value) == 0) return true; \
    current = current->next; \
  } \
  return false; \
} \
 \
bool SET ## _isSubset(const SET *a, const SET *b) { \
  for (size_t i = 0; i < a->capacity; i++) { \
    SET ## Node *current = a->buckets[i]; \
    while (current) { \
      if (!SET ## _contains(b, current->value)) return false; \
      current = current->next; \
    } \
  } \
  return true; \
} \
 \
bool SET ## _areEqual(const SET *a, const SET *b) { \
  return SET ## _size(a) == SET ## _size(b) && SET ## _isSubset(a, b); \
} \
 \
bool SET ## _areDisjoint(const SET *a, const SET *b) { \
  for (size_t i = 0; i < a->capacity; i++) { \
    SET ## Node *current = a->buckets[i]; \
    while (current) { \
      if (SET ## _contains(b, current->value)) return false; \
      current = current->next; \
    } \
  } \
  return true; \
} \
 \
size_t SET ## _size(const SET *set) { \
  return set->size; \
} \
 \
SET *SET ## _empty() { \
  SET *set = ALLOCATE(sizeof(SET)); \
  set->size = 0; \
  set->capacity = 4; \
  set->buckets = ALLOCATE(set->capacity * sizeof(SET ## Node *)); \
  for (size_t i = 0; i < set->capacity; i++) set->buckets[i] = NULL; \
  return set; \
} \
 \
void SET ## _clear(SET *set) { \
  for (size_t i = 0; i < set->capacity; i++) { \
    SET ## Node *current = set->buckets[i]; \
    while (current != NULL) { \
      SET ## Node *next = current->next; \
      FREE(current); \
      current = next; \
    } \
  } \
  set->size = 0; \
  SET ## _resize(set); \
} \
 \
void SET ## _destroy(SET *set) { \
  for (size_t i = 0; i < set->capacity; i++) { \
    SET ## Node *current = set->buckets[i]; \
    while (current) { \
      SET ## Node *next = current->next; \
      FREE(current); \
      current = next; \
    } \
  } \
  FREE(set->buckets); \
  FREE(set); \
} \
 \
static void SET ## _resize(SET *set) { \
  size_t capacity = set->capacity; \
  SET ## Node **buckets = set->buckets; \
  set->capacity = set->size * 4; \
  set->buckets = ALLOCATE(set->capacity * sizeof(SET ## Node*)); \
  for (size_t i = 0; i < set->capacity; i++) set->buckets[i] = NULL; \
  for (size_t i = 0; i < capacity; i++) { \
    SET ## Node *current = buckets[i]; \
    while (current) { \
      SET ## Node *next = current->next; \
      size_t index = HASH(current->value) % set->capacity; \
      current->next = set->buckets[index]; \
      set->buckets[index] = current; \
      current = next; \
    } \
  } \
  FREE(buckets); \
} \
 \
void SET ## _insert(SET *set, TYPE value) { \
  if (SET ## _contains(set, value)) return; \
  size_t index = HASH(value) % set->capacity; \
  SET ## Node *node = ALLOCATE(sizeof(SET ## Node)); \
  node->value = value; \
  node->next = set->buckets[index]; \
  set->buckets[index] = node; \
  set->size++; \
  if (2 * set->size >= set->capacity) SET ## _resize(set); \
} \
 \
void SET ## _delete(SET *set, TYPE value) { \
  size_t index = HASH(value) % set->capacity; \
  SET ## Node *current = set->buckets[index]; \
  SET ## Node *previous = NULL; \
  while (current) { \
    if (COMPARE(current->value, value) == 0) { \
      if (previous) previous->next = current->next; \
      else set->buckets[index] = current->next; \
      FREE(current); \
      set->size--; \
      if (set->size > 0 && 8 * set->size <= set->capacity) SET ## _resize(set); \
      return; \
    } \
    previous = current; \
    current = current->next; \
  } \
} \
 \
SET *SET ## _singleton(TYPE value) { \
  SET *set = SET ## _empty(); \
  SET ## _insert(set, value); \
  return set; \
} \
 \
void SET ## _insertSet(SET *destination, const SET *source) { \
  for (size_t i = 0; i < source->capacity; i++) { \
    SET ## Node *current = source->buckets[i]; \
    while (current) { \
      SET ## _insert(destination, current->value); \
      current = current->next; \
    } \
  } \
} \
 \
void SET ## _insertIntersection(SET *destination, const SET *a, const SET *b) { \
  for (size_t i = 0; i < a->capacity; i++) { \
    SET ## Node *current = a->buckets[i]; \
    while (current) { \
      if (SET ## _contains(b, current->value)) SET ## _insert(destination, current->value); \
      current = current->next; \
    } \
  } \
} \
 \
void SET ## _insertSubtraction(SET *destination, const SET *a, const SET *b) { \
  for (size_t i = 0; i < a->capacity; i++) { \
    SET ## Node *current = a->buckets[i]; \
    while (current) { \
      if (!SET ## _contains(b, current->value)) SET ## _insert(destination, current->value); \
      current = current->next; \
    } \
  } \
} \
 \
SET *SET ## _copy(const SET *set) { \
  SET *copy = SET ## _empty(); \
  SET ## _insertSet(copy, set); \
  return copy; \
} \
 \
SET *SET ## _union(const SET *a, const SET *b) { \
  SET *united = SET ## _empty(); \
  SET ## _insertSet(united, a); \
  SET ## _insertSet(united, b); \
  return united; \
} \
 \
SET *SET ## _intersection(const SET *a, const SET *b) { \
  SET *intersection = SET ## _empty(); \
  SET ## _insertIntersection(intersection, a, b); \
  return intersection; \
} \
 \
SET *SET ## _difference(const SET *a, const SET *b) { \
  SET *subtraction = SET ## _empty(); \
  SET ## _insertSubtraction(subtraction, a, b); \
  return subtraction; \
} \
 \
SET *SET ## _symmetricDifference(const SET *a, const SET *b) { \
  SET *symmetric = SET ## _empty(); \
  SET ## _insertSubtraction(symmetric, a, b); \
  SET ## _insertSubtraction(symmetric, b, a); \
  return symmetric; \
} \
 \
void SET ## _print(const SET* set) { \
  printf("{"); \
  bool first = true; \
  for (size_t i = 0; i < set->capacity; i++) { \
    SET ## Node* current = set->buckets[i]; \
    while (current) { \
      if (!first) printf(", "); \
      PRINT(current->value); \
      first = false; \
      current = current->next; \
    } \
  } \
  printf("}\n"); \
} \
 \
SET *SET ## _fromArray(const TYPE *array, size_t size) { \
  SET *set = SET ## _empty(); \
  for (size_t i = 0; i < size; i++) SET ## _insert(set, array[i]); \
  return set; \
} \
 \
TYPE *SET ## _toArray(const SET *set) { \
  if (!set || set->size == 0) return NULL; \
  TYPE *array = ALLOCATE(set->size * sizeof(TYPE)); \
  if (!array) return NULL; \
  size_t index = 0; \
  for (size_t i = 0; i < set->capacity; i++) { \
    SET ## Node *current = set->buckets[i]; \
    while (current) { \
      array[index++] = current->value; \
      current = current->next; \
    } \
  } \
  return array; \
} \

