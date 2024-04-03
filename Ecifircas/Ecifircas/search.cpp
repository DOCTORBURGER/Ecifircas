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

			// fail-hard beta cutoff
			if (score >= beta)
			{
				// node (move) fails high
				return beta;
			}

			// found a better move
			if (score > alpha)
			{
				// PV node (move)
				alpha = score;

				// if root move
				if (Ply == 0)
					// associate best move with the best score
					bestSoFar = move;
			}
		}

		if (oldAlpha != alpha)
			// init best move
			BestMove = bestSoFar;

		return alpha;
	}

	Move search()
	{
		int score = negamax(-50000, 50000, 5);

		return BestMove;
	}
}