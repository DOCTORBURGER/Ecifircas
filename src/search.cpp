#include "search.h"

#include <vector>

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

	int SearchNodes;

	int score_move(Move move)
	{
		if (move.flags() & CAPTURE) {
			Piece targetPiece = PAWN; // set to pawn to have default behavior for EP 

			for (Piece piece = PAWN; piece <= KING; piece++) {
				if (get_bit(Pieces[SideToMove ^ 1][piece], move.to_sq())) {
					targetPiece = piece;
					break;
				}
			}

			return AttackerTable[move.get_piece()][targetPiece];
		}
		else {

		}

		return 0;
	}

	void sort_moves(Moves& moves)
	{
		int moveCount = moves.get_count();
		std::vector<int> moveScores(moveCount);

		for (int moveIndex = 0; moveIndex < moves.get_count(); moveIndex++) 
			moveScores[moveIndex] = score_move(moves.get_move(moveIndex));

		for (int currentMove = 0; currentMove < moves.get_count(); currentMove++) {
			for (int nextMove = currentMove + 1; nextMove < moves.get_count(); nextMove++) {
				if (moveScores[currentMove] < moveScores[nextMove]) {
					int tempScore = moveScores[currentMove];
					moveScores[currentMove] = moveScores[nextMove];
					moveScores[nextMove] = tempScore;

					moves.swap(currentMove, nextMove);
				}
			}
		}
	}

	int quiescence(int alpha, int beta)
	{
		SearchNodes++;

		int standPat = evaluate();

		if (standPat >= beta)
			return beta;
		if (alpha < standPat)
			alpha = standPat;

		Moves moves;
		generate_moves(moves);

		sort_moves(moves);

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

		SearchNodes++;

		int legalMoves = 0;

		Move bestSoFar;

		int oldAlpha = alpha;

		Moves moves;
		generate_moves(moves);

		sort_moves(moves);

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
		SearchNodes = 0;
		int score = negamax(-50000, 50000, 5);
		//std::cout << "Search nodes: " << SearchNodes << std::endl;

		return BestMove;
	}
}
