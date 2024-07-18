#include "lookup_table.h"
#include <bit>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>

void init_diagonals(LookupTable *lut);

void init_files(LookupTable *lut);

void init_squares(LookupTable *lut);

int generate_rank_attack(int o, int f);

void init_first_rank_attacks(LookupTable *lut);

LookupTable *init_lookup_table() {
  LookupTable *lookup_table = NULL;

  lookup_table = (LookupTable *)malloc(sizeof(LookupTable));
  if (lookup_table == NULL)
    return NULL;

  for (int i = 0; i < RANKS; i++) {
    lookup_table->clear_rank[i].reset();
    lookup_table->mask_rank[i].reset();
  }

  for (int i = 0; i < FILES; i++) {
    lookup_table->clear_file[i].reset();
    lookup_table->mask_file[i].reset();
  }

  for (int i = 0; i < DIAGONALS; i++) {
    lookup_table->mask_diagonal[i].reset();
    lookup_table->mask_antidiagonal[i].reset();
  }

  for (int i = 0; i < SQUARES; i++) {
    lookup_table->piece_lookup[i].reset();
  }

  init_squares(lookup_table);

  init_files(lookup_table);

  init_diagonals(lookup_table);

  init_first_rank_attacks(lookup_table);

  return lookup_table;
}

void init_squares(LookupTable *lut) {
  for (int i = 0; i < SQUARES; i++) {
    lut->piece_lookup[i].set(i, true);
  }
}

void init_files(LookupTable *lut) {
  int i;
  bitset<64> set = 0xFFFFFFFFFFFFFF00;
  for (i = 0; i < RANKS; i++) {
    lut->clear_rank[i] = set;
    lut->mask_rank[i] = set.flip();
    set.flip();
    set = rotl(set.to_ullong(), 8);
  }

  set.reset();
  for (i = 0; i < FILES; i++) {
    set.set(i * 8, true);
  }

  for (i = 0; i < FILES; i++) {
    lut->mask_file[i] = set;
    lut->clear_file[i] = set.flip();
    set.flip();
    set <<= 1;
  }
}

void init_diagonals(LookupTable *lut) {

  int i, j, z;
  for (i = 0; i < DIAGONALS / 2; i++) {
    for (j = i, z = 0; j < RANKS; j++, z++) {
      lut->mask_diagonal[i].set((j * 8) + z, true);
    }
  }

  int c;
  for (c = 15; c > 8; i--, c--) {
    for (j = i - 1, z = 0; j > 0; j--, z++) {
      lut->mask_diagonal[c].set(((j * 8) - 1) - z, true);
    }
  }

  for (c = 0, i = RANKS - 1; i >= 0; i--, c++) {
    for (j = i, z = 0; j >= 0; j--, z++) {
      lut->mask_antidiagonal[c].set((j * 8) + z, true);
    }
  }

  for (i = 2, c = 15; c > 8; i++, c--) {
    for (j = i, z = 0; j <= RANKS; j++, z++) {
      lut->mask_antidiagonal[c].set(((j * 8) - 1) - z, true);
    }
  }
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
