#include "bitboard.h"
#include "lookup_table.h"
#include <bit>
#include <bitset>
#include <iostream>

#define WHITE_PAWNS_BB 0
#define BLACK_PAWNS_BB 1
#define WHITE_ROOKS_BB 2
#define BLACK_ROOKS_BB 3
#define WHITE_KNIGHTS_BB 4
#define BLACK_KNIGHTS_BB 5
#define WHITE_BISHOPS_BB 6
#define BLACK_BISHOPS_BB 7
#define WHITE_QUEENS_BB 8
#define BLACK_QUEENS_BB 9
#define WHITE_KING_BB 10
#define BLACK_KING_BB 11

#define WHITE_PAWNS_INIT 0x000000000000FF00
#define WHITE_ROOKS_INIT 0x0000000000000081
#define WHITE_KNIGHTS_INIT 0x0000000000000042
#define WHITE_BISHOPS_INIT 0x0000000000000024
#define WHITE_QUEENS_INIT 0x0000000000000008
#define WHITE_KING_INIT 0x0000000000000010

#define BLACK_PAWNS_INIT 0x00FF000000000000
#define BLACK_ROOKS_INIT 0x8100000000000000
#define BLACK_KNIGHTS_INIT 0x4200000000000000
#define BLACK_BISHOPS_INIT 0x2400000000000000
#define BLACK_QUEENS_INIT 0x0800000000000000
#define BLACK_KING_INIT 0x1000000000000000

Bitboard::Bitboard() {
  whitePawns = WHITE_PAWNS_INIT;
  blackPawns = BLACK_PAWNS_INIT;
  whiteRooks = WHITE_ROOKS_INIT;
  blackRooks = BLACK_ROOKS_INIT;
  whiteKnights = WHITE_KNIGHTS_INIT;
  blackKnights = BLACK_KNIGHTS_INIT;
  whiteBishops = WHITE_BISHOPS_INIT;
  blackBishops = BLACK_BISHOPS_INIT;
  whiteQueens = WHITE_QUEENS_INIT;
  blackQueens = BLACK_QUEENS_INIT;
  whiteKing = WHITE_KING_INIT;
  blackKing = BLACK_KING_INIT;

  allWhitePieces = (whiteKing | whiteQueens | whiteBishops | whiteKnights |
                    whiteRooks | whitePawns);

  allBlackPieces = (blackKing | blackQueens | blackBishops | blackKnights |
                    blackRooks | blackPawns);

  allPieces = (allWhitePieces | allBlackPieces);

  emptySquares = ~allPieces;

  piecesBB[WHITE_PAWNS_BB] = whitePawns;
  piecesBB[BLACK_PAWNS_BB] = blackPawns;
  piecesBB[WHITE_ROOKS_BB] = whiteRooks;
  piecesBB[BLACK_ROOKS_BB] = blackRooks;
  piecesBB[WHITE_KNIGHTS_BB] = whiteKnights;
  piecesBB[BLACK_KNIGHTS_BB] = blackKnights;
  piecesBB[WHITE_BISHOPS_BB] = whiteBishops;
  piecesBB[BLACK_BISHOPS_BB] = blackBishops;
  piecesBB[WHITE_QUEENS_BB] = whiteQueens;
  piecesBB[BLACK_QUEENS_BB] = blackQueens;
  piecesBB[WHITE_KING_BB] = whiteKing;
  piecesBB[BLACK_KING_BB] = blackKing;

  turn = WHITE;
}

Bitboard::Bitboard(bitset<64> wP, bitset<64> bP, bitset<64> wR, bitset<64> bR,
                   bitset<64> wN, bitset<64> bN, bitset<64> wB, bitset<64> bB,
                   bitset<64> wQ, bitset<64> bQ, bitset<64> wK, bitset<64> bK,
                   Color turn_color) {
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

  piecesBB[WHITE_PAWNS_BB] = whitePawns;
  piecesBB[BLACK_PAWNS_BB] = blackPawns;
  piecesBB[WHITE_ROOKS_BB] = whiteRooks;
  piecesBB[BLACK_ROOKS_BB] = blackRooks;
  piecesBB[WHITE_KNIGHTS_BB] = whiteKnights;
  piecesBB[BLACK_KNIGHTS_BB] = blackKnights;
  piecesBB[WHITE_BISHOPS_BB] = whiteBishops;
  piecesBB[BLACK_BISHOPS_BB] = blackBishops;
  piecesBB[WHITE_QUEENS_BB] = whiteQueens;
  piecesBB[BLACK_QUEENS_BB] = blackQueens;
  piecesBB[WHITE_KING_BB] = whiteKing;
  piecesBB[BLACK_KING_BB] = blackKing;

  turn = turn_color;
}

