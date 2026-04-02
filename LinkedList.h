#define LINKED_LIST_DECLARATION(LIST, TYPE) \
 \
typedef struct LIST ## Node { \
  TYPE value; \
  struct LIST ## Node *previous, *next; \
} LIST ## Node; \
 \
unsigned LIST ## Node_size(const LIST ## Node *node); \
bool LIST ## Node_isLinked(const LIST ## Node *node); \
const LIST ## Node *LIST ## Node_last(const LIST ## Node *node); \
 \
typedef struct { \
  LIST ## Node *front, *rear; \
  unsigned size; \
} LIST; \
 \
bool LIST ## _isValid(const LIST *list); \
LIST *LIST ## _empty(); \
void LIST ## _destroy(LIST *list); \
LIST *LIST ## _clear(LIST *list); \
unsigned LIST ## _size(const LIST *list); \
bool LIST ## _isEmpty(const LIST *list); \
void LIST ## _push(LIST *list, TYPE value); \
void LIST ## _pushLeft(LIST *list, TYPE value); \
TYPE *LIST ## _peek(const LIST *list); \
TYPE *LIST ## _peekLeft(const LIST *list); \
void LIST ## _pop(LIST *list); \
void LIST ## _popLeft(LIST *list); \
TYPE *LIST ## _get(const LIST *list, unsigned index); \
int LIST ## _find(const LIST *list, TYPE value); \
unsigned LIST ## _count(const LIST *list, TYPE value); \
void LIST ## _concatenate(LIST *list1, LIST *list2); \
void LIST ## _pushArray(LIST *list, const TYPE *array, unsigned start, unsigned end); \
void LIST ## _pushList(LIST *destination, const LIST *source, unsigned start, unsigned end); \
LIST *LIST ## _set(const LIST *list, unsigned i, TYPE value); \
LIST *LIST ## _insert(const LIST *list, unsigned i, TYPE value); \
LIST *LIST ## _delete(const LIST *list, unsigned i); \
LIST *LIST ## _remove(const LIST *list, TYPE value); \
LIST *LIST ## _slice(const LIST *list, unsigned start, unsigned end); \
LIST *LIST ## _copy(const LIST *list); \
LIST *LIST ## _reverse(const LIST *list); \
LIST *LIST ## _repeat(const LIST *list, unsigned n); \


#define LINKED_LIST_DEFINITION(LIST, TYPE, ALLOCATE, FREE, COMPARE) \
 \
unsigned LIST ## Node_size(const LIST ## Node *node) { \
  if (node) return 1 + LIST ## Node_size(node->next); \
  return 0; \
} \
 \
bool LIST ## Node_isLinked(const LIST ## Node *node) { \
  return \
    !node || \
    ((!node->previous || node->previous->next == node) && \
    (!node->next || node->next->previous == node) && \
    LIST ## Node_isLinked(node->next)); \
} \
 \
const LIST ## Node *LIST ## Node_last(const LIST ## Node *node) { \
  if (!node || !node->next) return node; \
  return LIST ## Node_last(node->next); \
} \
 \
bool LIST ## _isValid(const LIST *list) { \
  return \
    list && \
    (list->size == 0 && !list->front && !list->rear || \
    list->size > 0 && list->front && list->rear && !list->front->previous && !list->rear->next); \
} \
 \
bool LIST ## _isValidLinear(const LIST *list) { \
  return \
    LIST ## _isValid(list) && \
    LIST ## Node_size(list->front) == list->size && \
    LIST ## Node_isLinked(list->front) && \
    LIST ## Node_last(list->front) == list->rear; \
} \
 \
LIST *LIST ## _empty() { \
  LIST *list = ALLOCATE(sizeof(LIST)); \
  list->front = list->rear = NULL; \
  list->size = 0; \
  return list; \
} \
 \
