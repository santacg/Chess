#include "bitboard.h"
#include "lookup_table.h"
#include <bit>
#include <bitset>
#include <cstdlib>
#include <iostream>

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

/* Private move generator functions */
bitset<64> generate_pawn_attacks(BitBoard *bb, LookupTable *lut, Color color,
                                 int pos);

bitset<64> generate_diagonal_moves(BitBoard *bb, LookupTable *lut, int pos);

bitset<64> generate_antidiagonal_moves(BitBoard *bb, LookupTable *lut, int pos);

bitset<64> generate_file_moves(BitBoard *bb, LookupTable *lut, int pos);

/* Attacking bitboards methods */

void nonSlidingAttacks();

void SlidingAttacks();

BitBoard *init_bitboard() {

  BitBoard *bb = (BitBoard *)malloc(sizeof(BitBoard));

  if (bb == NULL) {
    return NULL;
  }

  bb->whitePawns = WHITE_PAWNS_POS;
  bb->blackPawns = BLACK_PAWNS_POS;
  bb->whiteRooks = WHITE_ROOKS_POS;
  bb->blackRooks = BLACK_ROOKS_POS;
  bb->whiteKnights = WHITE_KNIGHTS_POS;
  bb->blackKnights = BLACK_KNIGHTS_POS;
  bb->whiteBishops = WHITE_BISHOPS_POS;
  bb->blackBishops = BLACK_BISHOPS_POS;
  bb->whiteQueens = WHITE_QUEENS_POS;
  bb->blackQueens = BLACK_QUEENS_POS;
  bb->whiteKing = WHITE_KING_POS;
  bb->blackKing = BLACK_KING_POS;

  bb->allWhitePieces = (bb->whiteKing | bb->whiteQueens | bb->whiteBishops |
                        bb->whiteKnights | bb->whiteRooks | bb->whitePawns);

  bb->allBlackPieces = (bb->blackKing | bb->blackQueens | bb->blackBishops |
                        bb->blackKnights | bb->blackRooks | bb->blackPawns);

  bb->allPieces = (bb->allWhitePieces | bb->allBlackPieces);

  bb->emptySquares = ~bb->allPieces;

  bb->piecesBB[WHITE_PAWNS_BB] = bb->whitePawns;
  bb->piecesBB[BLACK_PAWNS_BB] = bb->blackPawns;
  bb->piecesBB[WHITE_ROOKS_BB] = bb->whiteRooks;
  bb->piecesBB[BLACK_ROOKS_BB] = bb->blackRooks;
  bb->piecesBB[WHITE_KNIGHTS_BB] = bb->whiteKnights;
  bb->piecesBB[BLACK_KNIGHTS_BB] = bb->blackKnights;
  bb->piecesBB[WHITE_BISHOPS_BB] = bb->whiteBishops;
  bb->piecesBB[BLACK_BISHOPS_BB] = bb->blackBishops;
  bb->piecesBB[WHITE_QUEENS_BB] = bb->whiteQueens;
  bb->piecesBB[BLACK_QUEENS_BB] = bb->blackQueens;
  bb->piecesBB[WHITE_KING_BB] = bb->whiteKing;
  bb->piecesBB[BLACK_KING_BB] = bb->blackKing;

  bb->turn = WHITE;

  return bb;
}

