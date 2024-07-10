#include "chess.h"
#include <cstdlib>

using namespace std;

int main() {
  ChessBoard chessBoard = ChessBoard();

  auto pieces = chessBoard.getPieces();
  auto squares = chessBoard.getSquares();

  return EXIT_SUCCESS;
}
