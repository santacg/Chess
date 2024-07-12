#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "lookup_table.h"
#include <bitset>

using namespace std;

enum enumSquare {
  a1,
  b1,
  c1,
  d1,
  e1,
  f1,
  g1,
  h1,
  a2,
  b2,
  c2,
  d2,
  e2,
  f2,
  g2,
  h2,
  a3,
  b3,
  c3,
  d3,
  e3,
  f3,
  g3,
  h3,
  a4,
  b4,
  c4,
  d4,
  e4,
  f4,
  g4,
  h4,
  a5,
  b5,
  c5,
  d5,
  e5,
  f5,
  g5,
  h5,
  a6,
  b6,
  c6,
  d6,
  e6,
  f6,
  g6,
  h6,
  a7,
  b7,
  c7,
  d7,
  e7,
  f7,
  g7,
  h7,
  a8,
  b8,
  c8,
  d8,
  e8,
  f8,
  g8,
  h8
};

enum Color { WHITE, BLACK };

class Bitboard {
private:
  /* Lookup tables */
  LookupTable *lookupTable;

  /* White pieces positions */
  bitset<64> whitePawns;
  bitset<64> whiteRooks;
  bitset<64> whiteKnights;
  bitset<64> whiteBishops;
  bitset<64> whiteQueens;
  bitset<64> whiteKing;

  /* Black pieces positions */
  bitset<64> blackPawns;
  bitset<64> blackRooks;
  bitset<64> blackKnights;
  bitset<64> blackBishops;
  bitset<64> blackQueens;
  bitset<64> blackKing;

  /* Derived positions */
  bitset<64> allWhitePieces;
  bitset<64> allBlackPieces;
  bitset<64> allPieces;
  bitset<64> emptySquares;

public:
  Bitboard(bitset<64> wP, bitset<64> bP, bitset<64> wR, bitset<64> bR,
           bitset<64> wN, bitset<64> bN, bitset<64> wB, bitset<64> bB,
           bitset<64> wQ, bitset<64> bQ, bitset<64> wK, bitset<64> bK);

  ~Bitboard() { free(lookupTable); };

  /* Non sliding pieces move generators */
  bitset<64> generateKingMoves(Color color);

  bitset<64> generateKnightMoves(Color color, int pos);

  bitset<64> generatePawnMoves(Color color, int pos);

  /* Sliding pieces move generators */
  bitset<64> generateDiagonalMoves(int pos);

  bitset<64> generateAntiDiagonalMoves(int pos);

  bitset<64> generateBishopMoves(int pos);

  void printBoard();
};

#endif
