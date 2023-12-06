#include "./lib.h"

LinkedListNode *find_tail(LinkedListNode *list_head) {
  while (list_head->next) {
    list_head = list_head->next;
  }
  return list_head;
}

void append(LinkedListNode *list_head, LinkedListNode *new_tail) {
  LinkedListNode *old_tail = find_tail(list_head);
  new_tail->prev = old_tail;
  old_tail->next = new_tail;
}

void remove_node(LinkedListNode *node) {
  if (node->prev) {
    node->prev->next = node->next;
  }
  if (node->next) {
    node->next->prev = node->prev;
  }
}

void insert_after(LinkedListNode *reference_node, LinkedListNode *new_node) {
  LinkedListNode *reference_next = reference_node->next;
  reference_node->next = new_node;
  new_node->next = reference_next;
  new_node->prev = reference_node;
  if (reference_next) {
    reference_next->prev = new_node;
  }
}
