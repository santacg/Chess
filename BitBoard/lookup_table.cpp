#include "lookup_table.h"
#include <bit>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#define A_FILE 0x0101010101010101;
#define H_FILE 0x8080808080808080;
#define FIRST_RANK 0x00000000000000FF;
#define EIGHTH_RANK 0xFF00000000000000;
#define MAIN_DIAGONAL 0x8040201008040201;
#define MAIN_ANTIDIAGONAL 0x0102040810204080;

void init_diagonals(LookupTable *lut);

void init_files(LookupTable *lut);

void init_ranks(LookupTable *lut);

void init_squares(LookupTable *lut);

int generate_rank_attack(int o, int f);

void init_first_rank_attacks(LookupTable *lut);

LookupTable *init_lookup_table() {
  LookupTable *lookup_table = NULL;

  lookup_table = (LookupTable *)malloc(sizeof(LookupTable));
  if (lookup_table == NULL)
    return NULL;

  init_squares(lookup_table);

  init_files(lookup_table);

  init_ranks(lookup_table);

  init_diagonals(lookup_table);

  init_first_rank_attacks(lookup_table);

  return lookup_table;
}

void init_squares(LookupTable *lut) {
  for (int i = 0; i < SQUARES; i++) {
    lut->piece_lookup[i].reset();
    lut->piece_lookup[i].set(i, true);
  }
}

void init_files(LookupTable *lut) {
  int i;

  bitset<64> file = A_FILE;

  for (i = 0; i < FILES; i++) {
    lut->mask_file[i] = file;
    lut->clear_file[i] = ~file;
    file <<= 1;
  }
}

void init_ranks(LookupTable *lut) {
  int i;

  bitset<64> rank = FIRST_RANK;

  for (i = 0; i < RANKS; i++) {
    lut->mask_rank[i] = rank;
    lut->clear_rank[i] = ~rank;
    rank <<= 8;
  }
}

void init_diagonals(LookupTable *lut) {

  int i;

  bitset<64> diagonal = MAIN_DIAGONAL;

  for (i = 0; i < DIAGONALS / 2; i++) {
    lut->mask_diagonal[i] = diagonal;
    diagonal <<= 8;
  }

  diagonal = MAIN_DIAGONAL;
  diagonal >>= 8;
  for (i = 15; i >= DIAGONALS / 2; i--) {
    lut->mask_diagonal[i] = diagonal;
    diagonal >>= 8;
  }

  lut->mask_diagonal[DIAGONALS].reset();

  bitset<64> antidiagonal = MAIN_ANTIDIAGONAL;

  for (i = 0; i < DIAGONALS / 2; i++) {
    lut->mask_antidiagonal[i] = antidiagonal;
    antidiagonal >>= 8;
  }

  antidiagonal = MAIN_ANTIDIAGONAL;
  antidiagonal <<= 8;
  for (i = 15; i >= DIAGONALS / 2; i--) {
    lut->mask_antidiagonal[i] = antidiagonal;
    antidiagonal <<= 8;
  }

  lut->mask_antidiagonal[DIAGONALS].reset();
}

int generate_rank_attack(int o, int f) {
  int x, y;
  int b;
  y = 0;
  for (x = f - 1; x >= 0; --x) {
    b = 1 << x;
    y |= b;
    if ((o & b) == b)
      break;
  }
  for (x = f + 1; x < 8; ++x) {
    b = 1 << x;
    y |= b;
    if ((o & b) == b)
      break;
  }
  return y;
}

void init_first_rank_attacks(LookupTable *lut) {
  for (int i = 0; i < SQUARES; ++i) {
    for (int j = 0; j < RANKS; ++j) {
      lut->mask_first_rank_attacks[i * RANKS + j] =
          generate_rank_attack(i * 2, j);
    }
  }
}
