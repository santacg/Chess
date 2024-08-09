#include "bitboard.h"
#include "lookup_table.h"
#include "move.h"
#include "utils.h"
#include <bitset>
#include <cctype>
#include <endian.h>
#include <iostream>
#include <string>

/* Fen positions */
#define INITIAL_CHESS_POSITION                                                 \
  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"
#define RUY_LOPEZ                                                              \
  "r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq -"
#define SCANDI "rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6"

Bitboard parse_fen(string fen_str) {
  bitset<64> pieces[12];
  long unsigned int str_counter = 0;

  for (int i = RANKS - 1; i >= 0; --i) {
    for (int j = 0; j < FILES && str_counter < fen_str.length();
         ++j, ++str_counter) {
      char c = fen_str[str_counter];
      if (isdigit(c)) {
        j += (c - '1');
      } else {
        switch (c) {
        case 'P':
          pieces[0].set(i * 8 + j);
          break;
        case 'p':
          pieces[1].set(i * 8 + j);
          break;
        case 'R':
          pieces[2].set(i * 8 + j);
          break;
        case 'r':
          pieces[3].set(i * 8 + j);
          break;
        case 'N':
          pieces[4].set(i * 8 + j);
          break;
        case 'n':
          pieces[5].set(i * 8 + j);
          break;
        case 'B':
          pieces[6].set(i * 8 + j);
          break;
        case 'b':
          pieces[7].set(i * 8 + j);
          break;
        case 'Q':
          pieces[8].set(i * 8 + j);
          break;
        case 'q':
          pieces[9].set(i * 8 + j);
          break;
        case 'K':
          pieces[10].set(i * 8 + j);
          break;
        case 'k':
          pieces[11].set(i * 8 + j);
          break;
        case '/':
          --j;
          break;
        }
      }
    }
  }

  str_counter = fen_str.find(" ", str_counter);
  Color turn = (fen_str[++str_counter] == 'w') ? WHITE : BLACK;

  bitset<4> castling_rights;
  str_counter += 2;
  while (fen_str[str_counter] != ' ' && fen_str[str_counter] != '\0') {
    switch (fen_str[str_counter]) {
    case 'K':
      castling_rights.set(0);
      break;
    case 'Q':
      castling_rights.set(1);
      break;
    case 'k':
      castling_rights.set(2);
      break;
    case 'q':
      castling_rights.set(3);
      break;
    }
    ++str_counter;
  }

  ++str_counter;
  string en_passant = "";
  while (fen_str[str_counter] != ' ' && fen_str[str_counter] != '\0') {
    en_passant.push_back(fen_str[str_counter]);
    ++str_counter;
  }

  int en_passant_sq = no_square;
  if (en_passant != " " && en_passant != "-") {
    en_passant_sq = squareToCoordinate.at(en_passant);
  }

  return Bitboard(pieces, castling_rights, en_passant_sq, turn);
}

/* Performance test function */
int perft(Bitboard bitboard, int depth) {
  /* Base case */
  if (depth == 0) {
    return 1;
  }

  bitboard.generateMoves();

  /* General case */
  int nodes = 0;
  for (Move m : bitboard.getMoveList()) {
    bitboard.printBoard();
    Bitboard bb_cpy = bitboard.copyBoard();
    if (bitboard.makeMove(m))
      nodes += perft(bitboard, depth - 1);
    bitboard = bb_cpy;
  }

  return nodes;
}

int main() {
  LookupTable *lut = init_lookup_table();

  /**/
  /*Bitboard fen_bit_board = parse_fen(INITIAL_CHESS_POSITION);*/
  /*fen_bit_board.setLookupTable(lut);*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/

  /* Check test */
  /*fen_bit_board.makeMove(Move(e2, e4, DOUBLE_PAWN_PUSH, PAWN, WHITE));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.makeMove(Move(e7, e5, DOUBLE_PAWN_PUSH, PAWN, BLACK));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.makeMove(Move(e1, e2, QUIET_MOVE, KING, WHITE));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.makeMove(Move(d8, h4, QUIET_MOVE, QUEEN, BLACK));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.makeMove(Move(a2, a3, QUIET_MOVE, PAWN, WHITE));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.makeMove(Move(h4, e4, CAPTURE, QUEEN, BLACK));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.makeMove(Move(a3, a4, QUIET_MOVE, PAWN, WHITE));*/
  /*fen_bit_board.printBoard();*/
  /**/

  /* Few Ruy Lopez moves */
  /*fen_bit_board.makeMove(Move(e2, e4, DOUBLE_PAWN_PUSH, PAWN, WHITE));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/
  /*fen_bit_board.makeMove(Move(e7, e5, DOUBLE_PAWN_PUSH, PAWN, BLACK));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/
  /*fen_bit_board.makeMove(Move(g1, f3, QUIET_MOVE, KNIGHT, WHITE));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/
  /*fen_bit_board.makeMove(Move(b8, c6, QUIET_MOVE, KNIGHT, BLACK));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/
  /*fen_bit_board.makeMove(Move(f1, b5, QUIET_MOVE, BISHOP, WHITE));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/
  /*fen_bit_board.makeMove(Move(a7, a6, QUIET_MOVE, PAWN, BLACK));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/
  /*fen_bit_board.makeMove(Move(b5, a4, QUIET_MOVE, BISHOP, WHITE));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/
  /*fen_bit_board.makeMove(Move(g8, f6, QUIET_MOVE, KNIGHT, BLACK));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/
  /*fen_bit_board.makeMove(Move(e1, g1, KING_CASTLE, KING, WHITE));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/
  /*fen_bit_board.makeMove(Move(f8, e7, QUIET_MOVE, BISHOP, BLACK));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/
  /*fen_bit_board.makeMove(Move(f1, e1, QUIET_MOVE, ROOK, WHITE));*/
  /*fen_bit_board.printBoard();*/
  /*fen_bit_board.generateMoves();*/
  /*fen_bit_board.printMoveList();*/

  /* Performace test bitboard */
  Bitboard perft_bb = parse_fen(
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
  perft_bb.setLookupTable(lut);
  cout << "performance test nodes: " << perft(perft_bb, 2) << endl;

  free(lut);
  return 0;
}
