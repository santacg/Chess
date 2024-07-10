#include "chess_board.h"
#include <iostream>

ChessBoard::ChessBoard(bitset<64> wP, bitset<64> bP, bitset<64> wR,
                       bitset<64> bR, bitset<64> wN, bitset<64> bN,
                       bitset<64> wB, bitset<64> bB, bitset<64> wQ,
                       bitset<64> bQ, bitset<64> wK, bitset<64> bK) {
  whitePawns |= wP;
  blackPawns |= bP;
  whiteRooks |= wR;
  blackRooks |= bR;
  whiteKnights |= wN;
  blackKnights |= bN;
  whiteBishops |= wB;
  blackBishops |= bB;
  whiteQueens |= wQ;
  blackQueens |= bQ;
  whiteKing |= wK;
  blackKing |= bK;

  allWhitePieces |= whiteKing |= whiteQueens |= whiteBishops |= whiteKnights |=
      whiteRooks |= whitePawns;

  allBlackPieces |= blackKing |= blackQueens |= blackBishops |= blackKnights |=
      blackRooks |= blackPawns;

  allPieces |= allWhitePieces |= allBlackPieces;
}

void ChessBoard::printBoard() {
  int i, j;
  char c = 'a';
  
  for (i = 8; i > 0; i--) {
    cout << i << " ";

    for (j = 8; j > 0; j--) {
      if (allPieces[(i * 8) - j] == true) {
        cout << "1";
      }
      else {
        cout << "0";
      }

      cout << " ";
    }

    cout << endl;
  }

  cout << "  ";
  for (i = 0; i < 8; i++, c++) {
    cout << c << " ";
  } 

  cout << endl;
}
