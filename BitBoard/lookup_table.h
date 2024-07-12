#ifndef LOOKUP_TABLE_H
#define LOOKUP_TABLE_H

#include <bitset>

#define RANKS 8
#define FILES 8
#define SQUARES 64
#define DIAGONALS 16

using namespace std;

typedef struct {
  bitset<64> clear_rank[RANKS];
  bitset<64> clear_file[FILES];
  bitset<64> mask_rank[RANKS];
  bitset<64> mask_file[FILES];
  bitset<64> mask_diagonal[DIAGONALS];
  bitset<64> mask_antidiagonal[DIAGONALS];
  bitset<64> piece_lookup[SQUARES];
} LookupTable;

LookupTable *lookup_table_init();

#endif
