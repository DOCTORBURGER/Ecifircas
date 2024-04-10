#include "search.h"

#include "types.h"
#include "movegen.h"
#include "board.h"
#include "eval.h"
#include "bitboard.h"

namespace Ecifircas {
	int Ply = 0; // half move (ie. player move)

	Move BestMove;

	int AttackerTable[6][6] = {
        {105, 205, 305, 405, 505, 605 },
        {104, 204, 304, 404, 504, 604 },
        {103, 203, 303, 403, 503, 603 },
        {102, 202, 302, 402, 502, 602 },
        {101, 201, 301, 401, 501, 601 },
        {100, 200, 300, 400, 500, 600 }
    };

	int quiescence(int alpha, int beta)
	{
		int stand_pat = evaluate();

		if (stand_pat >= beta)
			return beta;
		if (alpha < stand_pat)
			alpha = stand_pat;

		Moves moves;
		generate_moves(moves);

		for (int i = 0; i < moves.get_count(); i++) {
			Move move = moves.get_move(i);
			copy_board();

			Ply++;

			if (make_move(move, ONLY_CAPTURES) == false) {
				Ply--;
				continue;
			}

			int score = -quiescence(-beta, -alpha);

			Ply--;

			restore_from_copy();

			if (score >= beta) {
				return beta;
			}
			if (score > alpha) {
				score = alpha;
			}
		}

		return alpha;
	}

	int negamax(int alpha, int beta, int depth)
	{
		if (depth == 0) {
			return quiescence(alpha, beta);
		}

		int legalMoves = 0;

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

			legalMoves++;

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

		if (legalMoves == 0) {
			if (is_square_attacked(Square(get_ls1b_index(Pieces[SideToMove][KING])), Color(SideToMove ^ 1))) {
				return -49000 + Ply;
			}
			else {
				return 0;
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