BitBoard *init_custom_bitboard(bitset<64> wP, bitset<64> bP, bitset<64> wR,
                               bitset<64> bR, bitset<64> wN, bitset<64> bN,
                               bitset<64> wB, bitset<64> bB, bitset<64> wQ,
                               bitset<64> bQ, bitset<64> wK, bitset<64> bK,
                               Color turn_color) {

  BitBoard *bb = (BitBoard *)malloc(sizeof(BitBoard));

  bb->whitePawns |= wP;
  bb->blackPawns |= bP;
  bb->whiteRooks |= wR;
  bb->blackRooks |= bR;
  bb->whiteKnights |= wN;
  bb->blackKnights |= bN;
  bb->whiteBishops |= wB;
  bb->blackBishops |= bB;
  bb->whiteQueens |= wQ;
  bb->blackQueens |= bQ;
  bb->whiteKing |= wK;
  bb->blackKing |= bK;

  bb->allWhitePieces = (bb->whiteKing | bb->whiteQueens | bb->whiteBishops |
                        bb->whiteKnights | bb->whiteRooks | bb->whitePawns);

  bb->allBlackPieces = (bb->blackKing | bb->blackQueens | bb->blackBishops |
                        bb->blackKnights | bb->blackRooks | bb->blackPawns);

  bb->allPieces = (bb->allWhitePieces | bb->allBlackPieces);

  bb->emptySquares = ~bb->allPieces;

  bb->piecesBB[WHITE_PAWNS_BB] = bb->whitePawns;
  bb->piecesBB[BLACK_PAWNS_BB] = bb->blackPawns;
  bb->piecesBB[WHITE_ROOKS_BB] = bb->whiteRooks;
  bb->piecesBB[BLACK_ROOKS_BB] = bb->blackRooks;
  bb->piecesBB[WHITE_KNIGHTS_BB] = bb->whiteKnights;
  bb->piecesBB[BLACK_KNIGHTS_BB] = bb->blackKnights;
  bb->piecesBB[WHITE_BISHOPS_BB] = bb->whiteBishops;
  bb->piecesBB[BLACK_BISHOPS_BB] = bb->blackBishops;
  bb->piecesBB[WHITE_QUEENS_BB] = bb->whiteQueens;
  bb->piecesBB[BLACK_QUEENS_BB] = bb->blackQueens;
  bb->piecesBB[WHITE_KING_BB] = bb->whiteKing;
  bb->piecesBB[BLACK_KING_BB] = bb->blackKing;

  bb->turn = turn_color;

  return bb;
}

