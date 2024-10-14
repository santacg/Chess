#include "../includes/bitboard.h"
#include "../includes/lookup_table.h"
#include "../includes/move.h"
#include "../includes/utils.h"
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

Bitboard parse_fen(LookupTable *lut, string fen_str) {
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

  return Bitboard(lut, pieces, castling_rights, en_passant_sq, turn);
}

/* UCI move parser */
Move uci_parser(string uci_str, Bitboard bb) {

  string source_square_str, target_square_str;
  char promotion_char;

  source_square_str.push_back(uci_str[0]);
  source_square_str.push_back(uci_str[1]);

  target_square_str.push_back(uci_str[2]);
  target_square_str.push_back(uci_str[3]);

  promotion_char = '\0';
  promotion_char = uci_str[4];

  unsigned int source_square = squareToCoordinate.at(source_square_str);
  unsigned int target_square = squareToCoordinate.at(target_square_str);

  bb.generateMoves();
  for (Move move : bb.getMoveList()) {
    if (move.getSourceSquare() == source_square &&
        move.getTargetSquare() == target_square) {

      if (promotion_char != '\0') {
        int flag = move.getFlag();

        int promotion_capture_flag = 1;
        if (target_square == source_square - 8 ||
            target_square == source_square + 8) {
          promotion_capture_flag = 0;
        }

        switch (promotion_char) {
        case 'q':
        case 'Q':
          if (flag == QUEEN_PROMOTION && promotion_capture_flag == 0) {
            return move;
          } else if (flag == QUEEN_PROMOTION_CAPTURE &&
                     promotion_capture_flag == 1) {
            return move;
          }
          break;
        case 'b':
        case 'B':
          if (flag == BISHOP_PROMOTION && promotion_capture_flag == 0) {
            return move;
          } else if (flag == BISHOP_PROMOTION_CAPTURE &&
                     promotion_capture_flag == 1) {
            return move;
          }
          break;
        case 'n':
        case 'N':
          if (flag == KNIGHT_PROMOTION && promotion_capture_flag == 0) {
            return move;
          } else if (flag == KNIGHT_PROMOTION_CAPTURE &&
                     promotion_capture_flag == 1) {
            return move;
          }
          break;
        case 'r':
        case 'R':
          if (flag == ROOK_PROMOTION && promotion_capture_flag == 0) {
            return move;
          } else if (flag == ROOK_PROMOTION_CAPTURE &&
                     promotion_capture_flag == 1) {
            return move;
          }
          break;
        }
        continue;
      }
      return move;
    }
  }

  return Move();
}

/* Performance test function */
int perft(Bitboard bitboard, int depth) {
  /* Base case */
  if (depth == 0) {
    return 1;
  }

  /* General case */
  int nodes = 0;
  for (Move m : bitboard.getMoveList()) {
    Bitboard bb_cpy = bitboard.copyBoard();
    if (bitboard.makeMove(m))
      nodes += perft(bitboard, depth - 1);
    bitboard = bb_cpy;
  }

  return nodes;
}

int main() {
  LookupTable *lut = init_lookup_table();
  Bitboard bb = parse_fen(lut, INITIAL_CHESS_POSITION);

  string uci_str;
  while (true) {
    bb.printBoard();
    cout << endl << "Insert UCI format string (exit to end): ";
    getline(cin, uci_str);

    if (uci_str == "exit") {
      break;
    }

    Move move = uci_parser(uci_str, bb);
    if (move.getPiece() == NO_PIECE) {
      cout << endl << "Illegal move" << endl;
    } else {
      bb.makeMove(move);
    }

    if (bb.isCheckmate(bb.getTurn())) {
      bb.printBoard();
      cout << ((bb.getTurn() == WHITE) ? "White " : "Black ") << "is Checkmated"
           << endl;
      break;
    } else if (bb.isStaleMate(bb.getTurn())) {
      bb.printBoard();
      cout << ((bb.getTurn() == WHITE) ? "White " : "Black ") << "is StaleMated"
           << endl;
      break;
    }
  }

  /* Performace test bitboard */
  /*Bitboard perft_bb = parse_fen(INITIAL_CHESS_POSITION);*/
  /*perft_bb.setLookupTable(lut);*/
  /*cout << "performance test nodes: " << perft(perft_bb, 4) << endl;*/
  /**/

  free(lut);
  return 0;
}
