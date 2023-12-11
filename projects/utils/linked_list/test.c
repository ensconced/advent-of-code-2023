#include "./lib.h"
#include <assert.h>
#include <stdio.h>

void test_basics(void) {
  LinkedList list = {.head = 0};

  int one = 1;
  int two = 2;
  int three = 3;
  LinkedListNode head = {.data = &one, .next = 0, .prev = 0};
  LinkedListNode body = {.data = &two, .next = 0, .prev = 0};
  LinkedListNode tail = {.data = &three, .next = 0, .prev = 0};
  append(&list, &head);
  append(&list, &body);
  append(&list, &tail);

  assert(find_tail(&list) == &tail);

  assert(list.head == &head);
  assert(head.prev == 0);
  assert(head.next == &body);
  assert(body.prev == &head);
  assert(body.next == &tail);
  assert(tail.prev == &body);
  assert(tail.next == 0);
  assert(length(&list) == 3);
}

void test_remove_body(void) {
  LinkedList list = {.head = 0};
  int one = 1;
  int two = 2;
  int three = 3;
  LinkedListNode head = {.data = &one, .next = 0, .prev = 0};
  LinkedListNode body = {.data = &two, .next = 0, .prev = 0};
  LinkedListNode tail = {.data = &three, .next = 0, .prev = 0};
  append(&list, &head);
  append(&list, &body);
  append(&list, &tail);

  remove_node(&list, &body);
  assert(length(&list) == 2);
  assert(head.next == &tail);
  assert(tail.prev == &head);
}

void test_remove_head(void) {
  LinkedList list = {.head = 0};
  int one = 1;
  int two = 2;
  int three = 3;
  LinkedListNode head = {.data = &one, .next = 0, .prev = 0};
  LinkedListNode body = {.data = &two, .next = 0, .prev = 0};
  LinkedListNode tail = {.data = &three, .next = 0, .prev = 0};
  append(&list, &head);
  append(&list, &body);
  append(&list, &tail);

  remove_node(&list, &head);
  assert(length(&list) == 2);
  assert(body.prev == 0);
  assert(body.next == &tail);
  assert(tail.prev == &body);
  assert(tail.next == 0);
}

void test_remove_tail(void) {
  LinkedList list = {.head = 0};
  int one = 1;
  int two = 2;
  int three = 3;
  LinkedListNode head = {.data = &one, .next = 0, .prev = 0};
  LinkedListNode body = {.data = &two, .next = 0, .prev = 0};
  LinkedListNode tail = {.data = &three, .next = 0, .prev = 0};
  append(&list, &head);
  append(&list, &body);
  append(&list, &tail);

  remove_node(&list, &tail);
  assert(length(&list) == 2);
  assert(head.prev == 0);
  assert(head.next == &body);
  assert(body.prev == &head);
  assert(body.next == 0);
}

void test_add_at_head(void) {
  LinkedList list = {.head = 0};
  int one = 1;
  int two = 2;
  int three = 3;
  int four = 4;
  LinkedListNode head = {.data = &one, .next = 0, .prev = 0};
  LinkedListNode body = {.data = &two, .next = 0, .prev = 0};
  LinkedListNode tail = {.data = &three, .next = 0, .prev = 0};
  LinkedListNode new_node = {.data = &four, .next = 0, .prev = 0};
  append(&list, &head);
  append(&list, &body);
  append(&list, &tail);

  add_at_head(&list, &new_node);
  assert(length(&list) == 4);
  assert(list.head == &new_node);
  assert(new_node.next == &head);
  assert(new_node.prev == 0);
  assert(head.prev == &new_node);
  assert(head.next == &body);
  assert(body.prev == &head);
  assert(body.next == &tail);
  assert(tail.next == 0);
  assert(tail.prev == &body);
}

int main(void) {
  printf("testing linked list lib\n");
  test_basics();
  test_remove_body();
  test_remove_head();
  test_remove_tail();
  test_add_at_head();
}