Bitboard::Bitboard(Color none) {
  whitePawns.reset();
  blackPawns.reset();
  whiteRooks.reset();
  blackRooks.reset();
  whiteKnights.reset();
  blackKnights.reset();
  whiteBishops.reset();
  blackBishops.reset();
  whiteQueens.reset();
  blackQueens.reset();
  whiteKing.reset();
  blackKing.reset();

  allWhitePieces = (whiteKing | whiteQueens | whiteBishops | whiteKnights |
                    whiteRooks | whitePawns);

  allBlackPieces = (blackKing | blackQueens | blackBishops | blackKnights |
                    blackRooks | blackPawns);

  allPieces = (allWhitePieces | allBlackPieces);

  emptySquares = ~allPieces;

  piecesBB[WHITE_PAWNS_BB] = whitePawns;
  piecesBB[BLACK_PAWNS_BB] = blackPawns;
  piecesBB[WHITE_ROOKS_BB] = whiteRooks;
  piecesBB[BLACK_ROOKS_BB] = blackRooks;
  piecesBB[WHITE_KNIGHTS_BB] = whiteKnights;
  piecesBB[BLACK_KNIGHTS_BB] = blackKnights;
  piecesBB[WHITE_BISHOPS_BB] = whiteBishops;
  piecesBB[BLACK_BISHOPS_BB] = blackBishops;
  piecesBB[WHITE_QUEENS_BB] = whiteQueens;
  piecesBB[BLACK_QUEENS_BB] = blackQueens;
  piecesBB[WHITE_KING_BB] = whiteKing;
  piecesBB[BLACK_KING_BB] = blackKing;

  turn = none;
}

void Bitboard::setLookupTable(LookupTable *lut) { lookupTable = lut; }

bitset<64> Bitboard::generateKingAttacks(int square) {
  bitset<64> kings, pieces;

  bitset<64> king = lookupTable->piece_lookup[square];

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

  return (left_move | right_move | upward_move | downward_move |
          diagonal_right_down_move | diagonal_left_down_move |
          diagonal_left_up_move | diagonal_right_up_move);
}

bitset<64> Bitboard::generateKingMoves(Color color) {
  bitset<64> pieces;

  color == WHITE ? (pieces = allWhitePieces) : (pieces = allBlackPieces);

  return generateKingAttacks(color) & ~pieces;
}

bitset<64> Bitboard::generateKnightAttacks(int square) {
  bitset<64> knights, pieces;

  knights = blackKnights | whiteKnights;
  bitset<64> knight = lookupTable->piece_lookup[square];

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

  return (up_left_move | up_right_move | down_left_move | down_right_move |
          right_up_move | right_down_move | left_up_move | left_down_move);
}

bitset<64> Bitboard::generateKnightMoves(Color color, int square) {
  bitset<64> pieces;

  color == WHITE ? (pieces = allWhitePieces) : (pieces = allBlackPieces);

  return generateKnightAttacks(square) & ~pieces;
}

bitset<64> Bitboard::generatePawnAttacks(Color color, int square) {
  bitset<64> pawn, pieces;

  color == WHITE ? (pawn = whitePawns, pieces = allBlackPieces)
                 : (pawn = blackPawns, pieces = allWhitePieces);

  pawn = lookupTable->piece_lookup[square];

  bitset<64> a_file_clipping = pawn & lookupTable->clear_file[0];
  bitset<64> h_file_clipping = pawn & lookupTable->clear_file[7];

  bitset<64> valid_attacks =
      (((pawn & a_file_clipping) << 7) | ((pawn & h_file_clipping) << 9)) &
      pieces;

  return valid_attacks;
}

