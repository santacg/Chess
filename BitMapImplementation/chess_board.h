#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "lookup_table.h"
#include <bitset>

using namespace std;

enum Color { WHITE, BLACK };

class ChessBoard {
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

public:
  ChessBoard(bitset<64> wP, bitset<64> bP, bitset<64> wR, bitset<64> bR,
             bitset<64> wN, bitset<64> bN, bitset<64> wB, bitset<64> bB,
             bitset<64> wQ, bitset<64> bQ, bitset<64> wK, bitset<64> bK);

  ~ChessBoard() { free(lookupTable); };

  bitset<64> generateKingMoves(Color color);

  bitset<64> generateKnightMoves(Color color, int pos);

  void printBoard();
};

#endif
