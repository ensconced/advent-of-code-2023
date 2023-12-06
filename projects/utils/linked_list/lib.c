#include "./lib.h"
#include <stddef.h>
#include <stdio.h>

LinkedListNode *find_tail(LinkedList *list) {
  LinkedListNode *current_node = list->head;
  while (current_node && current_node->next) {
    current_node = current_node->next;
  }
  return current_node;
}

void append(LinkedList *list, LinkedListNode *new_tail) {
  LinkedListNode *old_tail = find_tail(list);
  if (old_tail == 0) {
    list->head = new_tail;
  } else {
    new_tail->prev = old_tail;
    old_tail->next = new_tail;
  }
}

void remove_node(LinkedList *list, LinkedListNode *node) {
  if (node->prev) {
    node->prev->next = node->next;
  } else {
    list->head = node->next;
  }
  if (node->next) {
    node->next->prev = node->prev;
  }
}

void add_at_head(LinkedList *list, LinkedListNode *new_node) {
  LinkedListNode *old_head = list->head;
  new_node->next = old_head;
  if (old_head) {
    old_head->prev = new_node;
  }
  list->head = new_node;
}

size_t length(LinkedList *list) {
  size_t result = 0;
  LinkedListNode *current_node = list->head;
  while (current_node) {
    result++;
    current_node = current_node->next;
  }
  return result;
}
