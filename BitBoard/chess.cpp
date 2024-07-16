#include "bitboard.h"
#include "lookup_table.h"
#include <bitset>
#include <cctype>
#include <endian.h>
#include <iostream>
#include <string>

/* Standard initial chess position */
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

Bitboard parse_fen(string fen_str) {
  int i, j, str_counter;

  bitset<64> white_pawns;
  bitset<64> black_pawns;
  bitset<64> white_rooks;
  bitset<64> black_rooks;
  bitset<64> white_knights;
  bitset<64> black_knights;
  bitset<64> white_bishops;
  bitset<64> black_bishops;
  bitset<64> white_queens;
  bitset<64> black_queens;
  bitset<64> white_king;
  bitset<64> black_king;

  for (i = RANKS - 1, str_counter = 0; i >= 0; i--) {
    for (j = 0; j <= FILES; j++, str_counter++) {
      if (fen_str[str_counter] == 'P') {
        white_pawns.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == 'p') {
        black_pawns.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == 'R') {
        white_rooks.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == 'r') {
        black_rooks.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == 'N') {
        white_knights.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == 'n') {
        black_pawns.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == 'B') {
        white_bishops.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == 'b') {
        black_bishops.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == 'Q') {
        white_queens.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == 'q') {
        black_queens.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == 'K') {
        white_king.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == 'k') {
        black_king.set((i * 8) + j, true);
      } else if (fen_str[str_counter] == '/') {
        str_counter++;
        break;
      } else if (isdigit(fen_str[str_counter]) == true) {
        j += (fen_str[str_counter] - '0') - 1;
      }
    }
  }

  int blank_space = fen_str.find(" ");
  char turn_color = fen_str[blank_space + 1];

  Color turn;
  (turn_color == 'w') ? turn = WHITE : turn = BLACK;

  return Bitboard(white_pawns, black_pawns, white_rooks, black_rooks,
                  white_knights, black_knights, white_bishops, black_bishops,
                  white_queens, black_queens, white_king, black_king, turn);
}

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
  Bitboard fen_bit_board =
      parse_fen("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b");
  fen_bit_board.printBoard();
}
