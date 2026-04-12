void AvlList_replaceIf(AvlList *list, bool (*predicate)(void *), void *data);



void AvlList_replaceIf(AvlList *list, bool (*predicate)(void *), void *data) {
  if (!list) return NULL;
  if (predicate(list->data)) list->data = data;
  AvlList_replaceIf(list->left, predicate, data);
  AvlList_replaceIf(list->right, predicate, data);
}
