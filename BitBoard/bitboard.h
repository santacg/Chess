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

  /**
   * Generate attacks for king piece type
   *
   * @param int origin square of the king
   *
   * @return a bitset<64> containing the target squares
   * of the possible king attacks
   */
  bitset<64> generateKingAttacks(int square);

  /**
   * Generate attacks for knight piece type
   *
   * @param int origin square of the knight
   *
   * @return a bitset<64> containing the target squares
   * of the possible knight attacks
   */
  bitset<64> generateKnightAttacks(int square);

  /**
   * Generate attacks for pawn piece type
   *
   * @param Color side of the pawn
   * @param int origin square of the pawn
   *
   * @return a bitset<64> containing the target squares
   * of the possible pawn attacks
   */
  bitset<64> generatePawnAttacks(Color color, int square);

  /* Sliding pieces attack generators */

  /**
   * Generate attacks for bishop piece type
   *
   * @param int origin square of the bishop
   *
   * @return a bitset<64> containing the target squares
   * of the possible bishop attacks
   */
  bitset<64> generateBishopAttacks(int square);

  /**
   * Generate attacks for rook piece type
   *
   * @param int origin square of the rook
   *
   * @return a bitset<64> containing the target squares
   * of the possible rook attacks
   */
  bitset<64> generateRookAttacks(int square);

  /**
   * Generate attacks for queen piece type
   *
   * @param int origin square of the queen
   *
   * @return a bitset<64> containing the target squares
   * of the possible queen attacks
   */
  bitset<64> generateQueenAttacks(int square);

  /**
   * Generate moves for pawn piece type
   *
   * @param Color side of the pawn
   * @param int origin square of the pawn
   *
   * @return a bitset<64> containing the target squares
   * of the possible pawn moves
   */
  bitset<64> generatePawnMoves(Color color, int square);

  /* Attacking bitboards methods */

  /**
   * Generates all the attacks possible by knights, pawns and kings
   * in the current position, and saves them in attacking bitboards
   * members
   */
  void nonSlidingAttacks();

  /**
   * Generates all the attacks possible by bishops and rooks
   * in the current position, and saves them in attacking bitboards
   * members
   *
   * @note queens can be calculated as bishop + rook so there
   * is no need to specifically calculate attacks for queens
   */
  void slidingAttacks();

  /* Move processing function */

  /**
   * Generates all possible moves for a specific piece type
   * by extracting the pieces squares from a bitboard,
   * generating and categorizing each move and pushing it into moveList
   * member
   *
   * @param bitset<64> bb bitboard containing the pieces
   * @param Color color of the pieces
   * @param bitset<64> (Bitboard::*attackGenerator)(int) generic function that
   * generates attacks for the specific piece type given a square
   * @param int piece_type piece type i.e pawn, knight, bishop, rook, queen,
   * king
   */
  void pieceMoves(bitset<64> bb, Color color,
                  bitset<64> (Bitboard::*attackGenerator)(int), int piece_type);
  /**
   * Generates all possible moves for a specific piece type
   * by extracting the pieces squares from a bitboard,
   * generating and categorizing each move and pushing it into moveList
   * member
   *
   * @param bitset<64> bb bitboard containing the pieces
   * @param Color color of the pieces
   * @param bitset<64> (Bitboard::*moveGenerator)(Color, int) generic function
   * that generates all moves for the specific piece type given a square and
   * side
   * @param bitset<64> (Bitboard::*attackGenerator)(Color, int) generic function
   * that generates attacks for the specific piece type given a square and side
   * @param int piece_type piece type i.e pawn, knight, bishop, rook, queen,
   * king
   */
  void pieceMoves(bitset<64> bb, Color color,
                  bitset<64> (Bitboard::*moveGenerator)(Color, int),
                  bitset<64> (Bitboard::*attackGenerator)(Color, int));

  /* Castling generation */

  /**
   * Generates all the possible castling moves for a side
   *
   * @param Color side
   */
  void generateCastleMoves(Color side);

  /* Common chess methods */

  /**
   * Gets all the pieces attacking a square
   *
   * @param int square checked
   * @return a bitset<64> with all the pieces currently attacking the given
   * square
   */
  bitset<64> attacksToSquare(int square);

  /**
   * Checks if a square is attacked by the given side
   *
   * @param Color side to be checked
   * @param int square to be checked
   * @return true if the given square is being attacked by the side,
   * false otherwhise
   */
  bool isSquareAttacked(Color side, int square);

  /**
   * Checks if there is a check for a given side
   *
   * @param Color side to be checked
   * @return true if there is a checke for the given side,
   * false otherwhise
   */
  bool isCheck(Color side);

  /**
   * Checks if there is a checkmate for a given side
   *
   * @param Color side to be checked
   * @return true if there is a checkmate for the given side,
   * false otherwhise
   */
  bool isCheckmate(Color side);

  /* Updating methods */

  /**
   * Updates all derived bitboard
   */
  void updateDerivedBitboards();

public:
  /* Initialize a board with chess default starting position */
  Bitboard();

  /* Initialize a custom position */
  Bitboard(bitset<64> pieces[12], bitset<4> cR, int epSq, Color turn_color);

  /* Getters and setters */

  /**
   * Returns a vector containing the moveList member
   *
   * @return moveList member as a vector type
   */
  vector<Move> getMoveList();

  /**
   * Set the LookupTable struct for the Bitboard object
   *
   * @param pointer to the LookupTable struct
   */
  void setLookupTable(LookupTable *lut);

  /* Move methods */

  /**
   * Generate moves for the current position
   * and saves it to moveList member
   */
  void generateMoves();

  /**
   * Makes a move in the position
   *
   * @param a Move type object representing the move to be
   * made
   * @return true if the move is valid, false otherwhise
   */
  bool makeMove(Move move);

  /* Output methods */

  /**
   * Prints a bitboard with chess format
   * and 0s and 1s as the position of the
   * pieces
   *
   * @param bitboard to be printed
   */
  void printBitboard(bitset<64> bitboard);

  /**
   * Prints the current position with chess format
   * using ascii for the pieces
   */
  void printBoard();

  /**
   * Print the aviable moves for the current position
   */
  void printMoveList();

  /* Save board state */

  /**
   * Saves a copy of the Bitboard object
   *
   * @return Bitboard object copied from the current
   * bitboard
   */
  Bitboard copyBoard();
};

#endif
