#include "chess.h"
#include <cstdlib>

int main() {
  ChessBoard chessBoard = ChessBoard();

  auto pieces = chessBoard.getPieces();
  auto squares = chessBoard.getSquares();

  return EXIT_SUCCESS;
}
