#include "bitboard.h"
#include "lookup_table.h"
#include <bit>
#include <bitset>
#include <iostream>

Bitboard::Bitboard(bitset<64> wP, bitset<64> bP, bitset<64> wR, bitset<64> bR,
                   bitset<64> wN, bitset<64> bN, bitset<64> wB, bitset<64> bB,
                   bitset<64> wQ, bitset<64> bQ, bitset<64> wK, bitset<64> bK) {

  lookupTable = lookup_table_init();

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

  allWhitePieces = (whiteKing | whiteQueens | whiteBishops | whiteKnights |
                    whiteRooks | whitePawns);

  allBlackPieces = (blackKing | blackQueens | blackBishops | blackKnights |
                    blackRooks | blackPawns);

  allPieces = (allWhitePieces | allBlackPieces);

  emptySquares = ~allPieces;

  piecesBB[0] = whitePawns;
  piecesBB[1] = blackPawns;
  piecesBB[2] = whiteRooks;
  piecesBB[3] = blackRooks;
  piecesBB[4] = whiteKnights;
  piecesBB[5] = blackKnights;
  piecesBB[6] = whiteBishops;
  piecesBB[7] = blackBishops;
  piecesBB[8] = whiteQueens;
  piecesBB[9] = blackQueens;
  piecesBB[10] = whiteKing;
  piecesBB[11] = blackKing;

  turn = WHITE;
}

bitset<64> Bitboard::generateKingMoves(Color color) {
  bitset<64> king, pieces;

  color == WHITE ? (king = whiteKing, pieces = allWhitePieces)
                 : (king = blackKing, pieces = allBlackPieces);

  bitset<64> a_file_clipping = king & lookupTable->clear_file[0];
  bitset<64> h_file_clipping = king & lookupTable->clear_file[7];

  bitset<64> left_move = a_file_clipping >> 1;
  bitset<64> right_move = h_file_clipping << 1;
  bitset<64> upward_move = king << 8;
  bitset<64> downward_move = king >> 8;
  bitset<64> diagonal_left_up_move = a_file_clipping << 7;
  bitset<64> diagonal_left_down_move = a_file_clipping >> 9;
  bitset<64> diagonal_right_up_move = h_file_clipping << 9;
  bitset<64> diagonal_right_down_move = h_file_clipping >> 7;

  bitset<64> valid_moves =
      (left_move | right_move | upward_move | downward_move |
       diagonal_right_down_move | diagonal_left_down_move |
       diagonal_left_up_move | diagonal_right_up_move);

  valid_moves = valid_moves & ~pieces;

  return valid_moves;
}

bitset<64> Bitboard::generateKnightMoves(Color color, int pos) {
  bitset<64> knights, pieces;

  color == WHITE ? (knights = whiteKnights, pieces = allWhitePieces)
                 : (knights = blackKnights, pieces = allBlackPieces);

  bitset<64> knight = lookupTable->piece_lookup[pos] & knights;

  bitset<64> a_file_clipping = knight & lookupTable->clear_file[0];
  bitset<64> h_file_clipping = knight & lookupTable->clear_file[7];
  bitset<64> b_file_clipping = knight & lookupTable->clear_file[1];
  bitset<64> g_file_clipping = knight & lookupTable->clear_file[6];

  bitset<64> up_left_move = a_file_clipping << 15;
  bitset<64> up_right_move = h_file_clipping << 17;
  bitset<64> down_left_move = a_file_clipping >> 17;
  bitset<64> down_right_move = h_file_clipping >> 15;
  bitset<64> right_up_move = (h_file_clipping & g_file_clipping) << 10;
  bitset<64> right_down_move = (h_file_clipping & g_file_clipping) >> 6;
  bitset<64> left_up_move = (a_file_clipping & b_file_clipping) << 6;
  bitset<64> left_down_move = (a_file_clipping & b_file_clipping) >> 10;

  bitset<64> valid_moves =
      (up_left_move | up_right_move | down_left_move | down_right_move |
       right_up_move | right_down_move | left_up_move | left_down_move);

  valid_moves = valid_moves & ~pieces;

  return valid_moves;
}

bitset<64> Bitboard::generatePawnMoves(Color color, int pos) {
  bitset<64> pawn, pieces;
  int rank;

  color == WHITE ? (pawn = whitePawns, rank = 2, pieces = allBlackPieces)
                 : (pawn = blackPawns, rank = 5, pieces = allWhitePieces);

  pawn = lookupTable->piece_lookup[pos] & pawn;

  bitset<64> a_file_clipping = pawn & lookupTable->clear_file[0];
  bitset<64> h_file_clipping = pawn & lookupTable->clear_file[7];

  bitset<64> pawn_one_step, pawn_two_steps;
  /* dumb7fill for two steps generation wiht an unrolled loop*/
  if (color == WHITE) {
    pawn_one_step = (pawn << 8) & ~allPieces;
    pawn_two_steps =
        ((pawn_one_step & lookupTable->mask_rank[rank]) << 8) & ~allPieces;
  } else {
    pawn_one_step = (pawn >> 8) & ~allPieces;
    pawn_two_steps =
        ((pawn_one_step & lookupTable->mask_rank[rank]) >> 8) & ~allPieces;
  }

  bitset<64> valid_attacks =
      (((pawn & a_file_clipping) << 7) | ((pawn & h_file_clipping) << 9)) &
      pieces;

  bitset<64> valid_moves = pawn_one_step | pawn_two_steps | valid_attacks;

  return valid_moves;
}

