#include "bitboard.h"
#include "lookup_table.h"
#include "move.h"
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
  piecesBB[WHITE_PAWNS_BB] = WHITE_PAWNS_INIT;
  piecesBB[BLACK_PAWNS_BB] = BLACK_PAWNS_INIT;
  piecesBB[WHITE_ROOKS_BB] = WHITE_ROOKS_INIT;
  piecesBB[BLACK_ROOKS_BB] = BLACK_ROOKS_INIT;
  piecesBB[WHITE_KNIGHTS_BB] = WHITE_KNIGHTS_INIT;
  piecesBB[BLACK_KNIGHTS_BB] = BLACK_KNIGHTS_INIT;
  piecesBB[WHITE_BISHOPS_BB] = WHITE_BISHOPS_INIT;
  piecesBB[BLACK_BISHOPS_BB] = BLACK_BISHOPS_INIT;
  piecesBB[WHITE_QUEENS_BB] = WHITE_QUEENS_INIT;
  piecesBB[BLACK_QUEENS_BB] = BLACK_QUEENS_INIT;
  piecesBB[WHITE_KING_BB] = WHITE_KING_INIT;
  piecesBB[BLACK_KING_BB] = BLACK_KING_INIT;

  allWhitePieces = (piecesBB[WHITE_KING_BB] | piecesBB[WHITE_QUEENS_BB] |
                    piecesBB[WHITE_BISHOPS_BB] | piecesBB[WHITE_KNIGHTS_BB] |
                    piecesBB[WHITE_ROOKS_BB] | piecesBB[WHITE_PAWNS_BB]);

  allBlackPieces = (piecesBB[BLACK_KING_BB] | piecesBB[BLACK_QUEENS_BB] |
                    piecesBB[BLACK_BISHOPS_BB] | piecesBB[BLACK_KNIGHTS_BB] |
                    piecesBB[BLACK_ROOKS_BB] | piecesBB[BLACK_PAWNS_BB]);

  allPieces = (allWhitePieces | allBlackPieces);

  emptySquares = ~allPieces;

  turn = WHITE;
}

Bitboard::Bitboard(bitset<64> wP, bitset<64> bP, bitset<64> wR, bitset<64> bR,
                   bitset<64> wN, bitset<64> bN, bitset<64> wB, bitset<64> bB,
                   bitset<64> wQ, bitset<64> bQ, bitset<64> wK, bitset<64> bK,
                   Color side) {
  piecesBB[WHITE_PAWNS_BB] = wP;
  piecesBB[BLACK_PAWNS_BB] = bP;
  piecesBB[WHITE_ROOKS_BB] = wR;
  piecesBB[BLACK_ROOKS_BB] = bR;
  piecesBB[WHITE_KNIGHTS_BB] = wN;
  piecesBB[BLACK_KNIGHTS_BB] = bN;
  piecesBB[WHITE_BISHOPS_BB] = wB;
  piecesBB[BLACK_BISHOPS_BB] = bB;
  piecesBB[WHITE_QUEENS_BB] = wQ;
  piecesBB[BLACK_QUEENS_BB] = bQ;
  piecesBB[WHITE_KING_BB] = wK;
  piecesBB[BLACK_KING_BB] = bK;

  allWhitePieces = (piecesBB[WHITE_KING_BB] | piecesBB[WHITE_QUEENS_BB] |
                    piecesBB[WHITE_BISHOPS_BB] | piecesBB[WHITE_KNIGHTS_BB] |
                    piecesBB[WHITE_ROOKS_BB] | piecesBB[WHITE_PAWNS_BB]);

  allBlackPieces = (piecesBB[BLACK_KING_BB] | piecesBB[BLACK_QUEENS_BB] |
                    piecesBB[BLACK_BISHOPS_BB] | piecesBB[BLACK_KNIGHTS_BB] |
                    piecesBB[BLACK_ROOKS_BB] | piecesBB[BLACK_PAWNS_BB]);
  allPieces = (allWhitePieces | allBlackPieces);

  emptySquares = ~allPieces;

  turn = side;
}

