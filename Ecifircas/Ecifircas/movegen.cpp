#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <string>

#include "types.h"
#include "bitboard.h"
#include "board.h"
#include "movegen.h"\

namespace Ecifircas 
{
	void generate_moves(Moves& moveList) 
	{
		Square sourceSquare, destSquare;

		Bitboard bitboard, attacks;

		for (Piece piece = PAWN; piece <= KING; piece++) {
			bitboard = Pieces[SideToMove][piece];
			Bitboard emptySquares = ~Occupancies[BOTH];
			Color oppositeColor = (SideToMove) ? WHITE : BLACK;

			// Generate white pawn and castling king moves
			if (SideToMove == WHITE) {
				if (piece == PAWN) {
					// Handle promotions
					Bitboard promotions = shift_bit(bitboard & Rank7, NORTH) & emptySquares;
					Bitboard nonPromoters = bitboard & ~Rank7;

					Bitboard singlePushes = shift_bit(nonPromoters, NORTH) & emptySquares;
					Bitboard doublePushes = shift_bit(singlePushes & Rank3, NORTH) & emptySquares;

					while (promotions) {
						destSquare = Square(get_ls1b_index(promotions));
						sourceSquare = Square(destSquare - 8);
						
						moveList.add_move(Move(sourceSquare, destSquare, piece, WHITE, KNIGHT_PROMO));
						moveList.add_move(Move(sourceSquare, destSquare, piece, WHITE, BISHOP_PROMO));
						moveList.add_move(Move(sourceSquare, destSquare, piece, WHITE, ROOK_PROMO));
						moveList.add_move(Move(sourceSquare, destSquare, piece, WHITE, QUEEN_PROMO));

						pop_bit(promotions, destSquare);
					}

					while (singlePushes) {
						destSquare = Square(get_ls1b_index(singlePushes));

						moveList.add_move(Move(Square(destSquare - 8), destSquare, piece, WHITE, QUIET_MOVE));

						pop_bit(singlePushes, destSquare);
					}

					while (doublePushes) {
						destSquare = Square(get_ls1b_index(doublePushes));

						moveList.add_move(Move(Square(destSquare - 16), destSquare, piece, WHITE, DOUBLE_PAWN_PUSH));

						pop_bit(doublePushes, destSquare);
					}

					while (bitboard) {
						sourceSquare = Square(get_ls1b_index(bitboard));

						attacks = pawn_attacks_bb(sourceSquare, WHITE) & (Occupancies[BLACK] & ~Rank8);
						Bitboard promotionAttacks = pawn_attacks_bb(sourceSquare, WHITE) & (Occupancies[BLACK] & Rank8);
						Bitboard enpassantAttacks = pawn_attacks_bb(sourceSquare, WHITE) & (get_square_bb(EnpassantSquare) & Rank6);

						while (attacks) {
							destSquare = Square(get_ls1b_index(attacks));

							moveList.add_move(Move(sourceSquare, destSquare, piece, WHITE, CAPTURE));

							pop_bit(attacks, destSquare);
						}

						while (promotionAttacks) {
							destSquare = Square(get_ls1b_index(promotionAttacks));

							moveList.add_move(Move(sourceSquare, destSquare, piece, WHITE, KNIGHT_PROMO_CAPTURE));
							moveList.add_move(Move(sourceSquare, destSquare, piece, WHITE, BISHOP_PROMO_CAPTURE));
							moveList.add_move(Move(sourceSquare, destSquare, piece, WHITE, ROOK_PROMO_CAPTURE));
							moveList.add_move(Move(sourceSquare, destSquare, piece, WHITE, QUEEN_PROMO_CAPTURE));

							pop_bit(promotionAttacks, destSquare);
						}

						while (enpassantAttacks) {
							destSquare = Square(get_ls1b_index(enpassantAttacks));

							moveList.add_move(Move(sourceSquare, destSquare, piece, WHITE, EP_CAPTURE));

							pop_bit(enpassantAttacks, destSquare);
						}

						pop_bit(bitboard, sourceSquare);
					}
 				}

				if (piece == KING) {
					// Kingside castle
					if (CastleRights & WHITE_KINGSIDE) {
						if (get_bit(emptySquares, F1) && get_bit(emptySquares, G1)) {
							if (!is_square_attacked(E1, BLACK) && !is_square_attacked(F1, BLACK)) {
								moveList.add_move(Move(E1, G1, piece, SideToMove, KING_CASTLE));
							}
						}
					}

					// Queenside castle
					if (CastleRights & WHITE_QUEENSIDE) {
						if (get_bit(emptySquares, D1) && get_bit(emptySquares, C1) && get_bit(emptySquares, B1)) {
							if (!is_square_attacked(E1, BLACK) && !is_square_attacked(D1, BLACK)) {
								moveList.add_move(Move(E1, C1, piece, SideToMove, QUEEN_CASTLE));
							}
						}
					}
				}
			}
			// Black pawn and castling king moves
			else { 
				if (piece == PAWN) {
					// Handle promotions
					Bitboard promotions = shift_bit(bitboard & Rank2, SOUTH) & emptySquares;
					Bitboard nonPromoters = bitboard & ~Rank2;

					Bitboard singlePushes = shift_bit(nonPromoters, SOUTH) & emptySquares;
					Bitboard doublePushes = shift_bit(singlePushes & Rank6, SOUTH) & emptySquares;

					while (promotions) {
						destSquare = Square(get_ls1b_index(promotions));
						sourceSquare = Square(destSquare + 8);

						moveList.add_move(Move(sourceSquare, destSquare, piece, BLACK, KNIGHT_PROMO));
						moveList.add_move(Move(sourceSquare, destSquare, piece, BLACK, BISHOP_PROMO));
						moveList.add_move(Move(sourceSquare, destSquare, piece, BLACK, ROOK_PROMO));
						moveList.add_move(Move(sourceSquare, destSquare, piece, BLACK, QUEEN_PROMO));

						pop_bit(promotions, destSquare);
					}

					while (singlePushes) {
						destSquare = Square(get_ls1b_index(singlePushes));

						moveList.add_move(Move(Square(destSquare + 8), destSquare, piece, BLACK, QUIET_MOVE));

						pop_bit(singlePushes, destSquare);
					}

					while (doublePushes) {
						destSquare = Square(get_ls1b_index(doublePushes));

						moveList.add_move(Move(Square(destSquare + 16), destSquare, piece, BLACK, DOUBLE_PAWN_PUSH));

						pop_bit(doublePushes, destSquare);
					}

					while (bitboard) {
						sourceSquare = Square(get_ls1b_index(bitboard));

						attacks = pawn_attacks_bb(sourceSquare, BLACK) & (Occupancies[WHITE] & ~Rank1);
						Bitboard promotionAttacks = pawn_attacks_bb(sourceSquare, BLACK) & (Occupancies[WHITE] & Rank1);
						Bitboard enpassantAttacks = pawn_attacks_bb(sourceSquare, BLACK) & (get_square_bb(EnpassantSquare) & Rank3);

						while (attacks) {
							destSquare = Square(get_ls1b_index(attacks));

							moveList.add_move(Move(sourceSquare, destSquare, piece, BLACK, CAPTURE));

							pop_bit(attacks, destSquare);
						}

						while (promotionAttacks) {
							destSquare = Square(get_ls1b_index(promotionAttacks));

							moveList.add_move(Move(sourceSquare, destSquare, piece, BLACK, KNIGHT_PROMO_CAPTURE));
							moveList.add_move(Move(sourceSquare, destSquare, piece, BLACK, BISHOP_PROMO_CAPTURE));
							moveList.add_move(Move(sourceSquare, destSquare, piece, BLACK, ROOK_PROMO_CAPTURE));
							moveList.add_move(Move(sourceSquare, destSquare, piece, BLACK, QUEEN_PROMO_CAPTURE));

							pop_bit(promotionAttacks, destSquare);
						}

						while (enpassantAttacks) {
							destSquare = Square(get_ls1b_index(enpassantAttacks));

							moveList.add_move(Move(sourceSquare, destSquare, piece, WHITE, EP_CAPTURE));

							pop_bit(enpassantAttacks, destSquare);
						}

						pop_bit(bitboard, sourceSquare);
					}
				}

				if (piece == KING) {
					// Kingside castle
					if (CastleRights & BLACK_KINGSIDE) {
						if (get_bit(emptySquares, F8) && get_bit(emptySquares, G8)) {
							if (!is_square_attacked(E8, WHITE) && !is_square_attacked(F8, WHITE)) {
								moveList.add_move(Move(E8, G8, piece, SideToMove, KING_CASTLE));
							}
						}
					}

					// Queenside castle
					if (CastleRights & BLACK_QUEENSIDE) {
						if (get_bit(emptySquares, D8) && get_bit(emptySquares, C8) && get_bit(emptySquares, B8)) {
							if (!is_square_attacked(E8, WHITE) && !is_square_attacked(D8, WHITE)) {
								moveList.add_move(Move(E8, C8, piece, SideToMove, KING_CASTLE));
							}
						}
					}
				}
			}

			// Generate all other moves
			while (bitboard && piece != PAWN) {
				sourceSquare = (Square)get_ls1b_index(bitboard);

				attacks = attacks_bb(sourceSquare, Occupancies[BOTH], piece) & ~Occupancies[SideToMove];

				while (attacks) {
					destSquare = (Square)get_ls1b_index(attacks);

					if (!get_bit(Occupancies[oppositeColor], destSquare))
						moveList.add_move(Move(sourceSquare, destSquare, piece, SideToMove, QUIET_MOVE));

					else
						moveList.add_move(Move(sourceSquare, destSquare, piece, SideToMove, CAPTURE));

					pop_bit(attacks, destSquare);
				}

				pop_bit(bitboard, sourceSquare);
			}
		}
	}
}

