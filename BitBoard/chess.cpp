#include "bitboard.h"
#include "lookup_table.h"
#include "move.h"
#include "utils.h"
#include <bitset>
#include <cctype>
#include <endian.h>
#include <string>

/* Fen positions */
#define INITIAL_CHESS_POSITION                                                 \
  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq"
#define RUY_LOPEZ "r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq"
#define SCANDI "rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq"

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

  /* Get the turn side */
  str_counter = fen_str.find(" ");
  str_counter++;
  char turn_color = fen_str[str_counter];

  Color turn;
  (turn_color == 'w') ? turn = WHITE : turn = BLACK;
  str_counter++;

  /* Get the castling rights */
  bitset<4> castling_rights;

  str_counter++;
  while (fen_str[str_counter] != '\0') {
    if (fen_str[str_counter] == 'K') {
      castling_rights.set(0, true);
    } else if (fen_str[str_counter] == 'Q') {
      castling_rights.set(1, true);
    } else if (fen_str[str_counter] == 'k') {
      castling_rights.set(2, true);
    } else if (fen_str[str_counter] == 'q') {
      castling_rights.set(3, true);
    }
    str_counter++;
  }

  return Bitboard(white_pawns, black_pawns, white_rooks, black_rooks,
                  white_knights, black_knights, white_bishops, black_bishops,
                  white_queens, black_queens, white_king, black_king,
                  castling_rights, turn);
}

int main() {
  LookupTable *lut = init_lookup_table();

  Bitboard fen_bit_board = parse_fen(INITIAL_CHESS_POSITION);
  fen_bit_board.setLookupTable(lut);
  fen_bit_board.printBoard();
  fen_bit_board.generateMoves();
  fen_bit_board.printMoveList();

  /* Few Ruy Lopez moves */
  fen_bit_board.makeMove(Move(e2, e4, QUIET_MOVE, PAWN, WHITE));
  fen_bit_board.printBoard();
  fen_bit_board.makeMove(Move(e7, e5, QUIET_MOVE, PAWN, BLACK));
  fen_bit_board.printBoard();
  fen_bit_board.makeMove(Move(g1, f3, QUIET_MOVE, KNIGHT, WHITE));
  fen_bit_board.printBoard();
  fen_bit_board.makeMove(Move(b8, c6, QUIET_MOVE, KNIGHT, BLACK));
  fen_bit_board.printBoard();
  fen_bit_board.makeMove(Move(f1, b5, QUIET_MOVE, BISHOP, WHITE));
  fen_bit_board.printBoard();
  fen_bit_board.makeMove(Move(a7, a6, QUIET_MOVE, PAWN, BLACK));
  fen_bit_board.printBoard();
  fen_bit_board.makeMove(Move(b5, a4, QUIET_MOVE, BISHOP, WHITE));
  fen_bit_board.printBoard();
  fen_bit_board.makeMove(Move(g8, f6, QUIET_MOVE, KNIGHT, BLACK));
  fen_bit_board.printBoard();
  fen_bit_board.generateMoves();
  fen_bit_board.printMoveList();
  fen_bit_board.makeMove(Move(e1, g1, KING_CASTLE, KING, WHITE));
  fen_bit_board.printBoard();

  free(lut);
  return 0;
}