bitset<64> generate_king_moves(BitBoard *bb, LookupTable *lut, Color color) {
  bitset<64> king, pieces;

  color == WHITE ? (king = bb->whiteKing, pieces = bb->allWhitePieces)
                 : (king = bb->blackKing, pieces = bb->allBlackPieces);

  bitset<64> a_file_clipping = king & lut->clear_file[0];
  bitset<64> h_file_clipping = king & lut->clear_file[7];

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

bitset<64> generate_knight_moves(BitBoard *bb, LookupTable *lut, Color color,
                                 int pos) {
  bitset<64> knights, pieces;

  color == WHITE ? (knights = bb->whiteKnights, pieces = bb->allWhitePieces)
                 : (knights = bb->blackKnights, pieces = bb->allBlackPieces);

  bitset<64> knight = lut->piece_lookup[pos] & knights;

  bitset<64> a_file_clipping = knight & lut->clear_file[0];
  bitset<64> h_file_clipping = knight & lut->clear_file[7];
  bitset<64> b_file_clipping = knight & lut->clear_file[1];
  bitset<64> g_file_clipping = knight & lut->clear_file[6];

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

bitset<64> generate_pawn_attacks(BitBoard *bb, LookupTable *lut, Color color,
                                 int pos) {
  bitset<64> pawn, pieces;

  color == WHITE ? (pawn = bb->whitePawns, pieces = bb->allBlackPieces)
                 : (pawn = bb->blackPawns, pieces = bb->allWhitePieces);

  pawn = lut->piece_lookup[pos] & pawn;

  bitset<64> a_file_clipping = pawn & lut->clear_file[0];
  bitset<64> h_file_clipping = pawn & lut->clear_file[7];

  bitset<64> valid_attacks =
      (((pawn & a_file_clipping) << 7) | ((pawn & h_file_clipping) << 9)) &
      pieces;

  return valid_attacks;
}

bitset<64> generate_pawn_moves(BitBoard *bb, LookupTable *lut, Color color,
                               int pos) {
  bitset<64> pawn, pieces;
  int rank;

  color == WHITE
      ? (pawn = bb->whitePawns, rank = 2, pieces = bb->allBlackPieces)
      : (pawn = bb->blackPawns, rank = 5, pieces = bb->allWhitePieces);

  pawn = lut->piece_lookup[pos] & pawn;

  bitset<64> pawn_one_step, pawn_two_steps;

  /* dumb7fill for two steps generation wiht an unrolled loop*/
  if (color == WHITE) {
    pawn_one_step = (pawn << 8) & ~bb->allPieces;
    pawn_two_steps =
        ((pawn_one_step & lut->mask_rank[rank]) << 8) & ~bb->allPieces;
  } else {
    pawn_one_step = (pawn >> 8) & ~bb->allPieces;
    pawn_two_steps =
        ((pawn_one_step & lut->mask_rank[rank]) >> 8) & ~bb->allPieces;
  }

  bitset<64> valid_attacks = generate_pawn_attacks(bb, lut, color, pos);

  bitset<64> valid_moves = pawn_one_step | pawn_two_steps | valid_attacks;

  return valid_moves;
}

bitset<64> generate_diagonal_moves(BitBoard *bb, LookupTable *lut, int pos) {
  bitset<64> forward, reverse;

  int rank_index = pos / 8;
  int file_index = pos % 8;
  int diagonal_index = (rank_index - file_index) & 15;

  // Getting the pieces assocciated to the diagonal of the sliding piece
  bitset<64> occupancy = bb->allPieces;
  bitset<64> diagonal = lut->mask_diagonal[diagonal_index];

  forward = occupancy & diagonal;
  reverse = (bitset<64>)byteswap(forward.to_ulong());

  // Getting everything but the sliding piece given the diagonal pieces
  forward = (bitset<64>)(forward.to_ulong() -
                         (lut->piece_lookup[pos] << 1).to_ulong());
  reverse = (bitset<64>)(reverse.to_ulong() -
                         (byteswap((lut->piece_lookup[pos] << 1).to_ulong())));

  forward ^= (bitset<64>)byteswap(reverse.to_ulong());
  forward &= diagonal;

  return forward;
}

bitset<64> generate_antidiagonal_moves(BitBoard *bb, LookupTable *lut,
                                       int pos) {
  bitset<64> forward, reverse;

  int rank_index = pos / 8;
  int file_index = pos % 8;
  int anti_diagonal_index = (rank_index + file_index) ^ 7;

  // Getting the pieces assocciated to the diagonal of the sliding piece
  bitset<64> occupancy = bb->allPieces;
  bitset<64> anti_diagonal = lut->mask_antidiagonal[anti_diagonal_index];

  forward = occupancy & anti_diagonal;
  reverse = (bitset<64>)byteswap(forward.to_ulong());

  // Getting everything but the sliding piece given the diagonal pieces
  forward = (bitset<64>)(forward.to_ulong() -
                         (lut->piece_lookup[pos] << 1).to_ulong());
  reverse = (bitset<64>)(reverse.to_ulong() -
                         (byteswap((lut->piece_lookup[pos] << 1).to_ulong())));

  forward ^= (bitset<64>)byteswap(reverse.to_ulong());
  forward &= anti_diagonal;

  return forward;
}

bitset<64> generate_file_moves(BitBoard *bb, LookupTable *lut, int pos) {
  bitset<64> forward, reverse;

  int file_index = pos % 8;

  // Getting the pieces assocciated to the diagonal of the sliding piece
  bitset<64> occupancy = bb->allPieces;
  bitset<64> file = lut->mask_file[file_index];

  forward = occupancy & file;
  reverse = (bitset<64>)byteswap(forward.to_ulong());

  // Getting everything but the sliding piece given the diagonal pieces
  forward = (bitset<64>)(forward.to_ulong() -
                         (lut->piece_lookup[pos] << 1).to_ulong());
  reverse = (bitset<64>)(reverse.to_ulong() -
                         (byteswap((lut->piece_lookup[pos] << 1).to_ulong())));

  forward ^= (bitset<64>)byteswap(reverse.to_ulong());
  forward &= file;

  return forward;
}

bitset<64> generate_bishop_moves(BitBoard *bb, LookupTable *lut, int pos) {
  return generate_diagonal_moves(bb, lut, pos) |
         generate_antidiagonal_moves(bb, lut, pos);
}

bitset<64> generate_rook_moves(BitBoard *bb, LookupTable *lut, int pos) {
  return generate_file_moves(bb, lut, pos);
}

bitset<64> generate_queen_moves(BitBoard *bb, LookupTable *lut, int pos) {
  return generate_file_moves(bb, lut, pos) |
         generate_diagonal_moves(bb, lut, pos) |
         generate_antidiagonal_moves(bb, lut, pos);
}

void nonSlidingAttacks(BitBoard *bb) { int i; }

void print_bitset(bitset<64> bitboard) {
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

void print_bitboard(BitBoard *bb, LookupTable *lut) {
  int i, j;
  char c = 'a';

  /* Chess unicode ascii pieces */
  const char *asciiPieces[12] = {"♙", "♟", "♖", "♜", "♘", "♞",
                                 "♗", "♝", "♛", "♕", "♔", "♚"};
  cout << endl;

  for (i = RANKS - 1; i >= 0; i--) {
    cout << i << " ";

    for (j = 0; j < FILES; j++) {
      int square = (8 * i) + j;
      int piece = -1;

      for (int z = 0; z < 12; z++) {
        if ((bb->piecesBB[z] & lut->piece_lookup[square]).any() == true) {
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
  (bb->turn == WHITE) ? cout << "white" : cout << "black";

  cout << endl << endl;
}
