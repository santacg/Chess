#include "piece.h"

Piece::Piece() {}

Piece::Piece(PieceType p, Color c) {
  piece = p;
  color = c;
}

PieceType Piece::getType() { return piece; }

void Piece::setType(PieceType p) { piece = p; }

Color Piece::getColor() { return color; }

void Piece::setColor(Color c) { color = c; }
