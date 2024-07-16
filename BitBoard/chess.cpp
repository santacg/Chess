#include "bitboard.h"
#include "lookup_table.h"
#include <bitset>
#include <cctype>
#include <endian.h>
#include <iostream>
#include <string>

/* Fen positions */
#define INITIAL_CHESS_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w"
#define RUY_LOPEZ "r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b"
#define SCANDI "rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w"

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
        black_knights.set((i * 8) + j, true);
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
  Bitboard fen_bit_board = parse_fen(SCANDI);
  fen_bit_board.printBoard();
}
