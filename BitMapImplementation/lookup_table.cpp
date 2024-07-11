#include "lookup_table.h"
#include <bit>
#include <bitset>
#include <cmath>
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

  set.reset();
  set.set(0, true);
  for (i = 0; i < SQUARES; i++) {
    lookup_table->piece_lookup[i] = set;
    set <<= 1;
  }

  return lookup_table;
}
