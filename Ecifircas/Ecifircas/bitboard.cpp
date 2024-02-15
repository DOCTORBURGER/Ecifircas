#include "types.h"
#include <iostream>
#include <cstdint>

#include "bitboard.h"

namespace Ecifircas {
	Bitboard PawnAttacks[2][64];
	Bitboard PsuedoAttacks[6][64];

	void initialize_bitboards() {
		for (Square square = A1; square <= H8; square++) {
			PawnAttacks[WHITE][square] = pawn_attacks_bb(get_square_bb(square), WHITE);
			PawnAttacks[BLACK][square] = pawn_attacks_bb(get_square_bb(square), BLACK);
		}
	}
}