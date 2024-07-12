#include "bitboard.h"
#include "utils.h"
#include <bitset>

/* Standard initial chess position */
#define WHITE_PAWNS_POS 0x0000000000000000
#define WHITE_ROOKS_POS 0x0000000000000081
#define WHITE_KNIGHTS_POS 0x0000000000000042
#define WHITE_BISHOPS_POS 0x0000000000000024
#define WHITE_QUEENS_POS 0x0000000000000008
#define WHITE_KING_POS 0x0000000000000010

#define BLACK_PAWNS_POS 0x00FF000000000000
#define BLACK_ROOKS_POS 0x8100000000000000
#define BLACK_KNIGHTS_POS 0x4200000000000000
#define BLACK_BISHOPS_POS 0x2400000000000000
#define BLACK_QUEENS_POS 0x0800000000000000
#define BLACK_KING_POS 0x1000000000000000

int main() {
  bitset<64> white_pawns(WHITE_PAWNS_POS);
  bitset<64> black_pawns(BLACK_PAWNS_POS);
  bitset<64> white_rooks(WHITE_ROOKS_POS);
  bitset<64> black_rooks(BLACK_ROOKS_POS);
  bitset<64> white_knights(WHITE_KNIGHTS_POS);
  bitset<64> black_knights(BLACK_KNIGHTS_POS);
  bitset<64> white_bishops(WHITE_BISHOPS_POS);
  bitset<64> black_bishops(BLACK_BISHOPS_POS);
  bitset<64> white_queens(WHITE_QUEENS_POS);
  bitset<64> black_queens(BLACK_QUEENS_POS);
  bitset<64> white_king(WHITE_KING_POS);
  bitset<64> black_king(BLACK_KING_POS);

  Bitboard bit_board =
      Bitboard(white_pawns, black_pawns, white_rooks, black_rooks,
               white_knights, black_knights, white_bishops, black_bishops,
               white_queens, black_queens, white_king, black_king);

  bit_board.printBoard();

  bit_board.generateKingMoves(WHITE);
  bit_board.generateKingMoves(BLACK);
  bit_board.generateKnightMoves(WHITE, 1);
  bit_board.generateKnightMoves(BLACK, 63);
  bit_board.generatePawnMoves(WHITE, 8);
  bit_board.generatePawnMoves(BLACK, 48);
  bit_board.generateBishopMoves(2);
  print_bitset(bit_board.generateRookMoves(0));
  print_bitset(bit_board.generateQueenMoves(3));
}