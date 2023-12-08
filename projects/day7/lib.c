#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
ptr	-	pointer to the array to sort
count	-	number of elements in the array
size	-	size of each element in the array in bytes
comp	-	comparison function which returns ​a negative integer value if
the first argument is less than the second, a positive integer value if the
first argument is greater than the second and zero if the arguments are
equivalent. The signature of the comparison function should be equivalent to the
following:

 int cmp(const void *a, const void *b);

The function must not modify the objects passed to it and must return consistent
results when called for the same objects, regardless of their positions in the
array.
*/

bool is_five_of_a_kind(char *cards) {
  for (size_t i = 0; i < 5; ++i) {
    if (i == 0)
      continue;
    if (cards[i] != cards[i - 1])
      return false;
  }
  return true;
}

bool is_four_of_a_kind(char *cards) {
  int not_matching = 0;
  for (size_t i = 0; i < 5; ++i) {
    if (i == 0)
      continue;
    if (cards[i] != cards[i - 1]) {
      not_matching++;
      if (not_matching > 1)
        return false;
    }
  }
  return true;
}

bool is_full_house(char *cards) {
  char a = '\0';
  char b = '\0';
  for (size_t i = 0; i < 5; ++i) {
    char card = cards[i];
    if (a == '\0') {
      a = card;
    } else if (b == '\0') {
      b = card;
    } else if (card != a && card != b) {
      return false;
    }
  }
  return true;
}

bool is_three_of_a_kind(char *cards) {
  char seen_cards[] = {0, 0, 0};
  char seen_card_counts[] = {0, 0, 0};

  for (size_t i = 0; i < 5; ++i) {
    bool placed = false;
    char card = cards[i];
    for (size_t j = 0; j < 3; ++j) {
      if (card == seen_cards[j]) {
        seen_card_counts[j]++;
        placed = true;
        if (seen_card_counts[j] >= 3)
          return true;
      }
    }
    if (!placed) {
      for (size_t j = 0; j < 3; ++j) {
        if (seen_cards[j] == 0) {
          seen_cards[j] = card;
          seen_card_counts[j]++;
          break;
        }
      }
    }
  }
  return false;
}

bool is_two_pair(char *cards) {
  char seen_cards[] = {0, 0, 0};
  char seen_card_counts[] = {0, 0, 0};

  for (size_t i = 0; i < 5; ++i) {
    bool placed = false;
    char card = cards[i];
    for (size_t j = 0; j < 3; ++j) {
      if (card == seen_cards[j]) {
        seen_card_counts[j]++;
        placed = true;
      }
    }
    if (!placed) {
      for (size_t j = 0; j < 3; ++j) {
        if (seen_cards[j] == 0) {
          seen_cards[j] = card;
          seen_card_counts[j]++;
          break;
        }
      }
    }
  }
  int pairs = 0;
  for (size_t j = 0; j < 3; ++j) {
    if (seen_card_counts[j] >= 2) {
      pairs++;
    }
  }
  return pairs >= 2;
}

bool is_one_pair(char *cards) {
  int pair_count = 0;
  char pair_card = '\0';
  for (size_t i = 0; i < 5; ++i) {
    char card = cards[i];
    if (pair_card == '\0') {
      pair_card = card;
      pair_count++;
    } else if (card == pair_card) {
      pair_count++;
    }
  }
  return pair_count >= 2;
}

HandKind classify_hand(Hand *hand) {
  if (is_five_of_a_kind(hand->cards))
    return FIVE_OF_A_KIND;
  if (is_four_of_a_kind(hand->cards))
    return FOUR_OF_A_KIND;
  if (is_full_house(hand->cards))
    return FULL_HOUSE;
  if (is_three_of_a_kind(hand->cards))
    return THREE_OF_A_KIND;
  if (is_two_pair(hand->cards))
    return TWO_PAIR;
  if (is_one_pair(hand->cards))
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
    printf("hand: %s, class: %d\n", hand.cards, classify_hand(&hand));
    result += hand.bid * (int)(i + 1);
  }
  return result;
}
