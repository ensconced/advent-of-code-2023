#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t cards_in_a_hand = 5;
const size_t card_face_count = 13;
const char part1_card_ranks[card_face_count] = "23456789TJQKA";
const char part2_card_ranks[card_face_count] = "J23456789TQKA";

int *count_cards(char *cards) {
  int *counts = calloc(card_face_count, sizeof(int));
  if (counts == 0) {
    printf("failed to allocate counts\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < cards_in_a_hand; ++i) {
    char card = cards[i];
    if (card == 'J')
      continue;
    for (size_t j = 0; j < card_face_count; ++j) {
      if (part1_card_ranks[j] == card) {
        counts[j]++;
        break;
      }
    }
  }
  return counts;
}

char find_most_frequent_non_joker_char(char *cards) {
  int *counted_cards = count_cards(cards);
  char most_frequent_card = '\0';
  int most_frequent_card_count = 0;
  for (size_t i = 0; i < card_face_count; ++i) {
    int count = counted_cards[i];
    char card = part1_card_ranks[i];
    if (count > most_frequent_card_count) {
      most_frequent_card_count = count;
      most_frequent_card = card;
    }
  }
  if (most_frequent_card_count == '\0') {
    // it must be all jokers
    return 'A';
  }
  return most_frequent_card;
}

char *replace_jokers(char *cards) {
  char most_frequent_card = find_most_frequent_non_joker_char(cards);
  char *copy = malloc(cards_in_a_hand + 1);
  if (copy == 0) {
    printf("failed to allocate string copy\n");
    exit(EXIT_FAILURE);
  }
  strcpy(copy, cards);
  for (size_t i = 0; i < cards_in_a_hand; ++i) {
    char card = copy[i];
    if (card == 'J') {
      copy[i] = most_frequent_card;
    }
  }
  return copy;
}

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

int card_rank(char card, bool part2) {
  const char *ranks = part2 ? part2_card_ranks : part1_card_ranks;
  for (size_t i = 0;; ++i) {
    if (ranks[i] == card)
      return (int)i;
    if (ranks[i] == '\0') {
      printf("failed to find char %c in card ranks\n", card);
      exit(EXIT_FAILURE);
    }
  }
}

int lexicographic_hands_compare(Hand *hand_a, Hand *hand_b, bool part2) {
  for (size_t i = 0; i < 5; ++i) {
    int a_rank = card_rank(hand_a->cards[i], part2);
    int b_rank = card_rank(hand_b->cards[i], part2);
    if (a_rank != b_rank)
      return a_rank - b_rank;
  }
  return 0;
}

int compare_hands(const void *hand_a, const void *hand_b, bool part2) {
  HandKind hand_a_kind = classify_hand((Hand *)hand_a);
  HandKind hand_b_kind = classify_hand((Hand *)hand_b);
  if (hand_a_kind != hand_b_kind)
    return (int)hand_a_kind - (int)hand_b_kind;
  return lexicographic_hands_compare((Hand *)hand_a, (Hand *)hand_b, part2);
}

int compare_hands_part1(const void *hand_a, const void *hand_b) {
  return compare_hands(hand_a, hand_b, false);
}

int compare_hands_part2(const void *hand_a, const void *hand_b) {
  return compare_hands(hand_a, hand_b, true);
}

int get_result(char *input_path, bool part2) {
  Hands hands = parse_input(input_path, part2);

  qsort(hands.hands, hands.hands_len, sizeof(Hand),
        part2 ? compare_hands_part2 : compare_hands_part1);
  int result = 0;
  for (size_t i = 0; i < hands.hands_len; ++i) {
    Hand hand = hands.hands[i];
    result += hand.bid * (int)(i + 1);
  }
  return result;
}
