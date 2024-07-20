#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "lookup_table.h"
#include <bitset>

using namespace std;

enum Color { WHITE, BLACK, NONE };

enum enumSquare {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};

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

  /* Derived positions from pieces */
  bitset<64> allWhitePieces;
  bitset<64> allBlackPieces;
  bitset<64> allPieces;
  bitset<64> emptySquares;

  /* Bitboard serialization */
  bitset<64> piecesBB[12];

  /* Attacking Bitboards */
  bitset<64> pawnAttacks[2][SQUARES];
  bitset<64> knightAttacks[SQUARES];
  bitset<64> kingAttacks[SQUARES];
  bitset<64> bishopAttacks[SQUARES];
  bitset<64> rookAttacks[SQUARES];

  /* Chess game rules */
  Color turn;

  /* Utils */
  /* Chess unicode ascii pieces */
  const char *asciiPieces[12] = {"♙", "♟", "♖", "♜", "♘", "♞",
                                 "♗", "♝", "♕", "♛", "♔", "♚"};
  /* Conversion from coordinate to square notation */
  const char *coordinateToSquare[SQUARES] = {
  "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
  "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
  "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
  "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
  "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
  "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
  "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
  "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
  };

  /* Private move generator methods */
  /* Hyperbolee Quiessence algorithm */
  bitset<64> generateDiagonalAttacks(int pos);

  bitset<64> generateAntiDiagonalAttacks(int pos);

  bitset<64> generateFileAttacks(int pos);

  /* First rank attacks algorithm */
  bitset<64> generateRankAttacks(int pos);

  /* Non sliding pieces attack generators */
  bitset<64> generateKingAttacks(int pos);

  bitset<64> generateKnightAttacks(int pos);

  bitset<64> generatePawnAttacks(Color color, int pos);

  /* Sliding pieces attack generators */
  bitset<64> generateBishopAttacks(int pos);

  bitset<64> generateRookAttacks(int pos);

  bitset<64> generateQueenAttacks(int pos);

  /* Pawn move generator */
  bitset<64> generatePawnMoves(Color color, int pos);

  /* Attacking bitboards methods */
  void nonSlidingAttacks();

  void slidingAttacks();

  /* Move processing function */
  void pieceMoves(bitset<64> bb, Color color, 
      bitset<64>(Bitboard::*attackGenerator)(int));

  void pieceMoves(bitset<64> bb, Color color, 
      bitset<64>(Bitboard::*moveGenerator)(Color , int), 
      bitset<64>(Bitboard::*attackGenerator)(Color, int));

public:
  /* Initialize a board with chess default starting position */
  Bitboard();

  /* Initialize a custom position */
  Bitboard(bitset<64> wP, bitset<64> bP, bitset<64> wR, bitset<64> bR,
           bitset<64> wN, bitset<64> bN, bitset<64> wB, bitset<64> bB,
           bitset<64> wQ, bitset<64> bQ, bitset<64> wK, bitset<64> bK,
           Color turn_color);

  /* Initialize an empty board */
  Bitboard(Color none);

  void setLookupTable(LookupTable *lut);

  /* Common chess methods */
  bitset<64> attacksToSquare(int pos);

  bool isSquareAttacked(Color side, int pos);

  void generateMoves();

  /* Output methods */
  void printBitboard(bitset<64> bitboard);

  void printBoard();
};

#endif