void LIST ## _destroy(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  LIST ## Node *current = list->front; \
  while (current) { \
    LIST ## Node *next = current->next; \
    FREE(current); \
    current = next; \
  } \
  FREE(list); \
} \
 \
LIST *LIST ## _clear(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  LIST ## _destroy(list); \
  return LIST ## _empty(); \
} \
 \
unsigned LIST ## _size(const LIST *list) { \
  assert(LIST ## _isValid(list)); \
  return list->size; \
} \
 \
bool LIST ## _isEmpty(const LIST *list) { \
  assert(LIST ## _isValid(list)); \
  return list->size == 0; \
} \
 \
void LIST ## _push(LIST *list, TYPE value) { \
  assert(LIST ## _isValid(list)); \
  LIST ## Node *node = ALLOCATE(sizeof(LIST ## Node)); \
  node->value = value; \
  node->next = NULL; \
  node->previous = list->rear; \
  if (list->rear) list->rear->next = node; \
  else list->front = node; \
  list->rear = node; \
  list->size++; \
} \
 \
void LIST ## _pushLeft(LIST *list, TYPE value) { \
  assert(LIST ## _isValid(list)); \
  LIST ## Node *node = ALLOCATE(sizeof(LIST ## Node)); \
  node->value = value; \
  node->previous = NULL; \
  node->next = list->front; \
  if (list->front) list->front->previous = node; \
  else list->rear = node; \
  list->front = node; \
  list->size++; \
} \
 \
TYPE *LIST ## _peek(const LIST *list) { \
  assert(LIST ## _isValid(list)); \
  if (!list->rear) return NULL; \
  return &list->rear->value; \
} \
 \
TYPE *LIST ## _peekLeft(const LIST *list) { \
  assert(LIST ## _isValid(list)); \
  if (!list->front) return NULL; \
  return &list->front->value; \
} \
 \
void LIST ## _pop(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  assert(list->rear); \
  LIST ## Node *node = list->rear; \
  list->rear = node->previous; \
  if (list->rear) list->rear->next = NULL; \
  else list->front = NULL; \
  FREE(node); \
  list->size--; \
} \
 \
void LIST ## _popLeft(LIST *list) { \
  assert(LIST ## _isValid(list)); \
  assert(list->front); \
  LIST ## Node *node = list->front; \
  list->front = node->next; \
  if (list->front) list->front->previous = NULL; \
  else list->rear = NULL; \
  FREE(node); \
  list->size--; \
} \
 \
TYPE *LIST ## _get(const LIST *list, unsigned index) { \
  assert(LIST ## _isValidLinear(list)); \
  LIST ## Node *current = list->front; \
  for (unsigned i = 0; i < index && current; i++) current = current->next; \
  if (!current) return NULL; \
  return &current->value; \
} \
 \
int LIST ## _find(const LIST *list, TYPE value) { \
  assert(LIST ## _isValidLinear(list)); \
  int i = 0; \
  for (LIST ## Node *current = list->front; current; current = current->next) { \
    if (COMPARE(current->value, value) == 0) return i; \
    i++; \
  } \
  return -1; \
} \
 \
