#define HASH_MAP_DECLARATION(MAP, KEY, VALUE) \
 \
typedef struct MAP ## Node { \
  KEY key; \
  VALUE value; \
  struct MAP ## Node *next; \
} MAP ## Node; \
 \
typedef struct { \
  MAP ## Node **buckets; \
  size_t size; \
  size_t capacity; \
} MAP; \
 \
bool MAP ## _isEmpty(const MAP *map); \
bool MAP ## _contains(const MAP *map, const KEY key); \
size_t MAP ## _size(const MAP *map); \
MAP *MAP ## _empty(); \
MAP *MAP ## _singleton(KEY key, VALUE value); \
void MAP ## _clear(MAP *map); \
void MAP ## _destroy(MAP *map); \
VALUE *MAP ## _get(const MAP *map, const KEY key); \
void MAP ## _set(MAP *map, KEY key, VALUE value); \
void MAP ## _delete(MAP *map, const KEY key); \
void MAP ## _insertMap(MAP *destination, const MAP *source); \
MAP *MAP ## _copy(const MAP *map); \
void MAP ## _print(const MAP* map); \
MAP *MAP ## _fromArray(const KEY *keys, const VALUE *values, size_t size); \
KEY *MAP ## _keys(const MAP *map); \
VALUE *MAP ## _values(const MAP *map); \


#define HASH_MAP_DEFINITION(MAP, KEY, VALUE, ALLOCATE, FREE, HASH, COMPARE, PRINT) \
 \
static void MAP ## _resize(MAP *map); \
 \
static void MAP ## _resize(MAP *map) { \
  size_t capacity = map->capacity; \
  MAP ## Node **buckets = map->buckets; \
  map->capacity = map->size > 0 ? map->size * 4 : 4; \
  map->buckets = ALLOCATE(map->capacity * sizeof(MAP ## Node *)); \
  for (size_t i = 0; i < map->capacity; i++) map->buckets[i] = NULL; \
  for (size_t i = 0; i < capacity; i++) { \
    MAP ## Node *current = buckets[i]; \
    while (current != NULL) { \
      MAP ## Node *next = current->next; \
      size_t index = HASH(current->key) % map->capacity; \
      current->next = map->buckets[index]; \
      map->buckets[index] = current; \
      current = next; \
    } \
  } \
  FREE(buckets); \
} \
 \
bool MAP ## _isEmpty(const MAP *map) { \
  return MAP ## _size(map) == 0; \
} \
 \
bool MAP ## _contains(const MAP *map, const KEY key) { \
  size_t index = HASH(key) % map->capacity; \
  MAP ## Node *current = map->buckets[index]; \
  while (current != NULL) { \
    if (COMPARE(current->key, key) == 0) return true; \
    current = current->next; \
  } \
  return false; \
} \
 \
size_t MAP ## _size(const MAP *map) { \
  return map->size; \
} \
 \
MAP *MAP ## _empty() { \
  MAP *map = ALLOCATE(sizeof(MAP)); \
  map->size = 0; \
  map->capacity = 4; \
  map->buckets = ALLOCATE(map->capacity * sizeof(MAP ## Node *)); \
  for (size_t i = 0; i < map->capacity; i++) map->buckets[i] = NULL; \
  return map; \
} \
 \
MAP *MAP ## _singleton(KEY key, VALUE value) { \
  MAP *map = MAP ## _empty(); \
  MAP ## _insert(map, key, value); \
  return map; \
} \
 \
void MAP ## _clear(MAP *map) { \
  for (size_t i = 0; i < map->capacity; i++) { \
    MAP ## Node *current = map->buckets[i]; \
    while (current != NULL) { \
      MAP ## Node *next = current->next; \
      FREE(current); \
      current = next; \
    } \
  } \
  map->size = 0; \
  MAP ## _resize(map); \
} \
 \
void MAP ## _destroy(MAP *map) { \
  for (size_t i = 0; i < map->capacity; i++) { \
    MAP ## Node *current = map->buckets[i]; \
    while (current != NULL) { \
      MAP ## Node *next = current->next; \
      FREE(current); \
      current = next; \
    } \
  } \
  FREE(map->buckets); \
  FREE(map); \
} \
 \
