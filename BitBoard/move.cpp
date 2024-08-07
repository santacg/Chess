#include "move.h"

Move::Move(int source_square, int target_square, int fl, int piece_t) {
  sourceSquare = source_square;
  targetSquare = target_square;
  flag = fl;
  piece = piece_t;
}

unsigned long Move::getSourceSquare() { return sourceSquare.to_ulong(); }

unsigned long Move::getTargetSquare() { return targetSquare.to_ulong(); }

unsigned long Move::getFlags() { return flag.to_ulong(); }

unsigned long Move::getPiece() { return piece.to_ulong(); }

string Move::parsePiece() {
  int piece = getPiece();

  switch (piece) {
  case PAWN:
    return "";
  case KNIGHT:
    return "N";
  case BISHOP:
    return "B";
  case ROOK:
    return "R";
  case QUEEN:
    return "Q";
  case KING:;
    return "K";
  }

  return "";
}

string Move::formatToAlgebraic() {
  int target_square = getTargetSquare();

  int capture = 0;
  if (getFlags() == 4)
    capture = 1;

  string piece = parsePiece();

  string algebraic_notation;

  algebraic_notation.append(piece);
  if (capture == 1)
    algebraic_notation.append("x");
  algebraic_notation.append(coordinateToSquare[target_square]);

  return algebraic_notation;
}