unsigned LIST ## _count(const LIST *list, TYPE value) { \
  assert(LIST ## _isValidLinear(list)); \
  unsigned count = 0; \
  for (LIST ## Node *current = list->front; current; current = current->next) if (COMPARE(current->value, value) == 0) count++; \
  return count; \
} \
 \
void LIST ## _concatenate(LIST *list1, LIST *list2) { \
  assert(LIST ## _isValid(list1)); \
  assert(LIST ## _isValid(list2)); \
  if (list1->size == 0) { \
    list1->front = list2->front; \
    list1->rear = list2->rear; \
    list1->size = list2->size; \
  } else if (list2->size > 0) { \
    list1->rear->next = list2->front; \
    list2->front->previous = list1->rear; \
    list1->rear = list2->rear; \
    list1->size += list2->size; \
  } \
  list2->front = NULL; \
  list2->rear = NULL; \
  list2->size = 0; \
} \
 \
void LIST ## _pushArray(LIST *list, const TYPE *array, unsigned start, unsigned end) { \
  assert(LIST ## _isValidLinear(list)); \
  if (!array) return; \
  for (unsigned i = start; i < end; i++) LIST ## _push(list, array[i]); \
} \
 \
void LIST ## _pushList(LIST *destination, const LIST *source, unsigned start, unsigned end) { \
  assert(LIST ## _isValidLinear(destination)); \
  assert(LIST ## _isValidLinear(source)); \
  LIST ## Node *current = source->front; \
  for (unsigned i = 0; i < start && current; i++) current = current->next; \
  for (unsigned i = start; i < end && current; i++) { \
    LIST ## _push(destination, current->value); \
    current = current->next; \
  } \
} \
 \
LIST *LIST ## _set(const LIST *list, unsigned i, TYPE value) { \
  assert(LIST ## _isValidLinear(list)); \
  LIST *inserted = LIST ## _empty(); \
  LIST ## _pushList(inserted, list, 0, i); \
  LIST ## _push(inserted, value); \
  LIST ## _pushList(inserted, list, i + 1, LIST ## _size(list)); \
  return inserted; \
} \
 \
LIST *LIST ## _insert(const LIST *list, unsigned i, TYPE value) { \
  assert(LIST ## _isValidLinear(list)); \
  LIST *inserted = LIST ## _empty(); \
  LIST ## _pushList(inserted, list, 0, i); \
  LIST ## _push(inserted, value); \
  LIST ## _pushList(inserted, list, i, LIST ## _size(list)); \
  return inserted; \
} \
 \
LIST *LIST ## _delete(const LIST *list, unsigned i) { \
  assert(LIST ## _isValidLinear(list)); \
  LIST *deleted = LIST ## _empty(); \
  LIST ## _pushList(deleted, list, 0, i); \
  LIST ## _pushList(deleted, list, i + 1, LIST ## _size(list)); \
  return deleted; \
} \
 \
LIST *LIST ## _remove(const LIST *list, TYPE value) { \
  assert(LIST ## _isValidLinear(list)); \
  int i = LIST ## _find(list, value); \
  if (i < 0) return LIST ## _copy(list); \
  return LIST ## _delete(list, i); \
} \
 \
LIST *LIST ## _slice(const LIST *list, unsigned start, unsigned end) { \
  assert(LIST ## _isValidLinear(list)); \
  LIST *slice = LIST ## _empty(); \
  LIST ## _pushList(slice, list, start, end); \
  return slice; \
} \
 \
LIST *LIST ## _copy(const LIST *list) { \
  assert(LIST ## _isValidLinear(list)); \
  return LIST ## _slice(list, 0, LIST ## _size(list)); \
} \
 \
LIST *LIST ## _reverse(const LIST *list) { \
  assert(LIST ## _isValidLinear(list)); \
  LIST *reverse = LIST ## _empty(); \
  for (LIST ## Node *current = list->front; current; current = current->next) LIST ## _pushLeft(reverse, current->value); \
  return reverse; \
} \
 \
LIST *LIST ## _repeat(const LIST *list, unsigned n) { \
  assert(LIST ## _isValidLinear(list)); \
  LIST *reapeated = LIST ## _empty(); \
  for (unsigned i = 0; i < n; i++) LIST ## _pushList(reapeated, list, 0, LIST ## _size(list)); \
  return reapeated; \
} \
 \
LIST *LIST ## _rotate(const LIST *list, int n) { \
  assert(LIST ## _isValidLinear(list)); \
  n = (n % LIST ## _size(list) + LIST ## _size(list)) % LIST ## _size(list); \
  LIST *rotated = LIST ## _empty(); \
  LIST ## _pushList(rotated, list, n, LIST ## _size(list)); \
  LIST ## _pushList(rotated, list, 0, n); \
  return rotated; \
} \

