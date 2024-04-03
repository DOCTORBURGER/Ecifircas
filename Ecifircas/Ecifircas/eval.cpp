#include "eval.h"

#include "types.h"
#include "bitboard.h"
#include "board.h"

namespace Ecifircas {
	int materialScores[6] = {
		100,
		300,
		300,
		500,
		900,
		10000,
	};

	int evaluate()
	{
		int score = 0;

		Bitboard bitboard;

		Square square;

		for (Color color = WHITE; color <= BLACK; color++) {
			for (Piece piece = PAWN; piece <= KING; piece++) {
				bitboard = Pieces[color][piece];

				while (bitboard) {
					square = Square(get_ls1b_index(bitboard));

					score += (1 - 2 * color) * materialScores[piece];

					pop_bit(bitboard, square);
				}
			}
		}

		return score;
	}
}
