#pragma once

#include <cstdint>
#include <cstdlib>

#include "types.h"

namespace Ecifircas {
	constexpr Bitboard FileA = 0x0101010101010101ULL;
	constexpr Bitboard FileB = FileA << 1;
	constexpr Bitboard FileC = FileA << 2;
	constexpr Bitboard FileD = FileA << 3;
	constexpr Bitboard FileE = FileA << 4;
	constexpr Bitboard FileF = FileA << 5;
	constexpr Bitboard FileG = FileA << 6;
	constexpr Bitboard FileH = FileA << 7;

	constexpr Bitboard Rank1 = 0xFFULL;
	constexpr Bitboard Rank2 = Rank1 << 8;
	constexpr Bitboard Rank3 = Rank1 << (8 * 2);
	constexpr Bitboard Rank4 = Rank1 << (8 * 3);
	constexpr Bitboard Rank5 = Rank1 << (8 * 4);
	constexpr Bitboard Rank6 = Rank1 << (8 * 5);
	constexpr Bitboard Rank7 = Rank1 << (8 * 6);
	constexpr Bitboard Rank8 = Rank1 << (8 * 7);

	void initialize_bitboards();

	constexpr Bitboard get_square_bb(Square s) { return (1ULL << s); }

	// This will shift the set bits of a bitboard in a given direction
	constexpr Bitboard shift_bit(Bitboard bb, Direction dir) {
		return	dir == NORTH			? bb << 8
			:	dir == SOUTH			? bb >> 8
			:	dir == NORTH + NORTH	? bb << 16 // These double directions are meant to handle double pawn pushes
			:	dir == SOUTH + SOUTH	? bb >> 16
			:	dir == EAST				? (bb & ~FileH) << 1 // SETTING FILE BITS TO 0 HANDLES OFF-BOARD MOVEMENT
			:	dir == WEST				? (bb & ~FileA) >> 1
			:	dir == NORTH_EAST		? (bb & ~FileH) << 9
			:	dir == SOUTH_EAST		? (bb & ~FileH) >> 7
			:	dir == NORTH_WEST		? (bb & ~FileA) << 7
			:	dir == SOUTH_WEST		? (bb & ~FileA) >> 9
										: 0;
	}

	constexpr Bitboard pawn_attacks_bb(Bitboard bb, Color c) {
		return	c == WHITE	? shift_bit(bb, NORTH_WEST) | shift_bit(bb, NORTH_EAST)
							: shift_bit(bb, SOUTH_WEST) | shift_bit(bb, SOUTH_EAST);
	}
}