void Bitboard::setLookupTable(LookupTable *lut) { lookupTable = lut; }

bitset<64> Bitboard::generateKingAttacks(int square) {
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

bitset<64> Bitboard::generateKnightAttacks(int square) {
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

bitset<64> Bitboard::generatePawnAttacks(Color color, int square) {
  bitset<64> pawn;

  pawn = lookupTable->piece_lookup[square];

  bitset<64> a_file_clipping = pawn & lookupTable->clear_file[0];
  bitset<64> h_file_clipping = pawn & lookupTable->clear_file[7];

  bitset<64> attacks;
  if (color == WHITE) {
    attacks = a_file_clipping << 7 | h_file_clipping << 9;
  } else {
    attacks = a_file_clipping >> 9 | h_file_clipping >> 7;
  }

  return attacks;
}

bitset<64> Bitboard::generatePawnMoves(Color color, int square) {
  bitset<64> pawn;

  pawn = lookupTable->piece_lookup[square];
  bitset<64> pawn_one_step, pawn_two_steps;

  /* dumb7fill for two steps generation wiht an unrolled loop*/
  if (color == WHITE) {
    pawn_one_step = (pawn << 8) & emptySquares;
    pawn_two_steps =
        ((pawn_one_step & lookupTable->mask_rank[2]) << 8) & emptySquares;
  } else {
    pawn_one_step = (pawn >> 8) & emptySquares;
    pawn_two_steps =
        ((pawn_one_step & lookupTable->mask_rank[5]) >> 8) & emptySquares;
  }

  bitset<64> moves = pawn_one_step | pawn_two_steps;

  return moves;
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
    /* Get all possible pawn attacks for each color and each square */
    pawnAttacks[WHITE][i] = generatePawnAttacks(WHITE, i);
    pawnAttacks[BLACK][i] = generatePawnAttacks(BLACK, i);

    /* Get all possible knight attacks for each square */
    knightAttacks[i] = generateKnightAttacks(i);

    /* Get all possible king attacks for each square */
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
  knights = piecesBB[WHITE_KNIGHTS_BB] | piecesBB[BLACK_KNIGHTS_BB];
  kings = piecesBB[WHITE_KING_BB] | piecesBB[BLACK_KING_BB];
  bitset<64> queens = piecesBB[WHITE_QUEENS_BB] | piecesBB[BLACK_QUEENS_BB];
  bishopsAndQueens =
      piecesBB[WHITE_BISHOPS_BB] | piecesBB[BLACK_BISHOPS_BB] | queens;
  rooksAndQueens = piecesBB[WHITE_ROOKS_BB] | piecesBB[BLACK_ROOKS_BB] | queens;

  /* Intersect them with attacks sets */
  return (pawnAttacks[WHITE][square] & piecesBB[BLACK_PAWNS_BB],
          pawnAttacks[BLACK][square] & piecesBB[WHITE_PAWNS_BB],
          knightAttacks[square] & knights, kingAttacks[square] & kings,
          bishopAttacks[square] & bishopsAndQueens,
          rookAttacks[square] & rooksAndQueens);
}

bool Bitboard::isSquareAttacked(Color side, int square) {
  nonSlidingAttacks();
  slidingAttacks();

  /* Intersect side pieces with opponent attacks */
  bitset<64> pawns =
      (side == WHITE) ? piecesBB[WHITE_PAWNS_BB] : piecesBB[BLACK_PAWNS_BB];
  if ((pawnAttacks[(side == WHITE) ? BLACK : WHITE][square] & pawns).any())
    return true;

  bitset<64> knights =
      (side == WHITE) ? piecesBB[WHITE_KNIGHTS_BB] : piecesBB[BLACK_KNIGHTS_BB];
  if ((knightAttacks[square] & knights).any())
    return true;

  bitset<64> kings =
      (side == WHITE) ? piecesBB[WHITE_KING_BB] : piecesBB[BLACK_KING_BB];
  if ((kingAttacks[square] & kings).any())
    return true;

  bitset<64> queens =
      (side == WHITE) ? piecesBB[WHITE_QUEENS_BB] : piecesBB[BLACK_QUEENS_BB];

  bitset<64> bishopsAndQueens =
      (side == WHITE) ? piecesBB[WHITE_BISHOPS_BB] : piecesBB[BLACK_BISHOPS_BB];
  bishopsAndQueens |= bishopsAndQueens | queens;
  if ((bishopAttacks[square] & bishopsAndQueens).any())
    return true;

  bitset<64> rooksAndQueens =
      (side == WHITE) ? piecesBB[WHITE_ROOKS_BB] : piecesBB[BLACK_ROOKS_BB];
  rooksAndQueens |= rooksAndQueens | queens;
  if ((rookAttacks[square] & rooksAndQueens).any())
    return true;

  return false;
}

void Bitboard::pieceMoves(bitset<64> bb, Color color,
                          bitset<64> (Bitboard::*move_generator)(Color, int),
                          bitset<64> (Bitboard::*attack_generator)(Color,
                                                                   int)) {
  /* Loop over all the pieces in the bitboard */
  while (bb.any()) {
    /* Extract the less significant bit */
    int source_square = countr_zero(bb.to_ulong());

    /* Generate quiet moves */
    bitset<64> moves = (this->*move_generator)(color, source_square);

    /* Loop over all the moves generated */
    while (moves.any()) {
      int target_square = countr_zero(moves.to_ulong());
      moves.set(target_square, false);
      Move move = Move(source_square, target_square, QUIET_MOVE, PAWN);
      moveList.push_back(move);
    }

    bitset<64> pieces;
    (color == WHITE) ? pieces = allBlackPieces : pieces = allWhitePieces;

    /* Generate captures */
    bitset<64> captures =
        (this->*attack_generator)(color, source_square) & pieces;

    /* Loop over all the captures generated */
    while (captures.any()) {
      int target_square = countr_zero(captures.to_ulong());
      captures.set(target_square, false);
      Move move = Move(source_square, target_square, CAPTURE, PAWN);
      moveList.push_back(move);
    }

    /* Pop the less significant bit */
    bb.set(source_square, false);
  }
}

void Bitboard::pieceMoves(bitset<64> bb, Color color,
                          bitset<64> (Bitboard::*attackGenerator)(int),
                          int piece_type) {
  /* Loop over all the pieces in the bitboard */
  while (bb.any()) {
    /* Extract the less significant bit */
    int source_square = countr_zero(bb.to_ulong());

    /* Generate attacks */
    bitset<64> attacks = (this->*attackGenerator)(source_square);

    /* Generate quiet moves */
    bitset<64> moves = attacks & emptySquares;

    /* Generate captures */
    bitset<64> pieces;
    (color == WHITE) ? pieces = allBlackPieces : pieces = allWhitePieces;
    bitset<64> captures = attacks & pieces;

    /* Loop over all the moves generated */
    while (moves.any()) {
      int target_square = countr_zero(moves.to_ulong());
      moves.set(target_square, false);
      Move move = Move(source_square, target_square, QUIET_MOVE, piece_type);
      moveList.push_back(move);
    }

    /* Loop over all the captures generated */
    while (captures.any()) {
      int target_square = countr_zero(captures.to_ulong());
      captures.set(target_square, false);
      Move move = Move(source_square, target_square, CAPTURE, piece_type);
      moveList.push_back(move);
    }

    /* Pop the less significant bit */
    bb.set(source_square, false);
  }
}

void Bitboard::generateMoves() {

  /* Clear the move list */
  moveList.clear();

  /* Loop over all the bitboards */
  for (int i = 11; i >= 0; i--) {

    /* Get a copy of each piece bitboard */
    bitset<64> bb = piecesBB[i];

    /* Generate moves for each piece */
    if (i == WHITE_KING_BB) {
      pieceMoves(bb, WHITE, &Bitboard::generateKingAttacks, KING);
    } else if (i == BLACK_KING_BB) {
      pieceMoves(bb, BLACK, &Bitboard::generateKingAttacks, KING);
    } else if (i == WHITE_QUEENS_BB) {
      pieceMoves(bb, WHITE, &Bitboard::generateQueenAttacks, QUEEN);
    } else if (i == BLACK_QUEENS_BB) {
      pieceMoves(bb, BLACK, &Bitboard::generateQueenAttacks, QUEEN);
    } else if (i == WHITE_ROOKS_BB) {
      pieceMoves(bb, WHITE, &Bitboard::generateRookAttacks, ROOK);
    } else if (i == BLACK_ROOKS_BB) {
      pieceMoves(bb, BLACK, &Bitboard::generateRookAttacks, ROOK);
    } else if (i == WHITE_BISHOPS_BB) {
      pieceMoves(bb, WHITE, &Bitboard::generateBishopAttacks, BISHOP);
    } else if (i == BLACK_BISHOPS_BB) {
      pieceMoves(bb, BLACK, &Bitboard::generateBishopAttacks, BISHOP);
    } else if (i == WHITE_KNIGHTS_BB) {
      pieceMoves(bb, WHITE, &Bitboard::generateKnightAttacks, KNIGHT);
    } else if (i == BLACK_KNIGHTS_BB) {
      pieceMoves(bb, BLACK, &Bitboard::generateKnightAttacks, KNIGHT);
    } else if (i == WHITE_PAWNS_BB) {
      pieceMoves(bb, WHITE, &Bitboard::generatePawnMoves,
                 &Bitboard::generatePawnMoves);
    } else {
      pieceMoves(bb, BLACK, &Bitboard::generatePawnMoves,
                 &Bitboard::generatePawnAttacks);
    }
  }
}

Bitboard Bitboard::copyBoard() {
  Bitboard bitboard_cpy = *this;
  return bitboard_cpy;
}

void Bitboard::makeMove(Move move, Color color) {

  /* Get the move type */
  int flag = move.getFlag();
  int source_square = move.getSourceSquare();
  int target_square = move.getTargetSquare();
  int piece = move.getPiece();
  int pieceBB;
  (color == WHITE) ? pieceBB = piece * 2 : pieceBB = piece * 2 + 1;

  piecesBB[pieceBB].set(source_square, false);
  piecesBB[pieceBB].set(target_square, true);

  /* Make capture */
  if (flag == CAPTURE) {
    int i;
    (color == WHITE) ? i = 1 : i = 0;

    for (; i < 6; i = i + 2) {
      if (piecesBB[i].test(target_square) == true) {
        piecesBB[i].set(target_square, false);
        break;
      }
    }
  }

  allWhitePieces = (piecesBB[WHITE_KING_BB] | piecesBB[WHITE_QUEENS_BB] |
                    piecesBB[WHITE_BISHOPS_BB] | piecesBB[WHITE_KNIGHTS_BB] |
                    piecesBB[WHITE_ROOKS_BB] | piecesBB[WHITE_PAWNS_BB]);

  allBlackPieces = (piecesBB[BLACK_KING_BB] | piecesBB[BLACK_QUEENS_BB] |
                    piecesBB[BLACK_BISHOPS_BB] | piecesBB[BLACK_KNIGHTS_BB] |
                    piecesBB[BLACK_ROOKS_BB] | piecesBB[BLACK_PAWNS_BB]);

  allPieces = (allWhitePieces | allBlackPieces);

  emptySquares = ~allPieces;

  (turn == WHITE) ? turn = BLACK : turn = WHITE;
}

void Bitboard::printMoveList() {
  for (Move m : moveList) {
    m.printMove();
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
        bitset<64> pieces = piecesBB[z];
        if ((pieces & lookupTable->piece_lookup[square]).any() == true) {
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
