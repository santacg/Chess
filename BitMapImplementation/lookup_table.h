#ifndef LOOKUP_TABLE_H
#define LOOKUP_TABLE_H

#include <bitset>

#define RANKS 7 
#define FILES 7

using namespace std;

typedef struct {
  bitset<64> clear_rank[RANKS];
  bitset<64> clear_file[FILES];
  bitset<64> mask_rank[RANKS];
  bitset<64> mask_file[FILES];
} LookupTable;


LookupTable *lookup_table_init(); 

#endif