bitset<64> Bitboard::generatePawnMoves(Color color, int square) {
  bitset<64> pawn, pieces;
  int rank;

  color == WHITE ? (pawn = whitePawns, rank = 2, pieces = allBlackPieces)
                 : (pawn = blackPawns, rank = 5, pieces = allWhitePieces);

  pawn = lookupTable->piece_lookup[square] & pawn;

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

  bitset<64> valid_attacks = generatePawnAttacks(color, square);

  bitset<64> valid_moves = pawn_one_step | pawn_two_steps | valid_attacks;

  return valid_moves;
}

bitset<64> Bitboard::generateDiagonalAttacks(int square) {
  bitset<64> forward, reverse;

  int rank_index = square / 8;
  int file_index = square % 8;
  int diagonal_index = (rank_index - file_index) & 15;

  // Getting the pieces assocciated to the diagonal of the sliding piece
  bitset<64> occupancy = allPieces;
  bitset<64> diagonal = lookupTable->mask_diagonal[diagonal_index];

  forward = occupancy & diagonal;
  reverse = (bitset<64>)byteswap(forward.to_ulong());

  // Getting everything but the sliding piece given the diagonal pieces
  forward = (bitset<64>)(forward.to_ulong() -
                         (lookupTable->piece_lookup[square] << 1).to_ulong());
  reverse =
      (bitset<64>)(reverse.to_ulong() -
                   (byteswap(
                       (lookupTable->piece_lookup[square] << 1).to_ulong())));

  forward ^= (bitset<64>)byteswap(reverse.to_ulong());
  forward &= diagonal;

  return forward;
}

bitset<64> Bitboard::generateAntiDiagonalAttacks(int square) {
  bitset<64> forward, reverse;

  int rank_index = square / 8;
  int file_index = square % 8;
  int anti_diagonal_index = (rank_index + file_index) ^ 7;

  // Getting the pieces assocciated to the diagonal of the sliding piece
  bitset<64> occupancy = allPieces;
  bitset<64> anti_diagonal =
      lookupTable->mask_antidiagonal[anti_diagonal_index];

  forward = occupancy & anti_diagonal;
  reverse = (bitset<64>)byteswap(forward.to_ulong());

  // Getting everything but the sliding piece given the diagonal pieces
  forward = (bitset<64>)(forward.to_ulong() -
                         (lookupTable->piece_lookup[square] << 1).to_ulong());
  reverse =
      (bitset<64>)(reverse.to_ulong() -
                   (byteswap(
                       (lookupTable->piece_lookup[square] << 1).to_ulong())));

  forward ^= (bitset<64>)byteswap(reverse.to_ulong());
  forward &= anti_diagonal;

  return forward;
}

bitset<64> Bitboard::generateFileAttacks(int square) {
  bitset<64> forward, reverse;

  int file_index = square % 8;

  // Getting the pieces assocciated to the diagonal of the sliding piece
  bitset<64> occupancy = allPieces;
  bitset<64> file = lookupTable->mask_file[file_index];

  forward = occupancy & file;
  reverse = (bitset<64>)byteswap(forward.to_ulong());

  // Getting everything but the sliding piece given the diagonal pieces
  forward = (bitset<64>)(forward.to_ulong() -
                         (lookupTable->piece_lookup[square] << 1).to_ulong());
  reverse =
      (bitset<64>)(reverse.to_ulong() -
                   (byteswap(
                       (lookupTable->piece_lookup[square] << 1).to_ulong())));

  forward ^= (bitset<64>)byteswap(reverse.to_ulong());
  forward &= file;

  return forward;
}

bitset<64> Bitboard::generateRankAttacks(int square) {
  int file = square % 8;
  int rank = square / 8;

  int rank_x8 = rank * 8;
  int rank_occupancy_x2 = (allPieces >> rank_x8).to_ulong() & 2 * 63;

  bitset<64> attacks =
      lookupTable->mask_first_rank_attacks[4 * rank_occupancy_x2 + file];

  return attacks << rank_x8;
}

