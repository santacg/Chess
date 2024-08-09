#include "bitboard.h"
#include "move.h"
#include "utils.h"
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

  updateDerivedBitboards();

  turn = WHITE;
  castlingRights = 1111;
  enPassantSq = no_square;
}

Bitboard::Bitboard(bitset<64> pieces[12], bitset<4> cR, int epSq, Color side) {
  for (int i = 0; i < 12; i++) {
    piecesBB[i] = pieces[i];
  }

  updateDerivedBitboards();

  turn = side;
  castlingRights = cR;
  enPassantSq = epSq;
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

  /* Intersect own pieces with opponent attacks */
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

void Bitboard::generateCastleMoves() {
  /* White side king castling */
  if (castlingRights.test(0) == true) {
    /* Check if squares between king and rook are empty and if any of the end or
     * passing squares are attacked by an opponents piece */
    if ((emptySquares.test(5) == true && emptySquares.test(6) == true) &&
        (isSquareAttacked(BLACK, 5) == false &&
         isSquareAttacked(BLACK, 6) == false)) {
      moveList.push_back(Move(4, 6, KING_CASTLE, KING, WHITE));
    }
  }

  /* White side queen castling */
  if (castlingRights.test(1) == true) {
    if ((emptySquares.test(3) == true && emptySquares.test(2) == true &&
         emptySquares.test(1) == true) &&
        (isSquareAttacked(BLACK, 3) == false &&
         isSquareAttacked(BLACK, 2) == false)) {

      moveList.push_back(Move(4, 2, QUEEN_CASTLE, KING, WHITE));
    }
  }

  /* Black side king castling */
  if (castlingRights.test(2) == true) {
    if ((emptySquares.test(61) == true && emptySquares.test(62) == true) &&
        (isSquareAttacked(WHITE, 61) == false &&
         isSquareAttacked(WHITE, 62) == false)) {
      moveList.push_back(Move(60, 62, KING_CASTLE, KING, BLACK));
    }
  }

  /* Black side queen castling */
  if (castlingRights.test(3) == true) {
    if ((emptySquares.test(59) == true && emptySquares.test(58) == true &&
         emptySquares.test(57) == true) &&
        (isSquareAttacked(WHITE, 59) == false &&
         isSquareAttacked(WHITE, 58) == false)) {
      moveList.push_back(Move(60, 58, QUEEN_CASTLE, KING, BLACK));
    }
  }
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

      int flag;

      /* Define variables for rank changes based on the color */
      int rank_change = (color == WHITE) ? 16 : -16;
      int promotion_rank = (color == WHITE) ? 7 : 0;

      /* Check if it's a double pawn push move */
      if (target_square == source_square + rank_change) {
        flag = DOUBLE_PAWN_PUSH;
      }
      /* Check if it's a promotion */
      else if ((lookupTable->piece_lookup[target_square] &
                lookupTable->mask_rank[promotion_rank])
                   .any()) {
        flag = PROMOTION;
      } else {
        flag = QUIET_MOVE;
      }

      moves.set(target_square, false);

      if (flag == PROMOTION) {
        /* Generate all possible types of promotion */
        for (int i = KNIGHT_PROMOTION; i <= QUEEN_PROMOTION; i++) {
          moveList.push_back(
              Move(source_square, target_square, i, PAWN, color));
        }
      } else {
        moveList.push_back(
            Move(source_square, target_square, flag, PAWN, color));
      }
    }

    bitset<64> pieces;
    (color == WHITE) ? pieces = allBlackPieces : pieces = allWhitePieces;

    /* Generate attacks and captures */
    bitset<64> attacks = (this->*attack_generator)(color, source_square);
    bitset<64> captures = attacks & pieces;

    /* Loop over all the captures generated */
    int flag = CAPTURE;
    while (captures.any()) {
      int target_square = countr_zero(captures.to_ulong());

      /* Check for promotion captures */
      int promotion_rank = (color == WHITE) ? 7 : 0;
      if ((lookupTable->piece_lookup[target_square] &
           lookupTable->mask_rank[promotion_rank])
              .any() == true) {
        flag = PROMOTION_CAPTURE;
      }

      captures.set(target_square, false);

      /* Generate all possible promotion captures */
      if (flag == PROMOTION_CAPTURE) {
        for (int i = KNIGHT_PROMOTION_CAPTURE; i <= QUEEN_PROMOTION_CAPTURE;
             i++) {
          moveList.push_back(
              Move(source_square, target_square, i, PAWN, color));
        }
      } else {
        moveList.push_back(
            Move(source_square, target_square, CAPTURE, PAWN, color));
      }
    }

    /* Generate en passant capture */
    if (enPassantSq != no_square) {
      if ((attacks & lookupTable->piece_lookup[enPassantSq]).any() == true) {
        moveList.push_back(
            Move(source_square, enPassantSq, EP_CAPTURE, PAWN, color));
      }
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
      moveList.push_back(
          Move(source_square, target_square, QUIET_MOVE, piece_type, color));
    }

    /* Loop over all the captures generated */
    while (captures.any()) {
      int target_square = countr_zero(captures.to_ulong());
      captures.set(target_square, false);
      moveList.push_back(
          Move(source_square, target_square, CAPTURE, piece_type, color));
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
                 &Bitboard::generatePawnAttacks);
    } else {
      pieceMoves(bb, BLACK, &Bitboard::generatePawnMoves,
                 &Bitboard::generatePawnAttacks);
    }
  }

  generateCastleMoves();
}

Bitboard Bitboard::copyBoard() {
  Bitboard bitboard_cpy = *this;
  return bitboard_cpy;
}

