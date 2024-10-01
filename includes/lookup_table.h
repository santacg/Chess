#ifndef LOOKUP_TABLE_H
#define LOOKUP_TABLE_H

#include "utils.h"
#include <bitset>

using namespace std;

typedef struct {
  bitset<64> clear_rank[RANKS];
  bitset<64> clear_file[FILES];
  bitset<64> mask_rank[RANKS];
  bitset<64> mask_file[FILES];
  bitset<64> mask_diagonal[DIAGONALS + 1];
  bitset<64> mask_antidiagonal[DIAGONALS + 1];
  bitset<64> piece_lookup[SQUARES];
  bitset<64> mask_first_rank_attacks[SQUARES * RANKS];
} LookupTable;

LookupTable *init_lookup_table();

#endif
