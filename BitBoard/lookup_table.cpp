#include "lookup_table.h"
#include "utils.h"
#include <bit>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>

LookupTable *lookup_table_init() {
  LookupTable *lookup_table = NULL;

  lookup_table = (LookupTable *)malloc(sizeof(LookupTable));
  if (lookup_table == NULL)
    return NULL;

  int i;
  bitset<64> set = 0xFFFFFFFFFFFFFF00;
  for (i = 0; i < RANKS; i++) {
    lookup_table->clear_rank[i] = set;
    lookup_table->mask_rank[i] = set.flip();
    set.flip();
    set = rotl(set.to_ullong(), 8);
  }

  set.reset();
  for (i = 0; i < FILES; i++) {
    set.set(i * 8, true);
  }

  for (i = 0; i < FILES; i++) {
    lookup_table->mask_file[i] = set;
    lookup_table->clear_file[i] = set.flip();
    set.flip();
    set <<= 1;
  }

  for (i = 0; i < SQUARES; i++) {
    lookup_table->piece_lookup[i].set(i, true);
  }

  int j, z;
  for (i = 0; i < DIAGONALS / 2; i++) {
    for (j = i, z = 0; j < RANKS; j++, z++) {
      lookup_table->mask_diagonal[i].set((j * 8) + z, true);
    }
  }

  int c;
  for (c = 15; c > 8; i--, c--) {
    for (j = i - 1, z = 0; j > 0; j--, z++) {
      lookup_table->mask_diagonal[c].set(((j * 8) - 1) - z, true);
    }
  }

  for (c = 0, i = RANKS - 1; i >= 0; i--, c++) {
    for (j = i, z = 0; j >= 0; j--, z++) {
      lookup_table->mask_antidiagonal[c].set((j * 8) + z, true);
    }
  }

  for (i = 2, c = 15; c > 8; i++, c--) {
    for (j = i, z = 0; j <= RANKS; j++, z++) {
      lookup_table->mask_antidiagonal[c].set(((j * 8) - 1) - z, true);
    }
  }

  return lookup_table;
}
