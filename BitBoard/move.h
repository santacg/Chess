#ifndef MOVE_H
#define MOVE_H

#include <bitset>

#define PAWN 0
#define ROOK 1
#define KNIGHT 2
#define BISHOP 3
#define QUEEN 4
#define KING 5
#define NO_PIECE 6

#define QUIET_MOVE 0
#define DOUBLE_PAWN_PUSH 1
#define KING_CASTLE 2
#define QUEEN_CASTLE 3
#define CAPTURE 4
#define EP_CAPTURE 5
#define KNIGHT_PROMOTION 6
#define BISHOP_PROMOTION 7
#define ROOK_PROMOTION 8
#define QUEEN_PROMOTION 9
#define KNIGHT_PROMOTION_CAPTURE 10
#define BISHOP_PROMOTION_CAPTURE 11
#define ROOK_PROMOTION_CAPTURE 12
#define QUEEN_PROMOTION_CAPTURE 13
#define PROMOTION 14
#define PROMOTION_CAPTURE 15

/*
 * Move encoding format
 *
 * 0000 0000 0000 0011 1111 source square
 * 0000 0000 1111 1100 0000 target square
 * 0000 1111 0000 0000 0000 flags
 * 0111 0000 0000 0000 0000 pieces
 * 1000 0000 0000 0000 0000 color
 */

using namespace std;

class Move {
private:
  bitset<6> sourceSquare;
  bitset<6> targetSquare;
  bitset<4> flag;
  bitset<3> piece;
  bitset<1> color;

public:
  Move();

  Move(int source_square, int target_square, int fl, int piece_t, int clr);

  unsigned long getSourceSquare();

  unsigned long getTargetSquare();

  unsigned long getFlag();

  unsigned long getPiece();

  unsigned long getColor();

  string parsePiece();

  string formatFlag();

  string formatToAlgebraic();

  bool operator==(const Move &move);

  void printMove();
};

#endif
