#include "./lib.h"
#include <assert.h>

void test_append_and_find_tail(void) {
  int one = 1;
  int two = 2;
  int three = 3;
  LinkedListNode head = {.data = &one, .next = 0, .prev = 0};
  LinkedListNode body = {.data = &two, .next = 0, .prev = 0};
  LinkedListNode tail = {.data = &three, .next = 0, .prev = 0};
  append(&head, &body);
  append(&head, &tail);

  assert(find_tail(&head) == &tail);

  assert(head.prev == 0);
  assert(head.next == &body);
  assert(body.prev == &head);
  assert(body.next == &tail);
  assert(tail.prev == &body);
  assert(tail.next == 0);
}

void test_remove_body(void) {
  int one = 1;
  int two = 2;
  int three = 3;
  LinkedListNode head = {.data = &one, .next = 0, .prev = 0};
  LinkedListNode body = {.data = &two, .next = 0, .prev = 0};
  LinkedListNode tail = {.data = &three, .next = 0, .prev = 0};
  append(&head, &body);
  append(&head, &tail);

  remove(&body);
  assert(head.next == &tail);
  assert(tail.prev == &head);
}

void test_remove_head(void) {
  int one = 1;
  int two = 2;
  int three = 3;
  LinkedListNode head = {.data = &one, .next = 0, .prev = 0};
  LinkedListNode body = {.data = &two, .next = 0, .prev = 0};
  LinkedListNode tail = {.data = &three, .next = 0, .prev = 0};
  append(&head, &body);
  append(&head, &tail);

  remove(&head);
  assert(body.prev == 0);
  assert(body.next == &tail);
  assert(tail.prev == &body);
  assert(tail.next == 0);
}

void test_remove_tail(void) {
  int one = 1;
  int two = 2;
  int three = 3;
  LinkedListNode head = {.data = &one, .next = 0, .prev = 0};
  LinkedListNode body = {.data = &two, .next = 0, .prev = 0};
  LinkedListNode tail = {.data = &three, .next = 0, .prev = 0};
  append(&head, &body);
  append(&head, &tail);

  remove(&tail);
  assert(head.prev == 0);
  assert(head.next == &body);
  assert(body.prev == &head);
  assert(body.next == 0);
}

int main(void) {
  test_append_and_find_tail();
  test_remove_body();
  test_remove_head();
  test_remove_tail();
}