bitset<64> Bitboard::generateBishopAttacks(int square) {
  return generateDiagonalAttacks(square) | generateAntiDiagonalAttacks(square);
}

bitset<64> Bitboard::generateRookAttacks(int square) {
  return generateFileAttacks(square) | generateRankAttacks(square);
}

bitset<64> Bitboard::generateQueenAttacks(int square) {
  return generateBishopAttacks(square) | generateRookAttacks(square);
}

void Bitboard::nonSlidingAttacks() {
  int i;

  /* Loop over all squares */
  for (i = 0; i < SQUARES; i++) {
    /* Get all squaresible pawn attacks for each color and each square */
    pawnAttacks[WHITE][i] = generatePawnAttacks(WHITE, i);
    pawnAttacks[BLACK][i] = generatePawnAttacks(BLACK, i);

    /* Get all squaresible knight attacks for each square */
    knightAttacks[i] = generateKnightAttacks(i);

    /* Get all squaresible king attacks for each square */
    kingAttacks[i] = generateKingAttacks(i);
  }
}

void Bitboard::slidingAttacks() {
  int i;

  /* Loop over all squares */
  for (i = 0; i < SQUARES; i++) {
    /* Get all possible bishop attacks for each square */
    bishopAttacks[i] = generateBishopAttacks(i);

    /* Get all possible rook attacks for each square */
    rookAttacks[i] = generateRookAttacks(i);
  }
}

bitset<64> Bitboard::attacksToSquare(int square) {
  nonSlidingAttacks();
  slidingAttacks();

  bitset<64> knights, kings, bishopsAndQueens, rooksAndQueens;

  /* Union all the same moveset pieces */
  knights = whiteKnights | blackKnights;
  kings = whiteKing | blackKing;
  bitset<64> queens = whiteQueens | blackQueens;
  bishopsAndQueens = whiteBishops | blackBishops | queens;
  rooksAndQueens = whiteRooks | blackRooks | queens;

  /* Intersect them with attacks sets */
  return (pawnAttacks[WHITE][square] & blackPawns,
          pawnAttacks[BLACK][square] & whitePawns,
          knightAttacks[square] & knights, kingAttacks[square] & kings,
          bishopAttacks[square] & bishopsAndQueens,
          rookAttacks[square] & rooksAndQueens);
}

bool Bitboard::isSquareAttacked(Color side, int square) {
  nonSlidingAttacks();
  slidingAttacks();

  /* Intersect side pieces with opponent attacks */
  bitset<64> pawns = (side == WHITE) ? whitePawns : blackPawns;
  if ((pawnAttacks[(side == WHITE) ? BLACK : WHITE][square] & pawns).any())
    return true;

  bitset<64> knights = (side == WHITE) ? whiteKnights : blackKnights;
  if ((knightAttacks[square] & knights).any())
    return true;

  bitset<64> kings = (side == WHITE) ? whiteKing : blackKing;
  if ((kingAttacks[square] & kings).any())
    return true;

  bitset<64> queens = (side == WHITE) ? whiteQueens : blackQueens;

  bitset<64> bishopsAndQueens = (side == WHITE) ? whiteBishops : blackBishops;
  bishopsAndQueens |= bishopsAndQueens | queens;
  if ((bishopAttacks[square] & bishopsAndQueens).any())
    return true;

  bitset<64> rooksAndQueens = (side == WHITE) ? whiteRooks : blackRooks;
  if ((rookAttacks[square] & rooksAndQueens).any())
    return true;

  return false;
}

void Bitboard::generateMoves() {

  /* Loop over all the bitboards */
  for (int i = 0; i < 12; i++) {

    /* Get a copy of each bitboard */
    bitset<64> bb = piecesBB[i];

    /* Generate white pawns moves */
    if (i == WHITE_PAWNS_BB) {
      /* Get the squares of each white pawn */
    }
  }
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
  int i, j, z;
  char c = 'a';

  cout << endl;

  for (i = RANKS - 1; i >= 0; i--) {
    cout << i + 1 << " ";

    for (j = 0; j < FILES; j++) {
      int square = (8 * i) + j;
      int piece = -1;

      for (z = 0; z < 12; z++) {
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
};
