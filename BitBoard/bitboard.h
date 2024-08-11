#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "lookup_table.h"
#include "move.h"
#include "utils.h"
#include <bitset>
#include <vector>

using namespace std;

class Bitboard {
private:
  /* Lookup table */
  LookupTable *lookupTable;

  /* Derived squareitions from pieces */
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

  /* Move list */
  /* Per move generation */
  vector<Move> moveList;

  /* Chess game rules */
  Color turn;
  int enPassantSq;
  bitset<4> castlingRights;

  /* Move generation */

  /* Hyperbolee Quiessence algorithm */
  bitset<64> generateDiagonalAttacks(int square);

  bitset<64> generateAntiDiagonalAttacks(int square);

  bitset<64> generateFileAttacks(int square);

  /* First rank attacks algorithm */
  bitset<64> generateRankAttacks(int square);

  /* Non sliding pieces attack generators */
  bitset<64> generateKingAttacks(int square);

  bitset<64> generateKnightAttacks(int square);

  bitset<64> generatePawnAttacks(Color color, int square);

  /* Sliding pieces attack generators */
  bitset<64> generateBishopAttacks(int square);

  bitset<64> generateRookAttacks(int square);

  bitset<64> generateQueenAttacks(int square);

  /* Pawn move generator */
  bitset<64> generatePawnMoves(Color color, int square);

  /* Attacking bitboards methods */
  void nonSlidingAttacks();

  void slidingAttacks();

  /* Move processing function */
  void pieceMoves(bitset<64> bb, Color color,
                  bitset<64> (Bitboard::*attackGenerator)(int), int piece_type);

  void pieceMoves(bitset<64> bb, Color color,
                  bitset<64> (Bitboard::*moveGenerator)(Color, int),
                  bitset<64> (Bitboard::*attackGenerator)(Color, int));

  /* Castling generation */
  void generateCastleMoves(Color side);

  /* Common chess methods */
  bitset<64> attacksToSquare(int square);

  bool isSquareAttacked(Color side, int square);

  bool isCheck(Color side);

  bool isCheckmate(Color side);

  /* Updating methods */
  void updateDerivedBitboards();

public:
  /* Initialize a board with chess default starting position */
  Bitboard();

  /* Initialize a custom position */
  Bitboard(bitset<64> pieces[12], bitset<4> cR, int epSq, Color turn_color);

  /* Getters and setters */
  vector<Move> getMoveList();

  void setLookupTable(LookupTable *lut);

  /* Move methods */
  void generateMoves();

  bool makeMove(Move move);

  /* Output methods */
  void printBitboard(bitset<64> bitboard);

  void printBoard();

  void printMoveList();

  /* Save board state */
  Bitboard copyBoard();
};

#endif
