#include <stdbool.h>

typedef struct AvlList {
  void *data;
  struct AvlList *left, *right;
  unsigned height;
  unsigned size;
} AvlList;

typedef struct {
  AvlList *stack[64];
  int top;
} AvlListIterator;

AvlListIterator AvlList_begin(AvlList *list);
AvlListIterator AvlList_reverseBegin(AvlList *list);
int AvlListIterator_hasNext(AvlListIterator *iterator);
void *AvlListIterator_get(AvlListIterator *iterator);
void AvlListIterator_next(AvlListIterator *iterator);
void AvlListIterator_reverseNext(AvlListIterator *iterator);

bool AvlList_isValid(AvlList *list);
bool AvlList_isEmpty(AvlList *list);
unsigned AvlList_size(AvlList *list);
AvlList *AvlList_empty();
AvlList *AvlList_single(void *data);
AvlList *AvlList_fromArray(void **array, unsigned n);
void **AvlList_toArray(AvlList *list);
AvlList *AvlList_copy(AvlList *list);
void AvlList_free(AvlList *list);
void AvlList_reverse(AvlList *list);
void *AvlList_get(AvlList *list, unsigned i);
void AvlList_set(AvlList *list, unsigned i, void *data);
AvlList *AvlList_concat(AvlList *left, AvlList *right);
void AvlList_split(AvlList *list, unsigned i, AvlList **left, AvlList **right);

AvlList *AvlList_clear(AvlList *list);
AvlList *AvlList_replicate(AvlList *list, unsigned n);
AvlList *AvlList_insertList(AvlList *list, unsigned i, AvlList *other);
AvlList *AvlList_removeRange(AvlList *list, unsigned i, unsigned length);
AvlList *AvlList_slice(AvlList *list, unsigned i, unsigned length);
AvlList *AvlList_rotate(AvlList *list, int i);
AvlList *AvlList_compact(AvlList *list);
AvlList *AvlList_shuffle(AvlList *list);
AvlList *AvlList_zip(AvlList *list1, AvlList *list2);
AvlList *AvlList_unzipLeft(AvlList *list);
AvlList *AvlList_unzipRight(AvlList *list);

AvlList *AvlList_repeat(void *data, unsigned n);
AvlList *AvlList_insert(AvlList *list, unsigned i, void *data);
AvlList *AvlList_remove(AvlList *list, unsigned i);
AvlList *AvlList_push(AvlList *list, void *data);
void *AvlList_peek(AvlList *list);
AvlList *AvlList_pop(AvlList *list);
AvlList *AvlList_pushLeft(AvlList *list, void *data);
void *AvlList_peekLeft(AvlList *list);
AvlList *AvlList_popLeft(AvlList *list);

double AvlList_sum(AvlList *list);
double AvlList_product(AvlList *list);
double AvlList_average(AvlList *list);

int AvlList_indexOf(AvlList *list, void *target, int (*compare)(const void *, const void *));
int AvlList_lastIndexOf(AvlList *list, void *target, int (*compare)(const void *, const void *));
void AvlList_replace(AvlList *list, int (*compare)(const void *, const void *), void *target, void *replacement);
bool AvlList_isSorted(AvlList *list, int (*compare)(const void *, const void *));
AvlList *AvlList_sort(AvlList *list, int (*compare)(const void *, const void *));
AvlList *AvlList_elemIndices(AvlList *list, void *target, int (*compare)(const void *, const void *));
void *AvlList_min(AvlList *list, int (*compare)(const void *, const void *));
void *AvlList_max(AvlList *list, int (*compare)(const void *, const void *));
AvlList *AvlList_unique(AvlList *list, int (*compare)(const void *, const void *));
