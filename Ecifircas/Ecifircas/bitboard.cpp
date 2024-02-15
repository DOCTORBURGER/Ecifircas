#include "types.h"
#include <iostream>
#include <cstdint>

#include "bitboard.h"

namespace Ecifircas {
	Bitboard PawnAttacks[2][64];
	Bitboard PsuedoAttacks[6][64];

	Bitboard knight_shift(Bitboard bb, int shift) {
		return	shift == 17		? (bb & ~FileH) << 17
			:	shift == 15		? (bb & ~FileA) << 15
			:	shift == 10		? (bb & ~FileH & ~FileG) << 10
			:	shift == 6		? (bb & ~FileA & ~FileB) << 6
			:	shift == -6		? (bb & ~FileH & ~FileG) >> 6
			:	shift == -10	? (bb & ~FileA & ~FileB) >> 10
			:	shift == -15	? (bb & ~FileH) >> 15
			:	shift == -17	? (bb & ~FileA) >> 17
								: 0;
	}

	void initialize_bitboards() {
		for (Square square = A1; square <= H8; square++) {
			PawnAttacks[WHITE][square] = pawn_attacks_bb(get_square_bb(square), WHITE);
			PawnAttacks[BLACK][square] = pawn_attacks_bb(get_square_bb(square), BLACK);

			for (int moveOffset : { (int)NORTH, (int)SOUTH, (int)EAST, (int)WEST, (int)NORTH_WEST, (int)NORTH_EAST, (int)SOUTH_WEST, (int)SOUTH_EAST }) {
				PsuedoAttacks[KING][square] |= shift_bit(get_square_bb(square), (Direction)moveOffset);
			}

			for (int shift : { 17, 15, 10, 6, -6, -10, -15, -17 }) {
				PsuedoAttacks[KNIGHT][square] |= knight_shift(get_square_bb(square), shift);
			}
		}
	}
}