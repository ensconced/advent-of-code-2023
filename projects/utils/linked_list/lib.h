#pragma once
#include <stddef.h>

typedef struct LinkedListNode LinkedListNode;
struct LinkedListNode {
  void *data;
  LinkedListNode *prev;
  LinkedListNode *next;
};

typedef struct {
  LinkedListNode *head;
} LinkedList;

LinkedListNode *find_tail(LinkedList *list);
void append(LinkedList *list, LinkedListNode *new_tail);
void remove_node(LinkedList *list, LinkedListNode *node);
void add_at_head(LinkedList *list, LinkedListNode *new_node);
size_t length(LinkedList *list);