VALUE *MAP ## _get(const MAP *map, const KEY key) { \
  size_t index = HASH(key) % map->capacity; \
  MAP ## Node *current = map->buckets[index]; \
  while (current != NULL) { \
    if (COMPARE(current->key, key) == 0) return &current->value; \
    current = current->next; \
  } \
  return NULL; \
} \
 \
void MAP ## _set(MAP *map, KEY key, VALUE value) { \
  size_t index = HASH(key) % map->capacity; \
  MAP ## Node *current = map->buckets[index]; \
  while (current != NULL) { \
    if (COMPARE(current->key, key) == 0) { \
      current->key = key; \
      current->value = value; \
      return; \
    } \
    current = current->next; \
  } \
  MAP ## Node *node = ALLOCATE(sizeof(MAP ## Node)); \
  node->key = key; \
  node->value = value; \
  node->next = map->buckets[index]; \
  map->buckets[index] = node; \
  map->size++; \
  if (2 * map->size >= map->capacity) MAP ## _resize(map); \
} \
 \
void MAP ## _delete(MAP *map, const KEY key) { \
  size_t index = HASH(key) % map->capacity; \
  MAP ## Node *current = map->buckets[index]; \
  MAP ## Node *previous = NULL; \
  while (current != NULL) { \
    if (COMPARE(current->key, key) == 0) { \
      if (previous != NULL) previous->next = current->next; \
      else map->buckets[index] = current->next; \
      FREE(current); \
      map->size--; \
      if (map->size > 0 && 8 * map->size <= map->capacity) MAP ## _resize(map); \
      return; \
    } \
    previous = current; \
    current = current->next; \
  } \
} \
 \
void MAP ## _insertMap(MAP *destination, const MAP *source) { \
  for (size_t i = 0; i < source->capacity; i++) { \
    MAP ## Node *current = source->buckets[i]; \
    while (current) { \
      MAP ## _insert(destination, current->key, current->value); \
      current = current->next; \
    } \
  } \
} \
 \
MAP *MAP ## _copy(const MAP *map) { \
  MAP *copy = MAP ## _empty(); \
  MAP ## _insertMap(copy, map); \
  return copy; \
} \
 \
void MAP ## _print(const MAP* map) { \
  printf("{"); \
  bool first = true; \
  for (size_t i = 0; i < map->capacity; i++) { \
    MAP ## Node* current = map->buckets[i]; \
    while (current) { \
      if (!first) printf(", "); \
      PRINT(current->key); \
      printf(": "); \
      PRINT(current->value); \
      first = false; \
      current = current->next; \
    } \
  } \
  printf("}\n"); \
} \
 \
MAP *MAP ## _fromArray(const KEY *keys, const VALUE *values, size_t size) { \
  MAP *map = MAP ## _empty(); \
  for (size_t i = 0; i < size; i++) MAP ## _insert(map, keys[i], values[i]); \
  return map; \
} \
 \
KEY *MAP ## _keys(const MAP *map) { \
  if (map == NULL || map->size == 0) return NULL; \
  KEY *keys = ALLOCATE(map->size * sizeof(KEY)); \
  if (keys == NULL) return NULL; \
  size_t index = 0; \
  for (size_t i = 0; i < map->capacity; i++) { \
    MAP ## Node *current = map->buckets[i]; \
    while (current != NULL) { \
      keys[index++] = current->key; \
      current = current->next; \
    } \
  } \
  return keys; \
} \
 \
VALUE *MAP ## _values(const MAP *map) { \
  if (map == NULL || map->size == 0) return NULL; \
  VALUE *values = ALLOCATE(map->size * sizeof(VALUE)); \
  if (values == NULL) return NULL; \
  size_t index = 0; \
  for (size_t i = 0; i < map->capacity; i++) { \
    MAP ## Node *current = map->buckets[i]; \
    while (current != NULL) { \
      values[index++] = current->value; \
      current = current->next; \
    } \
  } \
  return values; \
} \