bitset<64> Bitboard::generateDiagonalMoves(int pos) {
  bitset<64> forward, reverse;

  int rank_index = pos / 8;
  int file_index = pos % 8;
  int diagonal_index = (rank_index - file_index) & 15;

  // Getting the pieces assocciated to the diagonal of the sliding piece
  bitset<64> occupancy = allPieces;
  bitset<64> diagonal = lookupTable->mask_diagonal[diagonal_index];

  forward = occupancy & diagonal;
  reverse = (bitset<64>)byteswap(forward.to_ulong());

  // Getting everything but the sliding piece given the diagonal pieces
  forward = (bitset<64>)(forward.to_ulong() -
                         (lookupTable->piece_lookup[pos] << 1).to_ulong());
  reverse =
      (bitset<64>)(reverse.to_ulong() -
                   (byteswap(
                       (lookupTable->piece_lookup[pos] << 1).to_ulong())));

  forward ^= (bitset<64>)byteswap(reverse.to_ulong());
  forward &= diagonal;

  return forward;
}

bitset<64> Bitboard::generateAntiDiagonalMoves(int pos) {
  bitset<64> forward, reverse;

  int rank_index = pos / 8;
  int file_index = pos % 8;
  int anti_diagonal_index = (rank_index + file_index) ^ 7;

  // Getting the pieces assocciated to the diagonal of the sliding piece
  bitset<64> occupancy = allPieces;
  bitset<64> anti_diagonal =
      lookupTable->mask_antidiagonal[anti_diagonal_index];

  forward = occupancy & anti_diagonal;
  reverse = (bitset<64>)byteswap(forward.to_ulong());

  // Getting everything but the sliding piece given the diagonal pieces
  forward = (bitset<64>)(forward.to_ulong() -
                         (lookupTable->piece_lookup[pos] << 1).to_ulong());
  reverse =
      (bitset<64>)(reverse.to_ulong() -
                   (byteswap(
                       (lookupTable->piece_lookup[pos] << 1).to_ulong())));

  forward ^= (bitset<64>)byteswap(reverse.to_ulong());
  forward &= anti_diagonal;

  return forward;
}

bitset<64> Bitboard::generateBishopMoves(int pos) {
  return generateDiagonalMoves(pos) | generateAntiDiagonalMoves(pos);
}

bitset<64> Bitboard::generateFileMoves(int pos) {
  bitset<64> forward, reverse;

  int file_index = pos % 8;

  // Getting the pieces assocciated to the diagonal of the sliding piece
  bitset<64> occupancy = allPieces;
  bitset<64> file = lookupTable->mask_file[file_index];

  forward = occupancy & file;
  reverse = (bitset<64>)byteswap(forward.to_ulong());

  // Getting everything but the sliding piece given the diagonal pieces
  forward = (bitset<64>)(forward.to_ulong() -
                         (lookupTable->piece_lookup[pos] << 1).to_ulong());
  reverse =
      (bitset<64>)(reverse.to_ulong() -
                   (byteswap(
                       (lookupTable->piece_lookup[pos] << 1).to_ulong())));

  forward ^= (bitset<64>)byteswap(reverse.to_ulong());
  forward &= file;

  return forward;
}

bitset<64> Bitboard::generateRookMoves(int pos) {
  return generateFileMoves(pos);
}

bitset<64> Bitboard::generateQueenMoves(int pos) {
  return generateFileMoves(pos) | generateDiagonalMoves(pos) |
         generateAntiDiagonalMoves(pos);
}

void Bitboard::printBitboard(bitset<64> bitboard) {
  int i, j;
  char c = 'a';

  cout << endl;

  for (i = RANKS; i > 0; i--) {
    cout << i << " ";

    for (j = FILES; j > 0; j--) {
      if (bitboard[(i * 8) - j] == true) {
        cout << "1";
      } else {
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

void Bitboard::printBoard() {
  int i, j;
  char c = 'a';

  cout << endl;

  for (i = RANKS - 1; i >= 0; i--) {
    cout << i << " ";

    for (j = 0; j < FILES; j++) {
      int square = (8 * i) + j;
      int piece = -1;

      for (int z = 0; z < 12; z++) {
        if ((piecesBB[z] & lookupTable->piece_lookup[square]).any() == true) {
          piece = z;
          break;
        }
      }

      (piece == -1) ? cout << '.' : cout << asciiPieces[piece];
      cout << " ";
    }

    cout << endl;
  }

  cout << "  ";
  for (i = 0; i < 8; i++, c++) {
    cout << c << " ";
  }

  cout << endl << endl;
  cout << "Side to move: ";
  (turn == WHITE) ? cout << "white" : cout << "black";

  cout << endl << endl;
}
