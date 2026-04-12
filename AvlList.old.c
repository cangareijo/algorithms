void *AvlList_find(AvlList *list, bool (*predicate)(void *));
void *AvlList_findLast(AvlList *list, bool (*predicate)(void *));
int AvlList_findIndex(AvlList *list, bool (*predicate)(void *));
void AvlList_replaceIf(AvlList *list, bool (*predicate)(void *), void *data);



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
  if (predicate(list->data)) return (int)AvlList_size(list->left);
  index = AvlList_findIndex(list->right, predicate);
  if (index != -1) return (int)AvlList_size(list->left) + 1 + index;
  return -1;
}

void AvlList_replaceIf(AvlList *list, bool (*predicate)(void *), void *data) {
  if (!list) return NULL;
  if (predicate(list->data)) list->data = data;
  AvlList_replaceIf(list->left, predicate, data);
  AvlList_replaceIf(list->right, predicate, data);
}
