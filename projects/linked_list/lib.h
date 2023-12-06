typedef struct LinkedListNode LinkedListNode;
struct LinkedListNode {
  void *data;
  LinkedListNode *prev;
  LinkedListNode *next;
};

LinkedListNode *find_tail(LinkedListNode *list_head);
void append(LinkedListNode *list_head, LinkedListNode *new_tail);
void remove_node(LinkedListNode *node);
void insert_after(LinkedListNode *reference_node, LinkedListNode *new_node);
