#include "move.h"
#include <iostream>

Move::Move(int source_square, int target_square, int fl, int piece_t) {
  sourceSquare = source_square;
  targetSquare = target_square;
  flag = fl;
  piece = piece_t;
}

unsigned long Move::getSourceSquare() { return sourceSquare.to_ulong(); }

unsigned long Move::getTargetSquare() { return targetSquare.to_ulong(); }

unsigned long Move::getFlag() { return flag.to_ulong(); }

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
  if (getFlag() == CAPTURE)
    capture = 1;

  string piece = parsePiece();

  string algebraic_notation;

  algebraic_notation.append(piece);
  if (capture == 1)
    algebraic_notation.append("x");
  algebraic_notation.append(coordinateToSquare[target_square]);

  return algebraic_notation;
}

bool Move::operator==(const Move &move) {

  if (sourceSquare.to_ulong() != move.sourceSquare.to_ulong())
    return false;

  if (targetSquare.to_ulong() != move.targetSquare.to_ulong())
    return false;

  if (flag.to_ulong() != move.flag.to_ulong())
    return false;

  if (piece.to_ulong() != move.piece.to_ulong())
    return false;

  return true;
}

void Move::printMove() {
  cout << "source square: ";
  cout << coordinateToSquare[sourceSquare.to_ulong()] << " ";
  cout << "target square: ";
  cout << coordinateToSquare[targetSquare.to_ulong()] << " ";
  cout << "flag: ";
  cout << flag.to_ulong() << " ";
  cout << "piece: ";
  string piece = parsePiece();

  (piece == "") ? cout << "P" << endl : cout << piece << endl;
}
