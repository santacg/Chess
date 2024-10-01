#include "../includes/move.h"
#include "../includes/utils.h"
#include <iostream>

Move::Move() {
  sourceSquare = no_square;
  targetSquare = no_square;
  flag = QUIET_MOVE;
  piece = NO_PIECE;
  color = NONE;
}

Move::Move(int source_square, int target_square, int fl, int piece_t, int clr) {
  sourceSquare = source_square;
  targetSquare = target_square;
  flag = fl;
  piece = piece_t;
  color = clr;
}

unsigned long Move::getSourceSquare() { return sourceSquare.to_ulong(); }

unsigned long Move::getTargetSquare() { return targetSquare.to_ulong(); }

unsigned long Move::getFlag() { return flag.to_ulong(); }

unsigned long Move::getPiece() { return piece.to_ulong(); }

unsigned long Move::getColor() { return color.to_ulong(); }

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

string Move::formatFlag() {
  switch (flag.to_ulong()) {
  case QUIET_MOVE:
    return "move";
  case DOUBLE_PAWN_PUSH:
    return "double pawn push";
  case KING_CASTLE:
    return "king side castling";
  case QUEEN_CASTLE:
    return "queen side castling";
  case CAPTURE:
    return "capture";
  case EP_CAPTURE:
    return "en passant capture";
  case KNIGHT_PROMOTION:
    return "knight promotion";
  case BISHOP_PROMOTION:
    return "bishop promotion";
  case ROOK_PROMOTION:
    return "rook promotion";
  case QUEEN_PROMOTION:
    return "queen promotion";
  case KNIGHT_PROMOTION_CAPTURE:
    return "knight promotion capture";
  case BISHOP_PROMOTION_CAPTURE:
    return "bishop promotion capture";
  case ROOK_PROMOTION_CAPTURE:
    return "rook promotion capture";
  case QUEEN_PROMOTION_CAPTURE:
    return "queen promotion capture";
  case PROMOTION:
    return "promotion";
  case PROMOTION_CAPTURE:
    return "promotion capture";
  default:
    return "unknown move";
  }
}

string Move::formatToAlgebraic() {
  int target_square = getTargetSquare();
  int flag = getFlag();
  string piece = parsePiece();

  string algebraic_notation;

  /* Check flag for castling */
  if (flag == KING_CASTLE) {
    algebraic_notation.append("0-0");
    return algebraic_notation;
  } else if (flag == QUEEN_CASTLE) {
    algebraic_notation.append("0-0-0");
    return algebraic_notation;
  }

  algebraic_notation.append(piece);
  if (flag == CAPTURE)
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

  if (color.to_ulong() != move.color.to_ulong())
    return false;

  return true;
}

void Move::printMove() {
  cout << "source square: ";
  cout << coordinateToSquare[sourceSquare.to_ulong()] << " ";
  cout << "target square: ";
  cout << coordinateToSquare[targetSquare.to_ulong()] << " ";
  cout << "flag: ";
  cout << formatFlag() << " ";
  int piece_t = (color.to_ulong() == WHITE) ? piece.to_ulong() * 2
                                            : piece.to_ulong() * 2 + 1;
  cout << "piece: " << asciiPieces[piece_t];
  cout << endl;
}
