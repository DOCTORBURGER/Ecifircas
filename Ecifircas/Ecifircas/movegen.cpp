#include <cstdint>
#include <cstdlib>

#include "types.h"
#include "bitboard.h"
#include "board.h"
#include "movegen.h"

namespace Ecifircas 
{
	void generate_moves() 
	{
		Square sourceSquare, destSquare;

		Bitboard bitboard, attacks;

		for (Piece piece = PAWN; piece <= KING; piece++) {
			bitboard = Pieces[SideToMove][piece];
			Bitboard emptySquares = ~Occupancies[BOTH];

			// Generate white pawn and castling king moves
			if (SideToMove == WHITE) {
				if (piece == PAWN) {
					// Handle promotions
					Bitboard promoters = shift_bit(shift_bit(bitboard & Rank7, NORTH) & emptySquares, SOUTH); // I shift back to get original square
					Bitboard nonPromoters = bitboard & ~Rank7;

					Bitboard singlePushes = shift_bit(nonPromoters, NORTH) & emptySquares;
					Bitboard doublePushes = shift_bit(singlePushes & Rank3, NORTH) & emptySquares;

					// Might redo this later, for now it works for the DESTINATIONS: SOURCE WILL BE A PROBLEM
					attacks = pawn_attacks_bb(nonPromoters, WHITE) & Occupancies[BLACK];
					Bitboard promotionAttacks = pawn_attacks_bb(bitboard & Rank7, WHITE) & Occupancies[BLACK];
					Bitboard enpassantAttacks = pawn_attacks_bb(bitboard & Rank5, WHITE) & get_square_bb(EnpassantSquare);
					int i = 0;
				}
			}
			// Black pawn and castling king moves
			else { 
				if (piece == PAWN) {
					// Handle promotions
					Bitboard promoters = shift_bit(shift_bit(bitboard & Rank2, SOUTH) & emptySquares, NORTH); // I shift back to get original square
					Bitboard nonPromoters = bitboard & ~Rank2;

					Bitboard singlePushes = shift_bit(nonPromoters, SOUTH) & emptySquares;
					Bitboard doublePushes = shift_bit(singlePushes & Rank6, SOUTH) & emptySquares;
				}
			}

			// Generate knight moves

			// Generate bishop moves

			// Generate rook moves

			// Generate queen moves

			// Generate king moves
		}
	}
}

