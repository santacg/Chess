#include "chess.h"
#include "piece.h"
#include "square.h"
#include "enums.h"

ChessBoard::ChessBoard() {
  int i, j;
  Color c = BLACK;

  for (i = 0; i < 8; i++) {

    if (i % 2 != 0) {
      c = WHITE;
    }

    for (j = 0; j < 8; j++) {
      square[i][j] = Square(i, j, c, nullptr);

      c == BLACK ? c = WHITE: c = BLACK;
    }
  }

  for (i = 0; i < 8; i++) {
    piece[i] = Piece(PAWN, WHITE);
  }
}

Square* ChessBoard::getSquares() { return *square; }

Piece* ChessBoard::getPieces() { return piece; }
