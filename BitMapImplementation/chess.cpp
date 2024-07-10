#include "chess_board.h"
#include "lookup_table.h"
#include <bitset>

#define WHITE_PAWNS_POS 0x000000000000FF00
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

  ChessBoard chess_board =
      ChessBoard(white_pawns, black_pawns, white_rooks, black_rooks,
                 white_knights, black_knights, white_bishops, black_bishops,
                 white_queens, black_queens, white_king, black_king);

  chess_board.printBoard();

  LookupTable *lookup_table = lookup_table_init(); 

  return 0;
}
