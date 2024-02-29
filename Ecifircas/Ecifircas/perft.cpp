#include <iostream>

#include "board.h"
#include "types.h"
#include "perft.h"
#include "movegen.h"

namespace Ecifircas
{
	long Nodes;

	void perft_driver(int depth)
	{
		if (depth == 0) {
			Nodes++;
			return;
		}
		
		Moves moves;

		generate_moves(moves);

		for (int curMoveIndex = 0; curMoveIndex < moves.get_count(); curMoveIndex++) {
			copy_board();

			if (!make_move(moves.get_move(curMoveIndex), ALL_MOVES)) {
				continue;
			}

			perft_driver(depth - 1);

			restore_from_copy();
		}
	}
}

