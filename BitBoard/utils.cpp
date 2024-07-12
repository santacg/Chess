#include "utils.h"
#include <bitset>
#include <iostream>

void print_bitset(bitset<64> bitset) {
  int i, j;
  char c = 'a';
  for (i = 8; i > 0; i--) {
    cout << i << " ";

    for (j = 8; j > 0; j--) {
      if (bitset[(i * 8) - j] == true) {
        cout << "1";
      } else {
        cout << "0";
      }

      cout << " ";
    }

    cout << endl;
  }

  cout << "  ";
  for (i = 0; i < 8; i++, c++) {
    cout << c << " ";
  }

  cout << endl;
}
