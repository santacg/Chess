#include "../includes/bitboard.h"
#include "../includes/lookup_table.h"
#include "../includes/move.h"
#include "../includes/utils.h"
#include <array>
#include <bitset>
#include <cctype>
#include <cstdlib>
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
int uci_parse_move(string uci_move_str, Bitboard *bb) {
  string source_square_str, target_square_str;
  char promotion;

  source_square_str.push_back(uci_move_str[0]);
  source_square_str.push_back(uci_move_str[1]);

  target_square_str.push_back(uci_move_str[2]);
  target_square_str.push_back(uci_move_str[3]);

  promotion = ' ';

  if (uci_move_str.length() == 5) {
    promotion = uci_move_str[4];
  }

  unsigned int source_square = squareToCoordinate.at(source_square_str);
  unsigned int target_square = squareToCoordinate.at(target_square_str);

  vector<Move> move_list = bb->getMoveList();

  Move move_found;
  bool found = false;

  for (Move move : move_list) {
    if (move.getSourceSquare() == source_square &&
        move.getTargetSquare() == target_square) {

      if (promotion == ' ') {
        move_found = move;
        found = true;
        break;
      }

      int flag = move.getFlag();

      bool capture_flag = (target_square == source_square - 8 ||
                           target_square == source_square + 8);

      // If statement probably more efficient and readable
      switch (promotion) {
      case 'q':
      case 'Q':
        if ((flag == QUEEN_PROMOTION && capture_flag == 0) ||
            (flag == QUEEN_PROMOTION_CAPTURE && capture_flag == 1)) {
          move_found = move;
          found = true;
        }
        break;
      case 'b':
      case 'B':
        if ((flag == BISHOP_PROMOTION && capture_flag == 0) ||
            (flag == BISHOP_PROMOTION_CAPTURE && capture_flag == 1)) {
          move_found = move;
          found = true;
        }
        break;
      case 'n':
      case 'N':
        if ((flag == KNIGHT_PROMOTION && capture_flag == 0) ||
            (flag == KNIGHT_PROMOTION_CAPTURE && capture_flag == 1)) {
          move_found = move;
          found = true;
        }
        break;
      case 'r':
      case 'R':
        if ((flag == ROOK_PROMOTION && capture_flag == 0) ||
            (flag == ROOK_PROMOTION_CAPTURE && capture_flag == 1)) {
          move_found = move;
          found = true;
        }
        break;
      }
    }

    if (found) {
      break;
    }
  }

  if (found == false) {
    return -1;
  }

  bool legal = bb->makeMove(move_found);

  if (legal == false) {
    return -1;
  }

  if (bb->isCheckmate(bb->getTurn())) {
    return bb->getTurn();
  } else if (bb->isStaleMate(bb->getTurn())) {
    return 3;
  }

  return 0;
}

/* UCI position parser */
void uci_parse_position(string uci_pos_str, Bitboard *bb, LookupTable *lut) {
  uci_pos_str.erase(0, 9);

  if (uci_pos_str.find("startpos") != uci_pos_str.npos) {
    *bb = Bitboard(lut);
    uci_pos_str.erase(0, 9);
  } else if (uci_pos_str.find("fen") != uci_pos_str.npos) {
    uci_pos_str.erase(0, 4);

    *bb = parse_fen(lut, uci_pos_str);
  } else {
    cout << "Bad UCI position command" << endl;
    return;
  }

  auto moves_pos = uci_pos_str.find("moves");

  if (moves_pos == uci_pos_str.npos) {
    return;
  }

  uci_pos_str.erase(0, moves_pos + 6);

  for (size_t i = 0; i + 1 < uci_pos_str.size(); i += 3) {
    string uci_move_str = uci_pos_str.substr(i, 4);
    cout << uci_move_str << endl;

    int res = uci_parse_move(uci_move_str, bb);

    if (res != 0) {
      break;
    }

    i += 2;
  }
}

array<int, 64> send_pos(Bitboard *bb) {
  array<int, 64> pieces = bb->getPiecesAtSquares();

  cout << "pos ";
  for (int i = 0; i < 64; i++) {
    cout << pieces[i] << "-";
  }
  cout << endl;

  return pieces;
}

void uci_loop() {
  string uci_command;

  // Print engine info
  cout << "id name Santachess 0.1" << endl;
  cout << "id author Carlos GS" << endl;

  // uciok - engine ready
  cout << "uciok" << endl;

  // wait for isready command
  do {
    getline(cin, uci_command);
  } while (uci_command != "isready");

  // Init everything
  LookupTable *lut = init_lookup_table();
  Bitboard bb = Bitboard(lut);

  // send readyok after receiving isready
  cout << "readyok" << endl;

  send_pos(&bb);
  // Get commands from stdin
  while (1) {
    // bb.printBoard();

    getline(cin, uci_command);

    if (uci_command.find("position") != uci_command.npos) {
      uci_parse_position(uci_command, &bb, lut);
      send_pos(&bb);
    } else if (uci_command.find("move") != uci_command.npos) {
      int move_res = uci_parse_move(uci_command.substr(5), &bb);
      if (move_res == -1) {
        cout << "illegal" << endl;
      } else if (move_res == 1 || move_res == 2) {
        send_pos(&bb);
        cout << "checkmate: " << move_res << endl;
      } else if (move_res == 2) {
        send_pos(&bb);
        cout << "stalemate" << endl;
      } else {
        send_pos(&bb);
      }
    } else if (uci_command == "quit") {
      break;
    } else {
      cout << "unknown command" << endl;
    }
  }

  free(lut);
  return;
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
    if (bitboard.makeMove(m) == true)
      nodes += perft(bitboard, depth - 1);
    bitboard = bb_cpy;
  }

  return nodes;
}

int main() {
  string command;

  getline(cin, command);

  if (command == "uci") {
    uci_loop();
  } else if (command == "perft") {
    // TO-DO use bb parsed from fen string
    LookupTable *lut = init_lookup_table();
    Bitboard perft_bb = Bitboard(lut);

    string fen_str;

    cout << "FEN (or press enter for default position): " << endl;
    getline(cin, fen_str);

    if (fen_str != "") {
      perft_bb = parse_fen(lut, fen_str);
    }

    perft_bb.printBoard();

    string depth_str;

    cout << "Enter desired depth: " << endl;
    getline(cin, depth_str);

    int depth = stoi(depth_str);

    cout << "performance test nodes: " << perft(perft_bb, depth) << endl;
  } else {
    cout << "engine cannot parse command" << endl;
    return -1;
  }

  return 0;
}
