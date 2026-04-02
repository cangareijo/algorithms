typedef struct {
  void *first;
  void *second;
} Pair;

typedef struct TreeList {
  void *data;
  struct TreeList *left, *right;
  unsigned height;
  unsigned size;
} TreeList;

typedef struct {
  TreeList *left;
  TreeList *right;
} SplitResult;

typedef struct {
  TreeList *stack[64];
  int top;
} TreeListIterator;

TreeListIterator TreeList_begin(TreeList *list);
TreeListIterator TreeList_reverseBegin(TreeList *list);
int TreeListIterator_hasNext(TreeListIterator *iterator);
void *TreeListIterator_next(TreeListIterator *iterator);
void *TreeListIterator_reverseNext(TreeListIterator *iterator);

bool TreeList_isValid(TreeList *list) {
int TreeList_isEmpty(TreeList *list);
unsigned TreeList_size(TreeList *list);
TreeList *TreeList_empty();
TreeList *TreeList_singleton(void *data);
void TreeList_free(TreeList *list);
TreeList *TreeList_clear(TreeList *list);
void *TreeList_get(TreeList *list, unsigned i);
void TreeList_set(TreeList *list, unsigned i, void *data);
TreeList *TreeList_concat(TreeList *left, TreeList *right);
SplitResult TreeList_split(TreeList *list, unsigned i);
TreeList *TreeList_insert(TreeList *list, unsigned i, void *data);
TreeList *TreeList_remove(TreeList *list, unsigned i);
TreeList *TreeList_insertList(TreeList *list, unsigned i, TreeList *other);
TreeList *TreeList_removeRange(TreeList *list, unsigned i, unsigned length);
TreeList *TreeList_push(TreeList *list, void *data);
void *TreeList_peek(TreeList *list);
TreeList *TreeList_pop(TreeList *list);
TreeList *TreeList_pushLeft(TreeList *list, void *data);
void *TreeList_peekLeft(TreeList *list);
TreeList *TreeList_popLeft(TreeList *list);
TreeList *TreeList_slice(TreeList *list, unsigned i, unsigned length);
TreeList *TreeList_copy(TreeList *list);
TreeList *TreeList_reverse(TreeList *list);
TreeList *TreeList_fromArray(void **array, unsigned n);
void **TreeList_toArray(TreeList *list);
TreeList *TreeList_rotate(TreeList *list, int k);
TreeList *TreeList_shuffle(TreeList *list);
TreeList *TreeList_compact(TreeList *list);
TreeList *TreeList_zip(TreeList *list1, TreeList *list2);
TreeList *TreeList_unzipLeft(TreeList *list);
TreeList *TreeList_unzipRight(TreeList *list);
double TreeList_sum(TreeList *list);
double TreeList_product(TreeList *list);
double TreeList_average(TreeList *list);

int TreeList_indexOf(TreeList *list, void *target, int (*compare)(const void *, const void *));
int TreeList_lastIndexOf(TreeList *list, void *target, int (*compare)(const void *, const void *));
void TreeList_replace(TreeList *list, int (*compare)(const void *, const void *), void *target, void *replacement);
bool TreeList_isSorted(TreeList *list, int (*compare)(const void *, const void *));
TreeList *TreeList_sort(TreeList *list, int (*compare)(const void *, const void *));
TreeList *TreeList_elemIndices(TreeList *list, void *target, int (*compare)(const void *, const void *));
void *TreeList_min(TreeList *list, int (*compare)(const void *, const void *));
void *TreeList_max(TreeList *list, int (*compare)(const void *, const void *));
TreeList *TreeList_unique(TreeList *list, int (*compare)(const void *, const void *));
