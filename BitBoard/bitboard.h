#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "lookup_table.h"
#include <bitset>

#define WHITE_PAWNS_BB 0
#define BLACK_PAWNS_BB 1
#define WHITE_ROOKS_BB 2
#define BLACK_ROOKS_BB 3
#define WHITE_KNIGHTS_BB 4
#define BLACK_KNIGHTS_BB 5
#define WHITE_BISHOPS_BB 6
#define BLACK_BISHOPS_BB 7
#define WHITE_QUEENS_BB 8
#define BLACK_QUEENS_BB 9
#define WHITE_KING_BB 10
#define BLACK_KING_BB 11

using namespace std;

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

  /* Derived positions from pieces */
  bitset<64> allWhitePieces;
  bitset<64> allBlackPieces;
  bitset<64> allPieces;
  bitset<64> emptySquares;

  /* Bitboard serialization */
  bitset<64> piecesBB[12];

  /* Attacking Bitboards */
  bitset<64> pawnAttacks[2][SQUARES];
  bitset<64> knightAttacks[2][SQUARES];

  /* Chess game rules */
  Color turn;

  /* Chess unicode ascii pieces */
  const char *asciiPieces[12] = {"♙", "♟", "♖", "♜", "♘", "♞",
                                 "♗", "♝", "♛", "♕", "♔", "♚"};

  /* Private move generator methods */
  bitset<64> generatePawnAttacks(Color color, int pos);

  bitset<64> generateDiagonalMoves(int pos);

  bitset<64> generateAntiDiagonalMoves(int pos);

  bitset<64> generateFileMoves(int pos);

  /* Attacking bitboards methods */
  void nonSlidingAttacks();

  void SlidingAttacks();

public:
  Bitboard(bitset<64> wP, bitset<64> bP, bitset<64> wR, bitset<64> bR,
           bitset<64> wN, bitset<64> bN, bitset<64> wB, bitset<64> bB,
           bitset<64> wQ, bitset<64> bQ, bitset<64> wK, bitset<64> bK);

  Bitboard(bitset<64> wP, bitset<64> bP, bitset<64> wR, bitset<64> bR,
           bitset<64> wN, bitset<64> bN, bitset<64> wB, bitset<64> bB,
           bitset<64> wQ, bitset<64> bQ, bitset<64> wK, bitset<64> bK,
           Color turn_color);

  ~Bitboard() { free(lookupTable); };

  /* Non sliding pieces move generators */
  bitset<64> generateKingMoves(Color color);

  bitset<64> generateKnightMoves(Color color, int pos);

  bitset<64> generatePawnMoves(Color color, int pos);

  /* Sliding pieces move generators */
  bitset<64> generateBishopMoves(int pos);

  bitset<64> generateRookMoves(int pos);

  bitset<64> generateQueenMoves(int pos);

  /* Output methods */
  void printBitboard(bitset<64> bitboard);

  void printBoard();
};

#endif
