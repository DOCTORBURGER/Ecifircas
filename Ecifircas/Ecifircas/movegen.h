#pragma once


#include "types.h"
#include <string>

namespace Ecifircas {
	class Moves
	{
	public:
		Moves()
		{
			count = 0;
		}

		inline void add_move(Move move)
		{
			moves[count] = move;
			count++;
		}

		inline void print_moves()
		{
			std::string output = "";
			for (int curMoveIndex = 0; curMoveIndex < count; curMoveIndex++) {
				Move move = moves[curMoveIndex];
				output += ("move: " + SquareToCoordinates[move.from_sq()] + SquareToCoordinates[move.to_sq()] + " ");

				MoveFlags flags = move.flags();

				output += (flags == QUIET_MOVE) ? "Quiet " : "";
				output += (flags == DOUBLE_PAWN_PUSH) ? "Double Pawn Push " : "";
				output += (flags == KING_CASTLE) ? "King Castle " : "";
				output += (flags == QUEEN_CASTLE) ? "Queen Castle " : "";
				output += (flags == CAPTURE) ? "Capture " : "";
				output += (flags == EP_CAPTURE) ? "EP Capture " : "";
				output += (flags == KNIGHT_PROMO) ? "Knight Promo " : "";
				output += (flags == BISHOP_PROMO) ? "Bishop Promo " : "";
				output += (flags == ROOK_PROMO) ? "Rook Promo" : "";
				output += (flags == QUEEN_PROMO) ? "Queen Promo" : "";
				output += (flags == KNIGHT_PROMO_CAPTURE) ? "Knight Promo Capture" : "";
				output += (flags == BISHOP_PROMO_CAPTURE) ? "Bishop Promo Capture" : "";
				output += (flags == ROOK_PROMO_CAPTURE) ? "Rook Promo Capture" : "";
				output += (flags == QUEEN_PROMO_CAPTURE) ? "Queen Promo Capture" : "";

				output += "\n";
			}

			std::cout << output << std::endl;
			std::cout << "moves: " << count << std::endl;
		}
	protected:
		Move moves[256];
		int count;
	};

	void generate_moves(Moves& moveList);
}
