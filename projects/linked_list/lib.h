typedef struct LinkedListNode LinkedListNode;
struct LinkedListNode {
  void *data;
  LinkedListNode *prev;
  LinkedListNode *next;
};

LinkedListNode *find_tail(LinkedListNode *list_head);
void append(LinkedListNode *list_head, LinkedListNode *new_tail);
void remove(LinkedListNode *node);
