#ifndef CHESS_H
#define CHESS_H

#include "square.h"
#include "piece.h"

class ChessBoard {
private:
  Square square[8][8];
  Piece piece[32];

public:
  ChessBoard(); 

  Square *getSquares(); 

  Piece *getPieces();
};

#endif
