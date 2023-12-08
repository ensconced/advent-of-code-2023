#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool has_n_runs(char *cards, size_t target_run_count, size_t target_run_len) {
  size_t run_count = 0;
  size_t run_len = 0;
  for (size_t i = 0; i < 5; ++i) {
    if (i == 0 || cards[i] == cards[i - 1]) {
      run_len++;
    } else {
      run_len = 1;
    }

    if (run_len == target_run_len) {
      run_len = 0;
      run_count++;
    }
  }
  return run_count >= target_run_count;
}

bool is_full_house(char *cards) {
  char a = '\0';
  char b = '\0';
  for (size_t i = 0; i < 5; ++i) {
    char card = cards[i];
    if (card != b && a == '\0') {
      a = card;
    } else if (card != a && b == '\0') {
      b = card;
    } else if (card != a && card != b) {
      return false;
    }
  }
  return true;
}

HandKind classify_hand(Hand *hand) {
  if (has_n_runs(hand->sorted_cards, 1, 5))
    return FIVE_OF_A_KIND;
  if (has_n_runs(hand->sorted_cards, 1, 4))
    return FOUR_OF_A_KIND;
  if (is_full_house(hand->sorted_cards))
    return FULL_HOUSE;
  if (has_n_runs(hand->sorted_cards, 1, 3))
    return THREE_OF_A_KIND;
  if (has_n_runs(hand->sorted_cards, 2, 2))
    return TWO_PAIR;
  if (has_n_runs(hand->sorted_cards, 1, 2))
    return ONE_PAIR;
  return HIGH_CARD;
}

int card_rank(char card) {
  static const char card_ranks[] = "23456789TJQKA";
  for (size_t i = 0;; ++i) {
    if (card_ranks[i] == card)
      return (int)i;
    if (card_ranks[i] == '\0') {
      printf("failed to find char %c in card ranks\n", card);
      exit(EXIT_FAILURE);
    }
  }
}

int lexicographic_hands_compare(Hand *hand_a, Hand *hand_b) {
  for (size_t i = 0; i < 5; ++i) {
    int a_rank = card_rank(hand_a->cards[i]);
    int b_rank = card_rank(hand_b->cards[i]);
    if (a_rank != b_rank)
      return a_rank - b_rank;
  }
  return 0;
}

int compare_hands(const void *hand_a, const void *hand_b) {
  HandKind hand_a_kind = classify_hand((Hand *)hand_a);
  HandKind hand_b_kind = classify_hand((Hand *)hand_b);
  if (hand_a_kind != hand_b_kind)
    return (int)hand_a_kind - (int)hand_b_kind;
  return lexicographic_hands_compare((Hand *)hand_a, (Hand *)hand_b);
}

int part1(char *input_path) {
  Hands hands = parse_input(input_path);
  qsort(hands.hands, hands.hands_len, sizeof(Hand), compare_hands);
  int result = 0;
  for (size_t i = 0; i < hands.hands_len; ++i) {
    Hand hand = hands.hands[i];
    // printf("cards: %s, sorted_cards: %s, class: ", hand.cards,
    //        hand.sorted_cards);
    // HandKind hand_kind = classify_hand(&hand);
    // switch (hand_kind) {
    // case HIGH_CARD: {
    //   printf("high card\n");
    //   break;
    // }
    // case ONE_PAIR: {
    //   printf("one pair\n");
    //   break;
    // }
    // case TWO_PAIR: {
    //   printf("two pair\n");
    //   break;
    // }
    // case THREE_OF_A_KIND: {
    //   printf("three of a kind\n");
    //   break;
    // }
    // case FULL_HOUSE: {
    //   printf("full house\n");
    //   break;
    // }
    // case FOUR_OF_A_KIND: {
    //   printf("four of a kind\n");
    //   break;
    // }
    // case FIVE_OF_A_KIND: {
    //   printf("five of a kind\n");
    //   break;
    // }
    // }
    result += hand.bid * (int)(i + 1);
  }
  return result;
}