void Bitboard::makeMove(Move move) {
  /* Reset en passant square */
  enPassantSq = no_square;

  /* Get the move type */
  int flag = move.getFlag();
  int source_square = move.getSourceSquare();
  int target_square = move.getTargetSquare();
  int piece = move.getPiece();
  int color = move.getColor();

  /* Get the correct piece bitboard taking into account the color */
  int pieceBB = piece * 2 + (color == BLACK);

  /* Remove piece from source square and add it to target square */
  piecesBB[pieceBB].set(source_square, false);
  piecesBB[pieceBB].set(target_square, true);

  /* Handle capture or promotion capture*/
  if (flag == CAPTURE || flag >= KNIGHT_PROMOTION_CAPTURE) {
    int i = (color == WHITE) ? 1 : 0;

    /* Skip same color piece bitboards */
    for (; i < 6; i += 2) {
      if (piecesBB[i].test(target_square) == true) {
        piecesBB[i].set(target_square, false);
        break;
      }
    }
  }

  /* Handle castling */
  if (flag == KING_CASTLE) {
    if (color == WHITE) {
      /* Move white rook */
      piecesBB[WHITE_ROOKS_BB].set(7, false);
      piecesBB[WHITE_ROOKS_BB].set(5, true);
    } else {
      /* Move black rook */
      piecesBB[BLACK_ROOKS_BB].set(63, false);
      piecesBB[BLACK_ROOKS_BB].set(61, true);
    }
  } else if (flag == QUEEN_CASTLE) {
    if (color == WHITE) {
      /* Move white rook */
      piecesBB[WHITE_ROOKS_BB].set(0, false);
      piecesBB[WHITE_ROOKS_BB].set(3, true);
    } else {
      /* Move black rook */
      piecesBB[BLACK_ROOKS_BB].set(56, false);
      piecesBB[BLACK_ROOKS_BB].set(59, true);
    }
  }

  /* Handle promotion */
  if (flag >= KNIGHT_PROMOTION) {
    /* Delete the pawn */
    piecesBB[pieceBB].set(target_square, false);

    /* Choose the promoted piece bitboard */
    switch (flag) {
    case QUEEN_PROMOTION:
    case QUEEN_PROMOTION_CAPTURE:
      pieceBB = (color == WHITE) ? WHITE_QUEENS_BB : BLACK_QUEENS_BB;
      break;
    case ROOK_PROMOTION:
    case ROOK_PROMOTION_CAPTURE:
      pieceBB = (color == WHITE) ? WHITE_ROOKS_BB : BLACK_ROOKS_BB;
      break;
    case KNIGHT_PROMOTION:
    case KNIGHT_PROMOTION_CAPTURE:
      pieceBB = (color == WHITE) ? WHITE_KNIGHTS_BB : BLACK_KNIGHTS_BB;
      break;
    case BISHOP_PROMOTION:
    case BISHOP_PROMOTION_CAPTURE:
      pieceBB = (color == WHITE) ? WHITE_BISHOPS_BB : BLACK_BISHOPS_BB;
      break;
    }

    /* Set promoted piece on piece bitboard */
    piecesBB[pieceBB].set(target_square, true);
  }

  /* Handle double pawn push */
  if (flag == DOUBLE_PAWN_PUSH) {
    enPassantSq = target_square + ((color == WHITE) ? -8 : 8);
  }

  /* Handle en passant */
  if (flag == EP_CAPTURE) {
    int en_passant_capture_sq = target_square + ((color == WHITE) ? -8 : 8);
    piecesBB[(color == WHITE) ? BLACK_PAWNS_BB : WHITE_PAWNS_BB].set(
        en_passant_capture_sq, false);
  }

  /* Update castling rights */
  int king_side_index = (color == WHITE) ? 0 : 2;
  int queen_side_index = king_side_index + 1;

  if (piece == KING) {
    /* Disable all castling rights */
    castlingRights.set(king_side_index, false);
    castlingRights.set(queen_side_index, false);
  } else if (piece == ROOK) {
    if (source_square == ((color == WHITE) ? 0 : 56)) {
      /* Disable queen side castling */
      castlingRights.set(queen_side_index, false);
    } else if (source_square == ((color == WHITE) ? 7 : 63)) {
      /* Disable king side castling */
      castlingRights.set(king_side_index, false);
    }
  }

  /* Update all derived bitboards */
  updateDerivedBitboards();

  /* Change turn */
  (turn == WHITE) ? turn = BLACK : turn = WHITE;
}

void Bitboard::updateDerivedBitboards() {
  allWhitePieces = (piecesBB[WHITE_KING_BB] | piecesBB[WHITE_QUEENS_BB] |
                    piecesBB[WHITE_BISHOPS_BB] | piecesBB[WHITE_KNIGHTS_BB] |
                    piecesBB[WHITE_ROOKS_BB] | piecesBB[WHITE_PAWNS_BB]);

  allBlackPieces = (piecesBB[BLACK_KING_BB] | piecesBB[BLACK_QUEENS_BB] |
                    piecesBB[BLACK_BISHOPS_BB] | piecesBB[BLACK_KNIGHTS_BB] |
                    piecesBB[BLACK_ROOKS_BB] | piecesBB[BLACK_PAWNS_BB]);

  allPieces = (allWhitePieces | allBlackPieces);

  emptySquares = ~allPieces;
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

  cout << "Castling rights: ";
  if (castlingRights.test(0) == true) {
    cout << "K";
  }
  if (castlingRights.test(1) == true) {
    cout << "Q";
  }
  if (castlingRights.test(2) == true) {
    cout << "k";
  }
  if (castlingRights.test(3) == true) {
    cout << "q";
  }
  if (castlingRights.any() == false) {
    cout << "-";
  }
  cout << endl << endl;

  cout << "En passant square: "
       << ((enPassantSq == no_square) ? "-" : coordinateToSquare[enPassantSq]);
  cout << endl << endl;
}
