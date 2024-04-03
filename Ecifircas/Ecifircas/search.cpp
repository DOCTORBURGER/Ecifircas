#include "search.h"

#include "types.h"
#include "movegen.h"
#include "board.h"
#include "eval.h"

namespace Ecifircas {
	int Ply = 0; // half move (ie. player move)

	Move BestMove;

	int negamax(int alpha, int beta, int depth)
	{
		if (depth == 0) {
			return evaluate();
		}

		Move bestSoFar;

		int oldAlpha = alpha;

		Moves moves;

		generate_moves(moves);

		for (int i = 0; i < moves.get_count(); i++) {
			Move move = moves.get_move(i);
			copy_board();

			Ply++;

			if (make_move(move, ALL_MOVES) == false) {
				Ply--;
				continue;
			}

			int score = -negamax(-beta, -alpha, depth - 1);

			Ply--;

			restore_from_copy();

			if (score >= beta)
			{
				return beta;
			}

			if (score > alpha)
			{
				alpha = score;

				if (Ply == 0)
					bestSoFar = move;
			}
		}

		if (oldAlpha != alpha)
			BestMove = bestSoFar;

		return alpha;
	}

	Move search()
	{
		int score = negamax(-50000, 50000, 3);

		return BestMove;
	}
}