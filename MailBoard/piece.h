#ifndef PIECE_H
#define PIECE_H

#include "enums.h"

enum PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

class Piece {
private:
  PieceType piece;
  Color color;

public:

  Piece();

  Piece(PieceType p, Color c);

  PieceType getType();

  void setType(PieceType p);

  Color getColor();

  void setColor(Color c);
};

#endif
