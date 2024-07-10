#include "lookup_table.h"
#include <bit>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <iostream>

LookupTable *lookup_table_init() {
  LookupTable *lookup_table = NULL;

  lookup_table = (LookupTable *)malloc(sizeof(LookupTable));
  if (lookup_table == NULL)
    return NULL;

  int i;
  bitset<64> set = 0xFFFFFFFFFFFFFF00;
  for (i = 0; i <= RANKS; i++) {
    lookup_table->clear_rank[i] = set;
    lookup_table->mask_rank[i] = set.flip();
    set.flip();
    set = rotl(set.to_ullong(), 8);
  }

  int j;
  for (i = 0; i < FILES; i++) {
    cout << lookup_table->clear_rank[i] << endl;
    cout << lookup_table->mask_rank[i] << endl;
  }

  return lookup_table;
}
